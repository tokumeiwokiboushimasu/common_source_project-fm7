/*
	Skelton for retropc emulator
	Author : Takeda.Toshiya
        Port to Qt : K.Ohta <whatisthis.sowhat _at_ gmail.com>
	Date   : 2006.08.18 -

	[ win32 main ] -> [ agar main ]
*/

#include <stdio.h>
#include <string>
#include <vector>
#include <QString>
#include <QTextCodec>
#include <QImage>
#include <QImageReader>
#include <QDateTime>
#include <QDir>

#include "common.h"
#include "fileio.h"
#include "emu.h"
#include "menuclasses.h"
#include "mainwidget.h"
#include "commonclasses.h"
#include "qt_main.h"
#include "emu_thread.h"
#include "joy_thread.h"
#include "draw_thread.h"

#include "qt_gldraw.h"
#include "qt_glutil_gl2_0.h"
#include "agar_logger.h"

#include "menu_disk.h"
#include "menu_bubble.h"
#include "menu_flags.h"
#include "dialog_movie.h"
#include "../avio/movie_saver.h"
// emulation core

EMU* emu;
QApplication *GuiMain = NULL;

// Start to define MainWindow.
class META_MainWindow *rMainWindow;

// buttons
#ifdef MAX_BUTTONS
#define MAX_FONT_SIZE 32
#endif

// menu
std::string cpp_homedir;
std::string cpp_confdir;
std::string my_procname;
std::string sRssDir;
bool now_menuloop = false;
static int close_notified = 0;
// timing control

// screen
unsigned int desktop_width;
unsigned int desktop_height;
//int desktop_bpp;
int prev_window_mode = 0;
bool now_fullscreen = false;

int window_mode_count;

void Ui_MainWindow::do_set_mouse_enable(bool flag)
{
#ifdef USE_MOUSE
	if(emu == NULL) return;
	emu->lock_vm();
	if(flag) {
		graphicsView->grabMouse();
		emu->enable_mouse();
	} else {
		graphicsView->releaseMouse();
		emu->disable_mouse();
	}
	emu->unlock_vm();
#endif	
}

void Ui_MainWindow::do_toggle_mouse(void)
{
#ifdef USE_MOUSE
	if(emu == NULL) return;
	emu->lock_vm();
	bool flag = emu->is_mouse_enabled();
	if(!flag) {
		graphicsView->grabMouse();
		emu->enable_mouse();
	} else {
		graphicsView->releaseMouse();
		emu->disable_mouse();
	}
	emu->unlock_vm();
#endif	
}

void Ui_MainWindow::rise_movie_dialog(void)
{
	CSP_DialogMovie *dlg = new CSP_DialogMovie(hSaveMovieThread, using_flags);
	dlg->setWindowTitle(QApplication::translate("CSP_DialogMovie", "Configure movie encodings", 0));
	dlg->show();
}

