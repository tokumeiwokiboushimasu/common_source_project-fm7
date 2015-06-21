/*
 * Common Source code Project:
 * Ui->Qt->MainWindow for X1TurboZ .
 * (C) 2015 K.Ohta <whatisthis.sowhat _at_ gmail.com>
 *   License : GPLv2
 *   History :
 * Jan 14, 2015 : Initial, many of constructors were moved to qt/gui/menu_main.cpp.
 */

#include <QtCore/QVariant>
#include <QtGui>
#include "menuclasses.h"
#include "emu.h"
#include "qt_main.h"


void META_MainWindow::setupUI_Emu(void)
{
   int i;
   QString tmps;
   
}

void META_MainWindow::retranslateUi(void)
{
  int i;
   
  retranslateControlMenu("System Reset",  true);
  retranslateFloppyMenu(0, 1);
  retranslateFloppyMenu(1, 2);
  retranslateCMTMenu();
  retranslateSoundMenu();
  retranslateScreenMenu();
   
  this->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
  
  
  actionAbout->setText(QApplication::translate("MainWindow", "About...", 0, QApplication::UnicodeUTF8));
  

  menuEmulator->setTitle(QApplication::translate("MainWindow", "Emulator", 0, QApplication::UnicodeUTF8));
  menuMachine->setTitle(QApplication::translate("MainWindow", "Machine", 0, QApplication::UnicodeUTF8));
  
  menuHELP->setTitle(QApplication::translate("MainWindow", "HELP", 0, QApplication::UnicodeUTF8));
   // Set Labels
} // retranslateUi



META_MainWindow::META_MainWindow(QWidget *parent) : Ui_MainWindow(parent)
{
   setupUI_Emu();
   retranslateUi();
}


META_MainWindow::~META_MainWindow()
{
}

//QT_END_NAMESPACE



