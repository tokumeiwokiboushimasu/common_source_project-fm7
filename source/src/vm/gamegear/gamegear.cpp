/*
	SEGA GAME GEAR Emulator 'yaGAME GEAR'

	Author : tanam
	Date   : 2013.08.24-

	[ virtual machine ]
*/

#include "gamegear.h"
#include "../../emu.h"
#include "../device.h"
#include "../event.h"

#include "../datarec.h"
#include "../disk.h"
#include "../i8251.h"
#include "../i8255.h"
#include "../io.h"
#include "../noise.h"
#include "../sn76489an.h"
#include "../315-5124.h"
#include "../upd765a.h"
#include "../z80.h"

#ifdef USE_DEBUGGER
#include "../debugger.h"
#endif

#include "keyboard.h"
#include "./memory.h"
#include "./system.h"

// ----------------------------------------------------------------------------
// initialize
// ----------------------------------------------------------------------------
using GAMEGEAR::KEYBOARD;
using GAMEGEAR::MEMORY;
using GAMEGEAR::SYSTEM;

VM::VM(EMU_TEMPLATE* parent_emu) : VM_TEMPLATE(parent_emu)
{
	// create devices
	first_device = last_device = NULL;
	dummy = new DEVICE(this, emu);	// must be 1st device
	event = new EVENT(this, emu);	// must be 2nd device
	dummy->set_device_name(_T("1st Dummy"));

	drec = new DATAREC(this, emu);
	drec->set_context_noise_play(new NOISE(this, emu));
	drec->set_context_noise_stop(new NOISE(this, emu));
	drec->set_context_noise_fast(new NOISE(this, emu));
	sio = new I8251(this, emu);
	pio_k = new I8255(this, emu);
	pio_k->set_device_name(_T("8255 PIO (Keyboard)"));
	pio_f = new I8255(this, emu);
	pio_f->set_device_name(_T("8255 PIO (Floppy I/F)"));
	io = new IO(this, emu);
	io->space = 0x100;
	psg = new SN76489AN(this, emu);
#ifdef USE_DEBUGGER
//	psg->set_context_debugger(new DEBUGGER(this, emu));
#endif
	vdp = new _315_5124(this, emu);
	fdc = new UPD765A(this, emu);
#ifdef USE_DEBUGGER
	//fdc->set_context_debugger(new DEBUGGER(this, emu));
#endif
	fdc->set_context_noise_seek(new NOISE(this, emu));
	fdc->set_context_noise_head_down(new NOISE(this, emu));
	fdc->set_context_noise_head_up(new NOISE(this, emu));
	cpu = new Z80(this, emu);

	key = new KEYBOARD(this, emu);
	memory = new MEMORY(this, emu);
	system = new SYSTEM(this, emu);
	// set contexts
	event->set_context_cpu(cpu);
	event->set_context_sound(psg);
	event->set_context_sound(drec);
	event->set_context_sound(fdc->get_context_noise_seek());
	event->set_context_sound(fdc->get_context_noise_head_down());
	event->set_context_sound(fdc->get_context_noise_head_up());
	event->set_context_sound(drec->get_context_noise_play());
	event->set_context_sound(drec->get_context_noise_stop());
	event->set_context_sound(drec->get_context_noise_fast());

	drec->set_context_ear(pio_k, SIG_I8255_PORT_B, 0x80);
	pio_k->set_context_port_c(key, SIG_KEYBOARD_COLUMN, 0x07, 0);
	pio_k->set_context_port_c(drec, SIG_DATAREC_REMOTE, 0x08, 0);
	pio_k->set_context_port_c(drec, SIG_DATAREC_MIC, 0x10, 0);
	pio_f->set_context_port_c(fdc, SIG_UPD765A_MOTOR_NEG, 2, 0);
	pio_f->set_context_port_c(fdc, SIG_UPD765A_TC, 4, 0);
	pio_f->set_context_port_c(fdc, SIG_UPD765A_RESET, 8, 0);
	pio_f->set_context_port_c(memory, SIG_MEMORY_SEL, 0x40, 0);
	fdc->set_context_irq(pio_f, SIG_I8255_PORT_A, 1);
	fdc->set_context_index(pio_f, SIG_I8255_PORT_A, 4);

	key->set_context_cpu(cpu);
	key->set_context_pio(pio_k);
	system->set_context_key(key);
	vdp->set_context_psg(psg);
	vdp->set_context_key(key);
///	vdp->set_context_cpu(cpu);

	// cpu bus
	cpu->set_context_mem(memory);
	cpu->set_context_io(io);
	cpu->set_context_intr(system);
#ifdef USE_DEBUGGER
	cpu->set_context_debugger(new DEBUGGER(this, emu));
#endif

	// i/o bus
	io->set_iomap_range_rw(0x00, 0x06, system);	// GG  START
	io->set_iomap_single_w(0x80, system);		// COL TENKEY
	io->set_iomap_single_w(0xc0, system);		// COL JOYPAD
	io->set_iomap_range_rw(0xfc, 0xfe, system);	// COL JOYPAD
	io->set_iomap_range_rw(0xff, 0xff, psg);	// COL PSG
	io->set_iomap_range_rw(0x7e, 0x7f, vdp);	// SG  VDP
	io->set_iomap_range_rw(0xbe, 0xbf, vdp);	// SG  VDP
	io->set_iomap_range_rw(0xdc, 0xdf, pio_k);	// SG  KEY
	io->set_iomap_range_rw(0xe0, 0xe3, fdc);	// SG  FDD
	io->set_iomap_range_rw(0xe4, 0xe7, pio_f);	// SG  FDD
	io->set_iomap_range_rw(0xe8, 0xe9, sio);	// SG  SERIAL

	// initialize all devices
#if defined(__GIT_REPO_VERSION)
	set_git_repo_version(__GIT_REPO_VERSION);
#endif
	initialize_devices();

	// BIOS
	memory->bios();

	for(int i = 0; i < 4; i++) {
		fdc->set_drive_type(i, DRIVE_TYPE_2D);
	}
}

