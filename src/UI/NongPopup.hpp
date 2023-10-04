#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include "../types/SongInfo.hpp"
#include "../types/FetchStatus.hpp"
#include "../managers/NongManager.hpp"
#include "NongCell.hpp"
#include "NongAddPopup.hpp"
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace geode::prelude;

class NongPopup : public Popup<int, CustomSongWidget*> {
protected:
    int m_songID;
    Ref<CustomSongWidget> m_parentWidget;
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
    void createSettingsButton();
    void createRemoveAllButton();
    void createFetchSongHubMenu();

    void openAddPopup(CCObject*);
    void fetchSongHub(CCObject*);
    void deleteAllNongs(CCObject*);

    void updateParentSizeAndIDLabel(SongInfo const& song, int songID = 0);
    void updateParentWidget(SongInfo const& song);

    void onSFHFetched(nongd::FetchStatus result);
    void onSettings(CCObject*);
    void onClose(CCObject* object) {
        this->m_parentWidget->release();
        Popup::onClose(object);
    }
public:
    int getSongID();
    static NongPopup* create(int songID, CustomSongWidget* parent);
    void setActiveSong(SongInfo const& song);
    void addSong(SongInfo const& song);
    void deleteSong(SongInfo const& song);
};