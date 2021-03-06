#ifndef WIDGET_H
#define WIDGET_H
#include "ui_mainwidget.h"
#include "thread/timeThread.h"
#include"testPower/RSMV_option.h"
#include "xl618.h"
#include <QWidget>
#include "QToolButton"
#include "QTimer"
#include "phasor/ME_initPhasor.h"

class QextSerialPort;
class Histogram;

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();
    
    void  show_MsBox(QString strMessages,int msec);                        //提示框
    void  set_TblWdiget_Header(QTableWidget *TblWiget,bool H,bool V);
    void  remove_TblWdiget_Row(QTableWidget *TblWiget);

    void  init_serPort();                                                  //初始化串口界面
    void  init_timeThreadTimer_connect();
    void  write_ini(QString path,QString arg);
    void  read_ini();
private:
    Ui::MainWidget *ui;

//侧边栏
    void  init_sideBar();
    QList<QToolButton *> sideBar_BtnList;
    QList<QPushButton *> serPort_shorCut_list;
    bool eventFilter(QObject *target,QEvent *event);

    QTimer *elapseTimer;
    QTime   elapseTime;   //运行时间
    int     elapseTimeCal;


    timeThread        timeThreadTimer ;
    QPolygonF         ESTD_PolygonF;//坐标点

    double            xval[300];  //波形图目前这样设置不会有问题

    void init_ES_wave();
    void init_ESTD_wave();

    void init_elapseTime();//定时器时间
    void init_rms_harmonic();
    void init_RSMV_phasor();
    void init_RSMV_harmonic();
    void set_RSMV_harmonic_histogram( uint numValues, const double *values);

    void startRD();
    void startRS();
    void startME();
    void startWAVE();
    void startES();
    void startESTD();
    void startHAR();
    void startELAPSETIME();
    void startENERGY_PUL();
    void startENERGY_STD();
    void clean_main_SkWidget_1();

    QString getTemplatePath();
    void fillReplaceDocStructList(replaceDocTypeList *list, QString searchStr, QString replaceStr) ;
    void fillReplaceDocTypeList( QTableWidget *modelDiscriptor, replaceDocTypeList *replaceDocTypeList_Data);

private slots:
    void changeButtonStatus();
    void slot_serPort_shorCut_PsBtn();

    //串口
    void  slot_serPort_open();
    void  slot_serPort_close();
    void  slt_serPort_RecDataToUI(const QByteArray &,int);
    void on_serPort_CleanRec_PsBtn_clicked();
    void on_serPort_CleanSend_PsBtn_clicked();
    void on_serPort_Send_PsBtn_clicked();

    void slt_RD_update(pRDTYPE);
    void slt_RS_update(pRSTYPE);

    void slt_ME_update(pMETYPE);
    void slt_ES_update(pESTYPE );
    void slt_ENERGY_PUL_update(pPULSEPOW );
    void slt_ENERGY_STD_update(pPULSEPOW );
    void slt_wave_update();
    void slt_RSMV_harmonic_update();
    void slt_battery_update(QString str);

    void slt_HAR_update(pHARTYPE);

    void slt_elapseTimer_timeout();

    void on_start_PsBtn_clicked(bool checked);

    void on_serPort_write_SP_PsBtn_clicked();
    void on_serPort_read_SP_PsBtn_clicked();

    void on_serPort_portName_CbBox_activated(int index);
    void on_serPort_dataBit_CbBox_activated(int index);
    void on_serPort_baudRate_CbBox_activated(int index);
    void on_serPort_stopBit_CbBox_activated(int index);
    void on_serPort_parity_CbBox_activated(int index);
    void on_serPort_flowCtl_CbBox_activated(int index);

    void on_RSMV_clearFrom_PsBtn_clicked();

    void on_RSMV_buildFrom_PsBtn_clicked();

    void on_RSMV_readFrom_PsBtn_clicked();

    void on_RSMV_saveFormMsg_PsBtn_clicked();

    void on_ES_PE_zoomIn_PsBtn_clicked();

    void on_ES_PE_zoomOut_PsBtn_clicked();

    void on_keyBoard_PsBtn_clicked();

    void on_ES_insertForm_PsBtn_clicked();

    void on_RSMV_Harmonic_Range_HSlider_valueChanged(int value);

    void on_unlock_PsBtn_clicked();

    void on_ES_wave_CkBox_clicked(bool checked);

    void on_ME_energy_testEqpt_LnEdit_textChanged(const QString &arg1);

    void on_ME_time_testEqpt_LnEdit_textChanged(const QString &arg1);

private:
    QComboBox       *SP_chargeType_cbbox;
    QComboBox       *SP_loadType_cbbox;
    QComboBox       *SP_measureType_cbbox;

    ReceiveSMV       RSMV;
    PhasorPaint      *ME_phasorPaint;
    Histogram        *RSMV_harmonic_histogram;
};

#endif // WIDGET_H