VM::~VM()
{
	// delete all devices
	for(DEVICE* device = first_device; device;) {
		DEVICE *next_device = device->next_device;
		device->release();
		delete device;
		device = next_device;
	}
}

// ----------------------------------------------------------------------------
// drive virtual machine
// ----------------------------------------------------------------------------


void VM::run()
{
	__UNLIKELY_IF(event == nullptr) return;
	event->drive();
}

// ----------------------------------------------------------------------------
// debugger
// ----------------------------------------------------------------------------

#ifdef USE_DEBUGGER
DEVICE *VM::get_cpu(int index)
{
	if(index == 0) {
		return cpu;
	}
	return NULL;
}
#endif

// ----------------------------------------------------------------------------
// draw screen
// ----------------------------------------------------------------------------

void VM::draw_screen()
{
	__UNLIKELY_IF(vdp == nullptr) return;
	vdp->draw_screen();
}

// ----------------------------------------------------------------------------
// soud manager
// ----------------------------------------------------------------------------

void VM::initialize_sound(int rate, int samples)
{
	// init sound manager
	event->initialize_sound(rate, samples);

	// init sound gen
	psg->initialize_sound(rate, 3579545, 4000);
}

uint16_t* VM::create_sound(int* extra_frames)
{
	return event->create_sound(extra_frames);
}

int VM::get_sound_buffer_ptr()
{
	return event->get_sound_buffer_ptr();
}

#ifdef USE_SOUND_VOLUME
void VM::set_sound_device_volume(int ch, int decibel_l, int decibel_r)
{
	if(ch == 0) {
		psg->set_volume(0, decibel_l, decibel_r);
	} else if(ch == 1) {
		drec->set_volume(0, decibel_l, decibel_r);
	} else if(ch == 2) {
		fdc->get_context_noise_seek()->set_volume(0, decibel_l, decibel_r);
		fdc->get_context_noise_head_down()->set_volume(0, decibel_l, decibel_r);
		fdc->get_context_noise_head_up()->set_volume(0, decibel_l, decibel_r);
	} else if(ch == 3) {
		drec->get_context_noise_play()->set_volume(0, decibel_l, decibel_r);
		drec->get_context_noise_stop()->set_volume(0, decibel_l, decibel_r);
		drec->get_context_noise_fast()->set_volume(0, decibel_l, decibel_r);
	}
}
#endif

