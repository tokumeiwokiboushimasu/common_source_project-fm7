/*
	FUJITSU FMR-50 Emulator 'eFMR-50'
	FUJITSU FMR-60 Emulator 'eFMR-60'

	Author : Takeda.Toshiya
	Date   : 2008.05.01 -

	[ keyboard ]
*/

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "../vm.h"
#include "../../emu.h"
#include "../device.h"

/*
	ひらがな/ローマ字	ひらがな
	半角/全角		半角/全角
	変換			変換
	無変換			無変換
	かな/漢字
	カタカナ
	前行			PgUp
	次行			PgDn
	実行			F12
	取消			F11
	COPY
*/

static const int key_table[256] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x10,0x00,0x00,0x00,0x1D,0x00,0x00,
	0x53,0x52,0x00,0x7C,0x55,0x52,0x00,0x00,0x00,0x71,0x00,0x01,0x58,0x57,0x00,0x00,
	0x35,0x6E,0x70,0x00,0x4E,0x4F,0x4D,0x51,0x50,0x00,0x00,0x00,0x00,0x48,0x4B,0x00,
	0x0B,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x1E,0x2E,0x2C,0x20,0x13,0x21,0x22,0x23,0x18,0x24,0x25,0x26,0x30,0x2F,0x19,
	0x1A,0x11,0x14,0x1F,0x15,0x17,0x2D,0x12,0x2B,0x16,0x2A,0x00,0x00,0x00,0x00,0x00,
	0x46,0x42,0x43,0x44,0x3E,0x3F,0x40,0x3A,0x3B,0x3C,0x36,0x38,0x00,0x39,0x47,0x37,
	0x5D,0x5E,0x5F,0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x72,0x73,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x7D,0x6B,0x6C,0x6D,0x57,0x58,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x28,0x27,0x31,0x0C,0x32,0x33,
	0x28,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x0E,0x29,0x0D,0x00,
	0x00,0x00,0x34,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

class FIFO;

class KEYBOARD : public DEVICE
{
private:
	DEVICE* d_pic;
	
	FIFO *key_buf;
	uint8_t kbstat, kbdata, kbint, kbmsk;
	uint8_t table[256];
	
public:
	KEYBOARD(VM* parent_vm, EMU* parent_emu) : DEVICE(parent_vm, parent_emu)
	{
		set_device_name(_T("Keyboard"));
	}
	~KEYBOARD() {}
	
	// common functions
	void initialize();
	void release();
	void reset();
	void write_io8(uint32_t addr, uint32_t data);
	uint32_t read_io8(uint32_t addr);
	void event_frame();
	void decl_state();
	void save_state(FILEIO* state_fio);
	bool load_state(FILEIO* state_fio);
	
	// unique functions
	void set_context_pic(DEVICE* device)
	{
		d_pic = device;
	}
	void key_down(int code);
	void key_up(int code);
};

#endif
