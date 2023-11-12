#include <Geode/binding/MusicDownloadManager.hpp>
#include <Geode/modify/MusicDownloadManager.hpp>

#include "../managers/nong_manager.hpp"
#include "../types/song_info.hpp"
#include "../filesystem.hpp"

class $modify(MusicDownloadManager) {
	gd::string pathForSong(int id) {
		auto res = NongManager::get()->getNongs(id);
		if (res.has_error()) {
			return MusicDownloadManager::pathForSong(id);
		}
		auto currentData = res.unwrap();
		if (fs::exists(fs::path(currentData.active))) {
			return currentData.active.string();
		}
		return MusicDownloadManager::pathForSong(id);
	}
};