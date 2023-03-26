#pragma once

#include <Geode/Geode.hpp>
#include <string>
#include <vector>

#include "../types/SongInfo.hpp"
#include "NongCell.hpp"

USE_GEODE_NAMESPACE();

class NongLayer : public CCLayer
{
protected:
    int m_songID;
    ghc::filesystem::path m_nongPath;

    GJListLayer* m_list = nullptr;
    CCMenu* m_backMenu = nullptr;

    CCMenuItemSpriteExtra* m_backButton = nullptr;

    virtual bool init(int songID);
    virtual void keyBackClicked();

    void onBackButton(CCObject*);
    void createBackground();
    void createBackButtonMenu();
    void createPath();
    void createList();
    CCArray* createNongCells();
    std::vector<SongInfo> getNongs() const;
    CCSize getCellSize() const;
    CCSize getListSize() const;

    bool doNongsExist();
public:
    static NongLayer* create(int songID);
    void onNongLayer(CCObject*);
};