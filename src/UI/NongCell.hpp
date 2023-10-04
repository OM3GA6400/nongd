#pragma once

#include <Geode/Geode.hpp>
#include "../types/SongInfo.hpp"
#include "ListCell.hpp"
#include "NongPopup.hpp"

using namespace geode::prelude;

class NongPopup;

class NongCell : public ListCell {
protected:
    SongInfo m_songInfo;
    CCLabelBMFont* m_songNameLabel;
    CCLabelBMFont* m_authorNameLabel;
    CCLayer* m_songInfoLayer;

    NongPopup* m_parentPopup;

    bool init(SongInfo info, NongPopup* parentPopup, CCSize const& size, bool selected, bool isDefault);

    virtual void FLAlert_Clicked(FLAlertLayer*, bool btn2);
public:
    static NongCell* create(SongInfo info, NongPopup* parentPopup, CCSize const& size, bool selected, bool isDefault);
    void onSet(CCObject*);
    void deleteSong(CCObject*);
};