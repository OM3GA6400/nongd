#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/MusicDownloadManager.hpp>
#include <Geode/modify/CustomSongWidget.hpp>
#include <Geode/modify/SongInfoObject.hpp>
#include <Geode/utils/cocos.hpp>
#include "UI/NongPopup.hpp"
#include "types/SongInfo.hpp"
#include "NongManager.hpp"

#include <string>

using namespace geode::prelude;

class $modify(MyCustomSongWidget, CustomSongWidget) {
	bool init(SongInfoObject* songInfo, LevelSettingsObject* levelSettings, bool p2, bool p3, bool p4, bool p5, bool hideBackground) {
		if (!CustomSongWidget::init(songInfo, levelSettings, p2, p3, p4, p5, hideBackground)) return false;
		auto songNameLabel = this->getChildByID("song-name-label");
		songNameLabel->setVisible(false);

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

		auto nong = NongManager::getActiveNong(songInfo->m_songID);

		this->m_songInfo->m_artistName = nong.authorName;
		this->m_songInfo->m_songName = nong.songName;
		this->updateSongObject(this->m_songInfo);

		return true;
	}

	void addMenuItemLabel(std::string const& text, int songID) {
		auto menu = CCMenu::create();
		menu->setID("song-name-menu");

		auto label = CCLabelBMFont::create(text.c_str(), "bigFont.fnt");
		label->limitLabelWidth(220.f, 0.8f, 0.1f);
		auto songNameMenuLabel = CCMenuItemLabel::create(
			label,
			this,
			menu_selector(MyCustomSongWidget::addNongLayer)
		);
		songNameMenuLabel->setTag(songID);
		// I am not even gonna try and understand why this works, but this places the label perfectly in the menu
		auto labelScale = label->getScale();
		songNameMenuLabel->setContentSize({ 220.f, labelScale * 30 });
		songNameMenuLabel->setID("song-name-label");
		songNameMenuLabel->setPosition(ccp(0.f, 0.f));
		songNameMenuLabel->setAnchorPoint(ccp(0.f, 0.5f));
		menu->addChild(songNameMenuLabel);
		menu->setContentSize(ccp(220.f, 25.f));
		menu->setPosition(ccp(-140.f, 27.5f));

		this->addChild(menu);
	}

	void updateSongNameLabel(std::string const& text) {
		auto menu = this->getChildByID("song-name-menu");
		auto labelMenuItem = typeinfo_cast<CCMenuItemLabel*>(menu->getChildByID("song-name-label"));
		labelMenuItem->setString(text.c_str());
		auto child = typeinfo_cast<CCLabelBMFont*>(labelMenuItem->getChildren()->objectAtIndex(0));
		child->limitLabelWidth(220.f, 0.8f, 0.1f);
		auto labelScale = child->getScale();
		labelMenuItem->setContentSize({ 220.f, labelScale * 30 });
	}

	void updateIDAndSizeLabel(SongInfo const& song) {
		auto label = typeinfo_cast<CCLabelBMFont*>(this->getChildByID("id-and-size-label"));
		auto sizeText = NongManager::getFormattedSize(song);
		auto labelText = "SongID: NONG  Size: " + sizeText;

		label->setString(labelText.c_str());
	}

	void updateSongObject(SongInfoObject* song) {
		CustomSongWidget::updateSongObject(song);
		if (auto found = this->getChildByID("song-name-menu")) {
			this->updateSongNameLabel(song->m_songName);
			return;
		}
		this->addMenuItemLabel(song->m_songName, song->m_songID);
	}

	void addNongLayer(CCObject* target) {
		auto popup = NongPopup::create(target->getTag());
		popup->m_noElasticity = true;
		popup->show();
	}
};

// Keeping this here for good measure
class $modify(MusicDownloadManager) {
	gd::string pathForSong(int id) {
		auto currentData = NongManager::getNongs(id);
		if (ghc::filesystem::exists(currentData.active)) {
			return currentData.active.string();
		}
		return MusicDownloadManager::pathForSong(id);
	}
};