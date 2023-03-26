#pragma once

#include <Geode/Geode.hpp>

USE_GEODE_NAMESPACE();

struct SongInfo : public CCObject {
public:
    ghc::filesystem::path getSongPath() { return m_path; }
    std::string getSongName() { return m_songName; }
    std::string getSongAuthor() { return m_authorName; }

    SongInfo(ghc::filesystem::path path, std::string songName, std::string authorName) {
        m_path = path;
        m_songName = songName;
        m_authorName = authorName;
    }
    SongInfo() {}
private:
    ghc::filesystem::path m_path;
    std::string m_songName;
    std::string m_authorName;
};