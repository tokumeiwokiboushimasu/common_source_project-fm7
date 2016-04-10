/*
 * Common source code project -> FM-7 -> Display -> Vram access
 * (C) 2015 K.Ohta <whatisthis.sowhat _at_ gmail.com>
 * History:
 *  Sep 27, 2015 : Split from display.cpp .
 */

#include "fm7_display.h"

uint8_t DISPLAY::read_vram_l4_400l(uint32_t addr, uint32_t offset)
{
#if defined(_FM77L4)
	if(addr < 0x8000) {
		if(workram) {
			uint32_t raddr = addr & 0x3fff;
			if((multimode_accessmask & 0x04) == 0) {
				return gvram[0x8000 + (raddr + offset) & 0x7fff];
			}
			return 0xff;
		}
		pagemod = addr & 0x4000;
		return gvram[((addr + offset) & mask) | pagemod];
	} else if(addr < 0x9800) {
		return textvram[addr & 0x0fff];
	} else { // $9800-$bfff
		return subrom_l4[addr - 0x9800];
	}
#endif
	return 0xff;
}

void DISPLAY::write_vram_l4_400l(uint32_t addr, uint32_t offset, uint32_t data)
{
#if defined(_FM77L4)
	if(addr < 0x8000) {
		if(workram) {
			uint32_t raddr = addr & 0x3fff;
			if((multimode_accessmask & 0x04) == 0) {
				gvram[0x8000 + (raddr + offset) & 0x7fff] = (uint8_t)data;
			}
			return;
		}
		pagemod = addr & 0x4000;
		gvram[((addr + offset) & mask) | pagemod] = (uint8_t)data;
	} else if(addr < 0x9800) {
	  textvram[addr & 0x0fff] = (uint8_t)data;
	} else { // $9800-$bfff
		//return subrom_l4[addr - 0x9800];
	}
	return;
#endif	
}

inline void DISPLAY::GETVRAM_8_200L(int yoff, scrntype_t *p, uint32_t mask,  uint32_t yoff_d1, uint32_t yoff_d2,
									bool window_inv = false)
{
	register uint8_t b, r, g;
	register uint32_t dot;
	uint32_t yoff_d;
#if defined(_FM77AV40EX) || defined(_FM77AV40SX)
	int dpage = vram_display_block;
#endif
	if(p == NULL) return;
#if defined(_FM77AV_VARIANTS)
	if(display_page == 1) { // Is this dirty?
		yoff_d = yoff_d2;
	} else {
		yoff_d = yoff_d1;
	}
#else
	yoff_d = offset_point;
#endif	
	yoff_d = (yoff + yoff_d) & 0x3fff;
#if defined(_FM77AV40EX) || defined(_FM77AV40SX)
	if(window_inv) {
		if(dpage == 0) {
			dpage = 1;
		} else {
			dpage = 0;
		}
	}
	if(dpage != 0) yoff_d += 0x18000;
#endif
	b = r = g = 0;
#if defined(_FM77AV_VARIANTS)
	if(display_page == 1) {
		if(mask & 0x01) b = gvram_shadow[yoff_d + 0x0c000];
		if(mask & 0x02) r = gvram_shadow[yoff_d + 0x10000];
		if(mask & 0x04) g = gvram_shadow[yoff_d + 0x14000];
	} else {
		if(mask & 0x01) b = gvram_shadow[yoff_d + 0x00000];
		if(mask & 0x02) r = gvram_shadow[yoff_d + 0x04000];
		if(mask & 0x04) g = gvram_shadow[yoff_d + 0x08000];
	}
#else
	if(mask & 0x01) b = gvram[yoff_d + 0x00000];
	if(mask & 0x02) r = gvram[yoff_d + 0x04000];
	if(mask & 0x04) g = gvram[yoff_d + 0x08000];
#endif	
	dot = ((g & 0x80) >> 5) | ((r & 0x80) >> 6) | ((b & 0x80) >> 7);
	p[0] = dpalette_pixel[dot];
	dot = ((g & 0x40) >> 4) | ((r & 0x40) >> 5) | ((b & 0x40) >> 6);
	p[1] = dpalette_pixel[dot];
	dot = ((g & 0x20) >> 3) | ((r & 0x20) >> 4) | ((b & 0x20) >> 5);
	p[2] = dpalette_pixel[dot];
	dot = ((g & 0x10) >> 2) | ((r & 0x10) >> 3) | ((b & 0x10) >> 4);
	p[3] = dpalette_pixel[dot];
					
	dot = ((g & 0x8) >> 1) | ((r & 0x8) >> 2) | ((b & 0x8) >> 3);
	p[4] = dpalette_pixel[dot];
	dot = (g & 0x4) | ((r & 0x4) >> 1) | ((b & 0x4) >> 2);
	p[5] = dpalette_pixel[dot];
	dot = ((g & 0x2) << 1) | (r & 0x2) | ((b & 0x2) >> 1);
	p[6] = dpalette_pixel[dot];
	dot = ((g & 0x1) << 2) | ((r & 0x1) << 1) | (b & 0x1);
	p[7] = dpalette_pixel[dot];
}

