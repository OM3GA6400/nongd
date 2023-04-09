# NONGD

<img src="./logo.png" alt="NONGD logo" />

NONGD is a song manager for Geometry Dash. It swaps newgrounds songs with their NONG counterparts.

## What is NONG?

NONG stands for **Not On NewGrounds**. Basically, it means any song that is not on Newgrounds that was replaced manually through the game files. 

NONGs have always been a hassle to manage, because some level creators use popular Newgrounds song IDs and replace them with a NONG. So you have to swap those song files around quite a bit if you play a level with the Newgrounds song and a level with a NONG song.

## Enter NONGD

NONGD stands for **Not On NewGrounds, Duh**. It automates the process of swapping NONGs with their Newgrounds counterparts. It even allows for multiple NONGs on the same song ID.

## How does it work?

You download your NONGs using your method of choice. After getting the mp3 file, you need to fill out the song name and author name. This is mostly for remembering which song is which.

Note that where you place your songs is up to you. If you happen to move the song after you added in in NONGD, it will be automatically removed from the list when you interact with that song ID, and you will have to add it again to the list.

Alternatively, you can download NONG data from Song File Hub, which is all tightly integrated inside the mod! It's still quite unoptimized, but i'm looking for a way to optimize it further.

## Where can I find the NONGD menu?

The menu can be accessed by clicking on the song name label in either the level view screen or in the song select screen in the editor.

## Details for mod makers

NONGd makes some pretty heavy adjustments in CustomSongWidget. If you want to update labels in there, you will have to do an update for song-name-menu and nongd-id-and-size-label. song-name-menu is a CCMenuItemSpriteExtra which contains a label, and nongd-id-and-size-label is just a label. Their uses are pretty self explanatory I guess.