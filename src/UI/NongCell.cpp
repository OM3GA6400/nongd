#include "NongCell.hpp"

bool NongListCell::init(CCLayer* layer, CCSize const& size) {
    m_width = size.width;
    m_height = size.height;
    m_layer = layer;
    this->setContentSize(size);
    this->setID("nong-list-cell");
    return true;
}

void NongListCell::draw() {
    reinterpret_cast<StatsCell*>(this)->StatsCell::draw();
}

// NongCell

void NongCell::onSet() {
    this->m_parentPopup->setActiveSong(this->m_songInfo);
}

bool NongCell::init(SongInfo info, NongPopup* parentPopup, CCSize const& size, bool selected) {
    if (!NongListCell::init(parentPopup, size)) return false;
    this->m_songInfo = info;
    this->m_parentPopup = parentPopup;

    if (!selected) {
        auto button = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Set"),
            this,
            nullptr
        );
        button->setAnchorPoint(ccp(0.5f, 0.5f));
        auto menu = CCMenu::create();
        menu->addChild(button);
        menu->setAnchorPoint(ccp(0, 0));
        menu->setPosition(ccp(337.f, 29.f));
        this->addChild(menu);
    }

    this->m_songInfoLayer = CCLayer::create();

    this->m_songNameLabel = CCLabelBMFont::create(m_songInfo.songName.c_str(), "bigFont.fnt");
    this->m_songNameLabel->setAnchorPoint({0, 0});
    this->m_songNameLabel->setPosition(ccp(14.f, 28.f));
    this->m_songNameLabel->setScale(0.8f);

    if (selected) {
        this->m_songNameLabel->setColor(ccc3(0, 255, 0));
    }

    this->m_authorNameLabel = CCLabelBMFont::create(m_songInfo.authorName.c_str(), "goldFont.fnt");
    this->m_authorNameLabel->setAnchorPoint({0, 0});
    this->m_authorNameLabel->setPosition(ccp(14.f, 9.f));
    this->m_authorNameLabel->setScale(0.7f);

    this->m_songInfoLayer->addChild(this->m_authorNameLabel);
    this->m_songInfoLayer->addChild(this->m_songNameLabel);

    this->addChild(m_songInfoLayer);
    return true;
}

NongCell* NongCell::create(SongInfo info, NongPopup* parentPopup, CCSize const& size, bool selected) {
    auto ret = new NongCell();
    if (ret && ret->init(info, parentPopup, size, selected)) {
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}