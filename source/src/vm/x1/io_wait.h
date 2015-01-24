/*
	SHARP X1 Emulator 'eX1'
	SHARP X1twin Emulator 'eX1twin'
	SHARP X1turbo Emulator 'eX1turbo'

	Author : Takeda.Toshiya
	Date   : 2013.07.27-

	[ vram wait tables ]
*/

#ifndef _IO_WAIT_H_
#define _IO_WAIT_H_

// based on x1_vwait_20120507 by Mr.Sato (http://x1center.org/)

static const char vram_wait_40[] = {
	6,5,4,2,5,4,3,6,4,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,
	2,6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,4,2,5,4,3,6,4,
	3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,
	4,3,6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,4,2,5,4,3,6,4,3,2,5,4,2,5,4,3,6,
	5,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,3,2,
	5,4,3,6,4,3,6,5,4,2,5,4,3,6,4,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,5,4,3,
	6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,4,
	2,5,4,3,6,4,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,6,4,
	3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,3,2,5,4,3,6,4,3,2,5,
	4,2,5,4,3,6,5,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,2,6,
	4,3,6,5,3,2,5,4,3,6,4,3,6,5,3,2,5,4,3,6,4,3,2,5,4,2,5,4,3,6,4,3,2,
	5,4,2,5,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,3,
	6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,4,2,5,4,3,6,4,3,2,5,4,2,5,4,3,6,5,3,
	2,5,4,2,5,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,3,2,5,4,
	3,6,4,3,6,5,4,2,5,4,3,6,4,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,5,4,3,6,5,
	3,2,5,4,2,6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,4,2,5,
	4,3,6,4,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,6,4,3,6,
	5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,4,2,5,4,3,6,4,3,2,5,4,2,
	5,4,3,6,5,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,2,6,4,3,
	6,5,3,2,5,4,3,6,4,3,6,5,3,2,5,4,3,6,4,3,2,5,4,2,5,4,3,6,5,3,2,5,4,
	2,5,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,3,6,4,
	3,6,5,3,2,5,4,3,6,4,3,2,5,4,2,5,4,3,6,4,3,2,5,4,2,5,4,3,6,5,3,2,5,
	4,2,6,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,3,2,5,4,3,6,
	4,3,6,5,4,2,5,4,3,6,4,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,5,4,3,6,5,3,2,
	5,4,2,6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,4,2,5,4,3,
	6,4,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,
	2,5,4,3,6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,4,2,5,4,3,6,4,3,2,5,4,2,5,4,
	3,6,5,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,2,6,4,3,6,5,
	3,2,5,4,3,6,4,3,6,5,4,2,5,4,3,6,4,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,5,
	4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,3,6,4,3,6,
	5,3,2,5,4,3,6,4,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,
	6,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,3,2,5,4,3,6,4,3,
	6,5,4,2,5,4,3,6,4,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,
	2,6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,4,2,5,4,3,6,4,
	3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,
	4,3,6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,4,2,5,4,3,6,4,3,2,5,4,2,5,4,3,6,
	5,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,3,2,
	5,4,3,6,4,3,6,5,4,2,5,4,3,6,4,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,5,4,3,
	6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,4,
	2,5,4,3,6,4,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,6,4,
	3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,3,2,5,4,3,6,4,3,2,5,
	4,2,5,4,3,6,5,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,2,6,
	4,3,6,5,3,2,5,4,3,6,4,3,6,5,3,2,5,4,3,6,4,3,2,5,4,2,5,4,3,6,4,3,2,
	5,4,2,5,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,3,
	6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,4,2,5,4,3,6,4,3,2,5,4,2,5,4,3,6,5,3,
	2,5,4,2,5,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,3,2,5,4,
	3,6,4,3,6,5,4,2,5,4,3,6,4,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,5,4,3,6,5,
	3,2,5,4,2,6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,4,2,5,
	4,3,6,4,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,6,4,3,6,
	5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,4,2,5,4,3,6,4,3,2,5,4,2,
	5,4,3,6,5,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,2,6,4,3,
	6,5,3,2,5,4,3,6,4,3,6,5,3,2,5,4,3,6,4,3,2,5,4,2,5,4,3,6,5,3,2,5,4,
	2,5,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,3,6,4,
	3,6,5,3,2,5,4,3,6,4,3,2,5,4,2,5,4,3,6,4,3,2,5,4,2,5,4,3,6,5,3,2,5,
	4,2,6,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,3,2,5,4,3,6,
	4,3,6,5,4,2,5,4,3,6,4,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,5,4,3,6,5,3,2,
	5,4,2,6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,4,2,5,4,3,
	6,4,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,
	2,5,4,3,6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,4,2,5,4,3,6,4,3,2,5,4,2,5,4,
	3,6,5,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,2,6,4,3,6,5,
	3,2,5,4,3,6,4,3,6,5,4,2,5,4,3,6,4,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,5,
	4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,3,6,4,3,6,
	5,3,2,5,4,3,6,4,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,5,4,3,6,5,3,2,5,4,2,
	6,4,3,6,5,3,2,5,4,2,6,4,3,6,5,3,2,5,4,3,6,4,3,6,5,3,2,5,4,3,6,4,3,
};