void Ui_MainWindow::LaunchEmuThread(void)
{
	QString objNameStr;
	GLDrawClass *glv = this->getGraphicsView();

	int drvs;
	
	hRunEmu = new EmuThreadClass(rMainWindow, emu, using_flags, this);
	connect(hRunEmu, SIGNAL(message_changed(QString)), this, SLOT(message_status_bar(QString)));
	connect(hRunEmu, SIGNAL(sig_is_enable_mouse(bool)), glv, SLOT(do_set_mouse_enabled(bool)));
	connect(glv, SIGNAL(sig_key_down(uint32_t, uint32_t, bool)), hRunEmu, SLOT(do_key_down(uint32_t, uint32_t, bool)));
	connect(glv, SIGNAL(sig_key_up(uint32_t, uint32_t)),hRunEmu, SLOT(do_key_up(uint32_t, uint32_t)));
	connect(this, SIGNAL(sig_quit_widgets()), glv, SLOT(do_stop_run_vm()));

	
	//connect(hRunEmu, SIGNAL(sig_finished()), this, SLOT(delete_emu_thread()));
	connect(this, SIGNAL(sig_vm_reset()), hRunEmu, SLOT(doReset()));
	connect(this, SIGNAL(sig_vm_specialreset()), hRunEmu, SLOT(doSpecialReset()));
	connect(this, SIGNAL(sig_vm_loadstate()), hRunEmu, SLOT(doLoadState()));
	connect(this, SIGNAL(sig_vm_savestate()), hRunEmu, SLOT(doSaveState()));

	connect(this, SIGNAL(sig_emu_update_config()), hRunEmu, SLOT(doUpdateConfig()));
	connect(this, SIGNAL(sig_emu_update_volume_level(int, int)), hRunEmu, SLOT(doUpdateVolumeLevel(int, int)));
	connect(this, SIGNAL(sig_emu_update_volume_balance(int, int)), hRunEmu, SLOT(doUpdateVolumeBalance(int, int)));
	connect(this, SIGNAL(sig_emu_start_rec_sound()), hRunEmu, SLOT(doStartRecordSound()));
	connect(this, SIGNAL(sig_emu_stop_rec_sound()), hRunEmu, SLOT(doStopRecordSound()));
	connect(this, SIGNAL(sig_emu_set_display_size(int, int, int, int)), hRunEmu, SLOT(doSetDisplaySize(int, int, int, int)));
	

#if defined(USE_FD1) || defined(USE_FD2) || defined(USE_FD3) || defined(USE_FD4) || \
    defined(USE_FD5) || defined(USE_FD6) || defined(USE_FD7) || defined(USE_FD8)
	connect(this, SIGNAL(sig_write_protect_disk(int, bool)), hRunEmu, SLOT(do_write_protect_disk(int, bool)));
	connect(this, SIGNAL(sig_open_disk(int, QString, int)), hRunEmu, SLOT(do_open_disk(int, QString, int)));
	connect(this, SIGNAL(sig_close_disk(int)), hRunEmu, SLOT(do_close_disk(int)));
	connect(hRunEmu, SIGNAL(sig_update_recent_disk(int)), this, SLOT(do_update_recent_disk(int)));
	connect(hRunEmu, SIGNAL(sig_change_osd_fd(int, QString)), this, SLOT(do_change_osd_fd(int, QString)));
	drvs = 0;
# if defined(USE_FD1)
	drvs = 1;
# endif
# if defined(USE_FD2)
	drvs = 2;
# endif
# if defined(USE_FD3)
	drvs = 3;
# endif
# if defined(USE_FD4)
	drvs = 4;
# endif
# if defined(USE_FD5)
	drvs = 5;
# endif
# if defined(USE_FD6)
	drvs = 6;
# endif
# if defined(USE_FD7)
	drvs = 7;
# endif
# if defined(USE_FD8)
	drvs = 8;
# endif
	for(int ii = 0; ii < drvs; ii++) {
		menu_fds[ii]->setEmu(emu);
		connect(menu_fds[ii], SIGNAL(sig_update_inner_fd(int ,QStringList , class Action_Control **, QStringList , int, bool)),
				this, SLOT(do_update_inner_fd(int ,QStringList , class Action_Control **, QStringList , int, bool)));
	}
#endif
#if defined(USE_TAPE)
	connect(this, SIGNAL(sig_play_tape(QString)), hRunEmu, SLOT(do_play_tape(QString)));
	connect(this, SIGNAL(sig_rec_tape(QString)),  hRunEmu, SLOT(do_rec_tape(QString)));
	connect(this, SIGNAL(sig_close_tape(void)),   hRunEmu, SLOT(do_close_tape(void)));
	connect(hRunEmu, SIGNAL(sig_change_osd_cmt(QString)), this, SLOT(do_change_osd_cmt(QString)));
# if defined(USE_TAPE_BUTTON)
	connect(this, SIGNAL(sig_cmt_push_play(void)), hRunEmu, SLOT(do_cmt_push_play(void)));
	connect(this, SIGNAL(sig_cmt_push_stop(void)), hRunEmu, SLOT(do_cmt_push_stop(void)));
	connect(this, SIGNAL(sig_cmt_push_fast_forward(void)), hRunEmu, SLOT(do_cmt_push_fast_forward(void)));
	connect(this, SIGNAL(sig_cmt_push_fast_rewind(void)),  hRunEmu, SLOT(do_cmt_push_fast_rewind(void)));
	connect(this, SIGNAL(sig_cmt_push_apss_forward(void)), hRunEmu, SLOT(do_cmt_push_apss_forward(void)));
	connect(this, SIGNAL(sig_cmt_push_apss_rewind(void)),  hRunEmu, SLOT(do_cmt_push_apss_rewind(void)));
# endif
#endif
#if defined(USE_QD1)
	connect(this, SIGNAL(sig_write_protect_quickdisk(int, bool)), hRunEmu, SLOT(do_write_protect_quickdisk(int, bool)));
	connect(this, SIGNAL(sig_open_quickdisk(int, QString)), hRunEmu, SLOT(do_open_quickdisk(int, QString)));
	connect(this, SIGNAL(sig_close_quickdisk(int)), hRunEmu, SLOT(do_close_quickdisk(int)));
	connect(hRunEmu, SIGNAL(sig_change_osd_qd(int, QString)), this, SLOT(do_change_osd_qd(int, QString)));
#endif
#if defined(USE_CART1)
	connect(this, SIGNAL(sig_open_cart(int, QString)), hRunEmu, SLOT(do_open_cart(int, QString)));
	connect(this, SIGNAL(sig_close_cart(int)), hRunEmu, SLOT(do_close_cart(int)));
#endif
#if defined(USE_COMPACT_DISC)
	connect(this, SIGNAL(sig_open_cdrom(QString)), hRunEmu, SLOT(do_open_cdrom(QString)));
	connect(this, SIGNAL(sig_close_cdrom()), hRunEmu, SLOT(do_eject_cdrom()));
	connect(hRunEmu, SIGNAL(sig_change_osd_cdrom(QString)), this, SLOT(do_change_osd_cdrom(QString)));
#endif	
#if defined(USE_LASER_DISC)
	connect(this, SIGNAL(sig_open_laserdisc(QString)), hRunEmu, SLOT(do_open_laser_disc(QString)));
	connect(this, SIGNAL(sig_close_laserdisc(void)), hRunEmu, SLOT(do_close_laser_disc(void)));
#endif
#if defined(USE_BINARY_FILE1)
	connect(this, SIGNAL(sig_load_binary(int, QString)), hRunEmu, SLOT(do_load_binary(int, QString)));
	connect(this, SIGNAL(sig_save_binary(int, QString)), hRunEmu, SLOT(do_save_binary(int, QString)));
#endif
#if defined(USE_BUBBLE1)
	connect(this, SIGNAL(sig_write_protect_bubble(int, bool)), hRunEmu, SLOT(do_write_protect_bubble_casette(int, bool)));
	connect(this, SIGNAL(sig_open_bubble(int, QString, int)), hRunEmu, SLOT(do_open_bubble_casette(int, QString, int)));
	connect(this, SIGNAL(sig_close_bubble(int)), hRunEmu, SLOT(do_close_bubble_casette(int)));
	connect(hRunEmu, SIGNAL(sig_update_recent_bubble(int)), this, SLOT(do_update_recent_bubble(int)));
	connect(hRunEmu, SIGNAL(sig_change_osd_bubble(int, QString)), this, SLOT(do_change_osd_bubble(int, QString)));
	drvs = 0;
# if defined(USE_BUBBLE1)
	drvs = 1;
# endif
# if defined(USE_BUBBLE2)
	drvs = 2;
# endif
	for(int ii = 0; ii < drvs; ii++) {
		menu_bubbles[ii]->setEmu(emu);
		connect(menu_bubbles[ii],
				SIGNAL(sig_update_inner_bubble(int ,QStringList , class Action_Control **, QStringList , int, bool)),
				this,
				SLOT(do_update_inner_bubble(int ,QStringList , class Action_Control **, QStringList , int, bool)));
	}
#endif
	
	connect(this, SIGNAL(quit_emu_thread()), hRunEmu, SLOT(doExit()));
	connect(hRunEmu, SIGNAL(sig_mouse_enable(bool)),
			this, SLOT(do_set_mouse_enable(bool)));
#ifdef USE_TAPE_BUTTON
	hRunEmu->set_tape_play(false);
#endif
#ifdef USE_LED_DEVICE
	connect(hRunEmu, SIGNAL(sig_send_data_led(quint32)), this, SLOT(do_recv_data_led(quint32)));
#endif
#ifdef USE_AUTO_KEY
	connect(this, SIGNAL(sig_start_auto_key(QString)), hRunEmu, SLOT(do_start_auto_key(QString)));
	connect(this, SIGNAL(sig_stop_auto_key()), hRunEmu, SLOT(do_stop_auto_key()));
#endif	
	//connect(actionExit_Emulator, SIGNAL(triggered()), hRunEmu, SLOT(doExit()));
	csp_logger->debug_log(CSP_LOG_INFO, CSP_LOG_TYPE_GENERAL, "EmuThread : Start.");
	objNameStr = QString("EmuThreadClass");
	hRunEmu->setObjectName(objNameStr);
	
	hDrawEmu = new DrawThreadClass(emu, emu->get_osd(), this);
	emu->set_parent_handler(hRunEmu, hDrawEmu);
	
#ifdef ONE_BOARD_MICRO_COMPUTER
	QImageReader *reader = new QImageReader(":/background.png");
	QImage *result = new QImage(reader->read()); // this acts as a default if the size is not matched
	glv->updateBitmap(result);
	emu->get_osd()->upload_bitmap(result);
	delete result;
	delete reader;
	emu->get_osd()->set_buttons();
#endif
	csp_logger->debug_log(CSP_LOG_INFO, CSP_LOG_TYPE_GENERAL, "DrawThread : Start.");
	connect(hDrawEmu, SIGNAL(sig_draw_frames(int)), hRunEmu, SLOT(print_framerate(int)));
	connect(hRunEmu, SIGNAL(window_title_changed(QString)), this, SLOT(do_set_window_title(QString)));
	connect(hDrawEmu, SIGNAL(message_changed(QString)), this, SLOT(message_status_bar(QString)));
	connect(actionCapture_Screen, SIGNAL(triggered()), glv, SLOT(do_save_frame_screen()));
		
	connect(hRunEmu, SIGNAL(sig_draw_thread(bool)), hDrawEmu, SLOT(doDraw(bool)), Qt::QueuedConnection);
	//connect(hRunEmu, SIGNAL(quit_draw_thread()), hDrawEmu, SLOT(doExit()));
	connect(this, SIGNAL(quit_draw_thread()), hDrawEmu, SLOT(doExit()));

//	connect(glv, SIGNAL(sig_draw_timing(bool)), hRunEmu, SLOT(do_draw_timing(bool)));
//	connect(hDrawEmu, SIGNAL(sig_draw_timing(bool)), hRunEmu, SLOT(do_draw_timing(bool)));
	
	connect(glv, SIGNAL(do_notify_move_mouse(int, int)),
			hRunEmu, SLOT(moved_mouse(int, int)));
	connect(glv, SIGNAL(do_notify_button_pressed(Qt::MouseButton)),
	        hRunEmu, SLOT(button_pressed_mouse(Qt::MouseButton)));
	connect(glv, SIGNAL(do_notify_button_released(Qt::MouseButton)),
			hRunEmu, SLOT(button_released_mouse(Qt::MouseButton)));
#ifdef USE_MOUSE
	connect(glv, SIGNAL(sig_toggle_mouse(void)),
			this, SLOT(do_toggle_mouse(void)));
#endif
	connect(hRunEmu, SIGNAL(sig_resize_screen(int, int)),
			glv, SLOT(resizeGL(int, int)));
	
	connect(glv, SIGNAL(sig_resize_uibar(int, int)),
			this, SLOT(resize_statusbar(int, int)));
	connect(hRunEmu, SIGNAL(sig_resize_uibar(int, int)),
			this, SLOT(resize_statusbar(int, int)));

	connect(emu->get_osd(), SIGNAL(sig_req_encueue_video(int, int, int)),
			hDrawEmu, SLOT(do_req_encueue_video(int, int, int)));
	connect(hRunEmu, SIGNAL(sig_finished()), glv, SLOT(releaseKeyCode(void)));
	connect(hRunEmu, SIGNAL(sig_finished()), this, SLOT(delete_emu_thread()));
	objNameStr = QString("EmuDrawThread");
	hDrawEmu->setObjectName(objNameStr);
	hDrawEmu->start();
	csp_logger->debug_log(CSP_LOG_INFO, CSP_LOG_TYPE_GENERAL, "DrawThread : Launch done.");

	hSaveMovieThread = new MOVIE_SAVER(640, 400,  30, emu->get_osd(), using_flags->get_config_ptr());
	
	connect(actionStart_Record_Movie->binds, SIGNAL(sig_start_record_movie(int)), hRunEmu, SLOT(doStartRecordVideo(int)));
	connect(this, SIGNAL(sig_start_saving_movie()),
			actionStart_Record_Movie->binds, SLOT(do_save_as_movie()));
	connect(actionStart_Record_Movie, SIGNAL(triggered()), this, SLOT(do_start_saving_movie()));

	connect(actionStop_Record_Movie->binds, SIGNAL(sig_stop_record_movie()), hRunEmu, SLOT(doStopRecordVideo()));
	connect(this, SIGNAL(sig_stop_saving_movie()), actionStop_Record_Movie->binds, SLOT(do_stop_saving_movie()));
	connect(hSaveMovieThread, SIGNAL(sig_set_state_saving_movie(bool)), this, SLOT(do_set_state_saving_movie(bool)));
	connect(actionStop_Record_Movie, SIGNAL(triggered()), this, SLOT(do_stop_saving_movie()));

	connect(emu->get_osd(), SIGNAL(sig_save_as_movie(QString, int, int)),
			hSaveMovieThread, SLOT(do_open(QString, int, int)));
	connect(emu->get_osd(), SIGNAL(sig_stop_saving_movie()), hSaveMovieThread, SLOT(do_close()));
	
	actionStop_Record_Movie->setIcon(QIcon(":/icon_process_stop.png"));
	actionStop_Record_Movie->setVisible(false);
	
	connect(this, SIGNAL(sig_movie_set_width(int)), hSaveMovieThread, SLOT(do_set_width(int)));
	connect(this, SIGNAL(sig_movie_set_height(int)), hSaveMovieThread, SLOT(do_set_height(int)));
 
	connect(emu->get_osd(), SIGNAL(sig_movie_set_width(int)), hSaveMovieThread, SLOT(do_set_width(int)));
	connect(emu->get_osd(), SIGNAL(sig_movie_set_height(int)), hSaveMovieThread, SLOT(do_set_height(int)));
   
	connect(emu->get_osd(), SIGNAL(sig_enqueue_audio(int16_t*, int)), hSaveMovieThread, SLOT(enqueue_audio(int16_t *, int)));
	connect(emu->get_osd(), SIGNAL(sig_enqueue_video(int, int, int, QImage *)),
			hSaveMovieThread, SLOT(enqueue_video(int, int, int, QImage *)), Qt::DirectConnection);
	connect(glv->extfunc, SIGNAL(sig_push_image_to_movie(int, int, int, QImage *)),
			hSaveMovieThread, SLOT(enqueue_video(int, int, int, QImage *)));
	connect(this, SIGNAL(sig_quit_movie_thread()), hSaveMovieThread, SLOT(do_exit()));

	objNameStr = QString("EmuMovieThread");
	hSaveMovieThread->setObjectName(objNameStr);
	hSaveMovieThread->start();
	csp_logger->debug_log(CSP_LOG_INFO, CSP_LOG_TYPE_GENERAL, "MovieThread : Launch done.");

	connect(action_SetupMovie, SIGNAL(triggered()), this, SLOT(rise_movie_dialog()));


	hRunEmu->start();
	csp_logger->debug_log(CSP_LOG_INFO, CSP_LOG_TYPE_GENERAL, "EmuThread : Launch done.");
	this->set_screen_aspect(using_flags->get_config_ptr()->window_stretch_type);
	emit sig_movie_set_width(SCREEN_WIDTH);
	emit sig_movie_set_height(SCREEN_HEIGHT);
}

