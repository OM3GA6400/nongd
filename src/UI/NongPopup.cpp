#include "NongPopup.hpp"

bool NongPopup::setup(int songID) {
    this->m_songID = songID;
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    // convenience function provided by Popup 
    // for adding/setting a title to the popup
    auto title = "NONGs for " + std::to_string(songID);
    this->setTitle(title);

    this->setSongs();
    this->createList();
    this->createAddButton();
    return true;
}

void NongPopup::createAddButton() {
    this->m_addButtonMenu = CCMenu::create();
    auto addButton = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png"),
        this,
        nullptr
    );
    
    this->m_addButtonMenu->addChild(addButton);
    this->m_addButtonMenu->setPosition(ccp(524.5f, 29.f));
    this->addChild(m_addButtonMenu);
}

NongPopup* NongPopup::create(int songID) {
    auto ret = new NongPopup();
    auto size = ret->getPopupSize();
    if (ret && ret->init(size.width, size.height, songID)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

CCSize NongPopup::getPopupSize() const {
    return { 500.f, 280.f };
}

void NongPopup::setSongs() {
    this->m_songs.clear();
    for (int i = 0; i < 10; i++) {
        SongInfo songInfo {
            ghc::filesystem::path("C:\\Users"),
            std::string("Magic Touch"),
            std::string("Romos")
        };
        m_songs.push_back(songInfo);
    }
}

CCArray* NongPopup::createNongCells() {
    auto songs = CCArray::create();
    if (this->m_activeSong.path.string() == "") {
        this->setActiveSongFromSave();
    }

    this->m_songs.push_back(this->m_activeSong);

    int i = 0;
    for (auto song : m_songs) {
        if (this->m_activeSong.path.string() == song.path.string()) {
            songs->addObject(NongCell::create(song, this, this->getCellSize(), true));
        } else {
            songs->addObject(NongCell::create(song, this, this->getCellSize()));
        }
        i++;
    }

    return songs;
}

void NongPopup::setActiveSongFromSave() {
    auto songIDstr = std::to_string(this->m_songID);
    char* userfolder = getenv("USERPROFILE");
    ghc::filesystem::path gdPath = std::string(userfolder);
    gdPath.append("AppData").append("Local").append("GeometryDash").append(songIDstr + ".mp3");
    SongInfo defaultSong = {
        gdPath,
        std::string("Default"),
        std::string("The default song!")
    };
    this->m_activeSong = Mod::get()->getSavedValue<SongInfo>(songIDstr, defaultSong);
}

CCSize NongPopup::getCellSize() const {
    return {
        this->getListSize().width,
        60.f
    };
}

CCSize NongPopup::getListSize() const {
    return { 400.f, 190.f };
}

void NongPopup::createList() {
    auto cells = this->createNongCells();
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    this->m_listLayer = CCLayer::create();
    this->m_list = ListView::create(
        cells,
        this->getCellSize().height,
        this->getListSize().width,
        this->getListSize().height
    );

    this->m_list->setPositionY(-10.f);

    auto sideLeft = CCSprite::createWithSpriteFrameName("GJ_commentSide_001.png");
    sideLeft->setAnchorPoint(ccp(0, 0));
    sideLeft->setScaleY(5.45f);
    sideLeft->setScaleX(1.2f);
    sideLeft->setPositionX(-3.f);
    sideLeft->setZOrder(9);

    auto sideTop = CCSprite::createWithSpriteFrameName("GJ_commentTop_001.png");
    sideTop->setAnchorPoint(ccp(0, 0));
    sideTop->setScaleX(1.15f);
    sideTop->setPosition(ccp(-3.f, 163.f));
    sideTop->setZOrder(9);

    auto sideBottom = CCSprite::createWithSpriteFrameName("GJ_commentTop_001.png");
    sideBottom->setFlipY(true);
    sideBottom->setAnchorPoint(ccp(0, 0));
    sideBottom->setPosition(ccp(-3.f, -15.f));
    sideBottom->setScaleX(1.15f);
    sideBottom->setZOrder(9);

    auto sideRight = CCSprite::createWithSpriteFrameName("GJ_commentSide_001.png");
    sideRight->setFlipX(true);
    sideRight->setScaleY(5.45f);
    sideRight->setScaleX(1.2f);
    sideRight->setAnchorPoint(ccp(0, 0));
    sideRight->setPositionX(396.f);
    sideRight->setZOrder(9);

    this->m_listLayer->addChild(sideLeft);
    this->m_listLayer->addChild(sideTop);
    this->m_listLayer->addChild(sideBottom);
    this->m_listLayer->addChild(sideRight);
    this->m_listLayer->addChild(this->m_list);

    // std::string title = "NONGs for " + std::to_string(this->m_songID);

    // this->m_list = GJListLayer::create(
    //     list, 
    //     title.c_str(), 
    //     ccc4(0, 0, 0, 180),
    //     this->getListSize().width,
    //     this->getListSize().height
    // );
    // this->m_list->setZOrder(2);
    this->m_listLayer->setPosition(winSize / 2 - m_list->getScaledContentSize() / 2);
    this->addChild(m_listLayer);
}

void NongPopup::setActiveSong(const SongInfo& song) {
    this->m_activeSong = song;
    auto songIDstr = std::to_string(this->m_songID);
    Mod::get()->setSavedValue<SongInfo>(songIDstr, song);
}