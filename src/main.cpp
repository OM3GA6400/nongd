#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/MusicDownloadManager.hpp>
#include "UI/NongPopup.hpp"

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
		return true;
	}

	void addNongLayer(CCObject* target) {
		auto popup = NongPopup::create(target->getTag());
		popup->m_noElasticity = true;
		popup->setZOrder(69);
		popup->show();
	}
};

// Keeping this here for good measure
// class $modify(MusicDownloadManager) {
// 	gd::string pathForSong(int id) {
// 		if (id == some_id) {
// 			return gd::string("Path");
// 		} else {
// 			return MusicDownloadManager::pathForSong(id);
// 		}
// 	}
// };