void Ui_MainWindow::LaunchJoyThread(void)
{
#if defined(USE_JOYSTICK)
	hRunJoy = new JoyThreadClass(emu, emu->get_osd(), using_flags, using_flags->get_config_ptr());
	connect(this, SIGNAL(quit_joy_thread()), hRunJoy, SLOT(doExit()));
	hRunJoy->setObjectName("JoyThread");
	hRunJoy->start();
#endif	
}

void Ui_MainWindow::StopJoyThread(void)
{
#if defined(USE_JOYSTICK)
	emit quit_joy_thread();
#endif	
}

void Ui_MainWindow::delete_joy_thread(void)
{
	//    delete hRunJoyThread;
	//  delete hRunJoy;
}

void Ui_MainWindow::on_actionExit_triggered()
{
	save_config(create_local_path(_T("%s.ini"), _T(CONFIG_NAME)));
	OnMainWindowClosed();
}

void Ui_MainWindow::OnWindowRedraw(void)
{
	if(emu) {
		//emu->update_screen();
	}
}

void Ui_MainWindow::OnWindowMove(void)
{
	if(emu) {
		emu->suspend();
	}
}


#ifdef USE_NOTIFY_POWER_OFF
bool Ui_MainWindow::GetPowerState(void)
{
	if(close_notified == 0) return true;
	return false;
}
#endif

