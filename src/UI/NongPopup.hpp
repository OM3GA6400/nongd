#pragma once

#include <Geode/Geode.hpp>
#include "../types/SongInfo.hpp"
#include "NongCell.hpp"
#include "../NongManager.hpp"
#include "NongAddPopup.hpp"
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

using namespace geode::prelude;

class NongPopup : public Popup<int> {
protected:
    CCMenu* m_addButtonMenu;
    CCLayer* m_listLayer;
    ListView* m_list;
    int m_songID;

    NongData m_songs;

    bool setup(int songID) override;
    CCSize getPopupSize() const;

    void setSongs();
    SongInfo getActiveSong();
    void saveSongsToJson();

    void createList();
    CCArray* createNongCells();
    CCSize getCellSize() const;
    CCSize getListSize() const;

    void createAddButton();

    void openAddPopup(CCObject*);
public:
    static NongPopup* create(int songID);
    void setActiveSong(SongInfo const& song);
    void addSong(SongInfo const& song);
    void deleteSong(SongInfo const& song);
};