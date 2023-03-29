#pragma once

#include <Geode/Geode.hpp>

USE_GEODE_NAMESPACE();

struct SongInfo {
    ghc::filesystem::path path;
    std::string songName;
    std::string authorName;
    bool selected = false;
};

template<>
struct json::Serialize<SongInfo> {
    static SongInfo from_json(json::Value const& value) {
        return SongInfo {
            .path = ghc::filesystem::path(value["path"].as_string()),
            .songName = value["songName"].as_string(),
            .authorName = value["authorName"].as_string(),
            .selected = value["selected"].as_bool()
        };
    }

    static json::Value to_json(SongInfo const& value) {
        auto obj = json::Object();
        obj["path"] = value.path.string();
        obj["songName"] = value.songName;
        obj["authorName"] = value.authorName;
        obj["selected"] = value.selected;
        return obj;
    }
};

template<>
struct json::Serialize<std::vector<SongInfo>> {
    static std::vector<SongInfo> from_json(json::Array const& values) {
        std::vector<SongInfo> ret;
        for (auto jsonValue : values) {
            SongInfo song {
                .path = ghc::filesystem::path(jsonValue["path"].as_string()),
                .songName = jsonValue["songName"].as_string(),
                .authorName = jsonValue["authorName"].as_string(),
                .selected = jsonValue["selected"].as_bool()
            };

            ret.push_back(song);
        }
        return ret;
    }

    static json::Value to_json(std::vector<SongInfo> const& values) {
        auto array = json::Array();
        for (auto song : values) {
            auto obj = json::Object();
            obj["path"] = song.path.string();
            obj["songName"] = song.songName;
            obj["authorName"] = song.authorName;
            obj["selected"] = song.selected;
            array.push_back(obj);
        }
        
        return array;
    }
};