void Ui_MainWindow::OnMainWindowClosed(void)
{
	// notify power off
#ifdef USE_NOTIFY_POWER_OFF
	if(emu) {
		if(!close_notified) {
			emu->lock_vm();
			emu->notify_power_off();
			emu->unlock_vm();
			close_notified = 1;
			return; 
		}
	}
#endif
	if(statusUpdateTimer != NULL) statusUpdateTimer->stop();
#ifdef USE_LED_DEVICE
	if(ledUpdateTimer != NULL) ledUpdateTimer->stop();
#endif
	emit quit_draw_thread();
	emit quit_joy_thread();
	emit quit_emu_thread();
	emit sig_quit_movie_thread();
	emit sig_quit_widgets();
	
	if(hSaveMovieThread != NULL) {
		hSaveMovieThread->wait();
		delete hSaveMovieThread;
	}
   
	if(hDrawEmu != NULL) {
		hDrawEmu->wait();
		delete hDrawEmu;
	}
	if(hRunEmu != NULL) {
		hRunEmu->wait();
		delete hRunEmu;
	}
#if defined(USE_JOYSTICK)
	if(hRunJoy != NULL) {
		hRunJoy->wait();
		delete hRunJoy;
	}
#endif	
	do_release_emu_resources();

	// release window
	if(now_fullscreen) {
		//ChangeDisplaySettings(NULL, 0);
	}
	now_fullscreen = false;
	return;
}

