#pragma once

#include <Geode/Geode.hpp>
#include "../types/SongInfo.hpp"
#include "NongCell.hpp"
#include "../NongManager.hpp"
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

USE_GEODE_NAMESPACE();

class NongPopup : public Popup<int> {
protected:
    CCMenu* m_addButtonMenu;
    CCLayer* m_listLayer;
    ListView* m_list;
    int m_songID;

    std::vector<SongInfo> m_songs;

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
public:
    static NongPopup* create(int songID);
    void setActiveSong(SongInfo song);
};