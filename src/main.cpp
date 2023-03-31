#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/MusicDownloadManager.hpp>
#include <Geode/modify/CustomSongWidget.hpp>
#include "UI/NongPopup.hpp"
#include "types/SongInfo.hpp"
#include "NongManager.hpp"

#include <string>

USE_GEODE_NAMESPACE();

class $modify(MyLevelInfoLayer, LevelInfoLayer){
	bool init(GJGameLevel* level){
		if (!LevelInfoLayer::init(level)) {
			return false;
		}

		auto rightSideMenu = this->getChildByID("right-side-menu");
		auto sprite = CircleButtonSprite::createWithSprite("test_button.png"_spr);
		auto nongButton = CCMenuItemSpriteExtra::create(
			sprite,
			this,
			menu_selector(MyLevelInfoLayer::addNongLayer)
		);
		nongButton->setTag(level->m_songID);

		rightSideMenu->addChild(nongButton);
		rightSideMenu->updateLayout();

		auto customSongWidget = static_cast<CustomSongWidget*>(this->getChildByID("custom-songs-widget"));
		auto songs = NongManager::getNongs(level->m_songID);

		auto songNameLabel = getChildOfType<CCLabelBMFont>(customSongWidget, 0);
		auto authorNameLabel = getChildOfType<CCLabelBMFont>(customSongWidget, 1);
		auto idAndSizeLabel = getChildOfType<CCLabelBMFont>(customSongWidget, 2);
		auto menu = getChildOfType<CCMenu>(customSongWidget, 0);

		// menu->setID("buttons-menu");
		// authorNameLabel->setID("author-name-label");
		// songNameLabel->setID("song-name-label");
		// idAndSizeLabel->setID("id-and-size-label");

		auto moreButton = getChildOfType<CCMenuItemSpriteExtra>(menu, 5);
		// moreButton->setID("more-button");

		for (auto savedSong : songs) {
			if (savedSong.selected && savedSong.songName != "Default") {
				auto authorName = "By " + savedSong.authorName;
				auto idAndSizeStr = std::string("SongID: NONG  Size: ") + NongManager::getFormattedSize(savedSong);
				idAndSizeLabel->setString(idAndSizeStr.c_str());
				songNameLabel->setString(savedSong.songName.c_str());
				songNameLabel->setColor(ccc3(187, 255, 164));
				authorNameLabel->setString(authorName.c_str());
				songNameLabel->limitLabelWidth(200, 0.8f, 0.2f);
				authorNameLabel->limitLabelWidth(130, 0.8f, 0.2f);
				moreButton->removeFromParent();
			}
		}

		return true;
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
		auto songs = NongManager::getNongs(id);
		for (auto song : songs) {
			if (song.selected) {
				return gd::string(song.path.string());
			}
		}
		return MusicDownloadManager::pathForSong(id);
	}
};