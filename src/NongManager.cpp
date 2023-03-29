#include "NongManager.hpp"

std::vector<SongInfo> NongManager::getNongs(int songID) {
    std::vector<SongInfo> songs;

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
    auto path = Mod::get()->getSaveDir().append("nong_data");
    if (!ghc::filesystem::exists(path)) {
        ghc::filesystem::create_directory(path);
        songs.push_back(defaultSong);
        NongManager::saveNongs(songs, songID);
        return songs;
    }
    path.append(songIDstr + ".json");
    if (!ghc::filesystem::exists(path)) {
        songs.push_back(defaultSong);
        NongManager::saveNongs(songs, songID);
        return songs;
    }

    std::ifstream input(path.string());
    std::stringstream buffer;
    buffer << input.rdbuf();
    input.close();
    auto json = json::parse(std::string_view(buffer.str())).as_array();
    return json::Serialize<std::vector<SongInfo>>::from_json(json);
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