#if defined(_FM77AV40) || defined(_FM77AV40EX) || defined(_FM77AV40SX)
inline void DISPLAY::GETVRAM_8_400L(int yoff, scrntype_t *p, uint32_t mask, uint32_t yoff_d1, uint32_t yoff_d2,
									bool window_inv = false)
{
	register uint8_t b, r, g;
	register uint32_t dot;
	uint32_t yoff_d;
# if defined(_FM77AV40EX) || defined(_FM77AV40SX)
	int dpage = vram_display_block;
# endif
	if(p == NULL) return;
	if(display_page == 1) { // Is this dirty?
		yoff_d = yoff_d2;
	} else {
		yoff_d = yoff_d1;
	}
	yoff_d = (yoff + (yoff_d << 1)) & 0x7fff;
# if defined(_FM77AV40EX) || defined(_FM77AV40SX)
	if(window_inv) {
		if(dpage == 0) {
			dpage = 1;
		} else {
			dpage = 0;
		}
	}
	if(dpage != 0) yoff_d += 0x18000;
# endif
	b = r = g = 0;
	if(mask & 0x01) b = gvram_shadow[yoff_d + 0x00000];
	if(mask & 0x02) r = gvram_shadow[yoff_d + 0x08000];
	if(mask & 0x04) g = gvram_shadow[yoff_d + 0x10000];

	dot = ((g & 0x80) >> 5) | ((r & 0x80) >> 6) | ((b & 0x80) >> 7);
	p[0] = dpalette_pixel[dot];
	dot = ((g & 0x40) >> 4) | ((r & 0x40) >> 5) | ((b & 0x40) >> 6);
	p[1] = dpalette_pixel[dot];
	dot = ((g & 0x20) >> 3) | ((r & 0x20) >> 4) | ((b & 0x20) >> 5);
	p[2] = dpalette_pixel[dot];
	dot = ((g & 0x10) >> 2) | ((r & 0x10) >> 3) | ((b & 0x10) >> 4);
	p[3] = dpalette_pixel[dot];
					
	dot = ((g & 0x8) >> 1) | ((r & 0x8) >> 2) | ((b & 0x8) >> 3);
	p[4] = dpalette_pixel[dot];
	dot = (g & 0x4) | ((r & 0x4) >> 1) | ((b & 0x4) >> 2);
	p[5] = dpalette_pixel[dot];
	dot = ((g & 0x2) << 1) | (r & 0x2) | ((b & 0x2) >> 1);
	p[6] = dpalette_pixel[dot];
	dot = ((g & 0x1) << 2) | ((r & 0x1) << 1) | (b & 0x1);
	p[7] = dpalette_pixel[dot];
}

