#include "nong.hpp"

void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void trim(std::string &s) {
    rtrim(s);
    ltrim(s);
}

void addNongsFromSFH(std::vector<SFHItem> const& songs, int songID) {
    auto nongsPath = Mod::get()->getSaveDir().append("nongs");
    if (!ghc::filesystem::exists(nongsPath)) {
        ghc::filesystem::create_directory(nongsPath);
    }
    auto nongs = nong::getNongs(songID);
    int index = 1;
    for (auto const& sfhSong : songs) {
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

        auto sfhSongName = sfhSong.songName;
        trim(sfhSongName);

        if (sfhSongName.find_first_of("-") == std::string::npos) {
            // probably dash doesn't exist because of unicode filtering, try to insert it by hand
            for (size_t i = 0; i < sfhSongName.size() - 1; i++) {
                if (sfhSongName.at(i) == ' ' && sfhSongName.at(i + 1) == ' ') {
                    sfhSongName.insert(i + 1, "-");
                }
            }
        }

        std::string songName = "-";
        std::string artistName = "-";
        std::stringstream ss;
        ss << sfhSongName;
        std::string part;
        size_t i = 0;
        while (std::getline(ss, part, '-')) {
            if (i == 0) {
                artistName = part;
                rtrim(artistName);
            } else {
                songName = part;
                ltrim(songName);
            }
            i++;
        }

        if (songName == "-") {
            auto temp = songName;
            songName = artistName;
            artistName = temp;
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

    nong::saveNongs(nongs, songID);
}

namespace nong {
    NongData getNongs(int songID) {
        auto path = getJsonPath(songID);

        std::ifstream input(path.string());
        std::stringstream buffer;
        buffer << input.rdbuf();
        input.close();

        auto json = json::parse(std::string_view(buffer.str()));
        return json::Serialize<NongData>::from_json(json);
    }

    SongInfo getActiveNong(int songID) {
        auto nongs = getNongs(songID);

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
    std::vector<SongInfo> validateNongs(int songID) {
        auto currentData = getNongs(songID);
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

            saveNongs(newData, songID);
        }

        return invalidSongs;
    }

    void saveNongs(NongData const& data, int songID) {
        auto path = getJsonPath(songID);
        auto json = json::Serialize<NongData>::to_json(data);
        std::ofstream output(path.string());
        output << json.dump();
        output.close();
    }

    void addNong(SongInfo const& song, int songID) {
        auto existingData = getNongs(songID);
        for (auto savedSong : existingData.songs) {
            if (song.path.string() == savedSong.path.string()) {
                // if song exists, just return
                return;
            }
        }
        existingData.songs.push_back(song);
        saveNongs(existingData, songID);
    }

    void deleteNong(SongInfo const& song, int songID) {
        std::vector<SongInfo> newSongs;
        auto existingData = getNongs(songID);
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
        saveNongs(newData, songID);
    }

    void createDefaultSongIfNull(SongInfo const& song, int songID) {
        if (!checkIfNongsExist(songID)) {
            std::vector<SongInfo> songs = { song };
            NongData nongData = {
                .active = song.path,
                .defaultPath = song.path,
                .songs = songs
            };
            saveNongs(nongData, songID);
        }
    }

    std::string getFormattedSize(SongInfo const& song) {
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

    ghc::filesystem::path getJsonPath(int songID) {
        auto songIDstr = std::to_string(songID);
        return Mod::get()->getSaveDir().append("nong_data").append(songIDstr + ".json");
    }

    bool checkIfNongsExist(int songID) {
        auto nongData = Mod::get()->getSaveDir().append("nong_data");
        if (!ghc::filesystem::exists(nongData)) {
            ghc::filesystem::create_directory(nongData);
        }
        auto path = getJsonPath(songID);
        if (!ghc::filesystem::exists(path)) return false;

        auto data = getNongs(songID);

        if (data.songs.size() == 1) return false;
        return true;
    }

    void fetchSFH(int songID, std::function<void(bool)> callback) {
        std::string url = "https://songfilehub.com/api/v1/nongs?id=" + std::to_string(songID);
        web::AsyncWebRequest()
            .fetch(url)
            .text()
            .then([callback, songID](std::string const& text) {
                json::Value data;
                try {
                    data = json::parse(text);
                } catch (json::JsonException e) {
                    log::error("Failed to parse JSON from SFH, trying to remove potential Unicode characters. String: {}", text);
                    auto copy = text;
                    copy.erase(std::remove_if(copy.begin(), copy.end(), [](char x) {
                        return static_cast<int>(x) < 0;
                    }), copy.end());
                    try {
                        data = json::parse(copy);
                    } catch (json::JsonException e) {
                        log::error("Failed to parse JSON even after Unicode filtering. String: {}", copy);
                        callback(false);
                        return;
                    }
                }
                std::vector<SFHItem> ret;
                if (!data.contains("songs") || !data["songs"].is_array()) {
                    callback(false);
                    return;
                }
                auto songs = data["songs"].as_array();
                for (auto const& song : songs) {
                    SFHItem item = {
                        .songName = song["songName"].as_string(),
                        .downloadUrl = song["downloadUrl"].as_string(),
                        .levelName = song.contains("levelNameMobile") ? song["levelNameMobile"].as_string() : "",
                        .songURL = song.contains("songURL") ? song["songURL"].as_string() : ""
                    };
                    ret.push_back(item);
                }
                addNongsFromSFH(ret, songID);
                callback(true);
            })
            .expect([callback](std::string const& error) {
                callback(false);
            })
            .cancelled([callback](auto r) {
                callback(false);
            });
    }
}