#include "NongManager.hpp"

NongData NongManager::getNongs(int songID) {
    auto path = NongManager::getJsonPath(songID);

    std::ifstream input(path.string());
    std::stringstream buffer;
    buffer << input.rdbuf();
    input.close();

    auto json = json::parse(std::string_view(buffer.str()));
    return json::Serialize<NongData>::from_json(json);
}

SongInfo NongManager::getActiveNong(int songID) {
    auto nongs = NongManager::getNongs(songID);

    for (auto &song : nongs.songs) {
        if (song.path == nongs.active) {
            return song;
        }
    }

    auto error = "NONGD error: no active nong is set for id " + std::to_string(songID);
    throw std::exception(error.c_str());
}

/**
 * Removes nongs that have invalid paths from the JSON.
 * 
 * @return the removed songs
*/
std::vector<SongInfo> NongManager::validateNongs(int songID) {
    auto currentData = NongManager::getNongs(songID);
    // Validate nong paths and delete those that don't exist anymore
    std::vector<SongInfo> invalidSongs;
    std::vector<SongInfo> validSongs;

    for (auto &song : currentData.songs) {
        if (!ghc::filesystem::exists(song.path) && currentData.defaultPath != song.path) {
            invalidSongs.push_back(song);
            if (song.path == currentData.active) {
                currentData.active = currentData.defaultPath;
            }
        } else {
            validSongs.push_back(song);
        }
    }

    if (invalidSongs.size() > 0) {
        NongData newData = {
            .active = currentData.active,
            .defaultPath = currentData.defaultPath,
            .songs = validSongs
        };

        NongManager::saveNongs(newData, songID);
    }

    return invalidSongs;
}

void NongManager::saveNongs(NongData const& data, int songID) {
    auto path = NongManager::getJsonPath(songID);
    auto json = json::Serialize<NongData>::to_json(data);
    std::ofstream output(path.string());
    output << json.dump();
    output.close();
}

void NongManager::addNong(SongInfo const& song, int songID) {
    auto existingData = NongManager::getNongs(songID);
    for (auto savedSong : existingData.songs) {
        if (song.path.string() == savedSong.path.string()) {
            // if song exists, just return
            return;
        }
    }
    existingData.songs.push_back(song);
    NongManager::saveNongs(existingData, songID);
}

void NongManager::deleteNong(SongInfo const& song, int songID) {
    std::vector<SongInfo> newSongs;
    auto existingData = NongManager::getNongs(songID);
    for (auto savedSong : existingData.songs) {
        if (savedSong.path == song.path) {
            if (song.path == existingData.active) {
                existingData.active = existingData.defaultPath;
            }
            if (song.songUrl != "local" && existingData.defaultPath != song.path && ghc::filesystem::exists(song.path)) {
                ghc::filesystem::remove(song.path);
            }
            continue;
        }
        newSongs.push_back(savedSong);
    }
    NongData newData = {
        .active = existingData.active,
        .defaultPath = existingData.defaultPath,
        .songs = newSongs
    };
    NongManager::saveNongs(newData, songID);
}

void NongManager::createDefaultSongIfNull(SongInfo const& song, int songID) {
    if (!NongManager::checkIfNongsExist(songID)) {
        std::vector<SongInfo> songs = { song };
        NongData nongData = {
            .active = song.path,
            .defaultPath = song.path,
            .songs = songs
        };
        NongManager::saveNongs(nongData, songID);
    }
}

std::string NongManager::getFormattedSize(SongInfo const& song) {
    try {
        auto size = ghc::filesystem::file_size(song.path);
        double toMegabytes = size / 1024.f / 1024.f;
        std::stringstream ss;
        ss << std::setprecision(3) << toMegabytes << "MB";
        return ss.str();
    } catch (ghc::filesystem::filesystem_error) {
        return "0.00MB";
    }
}

ghc::filesystem::path NongManager::getJsonPath(int songID) {
    auto songIDstr = std::to_string(songID);
    return Mod::get()->getSaveDir().append("nong_data").append(songIDstr + ".json");
}

bool NongManager::checkIfNongsExist(int songID) {
    auto nongData = Mod::get()->getSaveDir().append("nong_data");
    if (!ghc::filesystem::exists(nongData)) {
        ghc::filesystem::create_directory(nongData);
    }
    auto path = NongManager::getJsonPath(songID);
    if (!ghc::filesystem::exists(path)) return false;

    auto data = NongManager::getNongs(songID);

    if (data.songs.size() == 1) return false;
    return true;
}

void NongManager::fetchSFH(int songID, std::function<void(bool)> callback) {
    std::string url = "https://songfilehub.com/api/v1/nongs?id=" + std::to_string(songID);
    web::AsyncWebRequest()
        .fetch(url)
        .json()
        .then([callback, songID](json::Value const& data) {
            std::vector<SFHItem> ret;
            if (!data.contains("songs") || !data["songs"].is_array()) {
                callback(false);
            }
            auto songs = data["songs"].as_array();
            for (auto const& song : songs) {
                SFHItem item = {
                    .songName = song["songName"].as_string(),
                    .downloadUrl = song["downloadUrl"].as_string(),
                    .levelName = song.contains("levelNameMobile") && song["levelNameMobile"].is_string() ? song["levelNameMobile"].as_string() : "",
                    .songURL = song.contains("songURL") && song["songURL"].is_string() ? song["songURL"].as_string() : ""
                };
                ret.push_back(item);
            }
            NongManager::addNongsFromSFH(ret, songID);
            callback(true);
        })
        .expect([callback](std::string const& error) {
            callback(false);
        });
}

void NongManager::addNongsFromSFH(std::vector<SFHItem> const& songs, int songID) {
    auto nongsPath = Mod::get()->getSaveDir().append("nongs");
    if (!ghc::filesystem::exists(nongsPath)) {
        ghc::filesystem::create_directory(nongsPath);
    }
    auto nongs = NongManager::getNongs(songID);
    int index = 1;
    for (auto sfhSong : songs) {
        bool shouldSkip = false;
        auto path = nongsPath;
        path.append(std::to_string(songID) + "_" + sfhSong.levelName + ".mp3");
        size_t index = 1;
        while (ghc::filesystem::exists(path)) {
            path = nongsPath;
            path.append(std::to_string(songID) + "_" + sfhSong.levelName + "_" + std::to_string(index) + ".mp3");
            index++;
        }
        for (auto& localSong : nongs.songs) {
            if (localSong.songUrl == sfhSong.downloadUrl) {
                shouldSkip = true;
                break;
            }
        }
        if (shouldSkip) {
            continue;
        }
        std::string songName;
        std::string artistName;
        std::stringstream ss;
        ss << sfhSong.songName;
        std::string part;
        size_t i = 0;
        while (std::getline(ss, part, '-')) {
            if (i == 0) {
                artistName = part;
                NongManager::rtrim(artistName);
            } else {
                songName = part;
                NongManager::ltrim(songName);
            }
            i++;
        }

        SongInfo song = {
            .path = path,
            .songName = songName,
            .authorName = artistName,
            .songUrl = sfhSong.downloadUrl
        };

        if (!web::fetchFile(song.songUrl, song.path)) {
            // failed to download file, skip the songo
            continue;
        }

        if (sfhSong.levelName != "") {
            song.songName += " (" + sfhSong.levelName + ")";
        }
        nongs.songs.push_back(song);
    }

    NongManager::saveNongs(nongs, songID);
}

void NongManager::ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

void NongManager::rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void NongManager::trim(std::string &s) {
    NongManager::rtrim(s);
    NongManager::ltrim(s);
}