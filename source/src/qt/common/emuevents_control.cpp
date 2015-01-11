

#include "qt_emuevents.h"
#include "qt_main.h"
#include "qt_dialogs.h"
#include "emu_utils.h"

extern EMU *emu;

void Object_Menu_Control::OnReset(void)
{
    printf("Reset\n");
    if(emu) emu->reset();
}
  void Object_Menu_Control::OnSpecialReset(void)
  {
    printf("Special Reset\n");
    if(emu) emu->special_reset();
  }
#ifdef USE_STATE
  void Object_Menu_Control::OnLoadState(void) // Final entry of load state.
  {
    if(emu) emu->load_state();
  }
  
  void Object_Menu_Control::OnSaveState(void)
  {
    if(emu) emu->save_state();
  }
#endif
#ifdef USE_BOOT_MODE
  void Object_Menu_Control::OnBootMode(void)
  {
    config.boot_mode = bindValue;
    if(emu) {
      emu->update_config();
    }
  }
#endif

#ifdef USE_CPU_TYPE
 void Object_Menu_Control::OnCpuType(void)
 {
   config.cpu_type = bindValue;
   if(emu) {
     emu->update_config();
   }
 }
#endif

void Object_Menu_Control::OnCpuPower(void)
{
  config.cpu_power = bindValue;
  if(emu) {
    emu->update_config();
  }
}

#ifdef USE_AUTO_KEY
void Object_Menu_Control::OnStartAutoKey(void)
{
  if(emu) {
    emu->start_auto_key();
  }
}
void Object_Menu_Control::OnStopAutoKey(void)
{
  if(emu) {
    emu->stop_auto_key();
  }
}
#endif
#ifdef USE_DEBUGGER
 void Object_Menu_Control::OnOpenDebugger(void)
 {
   int no = bindValue;
   if((no < 0) || (no > 3)) return;
   if(emu) emu->open_debugger(no);
 }
void Object_Menu_Control::OnCloseDebugger(void )
 {
   if(emu) emu->close_debugger();
 }
#endif


// Will move to other file.
#if defined(USE_FD1) || defined(USE_FD2) || defined(USE_FD3) || defined(USE_FD4) || defined(USE_FD5) || defined(USE_FD6) || defined(USE_FD7) || defined(USE_FD8)
void OpenRecentFloppy(QWidget *parent, int drv, int num)
{
 std::string path;
 int i;
 if((num < 0) || (num > 7)) return;

 path = config.recent_disk_path[drv][num];
 for(int i = num; i > 0; i--) {
    strcpy(config.recent_disk_path[drv][i], config.recent_disk_path[drv][i - 1]);
 }
 strcpy(config.recent_disk_path[drv][0], path.c_str());
 if(emu) {
    open_disk(drv, path.c_str(), 0);
 }
}

void OnCloseFD(int drive)
{
  if(emu) close_disk(drive);
}
// Use Dialog
void OnOpenFD(QWidget *parent,int drive)
{
   open_disk_dialog(parent, drive);
}
  
void OnSelectD88Bank(int drive, int no)
{
  if((no < 0) || (no > 63)) return;
  if(emu && emu->d88_file[drive].cur_bank != no) {
    //DeleteD88List(drive);
    emu->open_disk(drive, emu->d88_file[drive].path, emu->d88_file[drive].bank[no].offset);
    emu->d88_file[drive].cur_bank = no;
    //BuildD88List(drive);
  }
}

void Floppy_SelectD88(int drive, int num)
{
  OnSelectD88Bank(drive, num);
  //  AGAR_DebugLog(AGAR_LOG_DEBUG, "Selected D88 %d, %d\n", drive, num);
}


#endif

#ifdef USE_DIPSWITCH
void OnToggleDipSw(int dipsw)
{
  if((dipsw < 0) || (dipsw > 31)) return;
  config.dipswitch ^= (1 << dipsw);
}
void OnChangeDipSw(int dipsw, int flag)
{
 
  if((dipsw < 0) || (dipsw > 31)) return;
  if(flag == 0) {
    config.dipswitch &= ~(1 << dipsw);
  } else {
    config.dipswitch |= (1 << dipsw);
  }
}
#endif
#ifdef USE_DEVICE_TYPE
void OnSetDeviceType(int devtype)
{
  if((devtype < 0) || (devtype > 7)) return;
  config.device_type = devtype;
}
#endif

#if defined(USE_CART1) || defined(USE_CART2)
void OnOpenCart(QWidget *parent, int drive)
{
  if(emu) open_cart_dialog(parent, drive);
}

