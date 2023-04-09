#pragma once

#include <Geode/Geode.hpp>
#include "../types/SongInfo.hpp"
#include "NongCell.hpp"
#include "../nong.hpp"
#include "NongAddPopup.hpp"
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

using namespace geode::prelude;

class NongPopup : public Popup<int, CustomSongWidget*> {
protected:
    int m_songID;
    CustomSongWidget* m_parentWidget;
    CCMenu* m_addButtonMenu;
    CCMenu* m_fetchSongHubMenu;
    CCLayer* m_listLayer;
    ListView* m_list;

    NongData m_songs;

    bool setup(int songID, CustomSongWidget* parent) override;
    CCSize getPopupSize() const;

    void setSongs();
    SongInfo getActiveSong();
    void saveSongsToJson();

    void createList();
    CCArray* createNongCells();
    CCSize getCellSize() const;
    CCSize getListSize() const;

    void createAddButton();
    void createFetchSongHubMenu();

    void openAddPopup(CCObject*);
    void fetchSongHub(CCObject*);

    void updateParentSizeAndIDLabel(SongInfo const& song, int songID = 0);
    void updateParentWidget(SongInfo const& song);

    void onSFHFetched(bool status);
public:
    int getSongID();
    static NongPopup* create(int songID, CustomSongWidget* parent);
    void setActiveSong(SongInfo const& song);
    void addSong(SongInfo const& song);
    void deleteSong(SongInfo const& song);
};