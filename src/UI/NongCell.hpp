#pragma once

#include <Geode/Geode.hpp>
#include "../types/SongInfo.hpp"
#include "NongPopup.hpp"

USE_GEODE_NAMESPACE();

class NongPopup;

class NongListCell : public CCLayer {
protected:
    float m_width;
    float m_height;
    CCLayer* m_layer;

    bool init(CCLayer* layer, CCSize const& size);
    void draw() override;
};

class NongCell : public NongListCell {
protected:
    SongInfo m_songInfo;
    CCLabelBMFont* m_songNameLabel;
    CCLabelBMFont* m_authorNameLabel;
    CCLayer* m_songInfoLayer;

    NongPopup* m_parentPopup;

    bool init(SongInfo info, NongPopup* parentPopup, CCSize const& size);
public:
    static NongCell* create(SongInfo info, NongPopup* parentPopup, CCSize const& size);
    void onSet(CCObject*);
};