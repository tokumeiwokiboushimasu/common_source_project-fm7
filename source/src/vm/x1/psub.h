/*
	SHARP X1 Emulator 'eX1'
	SHARP X1twin Emulator 'eX1twin'
	SHARP X1turbo Emulator 'eX1turbo'

	Author : Takeda.Toshiya
	Date   : 2009.03.15-

	[ pseudo sub cpu ]
*/

#ifndef _PSUB_H_
#define _PSUB_H_

#include "../vm.h"
#include "../../emu.h"
#include "../device.h"

#define SIG_PSUB_TAPE_REMOTE	0
#define SIG_PSUB_TAPE_END	1

class DATAREC;
class FIFO;

class PSUB : public DEVICE
{
private:
	DEVICE *d_cpu, *d_pio;
	DATAREC *d_drec;
#if defined(USE_SOUND_FILES)
	DEVICE *d_cmt_eject;
	//DEVICE *d_cmt_ffrew;
	DEVICE *d_cmt_play;
	DEVICE *d_cmt_stop;
#endif
	cur_time_t cur_time;
	int time_register_id;
	
	uint8_t databuf[32][8], *datap;
	uint8_t mode, inbuf, outbuf;
	bool ibf, obf;
	int cmdlen, datalen;
	
	FIFO* key_buf;
	const uint8_t* key_stat;
	int key_prev, key_break;
	bool key_shift, key_ctrl, key_graph;
	bool key_caps_locked, key_kana_locked;
	int key_register_id;
	
	bool play, rec, eot;
	
	bool iei, intr;
	uint32_t intr_bit;
	
	void update_intr();
	void process_cmd();
	void set_ibf(bool val);
	void set_obf(bool val);
	uint8_t get_key_low();
	uint16_t get_key(int code, bool repeat);
	
public:
	PSUB(VM* parent_vm, EMU* parent_emu) : DEVICE(parent_vm, parent_emu) {
		set_device_name(_T("PSEUDO SUBSYSTEM"));
#if defined(USE_SOUND_FILES)
		d_cmt_eject = NULL;
		//d_cmt_ffrew = NULL;
		d_cmt_play  = NULL;
		d_cmt_stop  = NULL;
#endif
	}
	~PSUB() {}
	
	// common functions
	void initialize();
	void release();
	void reset();
	void write_io8(uint32_t addr, uint32_t data);
	uint32_t read_io8(uint32_t addr);
	void write_signal(int id, uint32_t data, uint32_t mask);
	void event_callback(int event_id, int err);
	void save_state(FILEIO* state_fio);
	bool load_state(FILEIO* state_fio);
	
	// interrupt common functions
	void set_context_intr(DEVICE* device, uint32_t bit)
	{
		d_cpu = device;
		intr_bit = bit;
	}
	void set_intr_iei(bool val);
	uint32_t get_intr_ack();
	void notify_intr_reti();
	
	// unique functions
	void set_context_pio(DEVICE* device)
	{
		d_pio = device;
	}
	void set_context_drec(DATAREC* device)
	{
		d_drec = device;
	}
#if defined(USE_SOUND_FILES)
	void set_context_cmt_eject(DEVICE* device)
	{
		d_cmt_eject = device;
	}
	//void set_context_cmt_ffrew(DEVICE* device)
	//{
	//	d_cmt_ffrew = device;
	//}
	void set_context_cmt_play(DEVICE* device)
	{
		d_cmt_play = device;
	}
	void set_context_cmt_stop(DEVICE* device)
	{
		d_cmt_stop = device;
	}
#endif
	void key_down(int code, bool repeat);
	void key_up(int code);
	void play_tape(bool value);
	void rec_tape(bool value);
	void close_tape();
};

#endif

