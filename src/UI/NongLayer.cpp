#include "NongLayer.hpp"

bool NongLayer::init(int songID) {
    this->m_songID = songID;
    this->createPath();
    
    this->createBackground();
    this->createBackButtonMenu();

    this->createList();

    if (this->doNongsExist()) {
        log::info("Nongs exist!");
    } else {
        log::info("nongs do not exist!");
    }

    this->setKeyboardEnabled(true);
    this->setKeypadEnabled(true);
    this->setID("nong-layer"_spr);
    return true;
}

void NongLayer::createList() {
    auto cells = this->createNongCells();
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto list = ListView::create(
        cells,
        this->getCellSize().height,
        this->getListSize().width,
        this->getListSize().height
    );

    std::string title = "NONGs for ID " + std::to_string(this->m_songID);

    this->m_list = GJListLayer::create(list, title.c_str(), ccc4(0, 0, 0, 180), 358.f, 220.f);
    this->m_list->setZOrder(2);
    this->m_list->setPosition(winSize / 2 - m_list->getScaledContentSize() / 2);
    this->addChild(m_list);
}

CCArray* NongLayer::createNongCells() {
    auto songs = CCArray::create();

    SongInfo songInfo = {
        ghc::filesystem::path("C:\\Users"),
        "test",
        "test"
    };
    songs->addObject(NongCell::create(songInfo, this, this->getCellSize()));

    return songs;
}

CCSize NongLayer::getCellSize() const {
    return {
        this->getListSize().width,
        60.f
    };
}

CCSize NongLayer::getListSize() const {
    return { 358.f, 220.f };
}

void NongLayer::createPath() {
    auto songIDstr = std::to_string(this->m_songID);
    auto saveDir = Mod::get()->getSaveDir();
    auto nongPath = saveDir.append(songIDstr);
    this->m_nongPath = nongPath;
    if (!ghc::filesystem::exists(this->m_nongPath)) {
        ghc::filesystem::create_directory(this->m_nongPath);
    }
}

bool NongLayer::doNongsExist() {
    auto iterator = ghc::filesystem::directory_iterator(this->m_nongPath);
    return ghc::filesystem::is_empty(this->m_nongPath);
}

void NongLayer::createBackground() {
    auto backgroundSprite = CCSprite::create("GJ_gradientBG.png");
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto backgroundSize = backgroundSprite->getContentSize();

    backgroundSprite->setScaleX(winSize.width / backgroundSize.width);
    backgroundSprite->setScaleY(winSize.height / backgroundSize.height);
    backgroundSprite->setAnchorPoint({0, 0});
    backgroundSprite->setColor({0, 102, 255});
    backgroundSprite->setZOrder(-1);
    backgroundSprite->setID("background"_spr);

    this->addChild(backgroundSprite);
}

void NongLayer::createBackButtonMenu() {
    this->m_backButton = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
        this,
        menu_selector(NongLayer::onBackButton)
    );

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    this->m_backButton->setPosition({25, winSize.height - 25});
    this->m_backButton->setID("back-button"_spr);

    this->m_backMenu = CCMenu::create();
    this->m_backMenu->addChild(this->m_backButton);
    this->m_backMenu->setPosition({0, 0});
    this->m_backMenu->setAnchorPoint({0, 0});
    this->m_backMenu->setID("back-button-menu"_spr);

    this->addChild(this->m_backMenu);
}

void NongLayer::keyBackClicked() {
    CCDirector::sharedDirector()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
}

void NongLayer::onBackButton(CCObject* target) {
    keyBackClicked();
}

void NongLayer::onNongLayer(CCObject* target) {
    auto layer = NongLayer::create(target->getTag()); 

    auto scene = CCScene::create();
    scene->addChild(layer);
    auto transition = CCTransitionFade::create(0.5f, scene);
    CCDirector::sharedDirector()->pushScene(transition);
}

NongLayer* NongLayer::create(int songID) {
    auto ret = new NongLayer();
    if (ret && ret->init(songID)) {
        ret->autorelease();
        return ret;
    } 
    CC_SAFE_DELETE(ret);
    return ret;
}