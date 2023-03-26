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

    auto button = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("test"),
        this,
        nullptr
    );
    this->addChild(button);

    return true;
}