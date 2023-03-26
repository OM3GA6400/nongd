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

NongCell* NongCell::create(SongInfo info, CCLayer* layer, CCSize const& size) {
    auto ret = new NongCell();
    if (ret && ret->init(info, layer, size)) {
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool NongCell::init(SongInfo info, CCLayer* layer, CCSize const& size) {
    if (!NongListCell::init(layer, size)) return false;
    this->m_songInfo = info;

    // auto button = CCMenuItemSpriteExtra::create(
    //     ButtonSprite::create("test"),
    //     this,
    //     nullptr
    // );
    this->m_songInfoLayer = CCLayer::create();

    this->m_songNameLabel = CCLabelBMFont::create(m_songInfo.songName.c_str(), "bigFont.fnt");
    this->m_songNameLabel->setAnchorPoint({0, 0});
    this->m_songNameLabel->setPosition(ccp(9.f, 31.f));
    this->m_songNameLabel->setScale(0.8f);

    this->m_authorNameLabel = CCLabelBMFont::create(m_songInfo.authorName.c_str(), "goldFont.fnt");
    this->m_authorNameLabel->setAnchorPoint({0, 0});
    this->m_authorNameLabel->setPosition(ccp(9.f, 9.f));
    this->m_authorNameLabel->setScale(0.7f);

    this->m_songInfoLayer->addChild(this->m_authorNameLabel);
    this->m_songInfoLayer->addChild(this->m_songNameLabel);

    this->addChild(m_songInfoLayer);
    // this->addChild(button);

    return true;
}