#pragma once

#include <vector>
#include "types/SongInfo.hpp"
#include <Geode/Geode.hpp>
#include <sstream>

USE_GEODE_NAMESPACE();

class NongManager {
private:
    /**
     * Creates JSON for the songID if it doesn't exist
     * 
     * @return true if json was created, false if it already existed
    */
    static bool createJsonIfNull(int songID);
public:
    static void addNong(SongInfo song, int songID);
    static void deleteNong(SongInfo song, int songID);
    static std::vector<SongInfo> getNongs(int songID);
    static std::vector<SongInfo> validateNongs(int songID);
    static void saveNongs(const std::vector<SongInfo>& songs, int songID);
    static std::string getFormattedSize(SongInfo song);
    static SongInfo createDefaultSong(int songID);
};