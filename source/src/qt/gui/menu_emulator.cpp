/*
 * Common Source code Project:
 * Ui->Qt->gui->menu_emulator for generic.
 * (C) 2015 K.Ohta <whatisthis.sowhat _at_ gmail.com>
 *   License : GPLv2
 *   History :
 * Aug 13, 2019 : Split from menu_main.cpp.
 */

#include <QApplication>
#include <QVariant>
#include <QtGui>
#include <QPixmap>
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QDockWidget>
#include <QToolBar>
#include <QMenu>
#include <QMenuBar>
#include <QStyle>

#include "display_text_document.h"
#include "mainwidget_base.h"
#include "dock_disks.h"

#include "qt_gldraw.h"
//#include "emu.h"
#include "qt_main.h"
#include "menu_flags.h"
#include "csp_logger.h"
#include "common.h"
// Emulator
#include "dropdown_joystick.h"
#include "dropdown_joykey.h"
#include "dialog_set_key.h"

void Ui_MainWindowBase::do_set_roma_kana(bool flag)
{
	p_config->romaji_to_kana = flag;
	emit sig_set_roma_kana(flag);
}

void Ui_MainWindowBase::do_set_numpad_enter_as_fullkey(bool flag)
{
	p_config->numpad_enter_as_fullkey = flag;
}

void Ui_MainWindowBase::do_set_joy_to_key(bool flag)
{
	p_config->use_joy_to_key = flag;
}

void Ui_MainWindowBase::do_set_print_cpu_statistics(bool flag)
{
	p_config->print_statistics = flag;
	emit sig_emu_update_config();
}

void Ui_MainWindowBase::do_set_visible_virtual_media_none()
{
	driveData->setVisible(false);
	p_config->virtual_media_position = 0;
	set_screen_size(graphicsView->width(), graphicsView->height());

	pCentralLayout->setDirection(QBoxLayout::TopToBottom);
	do_resize_central_widget();
}

void Ui_MainWindowBase::do_set_visible_virtual_media_upper()
{
	driveData->setVisible(true);
	p_config->virtual_media_position = 1;
	set_screen_size(graphicsView->width(), graphicsView->height());
	emit sig_set_orientation_osd(1);
	pCentralLayout->setDirection(QBoxLayout::TopToBottom);
	pCentralLayout->removeWidget(driveData);
	pCentralLayout->removeWidget(graphicsView);
	pCentralLayout->addWidget(driveData);
	pCentralLayout->addWidget(graphicsView);
	
	do_resize_central_widget();
}

void Ui_MainWindowBase::do_set_visible_virtual_media_lower()
{
	driveData->setVisible(true);
	p_config->virtual_media_position = 2;
	set_screen_size(graphicsView->width(), graphicsView->height());
	emit sig_set_orientation_osd(2);
	pCentralLayout->setDirection(QBoxLayout::BottomToTop);
	pCentralLayout->removeWidget(driveData);
	pCentralLayout->removeWidget(graphicsView);
	pCentralLayout->addWidget(driveData);
	pCentralLayout->addWidget(graphicsView);
	do_resize_central_widget();
}

void Ui_MainWindowBase::do_resize_central_widget()
{
	bool vmedia_visible = false;
	int vmedia_height = 0;
	int _width = 640;
	int _height = 480;
	QRect rect;
	if(driveData != nullptr) {
		vmedia_visible = driveData->isVisible();
		if(vmedia_visible) {
			vmedia_height = driveData->height();
		}
	}
	if(graphicsView != nullptr) {
		_width = graphicsView->width();
		_height = graphicsView->height() + vmedia_height;
	}
	if((pCentralWidget == nullptr) || (pCentralLayout == nullptr)) {
		return;
	}
	rect.setRect(0, 0, _width, _height + 2);
	pCentralWidget->setGeometry(rect);
	pCentralLayout->update();
	pCentralWidget->setLayout(pCentralLayout);
	pCentralWidget->adjustSize();

	setCentralWidget(pCentralWidget);
	adjustSize();
	
}
void Ui_MainWindowBase::do_set_visible_virtual_media_left()
{
#if 0
	driveData->setVisible(true);
	p_config->virtual_media_position = 3;
	set_screen_size(graphicsView->width(), graphicsView->height());
	emit sig_set_orientation_osd(3);
	pCentralLayout->removeWidget(driveData);
	pCentralLayout->addWidget(driveData, 1, 0);
	//emit sig_set_display_osd(false);
#endif
}

