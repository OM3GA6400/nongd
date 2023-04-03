#pragma once

#include <vector>
#include "types/SongInfo.hpp"
#include <Geode/Geode.hpp>
#include <sstream>
#include <fstream>

using namespace geode::prelude;

class NongManager {
private:
    static ghc::filesystem::path getJsonPath(int songID);
public:
    static void addNong(SongInfo const& song, int songID);
    static void deleteNong(SongInfo const& song, int songID);
    static bool checkIfNongsExist(int songID);
    static NongData getNongs(int songID);
    static SongInfo getActiveNong(int songID);
    static std::vector<SongInfo> validateNongs(int songID);
    static void saveNongs(NongData const& data, int songID);
    static std::string getFormattedSize(SongInfo const& song);
    static void createDefaultSongIfNull(SongInfo const& song, int songID);
};