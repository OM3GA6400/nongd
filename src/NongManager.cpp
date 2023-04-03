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
        if (!ghc::filesystem::exists(song.path) && song.path != currentData.defaultPath) {
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
    auto path = NongManager::getJsonPath(songID);
    if (!ghc::filesystem::exists(path)) return false;

    auto data = NongManager::getNongs(songID);

    if (data.songs.size() == 1) return false;
    return true;
}