void OnCloseCart(int drive)
{
  if(emu) emu->close_cart(drive);
}

void OnRecentCart(int drive, int menunum)
{
  std::string path;
  int i;
  if(drive < 0) return;
#if !defined(USE_CART2)
  if(drive > 0) return;
#else
  if(drive > 1) return;
#endif
  if((menunum < 0) || (menunum > 7)) return;
  
  path = config.recent_cart_path[drv][menunum];
  for(int i = menunum; i > 0; i--) {
    strcpy(config.recent_cart_path[drive][i], config.recent_cart_path[drive][i - 1]);
  }
  strcpy(config.recent_cart_path[drive][0], path.c_str());
  if(emu) {
    emu->open_cart(drive, path.c_str());
  }
}
#endif


#if defined(USE_QD1) || defined(USE_QD2)
void OnOpenQD(QWidget *parent, int drive)
{
  if(emu) open_quickdisk_dialog(AGWIDGET(hWindow), drive);
}

void OnCloseQD(int drive)
{
  if(emu) emu->close_quickdisk(drive);
}

void OnRecentQD(int drive, int menunum)
{
  std::string path;
  int i;

  if((menunum < 0) || (menunum > 7)) return;
  path = config.recent_quickdisk_path[drive][menunum];
  
  for(int i = menunum; i > 0; i--) {
    strcpy(config.recent_quickdisk_path[drive][i], config.recent_quickdisk_path[drive][i - 1]);
  }
  strcpy(config.recent_quickdisk_path[drive][0], path.c_str());
  if(emu) {
    emu->open_quickdisk(drive, path.c_str());
  }
}
#endif
#ifdef USE_TAPE
void OnPlayTAPE(QWidget *parent)
{
  if(emu) open_tape_dialog(parent, true);
}

void OnRecTAPE(QWidget *parent)
{
  if(emu) open_tape_dialog(parent, false);
}

void OnCloseTAPE(void)
{
  if(emu) emu->close_tape();
}

void OnUseWaveShaperTAPE(QWidget *wid)
{
  config.wave_shaper = !config.wave_shaper;
  if(wid != NULL) {}
}

void OnDirectLoadMZT(QWidget *wid)
{
  config.direct_load_mzt = !config.direct_load_mzt;
  if(wid != NULL) {}
}

void OnRecentTAPE(int menunum)
{
  std::string path;
  int i;

  if((menunum < 0) || (menunum > 7)) return;
  path = config.recent_tape_path[menunum];
  for(int i = menunum; i > 0; i--) {
    strcpy(config.recent_tape_path[i], config.recent_tape_path[i - 1]);
  }
  strcpy(config.recent_tape_path[0], path.c_str());
  if(emu) {
    emu->play_tape(path.c_str());
  }
}
#endif
#ifdef USE_TAPE_BUTTON
void OnPushPlayButton(QWidget *parent)
{
  if(emu) emu->push_play();
}
void OnPushStopButton(QWidget *parent)
{
  if(emu) emu->push_stop();
}
#endif
// Implement LASER-DISC, BINARY
//

void OnStartRecordScreen(int num)
{

  const int fps[3] = {60, 30, 15};
  if((num < 0) || (num > 2)) return;
  if(emu) {
    emu->start_rec_sound();
    if(!emu->start_rec_video(fps[num])) {
      emu->stop_rec_sound();
    }
  }
}
void OnStopRecordScreen(void)
{
  if(emu) {
    emu->stop_rec_video();
    emu->stop_rec_sound();
  }
}

void OnScreenCapture(QWidget *parent)
{
  if(emu) emu->capture_screen();
}

void OnSetScreenMode(QMainWindow *MainWindow, QWidget *drawspace, int mode)
{
  if((mode < 0) || (mode > 7)) return;
  if(emu){
    set_window(MainWindow, mode);
  }
}

void OnFullScreen(QMainWindow *MainWindow, QWidget *drawspace, int mode)
{
}

void OnSetStretchMode(int mode)
{
  if((mode < 0) || (mode > 2)) return;
  // 0 = DOT
  // 1 = ASPECT
  // 2 = FILL
  config.stretch_type = mode;
  // On Common Sourcecode Project / Agar,
  // Scaling is done by Agar Widget.
  // So, does need below action?
  // Maybe, needs Agar's changing action. 
  if(emu) {
    emu->set_display_size(-1, -1, false);
  }
}

