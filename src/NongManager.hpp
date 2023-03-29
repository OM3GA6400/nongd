#pragma once

#include <vector>
#include "types/SongInfo.hpp"
#include <Geode/Geode.hpp>

USE_GEODE_NAMESPACE();

class NongManager {
public:
    static std::vector<SongInfo> getNongs(int songID);
    static void saveNongs(const std::vector<SongInfo>& songs, int songID);
};