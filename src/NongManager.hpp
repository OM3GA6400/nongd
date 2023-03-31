#pragma once

#include <vector>
#include "types/SongInfo.hpp"
#include <Geode/Geode.hpp>
#include <sstream>

USE_GEODE_NAMESPACE();

class NongManager {
public:
    static void addNong(SongInfo song, int songID);
    static void deleteNong(SongInfo song, int songID);
    static std::vector<SongInfo> getNongs(int songID);
    static void saveNongs(const std::vector<SongInfo>& songs, int songID);
    static std::string getFormattedSize(SongInfo song);
};