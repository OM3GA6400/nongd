#pragma once

#include <Geode/Geode.hpp>

USE_GEODE_NAMESPACE();

struct SongInfo {
    ghc::filesystem::path path;
    std::string songName;
    std::string authorName;
};

template<>
struct json::Serialize<SongInfo> {
    static SongInfo from_json(json::Value const& value) {
        return SongInfo {
            .path = ghc::filesystem::path(value["path"].as_string()),
            .songName = value["songName"].as_string(),
            .authorName = value["authorName"].as_string()
        };
    }

    static json::Value to_json(SongInfo const& value) {
        auto obj = json::Object();
        obj["path"] = value.path.string();
        obj["songName"] = value.songName;
        obj["authorName"] = value.authorName;
        return obj;
    }
};
