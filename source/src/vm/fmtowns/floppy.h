/*
	FUJITSU FMR-50 Emulator 'eFMR-50'
	FUJITSU FMR-60 Emulator 'eFMR-60'

	Author : Takeda.Toshiya
	Date   : 2008.04.30 -

	[ floppy ]
*/

#ifndef _FLOPPY_H_
#define _FLOPPY_H_

#include "../vm.h"
#include "../../emu.h"
#include "../device.h"

#define SIG_FLOPPY_IRQ	0

class MB8877;

namespace FMTOWNS {
class FLOPPY : public DEVICE
{
private:
	MB8877 *d_fdc;
	outputs_t output_intr_line;
	int drvreg, drvsel;
	bool irq, irqmsk, changed[4];
	void update_intr();
	
public:
	FLOPPY(VM_TEMPLATE* parent_vm, EMU* parent_emu) : DEVICE(parent_vm, parent_emu)
	{
		initialize_output_signals(&output_intr_line);
	}
	~FLOPPY() {}
	
	// common functions
	void initialize();
	void __FASTCALL write_io8(uint32_t addr, uint32_t data);
	uint32_t __FASTCALL read_io8(uint32_t addr);
	void __FASTCALL write_signal(int id, uint32_t data, uint32_t mask);
	void save_state(FILEIO* state_fio);
	bool load_state(FILEIO* state_fio);
	
	// unique functions
	void set_context_fdc(MB8877* device)
	{
		d_fdc = device;
	}
	void set_context_intr_line(DEVICE* dev, int id, uint32_t mask)
	{
		register_output_signal(&output_intr_line, dev, id, mask);
	}
	void change_disk(int drv)
	{
		changed[drv] = true;
	}
};
}

#endif

