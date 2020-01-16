#include <gui/screen3_screen/Screen3View.hpp>
#include "string.h"
#include <mp3dec.h>
#include <cstring>
#include <cstdlib>
#include "fatfs.h"
#include "stm32746g_discovery_audio.h"

#define DEBUG_ON 0

// Data read from the USB and fed to the MP3 Decoder
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

typedef enum Mp3_Player_State_Tag {
    PLAY,
    PAUSE,
    STOP,
    NEXT,
    PREV,
    FINISH,
	EMPTY
} Mp3_Player_State;

/* ------------------------------------------------------------------- */

static HMP3Decoder hMP3Decoder;
Mp3_Player_State state;
short output_buffer[AUDIO_OUT_BUFFER_SIZE];
uint8_t input_buffer[READ_BUFFER_SIZE];
static uint8_t *current_ptr;
static BSP_BUFFER_STATE out_buf_offs = BUFFER_OFFSET_NONE;
FIL input_file;
char paths[100][100];
int mp3FilesCounter = 0;
int currentFilePosition = -1;
int currentFileBytes = 0;
int currentFileBytesRead = 0;
static int buffer_leftover = 0;
static int in_buf_offs;
static int decode_result;
int has_been_paused = 0;
char gui_info_text[100];

DIR directory;
FILINFO info;

bool initialized = false;

void BSP_init(void);
void mp3_player_init(void);
void mp3_player_fsm(const char*);
void mp3_player_play(void *);
int mp3_player_process_frame();
int fill_input_buffer();
void copy_leftover();
void reset_player_state();




Screen3View::Screen3View()
{
	BSP_init();
}

void Screen3View::setupScreen()
{
    Screen3ViewBase::setupScreen();
}

void Screen3View::tearDownScreen()
{
    Screen3ViewBase::tearDownScreen();
}

/*void Screen3View::setProgressBar(int value){
	songProgress.setValue(value);
}*/

void Screen3View::playPrevSong()
{
	if(!initialized){
		mp3_player_fsm("");
	    initialized = !initialized;
	}else{
		BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
		out_buf_offs = BUFFER_OFFSET_NONE;
	}

	f_close(&input_file);
	//reset_player_state();
	if (currentFilePosition == 0)
		currentFilePosition = mp3FilesCounter - 1;
	else
		currentFilePosition--;

	Unicode::fromUTF8((const uint8_t*)paths[currentFilePosition], songNameBuffer, 30);
	songName.invalidate();

	if (f_open(&input_file, paths[currentFilePosition], FA_READ) != FR_OK) {
		return;
	}
	state = PLAY;
}
void Screen3View::playNextSong()
{
	if(!initialized){
		mp3_player_fsm("");
		initialized = !initialized;
	}else{
		BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
		out_buf_offs = BUFFER_OFFSET_NONE;
	}

	f_close(&input_file);
	//reset_player_state();
	if (currentFilePosition == mp3FilesCounter - 1)
		currentFilePosition = 0;
	else
		currentFilePosition++;

	Unicode::fromUTF8((const uint8_t*)paths[currentFilePosition], songNameBuffer, 30);
	songName.invalidate();

	if (f_open(&input_file, paths[currentFilePosition], FA_READ) != FR_OK) {
		return;
	}
	state = PLAY;
}
FIL fil;
FRESULT fr;

BaseType_t xReturned;
TaskHandle_t xHandle = NULL;

void Screen3View::playPause()
{
	if(!initialized){
		mp3_player_fsm("");
		initialized = !initialized;
	}

	if (f_findfirst(&directory, &info, "", paths[currentFilePosition]/*"*.mp3"*/) != FR_OK) {
		//Unicode::fromUTF8((const uint8_t*)"Error 1", songNameBuffer, 8);
		//songName.invalidate();
		return;
	}
	currentFileBytes = info.fsize;
	f_closedir(&directory);
	//sprintf(gui_info_text, "%s", paths[currentFilePosition]);
	//refresh_screen(gui_info_text);
	Unicode::fromUTF8((const uint8_t*)paths[currentFilePosition], songNameBuffer, 30);
	songName.invalidate();

	currentFilePosition++;
	currentFilePosition = currentFilePosition % mp3FilesCounter;
	if (f_open(&input_file, paths[currentFilePosition], FA_READ) != FR_OK) {
		return;
	}
	//while(1)

	//xReturned = xTaskCreate(&mp3_player_play,"PLAYTASK", 1000, NULL, osPriorityNormal, (TaskHandle_t*)&xHandle);
						//mp3_player_play,       /* Function that implements the task. */
	                    //"playing",          /* Text name for the task. */
	                    //1000,      /* Stack size in words, not bytes. */
	                    //NULL,    /* Parameter passed into the task. */
						//osPriorityNormal,/* Priority at which the task is created. */
	                    //NULL);

	mp3_player_play(NULL);

	f_close(&input_file);
	//currentFileBytesRead = 0;
}
void Screen3View::moveSongPosition(int value)
{
	songProgress.setValue(slider.getValue());
	BSP_AUDIO_OUT_SetVolume(slider.getValue());
}

