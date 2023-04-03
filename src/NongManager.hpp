#pragma once

#include <vector>
#include "types/SongInfo.hpp"
#include <Geode/Geode.hpp>
#include <sstream>
#include <fstream>

using namespace geode::prelude;

class NongManager {
private:
    /**
     * Creates JSON for the songID if it doesn't exist
     * 
     * @return true if json was created, false if it already existed
    */
    static bool createJsonIfNull(int songID);
    static ghc::filesystem::path getJsonPath(int songID);
public:
    static void addNong(SongInfo const& song, int songID);
    static void deleteNong(SongInfo const& song, int songID);
    static NongData getNongs(int songID);
    static SongInfo getActiveNong(int songID);
    static std::vector<SongInfo> validateNongs(int songID);
    static void saveNongs(NongData const& data, int songID);
    static std::string getFormattedSize(SongInfo const& song);
    static void createDefaultSongIfNull(SongInfo const& song, int songID);
};