extern "C" {

void LostFocus(QWidget *widget)
{
	if(emu) {
		emu->key_lost_focus();
	}
}
 
}  // extern "C"

void Ui_MainWindow::do_release_emu_resources(void)
{
	if(emu) {
		delete emu;
		emu = NULL;
	}
}

extern void get_long_full_path_name(_TCHAR* src, _TCHAR* dst);
extern _TCHAR* get_parent_dir(_TCHAR* file);
extern void get_short_filename(_TCHAR *dst, _TCHAR *file, int maxlen);


static void setup_logs(void)
{
	std::string archstr;
	std::string delim;
	int  bLogSYSLOG;
	int  bLogSTDOUT;
	char    *p;

	my_procname = "emu";
	my_procname = my_procname + CONFIG_NAME;
#if defined(Q_OS_WIN)
	delim = "\\";
#else
	delim = "/";
#endif
#if !defined(Q_OS_WIN)
	p = SDL_getenv("HOME");
	if(p == NULL) {
		p = SDL_getenv("PWD");
		if(p == NULL) {
			cpp_homedir = ".";
		} else {
			cpp_homedir = p;
		}
		std::string tmpstr;
		tmpstr = "Warning : Can't get HOME directory...Making conf on " +  cpp_homedir + delim;
		perror(tmpstr.c_str());
	} else {
		cpp_homedir = p;
	}
#else
	cpp_homedir = ".";
#endif	
	cpp_homedir = cpp_homedir + delim;
#ifdef _WINDOWS
	cpp_confdir = cpp_homedir + my_procname + delim;
#else
	cpp_confdir = cpp_homedir + ".config" + delim + my_procname + delim;
#endif
	bLogSYSLOG = (int)0;
	bLogSTDOUT = (int)1;
	//csp_logger = new CSP_Logger((bLogSYSLOG != 0), (bLogSTDOUT != 0), DEVICE_NAME); // Write to syslog, console
	
	archstr = "Generic";
#if defined(__x86_64__)
	archstr = "amd64";
#endif
#if defined(__i386__)
	archstr = "ia32";
#endif
	
	csp_logger->debug_log(CSP_LOG_INFO, CSP_LOG_TYPE_GENERAL, "Start Common Source Project '%s'", my_procname.c_str());
	csp_logger->debug_log(CSP_LOG_INFO, CSP_LOG_TYPE_GENERAL, "(C) Toshiya Takeda / Qt Version K.Ohta");
	csp_logger->debug_log(CSP_LOG_INFO, CSP_LOG_TYPE_GENERAL, "Architecture: %s", archstr.c_str());
	
	//csp_logger->debug_log(AGAR_LOG_INFO, " -? is print help(s).");
	csp_logger->debug_log(CSP_LOG_INFO, CSP_LOG_TYPE_GENERAL, "Moduledir = %s home = %s", cpp_confdir.c_str(), cpp_homedir.c_str()); // Debug
#if !defined(Q_OS_CYGWIN)	
	{
		QDir dir;
		dir.mkdir( QString::fromStdString(cpp_confdir));
	}
#endif   
   //AG_MkPath(cpp_confdir.c_str());
   /* Gettext */
#ifndef RSSDIR
#if defined(_USE_AGAR) || defined(_USE_QT)
	sRssDir = "/usr/local/share/";
#else
	sRssDir = "." + delim;
#endif
	sRssDir = sRssDir + "CommonSourceCodeProject" + delim + my_procname;
#else
	sRssDir = RSSDIR;
#endif
}

