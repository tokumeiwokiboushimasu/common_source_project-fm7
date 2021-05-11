/*
	Skelton for retropc emulator

	Origin : MAME
	Author : Takeda.Toshiya
	Date   : 2020.12.12-

	[ MC6843 / HD46503 ]
*/

// license:BSD-3-Clause
// copyright-holders:Antoine Mine
/**********************************************************************

  Copyright (C) Antoine Mine' 2007

  Motorola 6843 Floppy Disk Controller emulation.

**********************************************************************/

#ifndef _MC6843_H_ 
#define _MC6843_H_

//#include "vm_template.h"
//#include "../emu_template.h"
#include "device.h"

#ifndef offs_t
	typedef UINT32	offs_t;
#endif

#define SIG_MC6843_ACCESS	0
#define SIG_MC6843_DRIVEREG	1
#define SIG_MC6843_SIDEREG	2

class DISK;
class NOISE;

class DLL_PREFIX MC6843 : public DEVICE
{
private:
//	optional_device_array<legacy_floppy_image_device, 4> m_floppy;
	// drive info
	int __MAX_DRIVE;
	int __DRIVE_MASK;
	bool __FDC_DEBUG_LOG;
	
	struct {
		int target_track;
		int track;
		int sector;
		bool searching;
		bool access;
		bool head_load;
	} fdc[8];
	DISK* disk[8];

//	devcb_write_line m_write_irq;
	// output signals
	outputs_t outputs_irq;
	outputs_t outputs_drq;

	// drive noise
	NOISE* d_noise_seek;
	NOISE* d_noise_head_down;
	NOISE* d_noise_head_up;

	// /devices/imagedev/flopdrv.h
	struct chrn_id
	{
		unsigned char C;
		unsigned char H;
		unsigned char R;
		unsigned char N;
//		int data_id;            // id for read/write data command
		unsigned long flags;
	};

	/* registers */
	uint8_t m_CTAR;       /* current track */
	uint8_t m_CMR;        /* command */
	uint8_t m_ISR;        /* interrupt status */
	uint8_t m_SUR;        /* set-up */
	uint8_t m_STRA;       /* status */
	uint8_t m_STRB;       /* status */
	uint8_t m_SAR;        /* sector address */
	uint8_t m_GCR;        /* general count */
	uint8_t m_CCR;        /* CRC control */
	uint8_t m_LTAR;       /* logical address track (=track destination) */

	/* internal state */
	uint8_t  m_drive;
	uint8_t  m_side;
	uint8_t  m_data[128];   /* sector buffer */
	uint32_t m_data_size;   /* size of data */
	uint32_t m_data_idx;    /* current read/write position in data */
//	uint32_t m_data_id;     /* chrd_id for sector write */
//	uint8_t  m_index_pulse;
	uint32_t m_index_clock;

	/* trigger delayed actions (bottom halves) */
//	emu_timer* m_timer_cont;
	int m_timer_id;
	int m_seek_id;

//	legacy_floppy_image_device* floppy_image();
	void status_update();
	void cmd_end();
	void finish_STZ();
	void finish_SEK();
	int address_search(chrn_id* id);
	int address_search_read(chrn_id* id);
	void finish_RCR();
	void cont_SR();
	void cont_SW();

	void mc6843_device();

	// device-level overrides
	void device_start();
	void device_reset();
	void device_timer();

	uint8_t read(offs_t offset);
	void write(offs_t offset, uint8_t data);

	void set_drive(int drive);
	void set_side(int side);
//	void set_index_pulse(int index_pulse);

	void update_head_flag(int drv, bool head_load);

public:
	MC6843(VM_TEMPLATE* parent_vm, EMU_TEMPLATE* parent_emu) : DEVICE(parent_vm, parent_emu)
	{
		initialize_output_signals(&outputs_irq);
		initialize_output_signals(&outputs_drq);
		d_noise_seek = NULL;
		d_noise_head_down = NULL;
		d_noise_head_up = NULL;
		// these parameters may be modified before calling initialize()
		m_drive = m_side = 0;

		__MAX_DRIVE = 2;
		__DRIVE_MASK = 1;
		set_device_name(_T("MC6843 FDC"));
	}
	~MC6843() {}
	
	// common functions
	virtual void initialize();
	virtual void release();
	virtual void reset();
	virtual void __FASTCALL write_io8(uint32_t addr, uint32_t data);
	virtual uint32_t __FASTCALL read_io8(uint32_t addr);
	virtual void __FASTCALL write_dma_io8(uint32_t addr, uint32_t data);
	virtual uint32_t __FASTCALL read_dma_io8(uint32_t addr);
	virtual void __FASTCALL write_signal(int id, uint32_t data, uint32_t mask);
	virtual uint32_t __FASTCALL read_signal(int ch);
	virtual void __FASTCALL event_callback(int event_id, int err);
	virtual void update_config();
	virtual bool process_state(FILEIO* state_fio, bool loading);
	
	// unique functions
	void set_context_irq(DEVICE* device, int id, uint32_t mask)
	{
		register_output_signal(&outputs_irq, device, id, mask);
	}
	void set_context_drq(DEVICE* device, int id, uint32_t mask)
	{
		register_output_signal(&outputs_drq, device, id, mask);
	}
	void set_context_noise_seek(NOISE* device)
	{
		d_noise_seek = device;
	}
	NOISE* get_context_noise_seek()
	{
		return d_noise_seek;
	}
	void set_context_noise_head_down(NOISE* device)
	{
		d_noise_head_down = device;
	}
	NOISE* get_context_noise_head_down()
	{
		return d_noise_head_down;
	}
	void set_context_noise_head_up(NOISE* device)
	{
		d_noise_head_up = device;
	}
	NOISE* get_context_noise_head_up()
	{
		return d_noise_head_up;
	}
	DISK* get_disk_handler(int drv)
	{
		return disk[drv];
	}
	virtual void open_disk(int drv, const _TCHAR* file_path, int bank);
	virtual void close_disk(int drv);
	virtual bool __FASTCALL is_disk_inserted(int drv);
	virtual void __FASTCALL is_disk_protected(int drv, bool value);
	virtual bool __FASTCALL is_disk_protected(int drv);
	virtual uint8_t __FASTCALL get_media_type(int drv);
	virtual void __FASTCALL set_drive_type(int drv, uint8_t type);
	virtual uint8_t __FASTCALL get_drive_type(int drv);
	virtual void __FASTCALL set_drive_rpm(int drv, int rpm);
	virtual void __FASTCALL set_drive_mfm(int drv, bool mfm);
	virtual void __FASTCALL set_track_size(int drv, int size);
	/*
	 * @note: Return value is temporally value.
	 * 20210205 K.O
	 */
	virtual uint8_t fdc_status();
};

#endif
