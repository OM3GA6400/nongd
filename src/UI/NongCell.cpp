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

bool NongCell::init(SongInfo info, NongPopup* parentPopup, CCSize const& size) {
    if (!NongListCell::init(parentPopup, size)) return false;

    bool selected = info.selected;

    this->m_songInfo = info;
    this->m_parentPopup = parentPopup;

    CCMenuItemSpriteExtra* button;

    if (selected) {
        auto sprite = ButtonSprite::create("Set", "goldFont.fnt", "GJ_button_02.png");
        // sprite->setColor(ccc3(0, 204, 255));
        button = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            nullptr
        );
    } else {
        button = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Set"),
            this,
            menu_selector(NongCell::onSet)
        );
    }
    button->setAnchorPoint(ccp(0.5f, 0.5f));
    auto menu = CCMenu::create();
    menu->addChild(button);
    menu->setAnchorPoint(ccp(0, 0));
    menu->setPosition(ccp(337.f, 29.f));
    this->addChild(menu);

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

void NongCell::onSet(CCObject* target) {
    this->m_parentPopup->setActiveSong(this->m_songInfo);
}

NongCell* NongCell::create(SongInfo info, NongPopup* parentPopup, CCSize const& size) {
    auto ret = new NongCell();
    if (ret && ret->init(info, parentPopup, size)) {
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}