inline void DISPLAY::GETVRAM_256k(int yoff, scrntype_t *p, uint32_t mask,  uint32_t yoff_d1, uint32_t yoff_d2)
{
	register uint32_t b3, r3, g3;
	register uint32_t b4, r4, g4;
	register uint32_t btmp, rtmp, gtmp;
	
	register scrntype_t b, r, g;
	scrntype_t pixel;
	uint32_t _bit;
	int _shift;
	int cp;
	if(p == NULL) return;
	
	r3 = g3 = b3 = 0;
	r4 = g4 = b4 = 0;
	r = g = b = 0;
	
	//yoff_d1 = offset_point_bak;
	//yoff_d2 = offset_point_bank1_bak;
	yoff_d1 = (yoff + yoff_d1) & 0x1fff;
	yoff_d2 = (yoff + yoff_d2) & 0x1fff;
	if(mask & 0x01) {
		b3  = gvram_shadow[yoff_d1] << 24;
		b3 |= gvram_shadow[yoff_d1 + 0x02000] << 16;
		
		b3 |= gvram_shadow[yoff_d2 + 0x0c000] << 8;
		b3 |= gvram_shadow[yoff_d2 + 0x0e000] << 0;
	
		b4  = gvram_shadow[yoff_d1 + 0x18000] << 8;
		b4 |= gvram_shadow[yoff_d1 + 0x1a000] << 0;
	}
	if(mask & 0x02) {
		r3  = gvram_shadow[yoff_d1 + 0x04000] << 24;
		r3 |= gvram_shadow[yoff_d1 + 0x06000] << 16;
		r3 |= gvram_shadow[yoff_d2 + 0x10000] << 8;
		r3 |= gvram_shadow[yoff_d2 + 0x12000] << 0;
		r4  = gvram_shadow[yoff_d1 + 0x1c000] << 8;
		r4 |= gvram_shadow[yoff_d1 + 0x1e000] << 0;
	}

	if(mask & 0x04) {
		g3  = gvram_shadow[yoff_d1 + 0x08000] << 24;
		g3 |= gvram_shadow[yoff_d1 + 0x0a000] << 16;
		g3 |= gvram_shadow[yoff_d2 + 0x14000] << 8;
		g3 |= gvram_shadow[yoff_d2 + 0x16000] << 0;
		
		g4  = gvram_shadow[yoff_d1 + 0x20000] << 8;
		g4 |= gvram_shadow[yoff_d1 + 0x22000] << 0;
	}
	
	cp = 0;
	for(_shift = 7; _shift >= 0; _shift--) {
		_bit = 0x01010101 << _shift;
		r = g = b = 0;
		if(mask & 0x01) {
			btmp = (b3 & _bit) >> _shift;
			b = (((btmp & (0x01 << 24)) != 0) ? 0x80 : 0) | (((btmp & (0x01 << 16)) != 0)? 0x40 : 0)
				| (((btmp & (0x01 << 8)) != 0) ? 0x20 : 0) | (((btmp & 0x01) != 0) ? 0x10   : 0);
			btmp = (b4 & _bit) >> _shift;
			b = b | (((btmp & (0x01 << 8)) != 0) ? 0x08 : 0) | (((btmp & 0x01) != 0) ? 0x04 : 0);
		}
		if(mask & 0x02) {
			rtmp = (r3 & _bit) >> _shift;
			r = ((rtmp & (0x01 << 24)) ? 0x80 : 0) | ((rtmp & (0x01 << 16)) ? 0x40 : 0)
				| ((rtmp & (0x01 << 8)) ? 0x20 : 0) | ((rtmp & 0x01) ? 0x10   : 0);
			rtmp = (r4 & _bit) >> _shift;
			r = r | ((rtmp & (0x01 << 8)) ? 0x08 : 0) | ((rtmp & 0x01) ? 0x04 : 0);
		}
		if(mask & 0x04) {
			gtmp = (g3 & _bit) >> _shift;
			g = ((gtmp & (0x01 << 24)) ? 0x80 : 0) | ((gtmp & (0x01 << 16)) ? 0x40 : 0)
				| ((gtmp & (0x01 << 8)) ? 0x20 : 0) | ((gtmp & 0x01) ? 0x10   : 0);
			gtmp = (g4 & _bit) >> _shift;
			g = g | ((gtmp & (0x01 << 8)) ? 0x08 : 0) | ((gtmp & 0x01) ? 0x04 : 0);
		}
	
		pixel = RGB_COLOR(r, g, b);
		p[cp] = pixel;
		//p[cp + 1] = pixel;
		cp += 1;
	}
	
}
#endif