int MainLoop(int argc, char *argv[], config_t *cfg)
{
	char c;
	char strbuf[2048];
	bool flag;
	char homedir[PATH_MAX];
	int thread_ret;
	int w, h;
	setup_logs();
	cpp_homedir.copy(homedir, PATH_MAX - 1, 0);
	flag = FALSE;
	csp_logger->set_emu_vm_name(DEVICE_NAME); // Write to syslog, console
	/*
	 * Into Qt's Loop.
	 */
#if defined(USE_SDL2)
	SDL_Init(SDL_INIT_AUDIO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER);
#else
	SDL_Init(SDL_INIT_AUDIO | SDL_INIT_JOYSTICK | SDL_INIT_VIDEO);
	//SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
#endif
	csp_logger->debug_log(CSP_LOG_INFO, CSP_LOG_TYPE_GENERAL, "Audio and JOYSTICK subsystem was initialised.");
	GuiMain = new QApplication(argc, argv);
	load_config(create_local_path(_T("%s.ini"), _T(CONFIG_NAME)));
	
	USING_FLAGS *using_flags = new USING_FLAGS(cfg);
	rMainWindow = new META_MainWindow(using_flags);
	rMainWindow->connect(rMainWindow, SIGNAL(sig_quit_all(void)), rMainWindow, SLOT(deleteLater(void)));
	rMainWindow->setCoreApplication(GuiMain);
	
	csp_logger->debug_log(CSP_LOG_INFO, CSP_LOG_TYPE_GENERAL, "InitInstance() OK.");
  
	// disable ime
	
	// initialize emulation core
	rMainWindow->getWindow()->show();
	emu = new EMU(rMainWindow, rMainWindow->getGraphicsView(), using_flags);
	using_flags->set_emu(emu);
	using_flags->set_osd(emu->get_osd());
	csp_logger->set_osd(emu->get_osd());

#ifdef SUPPORT_DRAG_DROP
	// open command line path
	//	if(szCmdLine[0]) {
	//	if(szCmdLine[0] == _T('"')) {
	//		int len = strlen(szCmdLine);
	//		szCmdLine[len - 1] = _T('\0');
	//		szCmdLine++;
	//	}
	//	_TCHAR path[_MAX_PATH];
	//	get_long_full_path_name(szCmdLine, path);
	//	open_any_file(path);
	//}
#endif
	
	// set priority
	
	// main loop
	GLDrawClass *pgl = rMainWindow->getGraphicsView();
	pgl->setEmuPtr(emu);
	pgl->setFixedSize(pgl->width(), pgl->height());
	
#if defined(USE_JOYSTICK)
	rMainWindow->LaunchJoyThread();
#endif	
	rMainWindow->LaunchEmuThread();
	QObject::connect(GuiMain, SIGNAL(lastWindowClosed()),
					 rMainWindow, SLOT(on_actionExit_triggered()));
	GuiMain->exec();
	return 0;
}

