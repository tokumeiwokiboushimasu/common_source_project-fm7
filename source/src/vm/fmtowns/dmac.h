#pragma once

#include "../upd71071.h"

// Using original signal using after 1 << 12.
#define SIG_TOWNS_DMAC_ADDR_REG     4096
#define SIG_TOWNS_DMAC_WRAP_REG     4100
#define SIG_TOWNS_DMAC_ADDR_MASK    4104

namespace FMTOWNS {
class TOWNS_DMAC : public UPD71071
{
protected:
	uint8_t dma_wrap_reg;
	uint32_t dma_addr_mask;

	// Temporally workaround for SCSI.20200318 K.O
//	bool creg_set[4];
//	bool bcreg_set[4];
	virtual void __FASTCALL do_dma_inc_dec_ptr_8bit(int c) override;
	virtual void __FASTCALL do_dma_inc_dec_ptr_16bit(int c) override;
#if 0 /* Debugging */
	virtual bool __FASTCALL do_dma_epilogue(int c);
#endif

public:
	TOWNS_DMAC(VM_TEMPLATE* parent_vm, EMU_TEMPLATE* parent_emu) : UPD71071(parent_vm, parent_emu)
	{
		set_device_name(_T("FM-Towns uPD71071 DMAC"));
	}
	~TOWNS_DMAC() {}
	// common functions
	virtual void initialize() override;
	virtual void reset() override;

	virtual void __FASTCALL write_io8(uint32_t addr, uint32_t data) override;
	virtual uint32_t __FASTCALL read_io8(uint32_t addr) override;
	virtual void __FASTCALL write_io16(uint32_t addr, uint32_t data) override;
	virtual uint32_t __FASTCALL read_io16(uint32_t addr) override;

	virtual void __FASTCALL write_signal(int id, uint32_t data, uint32_t _mask) override;
	virtual uint32_t __FASTCALL read_signal(int id) override;

	virtual bool process_state(FILEIO* state_fio, bool loading) override;

	virtual bool get_debug_regs_info(_TCHAR *buffer, size_t buffer_len) override;
};

}