void mp3_player_fsm(const char* path)
{
    //BSP_init();
    state = PLAY;

    //touchscreen_loop_init();

	//sprintf(gui_info_text, "Initializing...");
	//refresh_screen(gui_info_text);
    if (f_opendir(&directory, path) != FR_OK) {
        return;
    }

    while(1) {
        if (f_readdir(&directory, &info) != FR_OK) {
            return;
        }
        if (info.fname[0] == 0)
            break;
        if (strstr(info.fname, ".mp3"))
            mp3FilesCounter++;
    }
    f_closedir(&directory);

    int i = 0;
    //paths = (char**)calloc(mp3FilesCounter, sizeof(char*));//(char**)malloc(sizeof(char*) * mp3FilesCounter);

    if (paths == NULL) {
        return;
    }

    if (f_opendir(&directory, path) != FR_OK) {
        return;
    }

    while(1) {
        if (f_readdir(&directory, &info) != FR_OK) {
        	return;
        }
        if (info.fname[0] == 0)
            break;
        if (strstr(info.fname, ".mp3")) {
            //paths[i] = (char*)malloc((strlen(info.fname) + 1) * sizeof(char));
			strcpy(paths[i], info.fname);
            i++;
        }
    }

	f_closedir(&directory);
	//reset_player_state();
	if (currentFilePosition == mp3FilesCounter - 1)
		currentFilePosition = 0;
	else
		currentFilePosition++;
}

// Initialize AUDIO_OUT
void BSP_init(void) {
	if(BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE1, 60, AUDIO_FREQUENCY_44K) == 0)
	{
	  BSP_AUDIO_OUT_SetAudioFrameSlot(CODEC_AUDIOFRAME_SLOT_02);
	}
}

// Play state handler



int player_state = 0;

void mp3_player_play(void* parameters)
{
	if (f_open(&input_file, paths[currentFilePosition], FA_READ) != FR_OK) {
		return;
	}
	//BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE1, 60, AUDIO_FREQUENCY_44K) == 0;
	//BSP_AUDIO_OUT_SetAudioFrameSlot(CODEC_AUDIOFRAME_SLOT_02);
	state = PLAY;
	//player_state = 1;

	if(mp3_player_process_frame() == 0) {
			state = PLAY;
			BSP_AUDIO_OUT_Play((uint16_t*)&output_buffer[0], AUDIO_OUT_BUFFER_SIZE * 2);
			//while(state == PLAY) ;

			f_close(&input_file);
			in_buf_offs = BUFFER_OFFSET_NONE;
	}

	/*fpos = 0;
	buf_offs = BUFFER_OFFSET_NONE;
	while(player_state)
		{
			unsigned int br;
			if(buf_offs == BUFFER_OFFSET_HALF)
			{
				f_read(&input_file, &buff[0], AUDIO_OUT_BUFFER_SIZE/2, &br);
				buf_offs = BUFFER_OFFSET_NONE;
				fpos += br;
			}
			if(buf_offs == BUFFER_OFFSET_FULL)
			{
				f_read(&input_file, &buff[AUDIO_OUT_BUFFER_SIZE / 2], AUDIO_OUT_BUFFER_SIZE / 2,  &br);
				buf_offs = BUFFER_OFFSET_NONE;
				fpos += br;
			}
			if( (br < AUDIO_OUT_BUFFER_SIZE/2) && fpos )
			{
			    BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
			    f_close(&input_file);
			    player_state = 0;
			    fpos = 0;
			    buf_offs = BUFFER_OFFSET_NONE;
			 }
		}
		vTaskDelay(1);*/
}

int times = 0;

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
		else if(decode_result == ERR_UNKNOWN)
		{
			state = NEXT;
			return -1;
		}
	}
	return 0;
}

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
