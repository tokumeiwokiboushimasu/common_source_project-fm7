/*
	Skelton for retropc emulator

	Author : Takeda.Toshiya
	Date   : 2022.07.03-

	[ Speaker ]
*/

#include "./speaker.h"
#include "../types/util_sound.h"

void SPEAKER::initialize()
{
	realtime = false;
	last_vol_l = last_vol_r = 0;
	
	register_frame_event(this);
}

void SPEAKER::reset()
{
	changed = 0;
	sample = prev_sample = 0;
	prev_clock = change_clock = get_current_clock();
}

void SPEAKER::write_signal(int id, uint32_t data, uint32_t mask)
{
	if(id == SIG_SPEAKER_SAMPLE) {
		if(sample != (data & mask)) {
			// mute if signal is not changed in 2 frames
			changed = 2;
			update_realtime_render();
			
			sample = data & mask;
			change_clock = get_current_clock();
		}
	}
}

void SPEAKER::event_frame()
{
	if(changed > 0 && --changed == 0) {
		update_realtime_render();
	}
}

void SPEAKER::update_realtime_render()
{
	bool value = (changed != 0);
	
	if(realtime != value) {
		set_realtime_render(this, value);
		realtime = value;
	}
}

void SPEAKER::mix(int32_t* buffer, int cnt)
{
	if(changed) {
		uint32_t cur_clock = get_current_clock();
		int cur_sample;
		
		if(change_clock > prev_clock) {
			cur_sample  = prev_sample * (change_clock - prev_clock) + sample * (cur_clock - change_clock);
			cur_sample /= cur_clock - prev_clock;
		} else {
			cur_sample = sample;
		}
		prev_sample = sample;
		prev_clock = cur_clock;
		
		int volume = max_vol * cur_sample / 256;
		
		last_vol_l = apply_volume(volume, volume_l);
		last_vol_r = apply_volume(volume, volume_r);
		
		for(int i = 0; i < cnt; i++) {
			*buffer++ += last_vol_l; // L
			*buffer++ += last_vol_r; // R
		}
	} else {
		// suppress petite noise when go to mute
		for(int i = 0; i < cnt; i++) {
			*buffer++ += last_vol_l; // L
			*buffer++ += last_vol_r; // R
			
			if(last_vol_l > 0) {
				last_vol_l--;
			} else if(last_vol_l < 0) {
				last_vol_l++;
			}
			if(last_vol_r > 0) {
				last_vol_r--;
			} else if(last_vol_r < 0) {
				last_vol_r++;
			}
		}
	}
}

void SPEAKER::set_volume(int ch, int decibel_l, int decibel_r)
{
	volume_l = decibel_to_volume(decibel_l);
	volume_r = decibel_to_volume(decibel_r);
}

void SPEAKER::initialize_sound(int rate, int volume)
{
	max_vol = volume;
}

#define STATE_VERSION	1

bool SPEAKER::process_state(FILEIO* state_fio, bool loading)
{
	if(!state_fio->StateCheckUint32(STATE_VERSION)) {
		return false;
	}
	if(!state_fio->StateCheckInt32(this_device_id)) {
		return false;
	}
	state_fio->StateValue(realtime);
	state_fio->StateValue(changed);
	state_fio->StateValue(sample);
	state_fio->StateValue(prev_sample);
	state_fio->StateValue(prev_clock);
	state_fio->StateValue(change_clock);
	
	// post process
	if(loading) {
		last_vol_l = last_vol_r = 0;
	}
	return true;
}

