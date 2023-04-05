#pragma once

#include <vector>
#include "types/SongInfo.hpp"
#include "types/SFHItem.hpp"
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>

using namespace geode::prelude;

namespace nong {
    /**
     * Adds a NONG to the JSON of a songID
     * 
     * @param song the song to add
     * @param songID the id of the song
    */
    void addNong(SongInfo const& song, int songID);

    /**
     * Removes a NONG from the JSON of a songID
     * 
     * @param song the song to remove
     * @param songID the id of the song
    */
    void deleteNong(SongInfo const& song, int songID);

    /**
     * Checks if any NONGS exist for a songID
     * 
     * @param songID the id of the song
     * @return true if the json exists, false otherwise
    */
    bool checkIfNongsExist(int songID);

    /**
     * Fetches all NONG data from the songID JSON
     * 
     * @param songID the id of the song
     * @return the data from the JSON
    */
    NongData getNongs(int songID);

    /**
     * Fetches the active song from the songID JSON
     * 
     * @param songID the id of the song
     * @return the song data
     * 
     * @throw std::exception if no nong is set as active
    */
    SongInfo getActiveNong(int songID);

    /**
     * Validates any local nongs that have an invalid path
     * 
     * @param songID the id of the song
     * 
     * @return an array of songs that were deleted as result of the validation
    */
    std::vector<SongInfo> validateNongs(int songID);

    /**
     * Saves NONGS to the songID JSON
     * 
     * @param data the data to save
     * @param songID the id of the song
    */
    void saveNongs(NongData const& data, int songID);

    /**
     * Formats a size in bytes to a x.xxMB string
     * 
     * @param song the song
     * 
     * @return the formatted size, with the format x.xxMB
    */
    std::string getFormattedSize(SongInfo const& song);

    /**
     * Creates the JSON file for a songID and adds the default song to it
     * 
     * @param song the default song, fetched from GD
     * @param songID the id of the song
    */
    void createDefaultSongIfNull(SongInfo const& song, int songID);

    /**
     * Fetches song data from Song File Hub for a songID
     * 
     * @param songID the id of the song
     * @param callback a callback that takes a boolean as an argument, which is the status of the request
    */
    void fetchSFH(int songID, std::function<void(bool)> callback);

    /**
     * Returns the JSON path for a songID
     * 
     * @param songID the id of the song
     * 
     * @return the path of the JSON
    */
    ghc::filesystem::path getJsonPath(int songID);
}