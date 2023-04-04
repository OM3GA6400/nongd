#include <Geode/Geode.hpp>
#include <Geode/modify/MusicDownloadManager.hpp>
#include <Geode/modify/CustomSongWidget.hpp>
#include <Geode/utils/cocos.hpp>
#include "UI/NongPopup.hpp"
#include "types/SongInfo.hpp"
#include "NongManager.hpp"

#include <string>

using namespace geode::prelude;

class $modify(MyCustomSongWidget, CustomSongWidget) {
	NongData m_nongData;

	bool init(SongInfoObject* songInfo, LevelSettingsObject* levelSettings, bool p2, bool p3, bool p4, bool p5, bool hideBackground) {
		if (!CustomSongWidget::init(songInfo, levelSettings, p2, p3, p4, p5, hideBackground)) return false;

		if (!this->getChildByID("song-name-label")) {
			this->setStringIDs();
		}

		auto songNameLabel = typeinfo_cast<CCLabelBMFont*>(this->getChildByID("song-name-label"));
		songNameLabel->setVisible(false);

		auto idAndSizeLabel = typeinfo_cast<CCLabelBMFont*>(this->getChildByID("id-and-size-label"));
		idAndSizeLabel->setVisible(false);
		auto newLabel = CCLabelBMFont::create("new", "bigFont.fnt");
		newLabel->setID("nongd-id-and-size-label");
		newLabel->setPosition(ccp(0.f, -32.f));
		newLabel->setScale(0.4f);
		this->addChild(newLabel);

		if (!NongManager::checkIfNongsExist(songInfo->m_songID)) {
			auto strPath = std::string(MusicDownloadManager::sharedState()->pathForSong(songInfo->m_songID));

			SongInfo defaultSong = {
				.path = ghc::filesystem::path(strPath),
				.songName = songInfo->m_songName,
				.authorName = songInfo->m_artistName,
				.songUrl = songInfo->m_songURL,
			};

			NongManager::createDefaultSongIfNull(defaultSong, songInfo->m_songID);
		}

		auto invalidSongs = NongManager::validateNongs(songInfo->m_songID);

		if (invalidSongs.size() > 0) {
			std::string invalidSongList = "";
			for (auto &song : invalidSongs) {
				invalidSongList += song.songName + ", ";
			}

			invalidSongList = invalidSongList.substr(0, invalidSongList.size() - 2);
			auto alert = FLAlertLayer::create("Invalid NONGs", "The NONGs [<cr>" + invalidSongList + "</c>] have been deleted, because their paths were invalid.", "Ok");
			alert->m_scene = this;
			alert->show();
		}

		this->m_fields->m_nongData = NongManager::getNongs(m_songInfo->m_songID);
		SongInfo nong;
		for (auto song : this->m_fields->m_nongData.songs) {
			if (song.path == this->m_fields->m_nongData.active) {
				nong = song;
			}
		}

		if (nong.songUrl != "local" && !ghc::filesystem::exists(nong.path)) {
			auto menu = this->getChildByID("buttons-menu");
			menu->getChildByID("download-button")->setVisible(true);
		}

		this->m_songInfo->m_artistName = nong.authorName;
		this->m_songInfo->m_songName = nong.songName;
		this->updateSongObject(this->m_songInfo);
		if (nong.path == this->m_fields->m_nongData.defaultPath) {
			this->updateIDAndSizeLabel(nong, this->m_songInfo->m_songID);
		} else {
			this->updateIDAndSizeLabel(nong);
		}

		return true;
	}

	// Temporary, will be removed when Geode implements the PR I submitted
	void setStringIDs() {
		getChildOfType<CCScale9Sprite>(this, 0)->setID("bg");
		getChildOfType<CCSprite>(this, 0)->setID("loading-bar");
		getChildOfType<CCLabelBMFont>(this, 0)->setID("song-name-label");
		getChildOfType<CCLabelBMFont>(this, 1)->setID("author-name-label");
		getChildOfType<CCLabelBMFont>(this, 2)->setID("id-and-size-label");
		getChildOfType<CCLabelBMFont>(this, 3)->setID("error-label");
		getChildOfType<CCMenu>(this, 0)->setID("buttons-menu");
		auto customSongWidgetMenu = this->getChildByID("buttons-menu");
		getChildOfType<CCMenuItemSpriteExtra>(customSongWidgetMenu, 0)->setID("download-button");
		getChildOfType<CCMenuItemSpriteExtra>(customSongWidgetMenu, 1)->setID("cancel-button");
		getChildOfType<CCMenuItemSpriteExtra>(customSongWidgetMenu, 2)->setID("use-button");
		getChildOfType<CCMenuItemSpriteExtra>(customSongWidgetMenu, 3)->setID("refresh-button");
		getChildOfType<CCMenuItemSpriteExtra>(customSongWidgetMenu, 4)->setID("play-song-button");
		getChildOfType<CCMenuItemSpriteExtra>(customSongWidgetMenu, 5)->setID("more-button");
	}
		
