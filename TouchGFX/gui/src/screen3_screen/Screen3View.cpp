#include <gui/screen3_screen/Screen3View.hpp>
#include "string.h"

Screen3View::Screen3View()
{

}

void Screen3View::setupScreen()
{
    Screen3ViewBase::setupScreen();
}

void Screen3View::tearDownScreen()
{
    Screen3ViewBase::tearDownScreen();
}

void Screen3View::playPrevSong()
{

}
void Screen3View::playNextSong()
{

}
FIL fil;
FRESULT fr;
void Screen3View::playPause()
{
	char *fileNameBuffer = (char*)malloc(sizeof(char)*30);
	fr = getFileName(fileNameBuffer);
	Unicode::fromUTF8((const uint8_t*) fileNameBuffer, songNameBuffer, 30);
	songName.invalidate();
}
void Screen3View::moveSongPosition(int value)
{

}
/*
void Screen3View::printErrorName(FRESULT fr, touchgfx::Unicode::UnicodeChar *songNameBuffer)
{
	switch(fr)
	{
	case FR_OK:
		Unicode::strncpy(songNameBuffer, "fr ok     ", 30);
		break;
	case FR_DISK_ERR:
		Unicode::strncpy(songNameBuffer, "fr disk err    ", 30);
		break;
	case FR_INT_ERR:
		Unicode::strncpy(songNameBuffer, "fr int err     ", 30);
		break;
	case FR_NOT_READY:
		Unicode::strncpy(songNameBuffer, "fr not ready    ", 30);
		break;
	case FR_NO_FILE:
		Unicode::strncpy(songNameBuffer, "fr no file      ", 30);
		break;
	case FR_NO_PATH:
		Unicode::strncpy(songNameBuffer, "fr no path      ", 30);
		break;
	case FR_INVALID_NAME:
		Unicode::strncpy(songNameBuffer, "fr inv name     ", 30);
		break;
	case FR_DENIED:
		Unicode::strncpy(songNameBuffer, "fr denied      ", 30);
		break;
	case FR_EXIST:
		Unicode::strncpy(songNameBuffer, "fr exist       ", 30);
		break;
	case FR_INVALID_OBJECT:
		Unicode::strncpy(songNameBuffer, "fr inv object    ", 30);
		break;
	case FR_WRITE_PROTECTED:
		Unicode::strncpy(songNameBuffer, "fr write protec    ", 30);
		break;
	case FR_INVALID_DRIVE:
		Unicode::strncpy(songNameBuffer, "fr inv drive    ", 30);
		break;
	case FR_NOT_ENABLED:
		Unicode::strncpy(songNameBuffer, "fr not enabled    ", 30);
		break;
	case FR_NO_FILESYSTEM:
			Unicode::strncpy(songNameBuffer, "fr no filesystem", 30);
			break;
	case FR_MKFS_ABORTED:
			Unicode::strncpy(songNameBuffer, "fr mkfs abort   ", 30);
			break;
	case FR_TIMEOUT:
			Unicode::strncpy(songNameBuffer, "fr timeout      ", 30);
			break;
	case FR_LOCKED:
			Unicode::strncpy(songNameBuffer, "fr locked      ", 30);
			break;
	case FR_INVALID_PARAMETER:
			Unicode::strncpy(songNameBuffer, "fr inv parameter  ", 30);
			break;
	case FR_NOT_ENOUGH_CORE:
			Unicode::strncpy(songNameBuffer, "FR_NOT_ENOUGH_CORE", 30);
			break;
	case FR_TOO_MANY_OPEN_FILES:
			Unicode::strncpy(songNameBuffer, "FR_TOO_MANY_OPEN_FILES", 30);
			break;
	default:
		Unicode::strncpy(songNameBuffer, "else          ", 30);
	}
}
*/