void Ui_MainWindow::do_update_inner_fd(int drv, QStringList base, class Action_Control **action_select_media_list,
				       QStringList lst, int num, bool use_d88_menus)
{
#if defined(USE_FD1)
	if(use_d88_menus) {
		for(int ii = 0; ii < using_flags->get_max_d88_banks(); ii++) {
			if(ii < emu->d88_file[drv].bank_num) {
				base << lst.value(ii);
				action_select_media_list[ii]->setText(lst.value(ii));
				action_select_media_list[ii]->setVisible(true);
				if(ii == num) action_select_media_list[ii]->setChecked(true);
			} else {
				if(action_select_media_list[ii] != NULL) {
					action_select_media_list[ii]->setText(QString::fromUtf8(""));
					action_select_media_list[ii]->setVisible(false);
				}
			}
		}
	}
#endif	
}

void Ui_MainWindow::do_update_inner_bubble(int drv, QStringList base, class Action_Control **action_select_media_list,
				       QStringList lst, int num, bool use_d88_menus)
{
#if defined(USE_BUBBLE1)	
	if(use_d88_menus) {
		for(int ii = 0; ii < using_flags->get_max_b77_banks(); ii++) {
			if(ii < emu->b77_file[drv].bank_num) {
				base << lst.value(ii);
				action_select_media_list[ii]->setText(lst.value(ii));
				action_select_media_list[ii]->setVisible(true);
				if(ii == num) action_select_media_list[ii]->setChecked(true);
			} else {
				if(action_select_media_list[ii] != NULL) {
					action_select_media_list[ii]->setText(QString::fromUtf8(""));
					action_select_media_list[ii]->setVisible(false);
				}
			}
		}
	}
#endif	
}

