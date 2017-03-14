/*
	SHARP MZ-700 Emulator 'EmuZ-700'
	SHARP MZ-800 Emulator 'EmuZ-800'
	SHARP MZ-1500 Emulator 'EmuZ-1500'

	Author : Takeda.Toshiya
	Date   : 2010.09.02 -

	[ emm ]
*/

#ifndef _EMM_H_
#define _EMM_H_

#include "../vm.h"
#include "../../emu.h"
#include "../device.h"

class EMM : public DEVICE
{
private:
	uint8_t *data_buffer;
	uint32_t data_addr;
	
public:
	EMM(VM* parent_vm, EMU* parent_emu) : DEVICE(parent_vm, parent_emu)
	{
		set_device_name(_T("EMM"));
	}
	~EMM() {}
	
	// common functions
	void initialize();
	void release();
	void reset();
	void write_io8(uint32_t addr, uint32_t data);
	uint32_t read_io8(uint32_t addr);
	void save_state(FILEIO* state_fio);
	bool load_state(FILEIO* state_fio);
};

#endif

