# NONGD

> **NOTE**: This mod is still in very early development, I'm still figuring out Geode so this will take a while. The README here is a proof of concept, basically what I want to achieve with this mod.

> **ANOTHER NOTE**: This mod only works with a fork of Geode that has CustomSongWidget::init() in the bindings and the string ids set for the layer. These changes are in a PR I submitted to the Geode team, and I'm waiting for them to put it in the game. Meanwhile, you can add the following to your GEODE_FOLDER/bindings/GeometryDash.bro, and then [compile Geode yourself using VSCode (step 5 onwards, since you already have Geode on your computer)](https://docs.geode-sdk.org/source/building#recommended-way-for-normal-people)
> ```
> ... (You have to find this class in the file, and add it)
> class CustomSongWidget : cocos2d::CCNode, MusicDownloadDelegate, FLAlertLayerProtocol {
> bool init(SongInfoObject*, LevelSettingsObject*, bool, bool, bool, bool, bool hideBackground) = win 0x685b0; // just add this line somewhere in the class
> ...
> }
> ```

NONGD is a song manager for Geometry Dash. It swaps newgrounds songs with their NONG counterparts.

## What is NONG?

NONG stands for **Not On NewGrounds**. Basically, it means any song that is not on Newgrounds that was replaced manually through the game files. 

NONGs have always been a hassle to manage, because some level creators use popular Newgrounds song IDs and replace them with a NONG. So you have to swap those song files around quite a bit if you play a level with the Newgrounds song and a level with a NONG song.

## Enter NONGD

NONGD stands for **Not On NewGrounds, Duh**. It automates the process of swapping NONGs with their Newgrounds counterparts. It even allows for multiple NONGs on the same song ID.

## How does it work? (how it will work, actually)

You download your NONGs using your method of choice. After getting the mp3 file, you need to fill out the song name and author name. This is mostly for remembering which song is which.

Note that where you place your songs is up to you. If you happen to move the song after you added in in NONGD, it will be automatically removed from the list when you interact with that song ID, and you will have to add it again to the list.

## Where can I find the NONGD menu?

So far the menu is in the level view page. I have plans to add it to the editor, wherever you have to interact with a song. I'll gladly accept suggestions on where it would be useful to have as a button. The mod itself is using only a few popups, so it won't be hard to implement in many places.