#ifdef USE_DEBUGGER
#include <../debugger/qt_debugger.h>

void Ui_MainWindow::OnOpenDebugger(int no)
{
	if((no < 0) || (no > 3)) return;
	//emu->open_debugger(no);
	VM *vm = emu->get_vm();

 	if(emu->now_debugging) 	this->OnCloseDebugger();
	if(!(emu->now_debugging && emu->debugger_thread_param.cpu_index == no)) {
		//emu->close_debugger();
		if(vm->get_cpu(no) != NULL && vm->get_cpu(no)->get_debugger() != NULL) {
			
			emu->hDebugger = new CSP_Debugger(this);
			QString objNameStr = QString("EmuDebugThread");
			emu->hDebugger->setObjectName(objNameStr);
			emu->hDebugger->debugger_thread_param.osd = emu->get_osd();
			emu->hDebugger->debugger_thread_param.vm = vm;
			emu->hDebugger->debugger_thread_param.cpu_index = no;
			emu->stop_record_sound();
			emu->stop_record_video();
			emu->now_debugging = true;
			connect(this, SIGNAL(quit_debugger_thread()), emu->hDebugger, SLOT(doExit()));
			//connect(this, SIGNAL(quit_debugger_thread()), emu->hDebugger, SLOT(close()));
			connect(emu->hDebugger, SIGNAL(sig_finished()), this, SLOT(OnCloseDebugger()));
			connect(emu->hDebugger, SIGNAL(sig_put_string(QString)), emu->hDebugger, SLOT(put_string(QString)));
			emu->hDebugger->show();
			emu->hDebugger->run();
		}
	}
}

void Ui_MainWindow::OnCloseDebugger(void )
{

//	emu->close_debugger();
 	if(emu->now_debugging) {
		if(emu->hDebugger->debugger_thread_param.running) {
			emit quit_debugger_thread();
			//emu->hDebugger->wait();
		}
		delete emu->hDebugger;
 		emu->hDebugger = NULL;
 		emu->now_debugging = false;
 	}
}
#endif