#if defined(_FM77AV_VARIANTS)
inline void DISPLAY::GETVRAM_4096(int yoff, scrntype_t *p, uint32_t mask, uint32_t yoff_d1, uint32_t yoff_d2,
								  bool window_inv = false)
{
	uint32_t b3, r3, g3;
	scrntype_t b, r, g;
	uint32_t idx;;
	scrntype_t pixel;
# if defined(_FM77AV40EX) || defined(_FM77AV40SX)
	int dpage = vram_display_block;
# endif
	if(p == NULL) return;
	
	//yoff_d1 = offset_point_bak;
	//yoff_d2 = offset_point_bank1_bak;
	yoff_d1 = (yoff + yoff_d1) & 0x1fff;
	yoff_d2 = (yoff + yoff_d2) & 0x1fff;
# if defined(_FM77AV40EX) || defined(_FM77AV40SX)
	if(window_inv) {
		if(dpage == 0) {
			dpage = 1;
		} else {
			dpage = 0;
		}
	}
	if(dpage != 0) {
		yoff_d1 += 0x18000;
		yoff_d2 += 0x18000;
	}
# endif

	b3  = gvram_shadow[yoff_d1] << 24;
	b3 |= gvram_shadow[yoff_d1 + 0x02000] << 16;
	r3  = gvram_shadow[yoff_d1 + 0x04000] << 24;
	r3 |= gvram_shadow[yoff_d1 + 0x06000] << 16;
		
	g3  = gvram_shadow[yoff_d1 + 0x08000] << 24;
	g3 |= gvram_shadow[yoff_d1 + 0x0a000] << 16;
		
	b3 |= gvram_shadow[yoff_d2 + 0x0c000] << 8;
	b3 |= gvram_shadow[yoff_d2 + 0x0e000] << 0;
		
	r3 |= gvram_shadow[yoff_d2 + 0x10000] << 8;
	r3 |= gvram_shadow[yoff_d2 + 0x12000] << 0;
	g3 |= gvram_shadow[yoff_d2 + 0x14000] << 8;
	g3 |= gvram_shadow[yoff_d2 + 0x16000] << 0;
   
	g = ((g3 & (0x80 << 24)) >> 20) | ((g3 & (0x80 << 16)) >> 13) | ((g3 & (0x80 << 8)) >> 6)  | ((g3 & 0x80) << 1);
	r = ((r3 & (0x80 << 24)) >> 24) | ((r3 & (0x80 << 16)) >> 17) | ((r3 & (0x80 << 8)) >> 10) | ((r3 & 0x80) >> 3);
	b = ((b3 & (0x80 << 24)) >> 28) | ((b3 & (0x80 << 16)) >> 21) | ((b3 & (0x80 << 8)) >> 14) | ((b3 & 0x80) >> 7);
	   
	idx = (g  | b | r ) & mask;
	pixel = analog_palette_pixel[idx];
	p[0] = pixel;
	//p[1] = pixel;

	g = ((g3 & (0x40 << 24)) >> 19) | ((g3 & (0x40 << 16)) >> 12) | ((g3 & (0x40 << 8)) >> 5)  | ((g3 & 0x40) << 2);
	r = ((r3 & (0x40 << 24)) >> 23) | ((r3 & (0x40 << 16)) >> 16) | ((r3 & (0x40 << 8)) >> 9)  | ((r3 & 0x40) >> 2);
	b = ((b3 & (0x40 << 24)) >> 27) | ((b3 & (0x40 << 16)) >> 20) | ((b3 & (0x40 << 8)) >> 13) | ((b3 & 0x40) >> 6);
	
	idx = (g  | b | r ) & mask;
	pixel = analog_palette_pixel[idx];
	p[1] = pixel;
	//p[3] = pixel;

	g = ((g3 & (0x20 << 24)) >> 18) | ((g3 & (0x20 << 16)) >> 11) | ((g3 & (0x20 << 8)) >> 4)  | ((g3 & 0x20) << 3);
	r = ((r3 & (0x20 << 24)) >> 22) | ((r3 & (0x20 << 16)) >> 15) | ((r3 & (0x20 << 8)) >> 8)  | ((r3 & 0x20) >> 1);
	b = ((b3 & (0x20 << 24)) >> 26) | ((b3 & (0x20 << 16)) >> 19) | ((b3 & (0x20 << 8)) >> 12) | ((b3 & 0x20) >> 5);
	   
	idx = (g  | b | r ) & mask;
	pixel = analog_palette_pixel[idx];
	p[2] = pixel;
	//p[5] = pixel;

	g = ((g3 & (0x10 << 24)) >> 17) | ((g3 & (0x10 << 16)) >> 10) | ((g3 & (0x10 << 8)) >> 3)  | ((g3 & 0x10) << 4);
	r = ((r3 & (0x10 << 24)) >> 21) | ((r3 & (0x10 << 16)) >> 14) | ((r3 & (0x10 << 8)) >> 7)  | ((r3 & 0x10) >> 0);
	b = ((b3 & (0x10 << 24)) >> 25) | ((b3 & (0x10 << 16)) >> 18) | ((b3 & (0x10 << 8)) >> 11) | ((b3 & 0x10) >> 4);
	   
	idx = (g  | b | r ) & mask;
	pixel = analog_palette_pixel[idx];
	p[3] = pixel;
	//p[7] = pixel;

	g = ((g3 & (0x8 << 24)) >> 16) | ((g3 & (0x8 << 16)) >> 9)  | ((g3 & (0x8 << 8)) >> 2)  | ((g3 & 0x8) << 5);
	r = ((r3 & (0x8 << 24)) >> 20) | ((r3 & (0x8 << 16)) >> 13) | ((r3 & (0x8 << 8)) >> 6)  | ((r3 & 0x8) << 1);
	b = ((b3 & (0x8 << 24)) >> 24) | ((b3 & (0x8 << 16)) >> 17) | ((b3 & (0x8 << 8)) >> 10) | ((b3 & 0x8) >> 3);
	   
	idx = (g  | b | r ) & mask;
	pixel = analog_palette_pixel[idx];
	p[4] = pixel;
	//p[9] = pixel;

	
	g = ((g3 & (0x4 << 24)) >> 15) | ((g3 & (0x4 << 16)) >> 8)  | ((g3 & (0x4 << 8)) >> 1) | ((g3 & 0x4) << 6);
	r = ((r3 & (0x4 << 24)) >> 19) | ((r3 & (0x4 << 16)) >> 12) | ((r3 & (0x4 << 8)) >> 5) | ((r3 & 0x4) << 2);
	b = ((b3 & (0x4 << 24)) >> 23) | ((b3 & (0x4 << 16)) >> 16) | ((b3 & (0x4 << 8)) >> 9) | ((b3 & 0x4) >> 2);
	   
	idx = (g  | b | r ) & mask;
	pixel = analog_palette_pixel[idx];
	p[5] = pixel;
	//p[11] = pixel;

	g = ((g3 & (0x2 << 24)) >> 14) | ((g3 & (0x2 << 16)) >> 7)  | ((g3 & (0x2 << 8)) >> 0) | ((g3 & 0x2) << 7);
	r = ((r3 & (0x2 << 24)) >> 18) | ((r3 & (0x2 << 16)) >> 11) | ((r3 & (0x2 << 8)) >> 4) | ((r3 & 0x2) << 3);
	b = ((b3 & (0x2 << 24)) >> 22) | ((b3 & (0x2 << 16)) >> 15) | ((b3 & (0x2 << 8)) >> 8) | ((b3 & 0x2) >> 1);
	   
	idx = (g  | b | r ) & mask;
	pixel = analog_palette_pixel[idx];
	p[6] = pixel;
	//p[13] = pixel;

	g = ((g3 & (0x1 << 24)) >> 13) | ((g3 & (0x1 << 16)) >> 6)  | ((g3 & (0x1 << 8)) << 1) | ((g3 & 0x1) << 8);
	r = ((r3 & (0x1 << 24)) >> 17) | ((r3 & (0x1 << 16)) >> 10) | ((r3 & (0x1 << 8)) >> 3) | ((r3 & 0x1) << 4);
	b = ((b3 & (0x1 << 24)) >> 21) | ((b3 & (0x1 << 16)) >> 14) | ((b3 & (0x1 << 8)) >> 7) | ((b3 & 0x1) >> 0);
	   
	idx = (g  | b | r ) & mask;
	pixel = analog_palette_pixel[idx];
	p[7] = pixel;
	//p[15] = pixel;
}
#endif


