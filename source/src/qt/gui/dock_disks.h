#ifndef _CSP_QT_DOCKWIDGET_BASE_H
#define _CSP_QT_DOCKWIDGET_BASE_H

#include <QObject>
#include <QToolBar>
#include <QDockWidget>
#include <QIcon>
#include <QString>
#include <QStringList>
#include <QPixmap>

class QLabel;

enum {
	CSP_DockDisks_Domain_Binary = 0,
	CSP_DockDisks_Domain_Bubble,
	CSP_DockDisks_Domain_Cart,
	CSP_DockDisks_Domain_CMT,
	CSP_DockDisks_Domain_CD,
	CSP_DockDisks_Domain_FD,
	CSP_DockDisks_Domain_HD,
	CSP_DockDisks_Domain_LD,
	CSP_DockDisks_Domain_QD,
};
	
QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QHBoxLayout;

class CSP_DockDisks : public QDockWidget {
	Q_OBJECT
protected:
	QVBoxLayout *VBox;
	QHBoxLayout *HBox;
	QWidget *WidgetV;
	QWidget *WidgetH;

	QLabel *lBinary[8];
	QLabel *lBubble[8];
	QLabel *lCart[8];
	QLabel *lCMT[2];
	QLabel *lCompactDisc[2];
	QLabel *lFloppyDisk[8];
	QLabel *lHardDisk[8];
	QLabel *lLaserDisc[2];
	QLabel *lQuickDisk[8];
	
	QString pBinary[8];
	QString pBubble[8];
	QString pCart[8];
	QString pCMT[2];
	QString pCompactDisc[2];
	QString pFloppyDisk[8];
	QString pHardDisk[8];
	QString pLaserDisc[2];
	QString pQuickDisk[8];
	
public:
	CSP_DockDisks(QWidget *parent, Qt::WindowFlags flags = 0, bool vertical = false);
	~CSP_DockDisks();

public slots:
	void updateLabel(int dom, int localnum, QString str);
	void updateMessage(int dom, int localnum, QString str);
	void setVisible(int dom, int localNum, bool enabled);
	void setPixmap(int dom, int localNum, const QPixmap &); 
};
QT_END_NAMESPACE

#endif
	