// ----------------------------------------------------------------------------
// user interface
// ----------------------------------------------------------------------------

void VM::open_cart(int drv, const _TCHAR* file_path)
{
	if(drv == 0) {
		memory->open_cart(file_path);
		if (check_file_extension(file_path, _T(".col"))) {
			vdp->set_console(0x00);
			vdp->set_context_irq(cpu, SIG_CPU_NMI, 1);
			memory->bios();
		} else {
			vdp->set_context_irq(cpu, SIG_CPU_IRQ, 1);
			if (check_file_extension(file_path, _T(".gg"))) {
				vdp->set_console(0x40);
			} else {
				vdp->set_console(0x20);
			}
		}
		reset();
	}
}

void VM::close_cart(int drv)
{
	if(drv == 0) {
		memory->close_cart();
		reset();
	}
}

bool VM::is_cart_inserted(int drv)
{
	if(drv == 0) {
		return memory->is_cart_inserted();
	} else {
		return false;
	}
}

void VM::open_floppy_disk(int drv, const _TCHAR* file_path, int bank)
{
	fdc->open_disk(drv, file_path, bank);
}

void VM::close_floppy_disk(int drv)
{
	fdc->close_disk(drv);
}

bool VM::is_floppy_disk_inserted(int drv)
{
	return fdc->is_disk_inserted(drv);
}

void VM::is_floppy_disk_protected(int drv, bool value)
{
	fdc->is_disk_protected(drv, value);
}

bool VM::is_floppy_disk_protected(int drv)
{
	return fdc->is_disk_protected(drv);
}

uint32_t VM::is_floppy_disk_accessed()
{
	return fdc->read_signal(0);
}

void VM::play_tape(int drv, const _TCHAR* file_path)
{
	bool remote = drec->get_remote();

	if(drec->play_tape(file_path) && remote) {
		// if machine already sets remote on, start playing now
		push_play(drv);
	}
}

void VM::rec_tape(int drv, const _TCHAR* file_path)
{
	bool remote = drec->get_remote();

	if(drec->rec_tape(file_path) && remote) {
		// if machine already sets remote on, start recording now
		push_play(drv);
	}
}

void VM::close_tape(int drv)
{
	emu->lock_vm();
	drec->close_tape();
	emu->unlock_vm();
	drec->set_remote(false);
}

bool VM::is_tape_inserted(int drv)
{
	return drec->is_tape_inserted();
}

bool VM::is_tape_playing(int drv)
{
	return drec->is_tape_playing();
}

bool VM::is_tape_recording(int drv)
{
	return drec->is_tape_recording();
}

int VM::get_tape_position(int drv)
{
	return drec->get_tape_position();
}

const _TCHAR* VM::get_tape_message(int drv)
{
	return drec->get_message();
}

void VM::push_play(int drv)
{
	drec->set_remote(false);
	drec->set_ff_rew(0);
	drec->set_remote(true);
}

void VM::push_stop(int drv)
{
	drec->set_remote(false);
}

void VM::push_fast_forward(int drv)
{
	drec->set_remote(false);
	drec->set_ff_rew(1);
	drec->set_remote(true);
}

void VM::push_fast_rewind(int drv)
{
	drec->set_remote(false);
	drec->set_ff_rew(-1);
	drec->set_remote(true);
}

bool VM::is_frame_skippable()
{
	if(event == nullptr) return false;
	return event->is_frame_skippable();
}


double VM::get_current_usec()
{
	if(event == nullptr) return 0.0;
	return event->get_current_usec();
}

uint64_t VM::get_current_clock_uint64()
{
	if(event == nullptr) return (uint64_t)0;
	return event->get_current_clock_uint64();
}

#define STATE_VERSION	1

bool VM::process_state(FILEIO* state_fio, bool loading)
{
	if(!(VM_TEMPLATE::process_state_core(state_fio, loading, STATE_VERSION))) {
		return false;
	}
	return true;
}