void DISPLAY::draw_screen()
{
//#if !defined(_FM77AV_VARIANTS)
	this->draw_screen2();
//#endif	
}

void DISPLAY::draw_screen2()
{
	int y;
	int x;
	scrntype_t *p, *pp;
	register int yoff;
	register uint32_t rgbmask;
	uint32_t yoff_d1, yoff_d2;
	uint16_t wx_begin, wx_end, wy_low, wy_high;
	
#if defined(_FM77AV40EX) || defined(_FM77AV40SX)
	{
		wx_begin = window_xbegin;
		wx_end   = window_xend;
		wy_low   = window_low;
		wy_high  = window_high;
		bool _flag = window_opened; 
		if((wx_begin < wx_end) && (wy_low < wy_high)) {
			window_opened = true;
		} else {
			window_opened = false;
		}
		if(_flag != window_opened) vram_wrote_shadow = true;
	}
#endif
	frame_skip_count++;
#if defined(_FM77AV_VARIANTS)
	{
		int factor = (config.dipswitch & FM7_DIPSW_FRAMESKIP) >> 28;
		if(frame_skip_count < factor) return;
		frame_skip_count = 0;
	}
	//offset_point_bank1_bak = offset_point_bank1;
	//offset_point_bak = offset_point;
	yoff_d2 = offset_point_bank1_bak;
	yoff_d1 = offset_point_bak;
#else
	{
		int factor = (config.dipswitch & FM7_DIPSW_FRAMESKIP) >> 28;
		if((frame_skip_count < factor) || !(vram_wrote_shadow)) return;
		//vram_wrote = false;
		frame_skip_count = 0;
	}
	yoff_d1 = yoff_d2 = offset_point;
#endif
	  // Set blank
	if(!crt_flag) {
		if(crt_flag_bak) {
			scrntype_t *ppp;
			if(display_mode == DISPLAY_MODE_8_200L) {
				emu->set_vm_screen_size(640, 200, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_WIDTH_ASPECT, WINDOW_HEIGHT_ASPECT);
				for(y = 0; y < 200; y++) {
					vram_draw_table[y] = false;
					ppp = emu->get_screen_buffer(y);
					if(ppp != NULL) memset(ppp, 0x00, 640 * sizeof(scrntype_t));
				}
			} else if(display_mode == DISPLAY_MODE_8_400L) {
				emu->set_vm_screen_size(640, 400, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_WIDTH_ASPECT, WINDOW_HEIGHT_ASPECT);
				for(y = 0; y < 400; y++) {
					vram_draw_table[y] = false;
					ppp = emu->get_screen_buffer(y);
					if(ppp != NULL) memset(ppp, 0x00, 640 * sizeof(scrntype_t));
				}
			} else { // 320x200
				emu->set_vm_screen_size(320, 200, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_WIDTH_ASPECT, WINDOW_HEIGHT_ASPECT);
				for(y = 0; y < 200; y++) {
					vram_draw_table[y] = false;
					ppp = emu->get_screen_buffer(y);
					if(ppp != NULL) memset(ppp, 0x00, 320 * sizeof(scrntype_t));
				}
			}
		}
		crt_flag_bak = crt_flag;
		return;
	}
	crt_flag_bak = crt_flag;
	if(!vram_wrote_shadow) return;
	vram_wrote_shadow = false;
	if(display_mode == DISPLAY_MODE_8_200L) {
		emu->set_vm_screen_size(640, 200, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_WIDTH_ASPECT, WINDOW_HEIGHT_ASPECT);
		yoff = 0;
		rgbmask = ~multimode_dispmask;
		for(y = 0; y < 200; y ++) {
#if defined(_FM77AV_VARIANTS)
			if(!vram_draw_table[y]) continue;
			vram_draw_table[y] = false;
#endif			
			p = emu->get_screen_buffer(y);
			if(p == NULL) continue;
			pp = p;
			yoff = y  * 80;
# if defined(_FM77AV40EX) || defined(_FM77AV40SX)
			if(window_opened && (wy_low <= y) && (wy_high > y)) {
					for(x = 0; x < 80; x++) {
						if((x >= wx_begin) && (x < wx_end)) {
							GETVRAM_8_200L(yoff, p, rgbmask, yoff_d1, yoff_d2, true);
						} else {
							GETVRAM_8_200L(yoff, p, rgbmask, yoff_d1, yoff_d2, false);
						}
						p += 8;
						yoff++;
					}
			} else
# endif
			{
				for(x = 0; x < 10; x++) {
					GETVRAM_8_200L(yoff + 0, p, rgbmask, yoff_d1, yoff_d2, false);
					p += 8;
					
					GETVRAM_8_200L(yoff + 1, p, rgbmask, yoff_d1, yoff_d2, false);
					p += 8;
					
					GETVRAM_8_200L(yoff + 2, p, rgbmask, yoff_d1, yoff_d2, false);
					p += 8;
					
					GETVRAM_8_200L(yoff + 3, p, rgbmask, yoff_d1, yoff_d2, false);
					p += 8;
					
					GETVRAM_8_200L(yoff + 4, p, rgbmask, yoff_d1, yoff_d2, false);
					p += 8;
					
					GETVRAM_8_200L(yoff + 5, p, rgbmask, yoff_d1, yoff_d2, false);
					p += 8;
						
					GETVRAM_8_200L(yoff + 6, p, rgbmask, yoff_d1, yoff_d2, false);
					p += 8;
			  
					GETVRAM_8_200L(yoff + 7, p, rgbmask, yoff_d1, yoff_d2, false);
					p += 8;
					yoff += 8;
				}
			}
		}
		return;
	}
# if defined(_FM77AV_VARIANTS)
	if(display_mode == DISPLAY_MODE_4096) {
		emu->set_vm_screen_size(320, 200, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_WIDTH_ASPECT, WINDOW_HEIGHT_ASPECT);
		uint32_t mask = 0;
		yoff = 0;
		rgbmask = multimode_dispmask;
		if((rgbmask & 0x01) == 0) mask = 0x00f;
		if((rgbmask & 0x02) == 0) mask = mask | 0x0f0;
		if((rgbmask & 0x04) == 0) mask = mask | 0xf00;
		for(y = 0; y < 200; y ++) {
			if(!vram_draw_table[y]) continue;
			vram_draw_table[y] = false;

			p = emu->get_screen_buffer(y);
			if(p == NULL) continue;
			pp = p;
			yoff = y * 40;
#  if defined(_FM77AV40EX) || defined(_FM77AV40SX)
			if(window_opened && (wy_low <= y) && (wy_high > y)) {
					for(x = 0; x < 40; x++) {
						if((x >= wx_begin) && (x < wx_end)) {
							GETVRAM_4096(yoff, p, mask, yoff_d1, yoff_d2, true);
						} else {
							GETVRAM_4096(yoff, p, mask, yoff_d1, yoff_d2, false);
						}
						p += 8;
						yoff++;
					}
			} else
#  endif
			{
				for(x = 0; x < 5; x++) {
					GETVRAM_4096(yoff + 0, p, mask, yoff_d1, yoff_d2);
					p += 8;
					
					GETVRAM_4096(yoff + 1, p, mask, yoff_d1, yoff_d2);
					p += 8;
					
					GETVRAM_4096(yoff + 2, p, mask, yoff_d1, yoff_d2);
					p += 8;
					
					GETVRAM_4096(yoff + 3, p, mask, yoff_d1, yoff_d2);
					p += 8;
					
					GETVRAM_4096(yoff + 4, p, mask, yoff_d1, yoff_d2);
					p += 8;
			  
					GETVRAM_4096(yoff + 5, p, mask, yoff_d1, yoff_d2);
					p += 8;
					
					GETVRAM_4096(yoff + 6, p, mask, yoff_d1, yoff_d2);
					p += 8;
					
					GETVRAM_4096(yoff + 7, p, mask, yoff_d1, yoff_d2);
					p += 8;
					yoff += 8;
				}
			}
		}
		return;
	}
#  if defined(_FM77AV40) || defined(_FM77AV40EX) || defined(_FM77AV40SX)
	else if(display_mode == DISPLAY_MODE_8_400L) {
		emu->set_vm_screen_size(640, 400, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_WIDTH_ASPECT, WINDOW_HEIGHT_ASPECT);
		yoff = 0;
		rgbmask = ~multimode_dispmask;
		for(y = 0; y < 400; y++) {
			if(!vram_draw_table[y]) continue;
			vram_draw_table[y] = false;

			p = emu->get_screen_buffer(y);
			if(p == NULL) continue;
			pp = p;
			yoff = y  * 80;
#    if defined(_FM77AV40EX) || defined(_FM77AV40SX)
			if(window_opened && (wy_low <= y) && (wy_high  > y)) {
				for(x = 0; x < 80; x++) {
					if((x >= wx_begin) && (x < wx_end)) {
						GETVRAM_8_400L(yoff, p, rgbmask, yoff_d1, yoff_d2, true);
					} else {
						GETVRAM_8_400L(yoff, p, rgbmask, yoff_d1, yoff_d2, false);
					}
					p += 8;
					yoff++;
				}
			} else
#    endif
			for(x = 0; x < 10; x++) {
				GETVRAM_8_400L(yoff + 0, p, rgbmask, yoff_d1, yoff_d2);
				p += 8;
			  
				GETVRAM_8_400L(yoff + 1, p, rgbmask, yoff_d1, yoff_d2);
				p += 8;

				GETVRAM_8_400L(yoff + 2, p, rgbmask, yoff_d1, yoff_d2);
				p += 8;

				GETVRAM_8_400L(yoff + 3, p, rgbmask, yoff_d1, yoff_d2);
				p += 8;

				GETVRAM_8_400L(yoff + 4, p, rgbmask, yoff_d1, yoff_d2);
				p += 8;
			  
				GETVRAM_8_400L(yoff + 5, p, rgbmask, yoff_d1, yoff_d2);
				p += 8;
			  
				GETVRAM_8_400L(yoff + 6, p, rgbmask, yoff_d1, yoff_d2);
				p += 8;
			  
				GETVRAM_8_400L(yoff + 7, p, rgbmask, yoff_d1, yoff_d2);
				p += 8;
				yoff += 8;
			}
		}
		return;
	} else if(display_mode == DISPLAY_MODE_256k) {
		emu->set_vm_screen_size(320, 200, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_WIDTH_ASPECT, WINDOW_HEIGHT_ASPECT);
		rgbmask = ~multimode_dispmask;
		for(y = 0; y < 200; y++) {
			if(!vram_draw_table[y]) continue;
			vram_draw_table[y] = false;
			p = emu->get_screen_buffer(y);
			if(p == NULL) continue;
			pp = p;
			yoff = y * 40;
			{
				for(x = 0; x < 5; x++) {
					GETVRAM_256k(yoff + 0, p, rgbmask, yoff_d1, yoff_d2);
					p += 8;
					
					GETVRAM_256k(yoff + 1, p, rgbmask, yoff_d1, yoff_d2);
					p += 8;
					
					GETVRAM_256k(yoff + 2, p, rgbmask, yoff_d1, yoff_d2);
					p += 8;
					
					GETVRAM_256k(yoff + 3, p, rgbmask, yoff_d1, yoff_d2);
					p += 8;
					
					GETVRAM_256k(yoff + 4, p, rgbmask, yoff_d1, yoff_d2);
					p += 8;
			  
					GETVRAM_256k(yoff + 5, p, rgbmask, yoff_d1, yoff_d2);
					p += 8;
					
					GETVRAM_256k(yoff + 6, p, rgbmask, yoff_d1, yoff_d2);
					p += 8;
					
					GETVRAM_256k(yoff + 7, p, rgbmask, yoff_d1, yoff_d2);
					p += 8;
					yoff += 8;
				}
			}
		}
		return;
	}
#  endif // _FM77AV40
# endif //_FM77AV_VARIANTS
}

bool DISPLAY::screen_update(void)
{
	if(crt_flag) {
		bool f = screen_update_flag;
		screen_update_flag = false;
		return f;
	} else {
		if(crt_flag_bak) return true;
	}
	return false;
}

void DISPLAY::reset_screen_update(void)
{
	screen_update_flag = false;
}
