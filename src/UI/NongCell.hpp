#pragma once

#include <Geode/Geode.hpp>
#include "../types/SongInfo.hpp"
#include "NongLayer.hpp"

USE_GEODE_NAMESPACE();

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

    bool init(SongInfo info, CCLayer* layer, CCSize const& size);
public:
    static NongCell* create(SongInfo info, CCLayer* layer, CCSize const& size);
};