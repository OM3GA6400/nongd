#pragma once

#include "NongPopup.hpp"
#include <Geode/Geode.hpp>
#include "../types/SongInfo.hpp"
#include "../random_string.hpp"

using namespace geode::prelude;

class NongPopup;

class NongAddPopup : public Popup<NongPopup*> {
protected:
    NongPopup* m_parentPopup;
    CCMenuItemSpriteExtra* m_selectSongButton;
    CCMenuItemSpriteExtra* m_addSongButton;
    CCMenu* m_selectSongMenu;
    CCMenu* m_addSongMenu;
    CCLabelBMFont* m_selectedSongPath;
    CCLayer* m_containerLayer;

    CCTextInputNode* m_songNameInput;
    CCTextInputNode* m_artistNameInput;

    ghc::filesystem::path m_songPath;

    bool setup(NongPopup* parent) override;
    void createSelectedSongLabel(const std::string&);
    void createInputs();

    CCSize getPopupSize();
    void openFile(CCObject*);
    void addSong(CCObject*);
public:
    static NongAddPopup* create(NongPopup* parent);
};
