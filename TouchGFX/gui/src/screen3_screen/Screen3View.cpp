#include <gui/screen3_screen/Screen3View.hpp>
#include "string.h"
#include "helix\pub\mp3dec.h"
#include "stm32746g_discovery_audio.h"
// Represents player states (FSM)
typedef enum Mp3_Player_State_Tag {
    PLAY,
    PAUSE,
    STOP,
    NEXT,
    PREV,
    FINISH,
	EMPTY
} Mp3_Player_State;

#define READ_BUFFER_SIZE 2 * MAINBUF_SIZE// + 216

// Max size of a single frame
#define DECODED_MP3_FRAME_SIZE MAX_NGRAN * MAX_NCHAN * MAX_NSAMP

// Decoded data ready to be passed out to output (always have space to hold 2 frames)
#define AUDIO_OUT_BUFFER_SIZE 2 * DECODED_MP3_FRAME_SIZE

// State of the offset of the BSP output buffer
typedef enum BSP_BUFFER_STATE_TAG {
    BUFFER_OFFSET_NONE = 0,
    BUFFER_OFFSET_HALF,
    BUFFER_OFFSET_FULL,
} BSP_BUFFER_STATE;

/* ------------------------------------------------------------------- */

static HMP3Decoder hMP3Decoder;
Mp3_Player_State state;
short output_buffer[AUDIO_OUT_BUFFER_SIZE];
uint8_t input_buffer[READ_BUFFER_SIZE];
static uint8_t *current_ptr;
static BSP_BUFFER_STATE out_buf_offs = BUFFER_OFFSET_NONE;
FIL input_file;
char** paths;
int mp3FilesCounter = 0;
int currentFilePosition = -1;
int currentFileBytes = 0;
int currentFileBytesRead = 0;
static int buffer_leftover = 0;
static int in_buf_offs;
static int decode_result;
int has_been_paused = 0;
char gui_info_text[100];

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

void BSP_init(void);
void mp3_player_init(void);
void mp3_player_play();
int mp3_player_process_frame();
int fill_input_buffer();
void copy_leftover();
void reset_player_state();

/* ------------------------------------------------------------------- */

// Main Finite State Machine of the player
// Handles all user input from the touchscreen, controlls the state of the player and updates the screen
// INPUT: path - non-relative path to the folder with MP3 files (can also contain other files, in which case
// 		it will just skip them during swtiching to the next/previous song)
// Initialize AUDIO_OUT
void BSP_init(void) {
	if(BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_BOTH, 100, AUDIO_FREQUENCY_44K) == 0)
	{
	  BSP_AUDIO_OUT_SetAudioFrameSlot(CODEC_AUDIOFRAME_SLOT_02);
	}
}

// Play state handler
void mp3_player_play(void)
{
	hMP3Decoder = MP3InitDecoder();
    if(mp3_player_process_frame() == 0) {
		state = PLAY;
		BSP_AUDIO_OUT_Play((uint16_t*)&output_buffer[0], AUDIO_OUT_BUFFER_SIZE * 2);
		while(1) {
			//TODO: update_progress_bar(((double)currentFileBytesRead) / currentFileBytes);
			Mp3_Player_State newState = PLAY;
			if (newState != EMPTY)
				state = newState;
            if (!has_been_paused && state == PAUSE) {
				// TODO: sprintf(gui_info_text, "PAUSED");
				// TODO: refresh_screen(gui_info_text);
				if(BSP_AUDIO_OUT_Pause() != AUDIO_OK) {
					return;
				}
				has_been_paused = 1;
            } else if(has_been_paused && state == PLAY) {
				// TODO: sprintf(gui_info_text, "%s", paths[currentFilePosition]);
				// TODO: refresh_screen(gui_info_text);
				if(BSP_AUDIO_OUT_Resume() != AUDIO_OK) {
					return;
				}
				has_been_paused = 0;
			}  else if (has_been_paused && state == PAUSE)
				continue;
			else if (state != PLAY) {
                break;
            }
		}
		BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
		out_buf_offs = BUFFER_OFFSET_NONE;
	}
	else
	{
		state = NEXT;
	}

	buffer_leftover = 0;
	current_ptr = NULL;
	MP3FreeDecoder(hMP3Decoder);
}

