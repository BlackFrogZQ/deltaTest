#pragma once
#include "basic.h"
#include "mainWindowDef.h"
#include "system/basic.h"
#include "controlWidget/controlWidget.h"
#include "stateLedWidget/plcStateLed.h"
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QMediaPlaylist>
#include <QMouseEvent>
#include <QThread>
class QLabel;
class QTextBrowser;
namespace TIGER_ProcessTool
{
    class CPythonDL;
}

class CMainWindow : public QWidget
{
    Q_OBJECT

public:
    CMainWindow(QWidget *parent = nullptr);
    ~CMainWindow();
    void printMsg(QString p_msg);

public slots:
    void slotUpdateImage(QImage p_image);
    void slotVideoImage(double p_exposureTime);
    void slotRecognitionPackage(bool p_start);
    void slotRecognitionResult(QImage p_image, double p_x, double p_y, int p_num);

protected:
    void initLayout();

private:
    QLabel *m_pImageLabel;
    QTextBrowser *m_pOutMsg;
    ControlWidget *m_pControlWidget;
    CPlcStateLed *m_pLedStateWidget;

    QMediaPlayer *m_player;
    QVideoWidget *m_videoWidget;
    QMediaPlaylist *m_medialist;
    QWidget *m_playerWidget;

    TIGER_ProcessTool::CPythonDL *m_pPythonDL;
    QThread m_visionThread;
    QImage m_targetImage;
    bool m_startRecognition;

public:
    double m_targetX;
    double m_targetY;
    int m_targetNum;
    bool m_isCorrect;
};

CMainWindow *mainWindow();