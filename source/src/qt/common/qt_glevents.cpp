/*
 * qt_events.cpp
 * (c) 2011 K.Ohta <whatisthis.sowhat@gmail.com>
 * Modified to Common Source code Project, License is changed to GPLv2.
 * 
 */


#include "emu.h"

#include <QtGui>
#include <QOpenGLWidget>
#include <QMouseEvent>
#ifdef _WINDOWS
#include <GL/gl.h>
#include <GL/glext.h>
#else
#include <GL/glx.h>
#include <GL/glxext.h>
#endif
#include <GL/glu.h>
#include "qt_gldraw.h"

void GLDrawClass::mouseMoveEvent(QMouseEvent *event)
{
	int xpos = event->x();
	int ypos = event->y();
	int d_ww, d_hh;
	int c_ww, c_hh;

	if((xpos < 0) || (ypos < 0)) return;
	if(draw_width >= this->width()) {
		d_ww = this->width();
		c_ww = this->width();
	} else {
		d_ww = draw_width;
		c_ww = this->width();
	}
	if(draw_height >= this->height()) {
		d_hh = this->height();
		c_hh = this->height();
	} else {
		d_hh = draw_height;
		c_hh = this->height();
	}
	int left = (c_ww - d_ww) / 2;
	int right = (c_ww - d_ww) / 2 + d_ww;
	int up = (c_hh - d_hh) / 2;
	int down = (c_hh - d_hh) / 2 + d_hh;

	if((xpos < left) || (xpos >= right)) return;
	if((ypos < up) || (ypos >= down)) return;

	xpos = xpos - left;
	ypos = ypos - up;
	double xx;
	double yy;
#if defined(USE_SCREEN_ROTATE)
	if(config.rotate_type) {
		xx = (double)ypos * ((double)SCREEN_WIDTH / (double)d_hh);
		yy = (double)xpos * ((double)SCREEN_HEIGHT / (double)d_ww);
	} else
#endif	  
	{
		xx = (double)xpos * ((double)SCREEN_WIDTH / (double)d_ww);
		yy = (double)ypos * ((double)SCREEN_HEIGHT / (double)d_hh);
	}

	emit do_notify_move_mouse((int)xx, (int) yy);
}

void GLDrawClass::mousePressEvent(QMouseEvent *event)
{
	emit do_notify_button_pressed(event->button());
}

void GLDrawClass::mouseReleaseEvent(QMouseEvent *event)
{
	emit do_notify_button_released(event->button());
}
