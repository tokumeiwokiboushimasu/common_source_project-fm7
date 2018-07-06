/*
	TOSHIBA PASOPIA 7 Emulator 'EmuPIA7'

	Author : Takeda.Toshiya
	Date   : 2006.09.20 -

	[ floppy ]
*/

#include "floppy.h"
#include "../upd765a.h"

void FLOPPY::initialize()
{
	intr = false;
}

void FLOPPY::write_io8(uint32_t addr, uint32_t data)
{
	switch(addr & 0xff) {
	case 0xe0:
		// tc off
		d_fdc->write_signal(SIG_UPD765A_TC, 0, 1);
		break;
	case 0xe2:
		// tc on
		d_fdc->write_signal(SIG_UPD765A_TC, 1, 1);
		break;
	case 0xe6:
		// fdc reset
		if(data & 0x80) {
			d_fdc->reset();
		}
		// motor on/off
		d_fdc->write_signal(SIG_UPD765A_MOTOR, data, 0x40);
		break;
	}
}

uint32_t FLOPPY::read_io8(uint32_t addr)
{
	switch(addr & 0xff) {
	case 0xe6:
		// fdc intr
		return intr ? 0x80 : 0;
	}
	return 0xff;
}

void FLOPPY::write_signal(int id, uint32_t data, uint32_t mask)
{
	if(id == SIG_FLOPPY_INTR) {
		intr = ((data & mask) != 0);
	}
}

#define STATE_VERSION	1

#include "../../statesub.h"

void FLOPPY::decl_state()
{
	enter_decl_state(STATE_VERSION);
	
	DECL_STATE_ENTRY_BOOL(intr);

	leave_decl_state();
}

void FLOPPY::save_state(FILEIO* state_fio)
{
	if(state_entry != NULL) {
		state_entry->save_state(state_fio);
	}
//	state_fio->FputUint32(STATE_VERSION);
//	state_fio->FputInt32(this_device_id);
	
//	state_fio->FputBool(intr);
}

bool FLOPPY::load_state(FILEIO* state_fio)
{
	bool mb = false;
	if(state_entry != NULL) {
		mb = state_entry->load_state(state_fio);
	}
	if(!mb) return false;
//	if(state_fio->FgetUint32() != STATE_VERSION) {
//		return false;
//	}
//	if(state_fio->FgetInt32() != this_device_id) {
//		return false;
//	}
//	intr = state_fio->FgetBool();
	return true;
}

