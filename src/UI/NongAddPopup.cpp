#include "NongAddPopup.hpp"

bool NongAddPopup::setup(NongPopup* parent) {
    this->setTitle("Add NONG");
    this->m_parentPopup = parent;
    this->m_selectSongButton = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Select mp3..."),
        this,
        menu_selector(NongAddPopup::openFile)
    );
    this->m_selectSongMenu = CCMenu::create();
    this->m_selectSongMenu->setID("select-file-menu");
    this->m_selectSongButton->setID("select-file-button");
    this->m_selectSongMenu->addChild(this->m_selectSongButton);
    this->m_selectSongMenu->setPosition(ccp(284.5f, 200.f));

    this->m_addSongButton = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Add"),
        this,
        menu_selector(NongAddPopup::addSong)
    );
    this->m_addSongMenu = CCMenu::create();
    this->m_addSongMenu->setID("add-song-menu");
    this->m_addSongButton->setID("add-song-button");
    this->m_addSongMenu->addChild(this->m_addSongButton);
    this->m_addSongMenu->setPosition(ccp(284.5f, 67.f));

    this->m_containerLayer = CCLayer::create();

    this->m_containerLayer->addChild(this->m_selectSongMenu);
    this->m_containerLayer->addChild(this->m_addSongMenu);
    this->addChild(this->m_containerLayer);
    this->createSelectedSongLabel("Select a file...");
    this->createInputs();

    return true;
}

NongAddPopup* NongAddPopup::create(NongPopup* parent) {
    auto ret = new NongAddPopup();
    auto size = ret->getPopupSize();
    if (ret && ret->init(size.width, size.height, parent)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

CCSize NongAddPopup::getPopupSize() {
    return { 320.f, 240.f };
}

void NongAddPopup::openFile(CCObject* target) {
    auto options = file::FilePickOptions {
        std::nullopt,
        {
            {
                "Music files",
                { "*.mp3" }
            }
        }
    };
    if (auto file = file::pickFile(file::PickMode::OpenFile, options)) {
        this->m_songPath = file.unwrap();
        this->createSelectedSongLabel(this->m_songPath.string());
    } else if (file.unwrapErr() != "Dialog cancelled") {
        FLAlertLayer::create("Failed to import mp3!", file.unwrapErr(), "Ok")->show();
    }
}

void NongAddPopup::createSelectedSongLabel(const std::string& label) {
    if (auto label = this->m_containerLayer->getChildByID("selected-song-label")) {
        this->m_containerLayer->removeChild(label);
    }

    this->m_selectedSongPath = CCLabelBMFont::create(label.c_str(), "goldFont.fnt");
    this->m_selectedSongPath->setID("selected-song-label");
    this->m_selectedSongPath->setScale(0.5f);
    this->m_selectedSongPath->setPosition(ccp(284.5f, 233.f));
    this->m_selectedSongPath->limitLabelWidth(300.f, 0.6f, 0.1f);
    this->m_selectedSongPath->setLineBreakWithoutSpace(true);

    this->m_containerLayer->addChild(this->m_selectedSongPath);
}

void NongAddPopup::createInputs() {
    auto bgSprite = CCScale9Sprite::create(
        "square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f }
    );
    bgSprite->setScale(0.7f);
    bgSprite->setColor({ 0, 0, 0 });
    bgSprite->setOpacity(75);
    bgSprite->setPosition(ccp(284.5f, 153.f));
    bgSprite->setContentSize({ 409.f, 54.f });
    bgSprite->setID("song-name-bg");

    this->m_songNameInput = CCTextInputNode::create(250.f, 20.f, "Song name", "bigFont.fnt");
    this->m_songNameInput->setID("song-name-input");
    this->m_songNameInput->setPosition(ccp(284.5f, 155.f));
    this->m_songNameInput->ignoreAnchorPointForPosition(true);
    this->m_songNameInput->setMaxLabelScale(0.7f);
    this->m_songNameInput->setLabelPlaceholderColor(ccc3(108, 153, 216));
    this->m_songNameInput->setMouseEnabled(true);
    this->m_songNameInput->setTouchEnabled(true);

    this->m_containerLayer->addChild(bgSprite);
    this->m_containerLayer->addChild(this->m_songNameInput);

    this->m_artistNameInput = CCTextInputNode::create(250.f, 20.f, "Artist name", "bigFont.fnt");
    this->m_artistNameInput->setID("artist-name-input");
    this->m_artistNameInput->setPosition(ccp(284.5f, 110.f));
    this->m_artistNameInput->ignoreAnchorPointForPosition(true);
    this->m_artistNameInput->setMaxLabelScale(0.7f);
    this->m_artistNameInput->setLabelPlaceholderColor(ccc3(108, 153, 216));

    auto bgSprite_artist = CCScale9Sprite::create(
        "square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f }
    );
    bgSprite_artist->setScale(0.7f);
    bgSprite_artist->setColor({ 0, 0, 0 });
    bgSprite_artist->setOpacity(75);
    bgSprite_artist->setPosition(ccp(284.5f, 108.f));
    bgSprite_artist->setContentSize({ 409.f, 54.f });
    bgSprite_artist->setID("artist-name-bg");

    this->m_containerLayer->addChild(bgSprite_artist);
    this->m_containerLayer->addChild(this->m_artistNameInput);
}

void NongAddPopup::addSong(CCObject* target) {
    auto artistName = std::string(this->m_artistNameInput->getString());
    auto songName = std::string(this->m_songNameInput->getString());
    if (!ghc::filesystem::exists(this->m_songPath)) {
        FLAlertLayer::create("Error", "The selected file is invalid!", "Ok")->show();
        return;
    }

    if (this->m_songPath.extension().string() != ".mp3") {
        FLAlertLayer::create("Error", "The selected file must be an mp3!", "Ok")->show();
        return;
    }

    if (songName == "") {
        FLAlertLayer::create("Error", "Song name is empty", "Ok")->show();
        return;
    }

    if (artistName == "") {
        FLAlertLayer::create("Error", "Artist name is empty", "Ok")->show();
        return;
    }

    SongInfo song = {
        .path = this->m_songPath,
        .songName = songName,
        .authorName = artistName,
        .songUrl = "local",
    };

    this->m_parentPopup->addSong(song);
    this->onClose(this);
}