void Ui_MainWindowBase::do_set_visible_virtual_media_right()
{
#if 0
	driveData->setVisible(true);
	p_config->virtual_media_position = 4;
	set_screen_size(graphicsView->width(), graphicsView->height());
	emit sig_set_orientation_osd(4);
	pCentralLayout->removeWidget(driveData);
	pCentralLayout->addWidget(driveData, 1, 2);
	//emit sig_set_display_osd(false);
#endif
}

#include "display_log.h"
#include "mouse_dialog.h"

void Ui_MainWindowBase::rise_log_viewer(void)
{
	Dlg_LogViewer *dlg = new Dlg_LogViewer(using_flags, csp_logger, NULL);
	dlg->show();
//	dlg->exec();
}

void Ui_MainWindowBase::rise_mouse_dialog(void)
{
	if(using_flags->is_use_mouse()) {
		Ui_MouseDialog* dlg = new Ui_MouseDialog(using_flags);
		dlg->setWindowTitle(QApplication::translate("Ui_MouseDialog", "Configure Mouse", 0));
		dlg->show();
	}
}

void Ui_MainWindowBase::rise_joystick_dialog(void)
{
	if((graphicsView != NULL) && (hRunJoy.get() != NULL)) {
		QStringList *lst = graphicsView->getVKNames();
		CSP_DropDownJoysticks *dlg = new CSP_DropDownJoysticks(NULL, lst, using_flags, hRunJoy);
		dlg->setWindowTitle(QApplication::translate("CSP_DropDownJoysticks", "Configure Joysticks", 0));
		dlg->show();
	}
}

void Ui_MainWindowBase::rise_joykey_dialog(void)
{
	if(graphicsView != NULL) {
		QStringList *lst = graphicsView->getVKNames();
		CSP_DropDownJoykey *dlg = new CSP_DropDownJoykey(NULL, lst, using_flags);
		dlg->setWindowTitle(QApplication::translate("CSP_DropDownJoysticks", "Configure Joystick to KEYBOARD", 0));
		dlg->show();
	}
}

void Ui_MainWindowBase::rise_movie_dialog(void)
{

}

#include "qt_input.h"

void Ui_MainWindowBase::rise_keyboard_dialog(void)
{
	if(graphicsView != NULL) {
		CSP_KeySetDialog *dlg = new CSP_KeySetDialog(NULL, graphicsView);
		dlg->setWindowTitle(QApplication::translate("KeySetDialog", "Configure Keyboard", 0));
		connect(this, SIGNAL(sig_add_keyname_table(uint32_t, QString)), dlg, SLOT(do_update_keyname_table(uint32_t, QString)));
		if(!(phys_key_name_map.isEmpty())) {
			for(auto i = phys_key_name_map.constBegin(); i != phys_key_name_map.constEnd(); ++i)
			{
				bool is_set = false;
//				if(!using_flags->is_notify_key_down_lr_shift()) {
					if(i.key() == VK_SHIFT) {
						emit sig_add_keyname_table(VK_LSHIFT, i.value());
						emit sig_add_keyname_table(VK_RSHIFT, i.value());
						is_set = true;
					} else if(i.key() == VK_MENU) {
						emit sig_add_keyname_table(VK_LMENU, i.value());
						emit sig_add_keyname_table(VK_RMENU, i.value());
						is_set = true;
					}
//				}
				if(i.key() == VK_CONTROL) {
					emit sig_add_keyname_table(VK_LCONTROL, i.value());
					emit sig_add_keyname_table(VK_RCONTROL, i.value());
					is_set = true;
				}
				if(p_config->numpad_enter_as_fullkey) {
					if(i.key() == VK_RETURN) {
						emit sig_add_keyname_table(VK_OEM_CSP_KPRET, i.value());
						emit sig_add_keyname_table(VK_RETURN, i.value());
						is_set = true;
					}
				}
				if(p_config->swap_kanji_pause) {
					if(i.key() == VK_KANJI) {
						emit sig_add_keyname_table(VK_PAUSE, i.value());
						is_set = true;
					} else if(i.key() == VK_PAUSE) {
						emit sig_add_keyname_table(VK_KANJI, i.value());
						is_set = true;
					}
				}

				if(!is_set) {
					emit sig_add_keyname_table(i.key(), i.value());
				}
			}
		}

		dlg->show();
	}
}

