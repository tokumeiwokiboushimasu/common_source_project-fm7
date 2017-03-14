/*
	Nintendo Family BASIC Emulator 'eFamilyBASIC'

	Origin : nester
	Author : Takeda.Toshiya
	Date   : 2010.08.11-

	[ memory ]
*/

#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "../vm.h"
#include "../../emu.h"
#include "../device.h"

class MEMORY : public DEVICE
{
private:
	DEVICE *d_cpu, *d_apu, *d_ppu, *d_drec;
	
	_TCHAR save_file_name[_MAX_PATH];
	
	const uint8_t* key_stat;
	const uint32_t* joy_stat;
	
	uint8_t header[16];
	uint8_t rom[0x8000];
	uint8_t ram[0x800];
	uint8_t save_ram[0x2000];
	uint32_t save_ram_crc32;
	
	uint8_t *spr_ram;
	uint16_t dma_addr;
	uint8_t frame_irq_enabled;
	
	bool pad_strobe;
	uint8_t pad1_bits, pad2_bits;
	
	bool kb_out;
	uint8_t kb_scan;
	
public:
	MEMORY(VM* parent_vm, EMU* parent_emu) : DEVICE(parent_vm, parent_emu)
	{
		set_device_name(_T("Memory Bus"));
	}
	~MEMORY() {}
	
	// common functions
	void initialize();
	void release();
	void reset();
	void write_data8(uint32_t addr, uint32_t data);
	uint32_t read_data8(uint32_t addr);
	void event_vline(int v, int clock);
	void event_callback(int event_id, int err);
	void save_state(FILEIO* state_fio);
	bool load_state(FILEIO* state_fio);
	
	// unique functions
	void set_context_cpu(DEVICE* device)
	{
		d_cpu = device;
	}
	void set_context_apu(DEVICE* device)
	{
		d_apu = device;
	}
	void set_context_ppu(DEVICE* device)
	{
		d_ppu = device;
	}
	void set_context_drec(DEVICE* device)
	{
		d_drec = device;
	}
	void set_spr_ram_ptr(uint8_t* ptr)
	{
		spr_ram = ptr;
	}
	void load_rom_image(const _TCHAR *file_name);
	void save_backup();
};

#endif