static const char vram_wait_80[] = {
	3,2,2,1,2,1,2,2,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,
	1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,2,2,1,2,1,2,2,1,
	2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,
	3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,2,2,1,2,1,2,2,1,2,1,2,2,1,2,1,2,3,
	1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,
	2,3,1,2,1,2,3,1,2,1,2,1,2,2,1,2,1,2,2,1,2,1,2,3,1,2,1,2,3,1,2,1,2,
	3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,
	1,2,3,2,2,1,2,1,2,2,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,
	2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,2,2,1,2,1,2,
	2,1,2,1,2,2,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,
	1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,1,2,2,1,2,1,2,2,1,2,1,
	2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,
	2,1,2,3,1,2,1,2,3,1,2,1,2,3,2,2,1,2,1,2,2,1,2,1,2,3,1,2,1,2,3,1,2,
	1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,
	1,2,1,2,3,2,2,1,2,1,2,2,1,2,1,2,2,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,
	2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,
	3,2,2,1,2,1,2,2,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,
	1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,2,2,1,2,1,2,2,1,
	2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,
	3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,2,2,1,2,1,2,2,1,2,1,2,2,1,2,1,2,3,
	1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,
	2,3,1,2,1,2,3,1,2,1,2,1,2,2,1,2,1,2,2,1,2,1,2,3,1,2,1,2,3,1,2,1,2,
	3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,
	1,2,3,2,2,1,2,1,2,2,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,
	2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,2,2,1,2,1,2,
	2,1,2,1,2,2,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,
	1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,1,2,2,1,2,1,2,2,1,2,1,
	2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,
	2,1,2,3,1,2,1,2,3,1,2,1,2,3,2,2,1,2,1,2,2,1,2,1,2,3,1,2,1,2,3,1,2,
	1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,
	1,2,1,2,3,2,2,1,2,1,2,2,1,2,1,2,2,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,
	2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,
	3,2,2,1,2,1,2,2,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,
	1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,2,2,1,2,1,2,2,1,
	2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,
	3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,2,2,1,2,1,2,2,1,2,1,2,2,1,2,1,2,3,
	1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,
	2,3,1,2,1,2,3,1,2,1,2,1,2,2,1,2,1,2,2,1,2,1,2,3,1,2,1,2,3,1,2,1,2,
	3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,
	1,2,3,2,2,1,2,1,2,2,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,
	2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,2,2,1,2,1,2,
	2,1,2,1,2,2,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,
	1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,1,2,2,1,2,1,2,2,1,2,1,
	2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,
	2,1,2,3,1,2,1,2,3,1,2,1,2,3,2,2,1,2,1,2,2,1,2,1,2,3,1,2,1,2,3,1,2,
	1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,
	1,2,1,2,3,2,2,1,2,1,2,2,1,2,1,2,2,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,
	2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,
	3,2,2,1,2,1,2,2,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,
	1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,2,2,1,2,1,2,2,1,
	2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,
	3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,2,2,1,2,1,2,2,1,2,1,2,2,1,2,1,2,3,
	1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,
	2,3,1,2,1,2,3,1,2,1,2,1,2,2,1,2,1,2,2,1,2,1,2,3,1,2,1,2,3,1,2,1,2,
	3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,
	1,2,3,2,2,1,2,1,2,2,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,
	2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,2,2,1,2,1,2,
	2,1,2,1,2,2,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,
	1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,1,2,2,1,2,1,2,2,1,2,1,
	2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,
	2,1,2,3,1,2,1,2,3,1,2,1,2,3,2,2,1,2,1,2,2,1,2,1,2,3,1,2,1,2,3,1,2,
	1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,
	1,2,1,2,3,2,2,1,2,1,2,2,1,2,1,2,2,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,
	2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,
};

#endif