#if defined(Q_OS_LINUX)
//#define _GNU_SOURCE
#include <unistd.h>
#include <sched.h>
#endif

void Ui_MainWindowBase::ConfigEmulatorMenu(void)
{
	int i;
	QString tmps;
	actionGroup_DispVirtualMedias = new QActionGroup(this);
	actionGroup_DispVirtualMedias->setExclusive(true);
	menu_DispVirtualMedias = new QMenu(this);
	menu_DispVirtualMedias->setToolTipsVisible(true);
	for(i = 0; i < 3; i++) {
		action_DispVirtualMedias[i] = new QAction(this);
		action_DispVirtualMedias[i]->setCheckable(true);
		action_DispVirtualMedias[i]->setChecked(false);
		if(i == p_config->virtual_media_position) action_DispVirtualMedias[i]->setChecked(true);
		action_DispVirtualMedias[i]->setEnabled(true);
		actionGroup_DispVirtualMedias->addAction(action_DispVirtualMedias[i]);
		menu_DispVirtualMedias->addAction(action_DispVirtualMedias[i]);
	}
	connect(action_DispVirtualMedias[0], SIGNAL(triggered()), this, SLOT(do_set_visible_virtual_media_none()));
	connect(action_DispVirtualMedias[1], SIGNAL(triggered()), this, SLOT(do_set_visible_virtual_media_upper()));
	connect(action_DispVirtualMedias[2], SIGNAL(triggered()), this, SLOT(do_set_visible_virtual_media_lower()));

	SET_ACTION_SINGLE_CONNECT(action_DriveInOpCode, true, true, (p_config->drive_vm_in_opecode), SIGNAL(toggled(bool)), SLOT(do_set_drive_vm_in_opecode(bool)));
	action_DriveInOpCode->setVisible(false);

	if(using_flags->is_use_joystick()) {
		SET_ACTION_SINGLE(action_UseJoykey, true, true, (p_config->use_joy_to_key));
		connect(action_UseJoykey, SIGNAL(toggled(bool)), this, SLOT(do_set_joy_to_key(bool)));
	}

	if(using_flags->is_use_auto_key()) {
		// ToDo: Setup if checked.
		SET_ACTION_SINGLE(action_UseRomaKana, true, true, (p_config->romaji_to_kana));
		connect(action_UseRomaKana, SIGNAL(toggled(bool)), this, SLOT(do_set_roma_kana(bool)));
	}
	SET_ACTION_SINGLE(action_NumPadEnterAsFullkey, true, true, (p_config->numpad_enter_as_fullkey));
	connect(action_NumPadEnterAsFullkey, SIGNAL(toggled(bool)), this, SLOT(do_set_numpad_enter_as_fullkey(bool)));

	SET_ACTION_SINGLE(action_PrintCpuStatistics, true, true, (p_config->print_statistics));
	connect(action_PrintCpuStatistics, SIGNAL(toggled(bool)), this, SLOT(do_set_print_cpu_statistics(bool)));

	// Cursor to ten key.
	menu_EmulateCursorAs = new QMenu(this);
	menu_EmulateCursorAs->setToolTipsVisible(true);
	actionGroup_EmulateCursorAs = new QActionGroup(this);
	actionGroup_EmulateCursorAs->setExclusive(true);
	{
		for(i = 0; i < 3; i++) {
			tmps = QString::number(i);
			action_EmulateCursorAs[i] = new QAction(this);
			action_EmulateCursorAs[i]->setObjectName(QString::fromUtf8("action_EmulateCursorAs", -1) + tmps);
			action_EmulateCursorAs[i]->setCheckable(true);
			action_EmulateCursorAs[i]->setData(QVariant(i));
			actionGroup_EmulateCursorAs->addAction(action_EmulateCursorAs[i]);
			menu_EmulateCursorAs->addAction(action_EmulateCursorAs[i]);
			if(i == p_config->cursor_as_ten_key) action_EmulateCursorAs[i]->setChecked(true);

			connect(action_EmulateCursorAs[i], SIGNAL(triggered()),
					this, SLOT(do_set_emulate_cursor_as()));
		}
	}

	SET_ACTION_CHECKABLE_SINGLE_CONNECT_NOMENU(actionSpeed_FULL, "actionSpeed_FULL", p_config->full_speed, SIGNAL(toggled(bool)), SLOT(do_emu_full_speed(bool)));

	if(using_flags->is_use_mouse()) {
		action_SetupMouse = new QAction(this);
	}
	if(using_flags->is_use_joystick()) {
		action_SetupJoystick = new QAction(this);
		action_SetupJoykey = new QAction(this);
	}

	SET_ACTION_CHECKABLE_SINGLE_CONNECT_NOMENU(action_FocusWithClick, "actionFocus_With_Click", p_config->focus_with_click, SIGNAL(toggled(bool)), SLOT(do_set_window_focus_type(bool)));

	action_Logging_FDC = NULL;
	if(using_flags->is_use_fd()) {
		SET_ACTION_SINGLE(action_Logging_FDC, true, true, (p_config->special_debug_fdc != 0));
		connect(action_Logging_FDC, SIGNAL(toggled(bool)), this, SLOT(do_set_logging_fdc(bool)));
	}
#if !defined(Q_OS_WIN)
	SET_ACTION_SINGLE(action_LogToSyslog, true, true, (p_config->log_to_syslog != 0));

	menuDevLogToSyslog = new QMenu(this);
	menuDevLogToSyslog->setToolTipsVisible(true);
	SET_ACTION_CONTROL_ARRAY(0, (CSP_LOG_TYPE_VM_DEVICE_END - CSP_LOG_TYPE_VM_DEVICE_0 + 1),
							 this, 
							 menuDevLogToSyslog, action_DevLogToSyslog, true, false,
							 dev_log_to_syslog,
							 SIGNAL(toggled(bool)),
							 SLOT(do_set_dev_log_to_syslog(bool)));
#endif

	SET_ACTION_SINGLE(action_LogToConsole, true, true, (p_config->log_to_console != 0));

	//menuDevLogToConsole = new QMenu(menuEmulator);
	menuDevLogToConsole = new QMenu(this);
	menuDevLogToConsole->setToolTipsVisible(true);

	SET_ACTION_CONTROL_ARRAY(0, (CSP_LOG_TYPE_VM_DEVICE_END - CSP_LOG_TYPE_VM_DEVICE_0 + 1),
							 this, 
							 menuDevLogToConsole, action_DevLogToConsole, true, false,
							 dev_log_to_console,
							 SIGNAL(toggled(bool)),
							 SLOT(do_set_dev_log_to_console(bool)));

	action_LogView = new QAction(this);
	connect(action_LogView, SIGNAL(triggered()),
			this, SLOT(rise_log_viewer()));

	long cpus = -1;
#if defined(Q_OS_LINUX)
	{
		cpus = sysconf(_SC_NPROCESSORS_ONLN);
	}
#endif
	menu_SetFixedCpu = NULL;
	action_ResetFixedCpu = NULL;
	for(i = 0; i < 128; i++) {
		action_SetFixedCpu[i] = NULL;
	}
	if(cpus > 0) {
		menu_SetFixedCpu = new QMenu(this);
		menu_SetFixedCpu->setToolTipsVisible(true);
		actionGroup_SetFixedCpu = new QActionGroup(this);
		actionGroup_SetFixedCpu->setExclusive(true);
		if(cpus >= 128) cpus = 128;

		action_ResetFixedCpu = new QAction(this);
		action_ResetFixedCpu->setObjectName(QString::fromUtf8("action_SetFixedCpu", -1) + tmps);
		action_ResetFixedCpu->setCheckable(true);
		action_ResetFixedCpu->setData(QVariant((int)-1));
		actionGroup_SetFixedCpu->addAction(action_ResetFixedCpu);
		menu_SetFixedCpu->addAction(action_ResetFixedCpu);

		for(i = 0; i < cpus; i++) {
			tmps = QString::number(i);
			action_SetFixedCpu[i] = new QAction(this);
			action_SetFixedCpu[i]->setObjectName(QString::fromUtf8("action_SetFixedCpu", -1) + tmps);
			action_SetFixedCpu[i]->setCheckable(true);
			action_SetFixedCpu[i]->setData(QVariant(i));
			actionGroup_SetFixedCpu->addAction(action_SetFixedCpu[i]);
			menu_SetFixedCpu->addAction(action_SetFixedCpu[i]);
		}
	}
	menu_SetRenderPlatform = new QMenu(this);
	menu_SetRenderPlatform->setToolTipsVisible(true);
	actionGroup_SetRenderPlatform = new QActionGroup(this);
	actionGroup_SetRenderPlatform->setExclusive(true);
	{
			int render_type = p_config->render_platform;
			int _major_version = p_config->render_major_version;
			int _minor_version = p_config->render_minor_version;
			//int _minor_version = p_config->render_minor_version; // ToDo
			for(i = 0; i < MAX_RENDER_PLATFORMS; i++) {
				tmps = QString::number(i);
				action_SetRenderPlatform[i] = new QAction(this);
				action_SetRenderPlatform[i]->setObjectName(QString::fromUtf8("action_SetRenderPlatform", -1) + tmps);
				action_SetRenderPlatform[i]->setCheckable(true);
				action_SetRenderPlatform[i]->setData(QVariant(i));
				actionGroup_SetRenderPlatform->addAction(action_SetRenderPlatform[i]);
				menu_SetRenderPlatform->addAction(action_SetRenderPlatform[i]);
				switch(i) {
				case RENDER_PLATFORMS_OPENGL3_MAIN:
					if((render_type == CONFIG_RENDER_PLATFORM_OPENGL_MAIN)
					   && (_major_version >= 3) && (_minor_version >= 0)) {
						action_SetRenderPlatform[i]->setChecked(true);
					}
					break;
				case RENDER_PLATFORMS_OPENGL2_MAIN:
					if((render_type == CONFIG_RENDER_PLATFORM_OPENGL_MAIN)
					   && (_major_version == 2) && (_minor_version >= 0)) {
						action_SetRenderPlatform[i]->setChecked(true);
					}
					break;
				case RENDER_PLATFORMS_OPENGL_CORE:
					if((render_type == CONFIG_RENDER_PLATFORM_OPENGL_CORE)
					   && (((_major_version == 4) && (_major_version >= 3)) || (_minor_version >= 5))) {
						action_SetRenderPlatform[i]->setChecked(true);
					}
					break;
				case RENDER_PLATFORMS_OPENGL_ES_2:
					if((render_type == CONFIG_RENDER_PLATFORM_OPENGL_ES)
					   && ((_major_version == 2) || ((_major_version == 3) && (_minor_version == 0)))) {
						action_SetRenderPlatform[i]->setChecked(true);
					}
					break;
				case RENDER_PLATFORMS_OPENGL_ES_31:
					if((render_type == CONFIG_RENDER_PLATFORM_OPENGL_ES)
					   && ((_major_version >= 4) || ((_major_version == 3) && (_minor_version >= 1)))) {
						action_SetRenderPlatform[i]->setChecked(true);
					}
					break;
				default: // Unknown.
					action_SetRenderPlatform[i]->setVisible(false);
					break;
				}
				connect(action_SetRenderPlatform[i], SIGNAL(triggered()),
						this, SLOT(do_select_render_platform(void)));
			}
	}
	action_SetupKeyboard = new QAction(this);

	action_SetupMovie = new QAction(this);

}