// Process next mp3 frame from the main file
int mp3_player_process_frame(void)
{
	MP3FrameInfo mp3FrameInfo;

	if (current_ptr == NULL && fill_input_buffer() != 0) return EOF;

	in_buf_offs = MP3FindSyncWord(current_ptr, buffer_leftover);

	while(in_buf_offs < 0)
	{
		if(fill_input_buffer() != 0) return EOF;
		if(buffer_leftover > 0)
		{
			buffer_leftover--;
			current_ptr++;
		}
		// END TODO
		in_buf_offs = MP3FindSyncWord(current_ptr, buffer_leftover);
	}

	current_ptr += in_buf_offs;
	buffer_leftover -= in_buf_offs;

	// get data from the frame header
	if(!(MP3GetNextFrameInfo(hMP3Decoder, &mp3FrameInfo, current_ptr) == 0 && mp3FrameInfo.nChans == 2 && mp3FrameInfo.version == 0))
	{
		// if header has failed
		if(buffer_leftover > 0)
		{
			buffer_leftover--;
			current_ptr++;
		}
		return 0;
	}

	// if feel the buffer with actual non-frame-header data if necessary
	if (buffer_leftover < MAINBUF_SIZE && fill_input_buffer() != 0) return EOF;

	// decode the right portion of the buffer
	if(out_buf_offs == BUFFER_OFFSET_HALF)
	{
		decode_result = MP3Decode(hMP3Decoder, &current_ptr, &buffer_leftover, output_buffer, 0);
		out_buf_offs = BUFFER_OFFSET_NONE;
	}

	if(out_buf_offs == BUFFER_OFFSET_FULL)
	{
		decode_result = MP3Decode(hMP3Decoder, &current_ptr, &buffer_leftover, &output_buffer[DECODED_MP3_FRAME_SIZE], 0);
		out_buf_offs = BUFFER_OFFSET_NONE;
	}

	// check results of the decoding process
	if(decode_result != ERR_MP3_NONE)
	{
		if(decode_result == ERR_MP3_INDATA_UNDERFLOW)
		{
			buffer_leftover = 0;
			if(fill_input_buffer() != 0) return EOF;
		}
	}

	return 0;
}

// Callback when half of audio out buffer is transefered
void BSP_AUDIO_OUT_TransferComplete_CallBack(void)
{
    out_buf_offs = BUFFER_OFFSET_FULL;

	if(mp3_player_process_frame() != 0)
	{
		BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
		state = NEXT;
	}
}

// Callback when all of audio out buffer is transefered
void BSP_AUDIO_OUT_HalfTransfer_CallBack(void)
{
    out_buf_offs = BUFFER_OFFSET_HALF;

	if(mp3_player_process_frame() != 0)
	{
		BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
		state = NEXT;
	}
}

// Fill the input buffer with mp3 data from the USB for the decoder
int fill_input_buffer()
{
	unsigned int actually_read, how_much_to_read;

	copy_leftover();

	how_much_to_read = READ_BUFFER_SIZE - buffer_leftover;

	// read from the input_file to fill the input_buffer fully
	f_read(&input_file, (BYTE *)input_buffer + buffer_leftover, how_much_to_read, &actually_read);

	currentFileBytesRead += actually_read;
	// if there's still data in  the file
	if (actually_read == how_much_to_read)
	{
		current_ptr = input_buffer;
		in_buf_offs = 0;
		buffer_leftover = READ_BUFFER_SIZE;
		return 0;
	}
	else return EOF;
}

// if there is some data left in the buffer copy it to the beginning
void copy_leftover()
{
	if(buffer_leftover > 0)
	{
		memcpy(input_buffer, current_ptr, buffer_leftover);
	}
}

// reset all the used data structures
void reset_player_state()
{
	buffer_leftover = 0;
    current_ptr = NULL;
    out_buf_offs = BUFFER_OFFSET_NONE;
}
