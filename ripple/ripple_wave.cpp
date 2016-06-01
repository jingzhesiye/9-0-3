#include "mainwidget.h"
#include "ui_mainwidget.h"




//KL
//CHANNAL;U1


//读取波形图数据,803r默认为256点
void MainWidget:: startWAVE()
{
    int sampleCnt =64;
    RSMV.wave_Ua_curve ->setRawSamples( 0,0, 0);
    RSMV.wave_Ub_curve ->setRawSamples( 0,0, 0);
    RSMV.wave_Uc_curve ->setRawSamples( 0,0, 0);

    ui->ES_wave_QwtPlot->replot();


    timeThreadTimer.wave_chlNum =1;                             //56 =111 000 =456 通道 7
    timeThreadTimer.wave_sampleCnt = sampleCnt;                       //采样数目

    for(UINT16 i = 0; i < sampleCnt; i++ )                                 //X坐标的值
    {
        xval[i] = FLOAT64( i ) * 3.0 / FLOAT64( sampleCnt - 1 );
    }

    timeThreadTimer.run(RSMV_WAVE);
}



void MainWidget::slt_wave_update( )
{
   // qDebug()<<"slt_wave_update";
#if 1
    UINT16 sampleCnt = 256 ;

    ui->RSMV_Harmonic_QwtPlot->setAxisScale(QwtPlot::yLeft  ,-timeThreadTimer.RSMV_arrayTemp[3]*1.5,timeThreadTimer.RSMV_arrayTemp[3]*1.5,timeThreadTimer.RSMV_arrayTemp[3]*1.5);

    RSMV.wave_vmax_marker->setLabel(QString::number(timeThreadTimer.RSMV_arrayTemp[3]));
    RSMV.wave_vmin_marker->setLabel(QString::number(-timeThreadTimer.RSMV_arrayTemp[3]));

    RSMV.wave_vmax_marker->setYValue( timeThreadTimer.RSMV_arrayTemp[3] );
    RSMV.wave_vmin_marker->setYValue( -timeThreadTimer.RSMV_arrayTemp[3] );

    RSMV.wave_Ua_curve ->setRawSamples( xval,timeThreadTimer.RSMV_wave_axesY[0], sampleCnt);
    RSMV.wave_Ub_curve ->setRawSamples( xval,timeThreadTimer.RSMV_wave_axesY[1], sampleCnt);
    RSMV.wave_Uc_curve ->setRawSamples( xval,timeThreadTimer.RSMV_wave_axesY[2], sampleCnt);

    //qDebug()<<"slt_wave_update"<<QString::number(timeThreadTimer.RSMV_arrayTemp[3]);
//    for(int i = 0; i < sampleCnt; i++ )
//    {
//        qDebug("%d",timeThreadTimer.RSMV_wave_axesY[0][i]);
//    }
    ui->RSMV_Harmonic_QwtPlot->replot();
    //timeThreadTimer.mutexUpdate.unlock();
#endif


}