#if defined(Q_OS_LINUX)
//#undef _GNU_SOURCE
#endif

void Ui_MainWindowBase::CreateEmulatorMenu(void)
{
	//menuEmulator->addAction(action_LogRecord);
	menuEmulator->addAction(action_FocusWithClick);
	menuEmulator->addAction(menu_DispVirtualMedias->menuAction());
	menuEmulator->addSeparator();
	menuEmulator->addAction(actionSpeed_FULL);
	menuEmulator->addAction(action_DriveInOpCode);
	menuEmulator->addSeparator();
	if(menu_SetFixedCpu != nullptr) {
		menuEmulator->addAction(menu_SetFixedCpu->menuAction());
	}
	menuEmulator->addAction(menu_SetRenderPlatform->menuAction());
	menuEmulator->addSeparator();
	if(using_flags->is_use_mouse()) {
		menuEmulator->addAction(action_SetupMouse);
	}
	if(using_flags->is_use_joystick()) {
		menuEmulator->addAction(action_SetupJoystick);
		menuEmulator->addAction(action_SetupJoykey);
	}
	menuEmulator->addAction(action_SetupKeyboard);
	menuEmulator->addAction(action_SetupMovie);
	menuEmulator->addSeparator();
	if(using_flags->is_use_auto_key()) {
		menuEmulator->addAction(action_UseRomaKana);
	}
	if(using_flags->is_use_joystick()) {
		menuEmulator->addAction(action_UseJoykey);
	}
	menuEmulator->addAction(action_NumPadEnterAsFullkey);
	menuEmulator->addSeparator();
	menuEmulator->addAction(menu_EmulateCursorAs->menuAction());
	if(action_Logging_FDC != NULL) {
		menuEmulator->addSeparator();
		menuEmulator->addAction(action_Logging_FDC);
	}
	menuEmulator->addAction(action_PrintCpuStatistics);
	menuEmulator->addSeparator();
	menuEmulator->addAction(action_LogView);
	menuEmulator->addSeparator();
	menuEmulator->addAction(action_LogToConsole);
	menuEmulator->addAction(menuDevLogToConsole->menuAction());
	menuEmulator->addSeparator();
#if !defined(Q_OS_WIN)
	menuEmulator->addAction(action_LogToSyslog);
	menuEmulator->addAction(menuDevLogToSyslog->menuAction());
#endif
	menuEmulator->addSeparator();
}