	void addMenuItemLabel(std::string const& text, int songID) {
		auto menu = CCMenu::create();
		menu->setID("song-name-menu");

		auto label = CCLabelBMFont::create(text.c_str(), "bigFont.fnt");
		label->limitLabelWidth(220.f, 0.8f, 0.1f);
		auto songNameMenuLabel = CCMenuItemSpriteExtra::create(
			label,
			this,
			menu_selector(MyCustomSongWidget::addNongLayer)
		);
		songNameMenuLabel->setTag(songID);
		// I am not even gonna try and understand why this works, but this places the label perfectly in the menu
		auto labelScale = label->getScale();
		songNameMenuLabel->setID("song-name-label");
		songNameMenuLabel->setPosition(ccp(0.f, 0.f));
		songNameMenuLabel->setAnchorPoint(ccp(0.f, 0.5f));
		menu->addChild(songNameMenuLabel);
		menu->setContentSize(ccp(220.f, 25.f));
		menu->setPosition(ccp(-140.f, 27.5f));
		auto layout = RowLayout::create();
		layout->setAxisAlignment(AxisAlignment::Start);
		layout->setAutoScale(false);
		songNameMenuLabel->setLayout(layout);
		songNameMenuLabel->setContentSize({ 220.f, labelScale * 30 });

		this->addChild(menu);
	}

	void updateSongNameLabel(std::string const& text) {
		auto menu = this->getChildByID("song-name-menu");
		auto labelMenuItem = typeinfo_cast<CCMenuItemSpriteExtra*>(menu->getChildByID("song-name-label"));
		auto child = typeinfo_cast<CCLabelBMFont*>(labelMenuItem->getChildren()->objectAtIndex(0));
		child->setString(text.c_str());
		child->limitLabelWidth(220.f, 0.8f, 0.1f);
		auto labelScale = child->getScale();
		labelMenuItem->setContentSize({ 220.f, labelScale * 30 });
		labelMenuItem->updateLayout();
	}

	void updateIDAndSizeLabel(SongInfo const& song, int songID = 0) {
		auto label = typeinfo_cast<CCLabelBMFont*>(this->getChildByID("nongd-id-and-size-label"));
		auto sizeText = NongManager::getFormattedSize(song);
		std::string labelText;
		if (songID != 0) {
			labelText = "SongID: " + std::to_string(songID) + "  Size: " + sizeText;
		} else {
			labelText = "SongID: NONG  Size: " + sizeText;
		}

		if (label) {
			label->setString(labelText.c_str());
		}
	}

	void updateSongObject(SongInfoObject* song) {
		if (!NongManager::checkIfNongsExist(song->m_songID)) {
			auto strPath = std::string(MusicDownloadManager::sharedState()->pathForSong(song->m_songID));

			SongInfo defaultSong = {
				.path = ghc::filesystem::path(strPath),
				.songName = song->m_songName,
				.authorName = song->m_artistName,
				.songUrl = song->m_songURL,
			};

			NongManager::createDefaultSongIfNull(defaultSong, song->m_songID);
		}
		SongInfo active;
		auto nongData = NongManager::getNongs(song->m_songID);
		for (auto nong : nongData.songs) {
			if (nong.path == nongData.active) {
				active = nong;
				song->m_songName = nong.songName;
				song->m_artistName = nong.authorName;
				if (nong.songUrl != "local") {
					song->m_songURL = nong.songUrl;
				}
			}
		}
		CustomSongWidget::updateSongObject(song);
		if (auto found = this->getChildByID("song-name-menu")) {
			this->updateSongNameLabel(song->m_songName);
		} else {
			this->addMenuItemLabel(song->m_songName, song->m_songID);
		}
		if (active.path == nongData.defaultPath) {
			this->updateIDAndSizeLabel(active, song->m_songID);
		} else {
			this->updateIDAndSizeLabel(active);
		}
	}

	void addNongLayer(CCObject* target) {
		auto popup = NongPopup::create(target->getTag(), this);
		popup->m_noElasticity = true;
		popup->show();
	}
};

// Keeping this here for good measure
class $modify(MusicDownloadManager) {
	gd::string pathForSong(int id) {
		if (!NongManager::checkIfNongsExist(id)) {
			return MusicDownloadManager::pathForSong(id);
		}
		auto currentData = NongManager::getNongs(id);
		if (ghc::filesystem::exists(currentData.active)) {
			return currentData.active.string();
		}
		return MusicDownloadManager::pathForSong(id);
	}
};