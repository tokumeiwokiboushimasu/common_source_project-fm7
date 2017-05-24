/*
	Skelton for retropc emulator

	Origin : MAME
	Author : Takeda.Toshiya
	Date   : 2008.11.04 -

	[ i8080 / i8085 ]
*/

#ifndef _I8080_BASE_H_ 
#define _I8080_BASE_H_

//#include "vm.h"
//#include "../emu.h"
#include "device.h"

#define SIG_I8080_INTR	0
//#ifdef HAS_I8085
#define SIG_I8085_RST5	1
#define SIG_I8085_RST6	2
#define SIG_I8085_RST7	3
#define SIG_I8085_SID	4
//#endif
#define SIG_I8080_INTE	5

//#ifdef USE_DEBUGGER
class DEBUGGER;
//#endif

class I8080_BASE : public DEVICE
{
protected:
	/* ---------------------------------------------------------------------------
	contexts
	--------------------------------------------------------------------------- */
	
	DEVICE *d_mem, *d_io, *d_pic;
//#ifdef USE_DEBUGGER
	DEBUGGER *d_debugger;
	DEVICE *d_mem_stored, *d_io_stored;
//#endif
	
	// output signals
	outputs_t outputs_busack;
	outputs_t outputs_sod;
	
	/* ---------------------------------------------------------------------------
	registers
	--------------------------------------------------------------------------- */
	
	int count;
	pair_t regs[4];
	uint16_t SP, PC, prevPC;
	uint16_t IM, RIM_IEN;
	bool HALT, BUSREQ, SID, afterEI;

public:
	I8080_BASE(VM* parent_vm, EMU* parent_emu) : DEVICE(parent_vm, parent_emu)
	{
		BUSREQ = false;
		SID = true;
		initialize_output_signals(&outputs_busack);
		initialize_output_signals(&outputs_sod);
		d_mem = d_pic = d_io = NULL;
		d_mem_stored = d_io_stored = NULL;
		d_debugger = NULL;
		set_device_name(_T("i8080 CPU"));
	}
	~I8080_BASE() {}
	virtual void initialize();
	virtual void reset();
	virtual int run(int clock);
	virtual void write_signal(int id, uint32_t data, uint32_t mask);
	uint32_t read_signal(int ch);
	void set_intr_line(bool line, bool pending, uint32_t bit);
	uint32_t get_pc()
	{
		return prevPC;
	}
	uint32_t get_next_pc()
	{
		return PC;
	}
//#ifdef USE_DEBUGGER
	void *get_debugger()
	{
		return d_debugger;
	}
	uint32_t get_debug_prog_addr_mask()
	{
		return 0xffff;
	}
	uint32_t get_debug_data_addr_mask()
	{
		return 0xffff;
	}
	void write_debug_data8(uint32_t addr, uint32_t data);
	uint32_t read_debug_data8(uint32_t addr);
	void write_debug_io8(uint32_t addr, uint32_t data);
	uint32_t read_debug_io8(uint32_t addr);
	bool write_debug_reg(const _TCHAR *reg, uint32_t data);
	void get_debug_regs_info(_TCHAR *buffer, size_t buffer_len);
	int debug_dasm(uint32_t pc, _TCHAR *buffer, size_t buffer_len);
//#endif
	// unique function
	void set_context_mem(DEVICE* device)
	{
		d_mem = device;
	}
	void set_context_io(DEVICE* device)
	{
		d_io = device;
	}
	void set_context_intr(DEVICE* device)
	{
		d_pic = device;
	}
	void set_context_busack(DEVICE* device, int id, uint32_t mask)
	{
		register_output_signal(&outputs_busack, device, id, mask);
	}
	void set_context_sod(DEVICE* device, int id, uint32_t mask)
	{
		register_output_signal(&outputs_sod, device, id, mask);
	}

};
#endif
