#include "NongManager.hpp"

std::vector<SongInfo> NongManager::getNongs(int songID) {
    if (NongManager::createJsonIfNull(songID)) {
        std::vector<SongInfo> songs;
        SongInfo defaultSong = NongManager::createDefaultSong(songID);
        songs.push_back(defaultSong);
        return songs;
    }

    auto songIDstr = std::to_string(songID);

    auto path = Mod::get()->getSaveDir().append("nong_data").append(songIDstr + ".json");
    std::vector<SongInfo> test;
    test.push_back(NongManager::createDefaultSong(songID));

    std::ifstream input(path.string());
    std::stringstream buffer;
    buffer << input.rdbuf();
    input.close();

    auto json = json::parse(std::string_view(buffer.str())).as_array();
    return json::Serialize<std::vector<SongInfo>>::from_json(json);
}

std::vector<SongInfo> NongManager::validateNongs(int songID) {
    auto songsToValidate = NongManager::getNongs(songID);
    // Validate nong paths and delete those that don't exist anymore
    std::vector<SongInfo> invalidSongs;
    std::vector<SongInfo> validSongs;

    size_t activeSongs = 0;

    for (auto &song : songsToValidate) {
        if (song.selected) activeSongs++;
        if (!ghc::filesystem::exists(song.path) && song.songName != "Default") {
            invalidSongs.push_back(song);
            if (song.selected) {
                activeSongs--;
            }
        } else {
            validSongs.push_back(song);
        }
    }

    // if something goes oopsie and i have multiple active songs, just activate the default
    if (activeSongs == 0 || activeSongs > 1) {
        for (auto &song : validSongs) {
            if (song.songName == "Default") {
                song.selected = true;
            } else {
                song.selected = false;
            }
        }
    }

    if (invalidSongs.size() > 0 || activeSongs == 0 || activeSongs > 1) {
        NongManager::saveNongs(validSongs, songID);
    }

    return invalidSongs;
}

bool NongManager::createJsonIfNull(int songID) {
    std::vector<SongInfo> songs;
    auto songIDstr = std::to_string(songID);
    SongInfo defaultSong = NongManager::createDefaultSong(songID);

    auto path = Mod::get()->getSaveDir().append("nong_data");
    if (!ghc::filesystem::exists(path)) {
        ghc::filesystem::create_directory(path);
        songs.push_back(defaultSong);
        NongManager::saveNongs(songs, songID);
        return true;
    }
    path.append(songIDstr + ".json");
    if (!ghc::filesystem::exists(path)) {
        songs.push_back(defaultSong);
        NongManager::saveNongs(songs, songID);
        return true;
    }

    return false;
}

SongInfo NongManager::createDefaultSong(int songID) {
    auto songIDstr = std::to_string(songID);
    char* userfolder = getenv("USERPROFILE");
    ghc::filesystem::path gdPath = std::string(userfolder);
    gdPath.append("AppData").append("Local").append("GeometryDash").append(songIDstr + ".mp3");
    SongInfo defaultSong = {
        gdPath,
        std::string("Default"),
        std::string("The default song!"),
        true
    };
    return defaultSong;
}

void NongManager::saveNongs(const std::vector<SongInfo>& songs, int songID) {
    auto songIDstr = std::to_string(songID);
    auto path = Mod::get()->getSaveDir().append("nong_data");
    if (!ghc::filesystem::exists(path)) {
        ghc::filesystem::create_directory(path);
    }
    path.append(songIDstr + ".json");
    auto json = json::Serialize<std::vector<SongInfo>>::to_json(songs);
    std::ofstream output(path.string());
    output << json.dump();
    output.close();
}

void NongManager::addNong(SongInfo song, int songID) {
    auto existingSongs = NongManager::getNongs(songID);
    for (auto savedSong : existingSongs) {
        if (song.path.string() == savedSong.path.string()) {
            // if song exists, just return
            return;
        }
    }
    existingSongs.push_back(song);
    NongManager::saveNongs(existingSongs, songID);
}

void NongManager::deleteNong(SongInfo song, int songID) {
    std::vector<SongInfo> newSongs;
    auto existingSongs = NongManager::getNongs(songID);
    for (auto savedSong : existingSongs) {
        if (savedSong.path.string() == song.path.string()) {
            continue;
        }
        newSongs.push_back(savedSong);
    }
    NongManager::saveNongs(newSongs, songID);
}

std::string NongManager::getFormattedSize(SongInfo song) {
    try {
        log::info("path: {}", song.path.string());
        auto size = ghc::filesystem::file_size(song.path);
        double toMegabytes = size / 1024.f / 1024.f;
        std::stringstream ss;
        ss << std::setprecision(3) << toMegabytes << "MB";
        return ss.str();
    } catch (ghc::filesystem::filesystem_error) {
        return "0.00MB";
    }
}