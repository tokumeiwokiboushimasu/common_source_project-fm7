/*
	FUJITSU FM7 Emulator 'eFM7'

	Author : K.Ohta
	Date   : 2015.01.01-

	[ virtual machine ]
*/

#ifndef _FM7_H_
#define _FM7_H_

#define DEVICE_NAME		"FUJITSU FM7"
#define CONFIG_NAME		"fm7"

// device informations for virtual machine

// TODO: check refresh rate
#define FRAMES_PER_SEC		60
#define LINES_PER_FRAME 	400
#define CPU_CLOCKS		2000000
#define SCREEN_WIDTH		640
#define SCREEN_HEIGHT		400
#define MAX_DRIVE		4
#define HAS_MC6809              
#define MB8877_MAX_CHIPS	1
#define MEMORY_ADDR_MAX		0x10000
#define MEMORY_BANK_SIZE	0x1000
//#define IO_ADDR_MAX		0x10000

// device informations for win32
#define USE_FD1
#define USE_FD2
#define NOTIFY_KEY_DOWN
#define NOTIFY_KEY_UP
#define USE_ALT_F10_KEY
#define USE_AUTO_KEY		5
#define USE_AUTO_KEY_RELEASE	6
#define USE_POWER_OFF
#define USE_ACCESS_LAMP
//#define USE_DEBUGGER

#define ENABLE_OPENCL // If OpenCL renderer is enabled, define here.

#include "../../common.h"

class EMU;
class DEVICE;
class EVENT;

class BEEP;
class MC6809;
class YM2203;
class MB8877;
class MEMORY;
class FILEIO;

class DISPLAY;
class FM7_SUBMEM;

class FM7_MAINMEM;
class FM7_MAINIO;

#if 0
class Z80;
#endif
class VM
{
protected:
	EMU* emu;
	
	// devices
	EVENT* event;
	
	MC6809* maincpu;
	MEMORY* mainmem;
	FM7_MAINIO* mainio;

	MB8877* fdc;
        YM2203* opn[3];
        YM2203* psg; // Is right? AY-3-8910 is right device.
        BEEP* beep;
#if 0
        Z80* z80cpu;
#endif
#if 1 // WILL Implement
        DEVICE* printer;
        DEVICE* mouse_opn;
	DEVICE* inteli_mouse; 
#endif	
	MC6809* subcpu;
        MEMORY* submem;

#if 1 // WILL Implement
        DISPLAY* display;
        KEYBOARD* keyboard;
#endif	
   
	int machine_version; // 0 = FM8 / 1 = FM7 / 2 = FM77AV / 3 = FM77AV40, etc...
        Uint32 bootmode;   
        Uint32 connected_opns;
        bool cycle_steal = false;
        bool clock_low = false;
        int mainfreq_type;
        Uint32 mainfreq_low;
        Uint32 mainfreq_high;
        Uint32 mainfreq_mmr;
        Uint32 mainfreq_high_mmr;
 
        Uint32 fdd_type[MAX_DRIVE];
        BOOL   fdd_connect[MAX_DRIVE];

        FILEIO* cmt_fileio;
        bool cmt_enabled = true; // 77AV40SX is disabled.
        bool cmt_play;
        bool cmt_rec;
        Uint32 cmt_bufptr;
   
public:
	// ----------------------------------------
	// initialize
	// ----------------------------------------
	
	VM(EMU* parent_emu);
	~VM();
	
	// ----------------------------------------
	// for emulation class
	// ----------------------------------------
	
	// drive virtual machine
	void reset();
	void notify_power_off();
	void run();
	
#ifdef USE_DEBUGGER
	// debugger
	DEVICE *get_cpu(int index);
#endif
	
	// draw screen
	void draw_screen();
	int access_lamp();
	
	// sound generation
	void initialize_sound(int rate, int samples);
	uint16* create_sound(int* extra_frames);
	int sound_buffer_ptr();
	
	// notify key
	void key_down(int code, bool repeat);
	void key_up(int code);
	
	// user interface
	void open_disk(int drv, _TCHAR* file_path, int offset);
	void close_disk(int drv);
	bool disk_inserted(int drv);
	bool now_skip();
	
	void update_config();
	
	// ----------------------------------------
	// for each device
	// ----------------------------------------
	
	// devices
	DEVICE* get_device(int id);
	DEVICE* dummy;
	DEVICE* first_device;
	DEVICE* last_device;
};

#endif