void Ui_MainWindowBase::retranslateOpMenuAny(QString _Text, QString _ToolTip, bool _visible)
{
	if(action_DriveInOpCode != nullptr) {
		action_DriveInOpCode->setText(_Text);
		action_DriveInOpCode->setToolTip(_ToolTip);
		action_DriveInOpCode->setVisible(_visible);
	}
}

void Ui_MainWindowBase::retranslateOpMenuZ80(bool _visible)
{
	QString _Text = QApplication::translate("MenuEmulator", "Drive VM in M1/R/W Cycle", 0);
	QString _ToolTip = QApplication::translate("MenuEmulator", "Process some events and wait per instruction.\nMake emulation more correctness.", 0);
	retranslateOpMenuAny(_Text, _ToolTip, _visible);
}

void Ui_MainWindowBase::retranslateEmulatorMenu(void)
{
	if(using_flags->is_use_mouse()) {
		action_SetupMouse->setText(QApplication::translate("MenuEmulator", "Configure Mouse", 0));
		action_SetupMouse->setToolTip(QApplication::translate("MenuEmulator", "Setup mouse sensitivity.", 0));
	}
	if(using_flags->is_use_joystick()) {
		action_SetupJoystick->setText(QApplication::translate("MenuEmulator", "Configure Joysticks", 0));
		action_SetupJoystick->setToolTip(QApplication::translate("MenuEmulator", "Configure assigning buttons/directions of joysticks.", 0));
		action_SetupJoykey->setText(QApplication::translate("MenuEmulator", "Configure Joystick to KEYBOARD", 0));
		action_SetupJoykey->setToolTip(QApplication::translate("MenuEmulator", "Configure assigning keycode to joystick buttons.\nThis feature using Joystick #1.", 0));
		action_UseJoykey->setText(QApplication::translate("MenuEmulator", "Joystick to KEYBOARD", 0));
		action_UseJoykey->setToolTip(QApplication::translate("MenuEmulator", "Use Joystick axis/buttons to input keyboard.\nThis feature using Joystick #1.", 0));
		action_SetupJoystick->setIcon(QIcon(":/icon_gamepad.png"));
	}
	if(using_flags->is_use_auto_key()) {
		action_UseRomaKana->setText(QApplication::translate("MenuEmulator", "ROMA-KANA Conversion", 0));
		action_UseRomaKana->setToolTip(QApplication::translate("MenuEmulator", "Use romaji-kana conversion assistant of emulator.", 0));
	}
	actionSpeed_FULL->setText(QApplication::translate("MenuEmulator", "Emulate as FULL SPEED", 0));
	actionSpeed_FULL->setToolTip(QApplication::translate("MenuEmulator", "Run emulation thread without frame sync.", 0));

	retranslateOpMenuZ80(false);

	action_NumPadEnterAsFullkey->setText(QApplication::translate("MenuEmulator", "Numpad's Enter is Fullkey's", 0));
	action_NumPadEnterAsFullkey->setToolTip(QApplication::translate("MenuEmulator", "Numpad's enter key makes full key's enter.\nUseful for some VMs.", 0));

	action_PrintCpuStatistics->setText(QApplication::translate("MenuEmulator", "Print Statistics", 0));
	action_PrintCpuStatistics->setToolTip(QApplication::translate("MenuEmulator", "Print statistics of CPUs (or some devices).\nUseful for debugging.", 0));

	if(action_Logging_FDC != NULL) {
		action_Logging_FDC->setText(QApplication::translate("MenuEmulator", "FDC: Turn ON Debug log.", 0));
		action_Logging_FDC->setToolTip(QApplication::translate("MenuEmulator", "Turn ON debug logging for FDCs.Useful to resolve issues from guest software.", 0));
	}
	// ToDo
	menu_EmulateCursorAs->setTitle(QApplication::translate("MenuEmulator", "Emulate cursor as", 0));
	menu_EmulateCursorAs->setToolTip(QApplication::translate("MenuEmulator", "Emulate cursor as ten-key.", 0));
	action_EmulateCursorAs[0]->setText(QApplication::translate("MenuEmulator", "None", 0));
	action_EmulateCursorAs[1]->setText(QApplication::translate("MenuEmulator", "2 4 6 8", 0));
	action_EmulateCursorAs[2]->setText(QApplication::translate("MenuEmulator", "1 2 3 5", 0));

	menuEmulator->setTitle(QApplication::translate("MenuEmulator", "Emulator", 0));


	action_FocusWithClick->setText(QApplication::translate("MenuEmulator", "Focus on click", 0));
	action_FocusWithClick->setToolTip(QApplication::translate("MenuEmulator", "If set, focus with click, not mouse-over.", 0));

	action_SetupKeyboard->setText(QApplication::translate("MenuEmulator", "Configure Keyboard", 0));
	action_SetupKeyboard->setToolTip(QApplication::translate("MenuEmulator", "Set addignation of keyboard.", 0));
	action_SetupKeyboard->setIcon(QIcon(":/icon_keyboard.png"));
	action_SetupMovie->setText(QApplication::translate("MenuEmulator", "Configure movie encoding", 0));
	action_SetupMovie->setToolTip(QApplication::translate("MenuEmulator", "Configure parameters of movie encoding.", 0));

	action_LogToConsole->setText(QApplication::translate("MenuEmulator", "Log to Console", 0));
	action_LogToConsole->setToolTip(QApplication::translate("MenuEmulator", "Enable logging to STDOUT if checked.", 0));
#if !defined(Q_OS_WIN)
	action_LogToSyslog->setText(QApplication::translate("MenuEmulator", "Log to Syslog", 0));
	action_LogToSyslog->setToolTip(QApplication::translate("MenuEmulator", "Enable logging to SYSTEM log.\nMay be having permission to system and using *nix OS.", 0));
	//action_LogRecord->setText(QApplication::translate("MenuEmulator", "Recording Log", 0));
#endif
	menuDevLogToConsole->setTitle(QApplication::translate("MenuEmulator", "Per Device", 0));
#if !defined(Q_OS_WIN)
	menuDevLogToSyslog->setTitle(QApplication::translate("MenuEmulator", "Per Device", 0));
#endif
	menu_SetRenderPlatform->setTitle(QApplication::translate("MenuEmulator", "Video Platform(need restart)", 0));
	if(menu_SetFixedCpu != NULL) {
		menu_SetFixedCpu->setTitle(QApplication::translate("MenuEmulator", "Occupy Fixed CPU", 0));

		if(action_ResetFixedCpu != NULL) {
			action_ResetFixedCpu->setText(QApplication::translate("MenuEmulator", "Using all CPU", 0));
			action_ResetFixedCpu->setToolTip(QApplication::translate("MenuEmulator", "Using all CPU to emulation.\nReset cpu usings.", 0));
		}
		for(int ii = 0; ii < 128; ii++) {
			if(action_SetFixedCpu[ii] != NULL) {
				QString numname = QString::number(ii);
				QString numtip = QApplication::translate("MenuEmulator", "Set Fixed logical CPU #%1 to be occupied by emulation thread.\nMay useful for heavy VM (i.e. using i386 CPU).\nStill implement LINUX host only, not another operating systems.", 0).arg(numname);
				action_SetFixedCpu[ii]->setText(QString::fromUtf8("CPU #") + numname);
				action_SetFixedCpu[ii]->setToolTip(numtip);
			}
		}
	}
	action_SetRenderPlatform[RENDER_PLATFORMS_OPENGL_ES_2]->setText(QApplication::translate("MenuEmulator", "OpenGL ES v2.0", 0));
	action_SetRenderPlatform[RENDER_PLATFORMS_OPENGL_ES_31]->setText(QApplication::translate("MenuEmulator", "OpenGL ES v3.1", 0));
	action_SetRenderPlatform[RENDER_PLATFORMS_OPENGL3_MAIN]->setText(QApplication::translate("MenuEmulator", "OpenGLv3.0", 0));
	action_SetRenderPlatform[RENDER_PLATFORMS_OPENGL2_MAIN]->setText(QApplication::translate("MenuEmulator", "OpenGLv2.0", 0));
	action_SetRenderPlatform[RENDER_PLATFORMS_OPENGL_CORE]->setText(QApplication::translate("MenuEmulator", "OpenGL(Core profile)", 0));

	action_SetRenderPlatform[RENDER_PLATFORMS_OPENGL_ES_2]->setToolTip(QApplication::translate("MenuEmulator", "Using OpenGL ES v2.0.\nThis is recommanded.\nIf changed, need to restart this emulator.", 0));
	action_SetRenderPlatform[RENDER_PLATFORMS_OPENGL_ES_31]->setToolTip(QApplication::translate("MenuEmulator", "Using OpenGL ES v3.1.\nThis is recommanded.\nIf changed, need to restart this emulator.", 0));
	action_SetRenderPlatform[RENDER_PLATFORMS_OPENGL3_MAIN]->setToolTip(QApplication::translate("MenuEmulator", "Using OpenGL v3.0(MAIN).\nThis is recommanded.\nIf changed, need to restart this emulator.", 0));
	action_SetRenderPlatform[RENDER_PLATFORMS_OPENGL2_MAIN]->setToolTip(QApplication::translate("MenuEmulator", "Using OpenGLv2.\nThis is fallback of some systems.\nIf changed, need to restart this emulator.", 0));
	action_SetRenderPlatform[RENDER_PLATFORMS_OPENGL_CORE]->setToolTip(QApplication::translate("MenuEmulator", "Using OpenGL core profile.\nThis still not implement.\nIf changed, need to restart this emulator.", 0));

	menu_DispVirtualMedias->setTitle(QApplication::translate("MenuEmulator", "Show Virtual Medias.", 0));
	action_DispVirtualMedias[0]->setText(QApplication::translate("MenuEmulator", "None.", 0));
	action_DispVirtualMedias[1]->setText(QApplication::translate("MenuEmulator", "Upper.", 0));
	action_DispVirtualMedias[2]->setText(QApplication::translate("MenuEmulator", "Lower.", 0));
	//action_DispVirtualMedias[3]->setText(QApplication::translate("MenuEmulator", "Left.", 0));
	//action_DispVirtualMedias[4]->setText(QApplication::translate("MenuEmulator", "Right.", 0));
	action_LogView->setText(QApplication::translate("MenuEmulator", "View Log", 0));
	action_LogView->setToolTip(QApplication::translate("MenuEmulator", "View emulator logs with a dialog.", 0));
}

void Ui_MainWindowBase::retranselateUi_Depended_OSD(void)
{
	for(int i=0; i < (CSP_LOG_TYPE_VM_DEVICE_END - CSP_LOG_TYPE_VM_DEVICE_0 + 1) ; i++) {
		const _TCHAR *p;
		p = using_flags->get_vm_node_name(i);
		do_update_device_node_name(i, p);
	}
}
