#include "xl618.h"
#include "stdio.h"
#include "QDebug"
#include <QTextStream>
#if 1
UINT8 xl618::getKL(pRKLTYPE data)
{
    FLOAT32 *fData;
    UINT8 retValue = ERR_UNIVERSAL;

    char *pSend = (char*)sendBuf;

    strcpy(pSend,"KL\n\rCHANNAL;U1"CR);
    pSend += strlen("KL\n\rCHANNAL;U1"CR);
    strcpy(pSend,"CHANNAL;I1"CR);
    pSend += strlen("CHANNAL;I1"CR);

    UINT16 frameSize = pSend - (char*)sendBuf;
    char *temp;

//   qDebug("pSend==%s\n",sendBuf);
//   qDebug("pSend");

    if((retValue = readOneFrame(frameSize,(char*)"KL",NULL,(char*)"KLACK",500)) == ERR_RIGHT)
    {//解析帧

        temp = strstr((char*)recvBuf,"U1;");
        if(temp)
        {
            for(UINT32 j = 0; j < 64; j ++)
            {
                sscanf(temp,"%E"CR,&data->U1[j]); //以指数形式输出单精度
                while(*(temp++) != '\n');
                //qDebug("xxxx%d==%f\n",j,data->U1[j]);
            }
        }

        temp = strstr((char*)recvBuf,"I1;");
        if(temp)
        {
            for(UINT32 j = 0; j < 64; j ++)
            {
                sscanf(temp,"%E"CR,&data->I1[j]);     //以指数形式输出单精度
                while(*(temp++) != '\n');
                //qDebug("xxxx%d==%f\n",j,data->U1[j]);
            }
        }
    }else
        retValue = ERR_RIGHT;

    return retValue;
}
#endif
UINT8 xl618::getLKL(UINT8 type,pLKLTYPE data)//	type:0~5位分别表示通道1~6 Ua~Ic，位置1表示读取该通道 data:存储通道数据缓冲区
{

        UINT8 retValue = ERR_UNIVERSAL;
        char *pSend = (char*)sendBuf;

        strcpy(pSend,STR_LKL);
        pSend += strlen(STR_LKL);

        UINT8 i;
        for(i= 0; i < 6; i ++)
        {
                if(type & (1 << i))
                {
                        *pSend = (i + '1');
                        pSend += 1;
                }
        }
        strcpy(pSend,";"CR);
        pSend += strlen(";"CR);

        UINT16 frameSize = pSend - (char*)sendBuf;

        IntHandle pH;
        INT32 *delay;
        INT32 *iData;
        INT32 iSize;
        char *temp;

        if((retValue = readOneFrame(frameSize,(char*)"LKL",NULL,(char*)"LKLACK",2)) == ERR_RIGHT)
        {//解析帧
                temp = (char*)&recvBuf[3];
                for(i = 0; i < 6; i ++)
                {
                        if((type & (1 << i)) == 0)
                                continue;

                        if(i == 0)
                        {
                                pH = data->CH1;
                                delay = &data->delay1;
                        }
                        else if(i == 1)
                        {
                                pH = data->CH2;
                                delay = &data->delay2;
                        }
                        else if(i == 2)
                        {
                                pH = data->CH3;
                                delay = &data->delay3;
                        }
                        else if(i == 3)
                        {
                                pH = data->CH4;
                                delay = &data->delay4;
                        }
                        else if(i == 4)
                        {
                                pH = data->CH5;
                                delay = &data->delay5;
                        }
                        else if(i == 5)
                        {
                                pH = data->CH6;
                                delay = &data->delay6;
                        }
                        else
                        {
                                retValue = ERR_KLDATA;

                                return retValue;
                        }

                        if(pH && *pH)
                        {
                                iData = (*pH)->elt;
                                iSize = (*pH)->dimSize * 4;

                                *delay = *((INT32*)temp);
                                memcpy(iData,&temp[sizeof(INT32)],iSize);
                                temp += iSize + 4;
                        }else
                        {
                                retValue = ERR_BUFFER;
                        }
                }
        }
        return retValue;
}

pLKLTYPE xl618:: newLKLBuf(UINT8 type,UINT16 cnt)  //type:0~5位分别表示通道1~6，cnt：采样点数
{
    pLKLTYPE retValue = new LKLTYPE;
    IntHandle IntHan = 0;
    INT32 i ;
    for(i = 0; i < 6; i ++)
    {
        switch(i)
        {
            case 0:
            if((type & (1 << i)) != 0)
            {
                IntHan = (IntHandle)(new IntHandle);
                *IntHan = (pINTARR)(new INT32[cnt + 1]);
                (*IntHan)->dimSize = cnt;
                retValue->CH1 = IntHan;
            }else
            {
                retValue->CH1 = 0;
            }
            break;

            case 1:
            if((type & (1 << i)) != 0)
            {
                IntHan = (IntHandle)(new IntHandle);
                *IntHan = (pINTARR)(new INT32[cnt + 1]);
                (*IntHan)->dimSize = cnt;
                retValue->CH2 = IntHan;
            }else
            {
                retValue->CH2 = 0;
            }
            break;

            case 2:
            if((type & (1 << i)) != 0)
            {
                IntHan = (IntHandle)(new IntHandle);
                *IntHan = (pINTARR)(new INT32[cnt + 1]);
                (*IntHan)->dimSize = cnt;
                retValue->CH3 = IntHan;
            }else
            {
                retValue->CH3 = 0;
            }
            break;

            case 3:
            if((type & (1 << i)) != 0)
            {
                IntHan = (IntHandle)(new IntHandle);
                *IntHan = (pINTARR)(new INT32[cnt + 1]);
                (*IntHan)->dimSize = cnt;
                retValue->CH4 = IntHan;
            }else
            {
                retValue->CH4 = 0;
            }
            break;

            case 4:
            if((type & (1 << i)) != 0)
            {
                IntHan = (IntHandle)(new IntHandle);
                *IntHan = (pINTARR)(new INT32[cnt + 1]);
                (*IntHan)->dimSize = cnt;
                retValue->CH5 = IntHan;
            }else
            {
                retValue->CH5 = 0;
            }
            break;

            case 5:
            if((type & (1 << i)) != 0)
            {
                IntHan = (IntHandle)(new IntHandle);
                *IntHan = (pINTARR)(new INT32[cnt + 1]);
                (*IntHan)->dimSize = cnt;
                retValue->CH6 = IntHan;
            }else
            {
                retValue->CH6 = 0;
            }
            break;

            default:
            break;
        }
    }
    return retValue;
}

void xl618:: deleteLKLBuf(pLKLTYPE lkl)
{
   // qDebug("lklstart");
    if(lkl)
    {
        if(lkl->CH1 && *(lkl->CH1))
        {
            delete []((INT32*)(*(lkl->CH1)));
        }

        if(lkl->CH2 && *(lkl->CH2))
        {
            delete []((INT32*)(*(lkl->CH2)));
        }

        if(lkl->CH3 && *(lkl->CH3))
        {
            delete []((INT32*)(*(lkl->CH3)));
        }

        if(lkl->CH4 && *(lkl->CH4))
        {
            delete []((INT32*)(*(lkl->CH4)));
        }

        if(lkl->CH5 && *(lkl->CH5))
        {
            delete []((INT32*)(*(lkl->CH5)));
        }

        if(lkl->CH6 && *(lkl->CH6))
        {
            delete []((INT32*)(*(lkl->CH6)));
        }
    delete lkl;
    }
}


pKLTYPE xl618:: newKLBuf(UINT8 type,UINT16 cnt)  //type:0~5位分别表示通道1~6，cnt：采样点数
{
    pKLTYPE retValue = new KLTYPE;

    //pLKLTYPE retValue = new LKLTYPE;

    FloHandle IntHan = 0;
    INT32 i ;
    for(i = 0; i < 6; i ++)
    {
        switch(i)
        {
            case 0:
            if((type & (1 << i)) != 0)
            {
                IntHan = (FloHandle)(new FloHandle);
                *IntHan = (pFLOARR)(new FLOAT32[cnt + 1]);
                (*IntHan)->dimSize = cnt;
                retValue->CH1 = IntHan;
            }else
            {
                retValue->CH1 = 0;
            }
            break;

            case 1:
            if((type & (1 << i)) != 0)
            {
                IntHan = (FloHandle)(new FloHandle);
                *IntHan = (pFLOARR)(new FLOAT32[cnt + 1]);
                (*IntHan)->dimSize = cnt;
                retValue->CH2 = IntHan;
            }else
            {
                retValue->CH2 = 0;
            }
            break;

            case 2:
            if((type & (1 << i)) != 0)
            {
                IntHan = (FloHandle)(new FloHandle);
                *IntHan = (pFLOARR)(new FLOAT32[cnt + 1]);
                (*IntHan)->dimSize = cnt;
                retValue->CH3 = IntHan;
            }else
            {
                retValue->CH3 = 0;
            }
            break;

            case 3:
            if((type & (1 << i)) != 0)
            {
                IntHan = (FloHandle)(new FloHandle);
                *IntHan = (pFLOARR)(new FLOAT32[cnt + 1]);
                (*IntHan)->dimSize = cnt;
                retValue->CH4 = IntHan;
            }else
            {
                retValue->CH4 = 0;
            }
            break;

            case 4:
            if((type & (1 << i)) != 0)
            {
                IntHan = (FloHandle)(new FloHandle);
                *IntHan = (pFLOARR)(new FLOAT32[cnt + 1]);
                (*IntHan)->dimSize = cnt;
                retValue->CH5 = IntHan;
            }else
            {
                retValue->CH5 = 0;
            }
            break;

            case 5:
            if((type & (1 << i)) != 0)
            {
                IntHan = (FloHandle)(new FloHandle);
                *IntHan = (pFLOARR)(new FLOAT32[cnt + 1]);
                (*IntHan)->dimSize = cnt;
                retValue->CH6 = IntHan;
            }else
            {
                retValue->CH6 = 0;
            }
            break;

            default:
            break;
        }
    }
    return retValue;
}

void xl618:: deleteKLBuf(pKLTYPE lkl)
{
   // qDebug("lklstart");
    if(lkl)
    {
        if(lkl->CH1 && *(lkl->CH1))
        {
            delete []((FLOAT32*)(*(lkl->CH1)));
        }

        if(lkl->CH2 && *(lkl->CH2))
        {
            delete []((FLOAT32*)(*(lkl->CH2)));
        }

        if(lkl->CH3 && *(lkl->CH3))
        {
            delete []((FLOAT32*)(*(lkl->CH3)));
        }

        if(lkl->CH4 && *(lkl->CH4))
        {
            delete []((FLOAT32*)(*(lkl->CH4)));
        }

        if(lkl->CH5 && *(lkl->CH5))
        {
            delete []((FLOAT32*)(*(lkl->CH5)));
        }

        if(lkl->CH6 && *(lkl->CH6))
        {
            delete []((FLOAT32*)(*(lkl->CH6)));
        }
    delete lkl;
    }
}



UINT8 xl618::getAppid(UINT16 *appid)
{

    UINT8 retValue = ERR_UNIVERSAL;
    char *pSend = (char*)sendBuf;
    UINT16 frameSize;
    char *temp;

    pSend += sprintf(pSend,(char*)"GETAPPID"CR);

    frameSize = pSend - (char*)sendBuf;
    if((retValue = readOneFrame(frameSize,(char*)"GETAPPID",NULL,NULL,0)) == ERR_RIGHT)
    {
            temp = strstr((char*)recvBuf,(char*)"APPID;");
            if(temp)
            {
                    sscanf(&temp[strlen("APPID;")],"0x%x",(unsigned int *)appid);
            }else
            {
                    retValue = ERR_RECV;
            }
    }

    return retValue;
}

UINT8 xl618::getSvID(char* svID)
{
    UINT8 retValue = ERR_UNIVERSAL;
    char *pSend = (char*)sendBuf;
    UINT16 frameSize;
    char *temp;
    char *temp2;

    pSend += sprintf(pSend,(char*)"GETSVID"CR);

    frameSize = pSend - (char*)sendBuf;
    if((retValue = readOneFrame(frameSize,(char*)"GETSVID",NULL,NULL,0)) == ERR_RIGHT)
    {
            temp = strstr((char*)recvBuf,(char*)"SVID;");
            if(temp)
            {
                    temp2 = strstr(temp,CR);
                    if(temp2)
                    {
                            memcpy(svID,&temp[strlen("SVID;")],1 + temp2 - &temp[strlen("SVID;")]);
                            svID[temp2 - &temp[strlen("SVID;")]] = 0;
                    }
            }else
            {
                    retValue = ERR_RECV;
            }
    }

    return retValue;
}

UINT8 xl618::setHRVirtual(pSET_HARMONIC_TYPE harmonic)
{

    UINT8 retValue = ERR_UNIVERSAL;
    char *pSend = (char*)sendBuf;
    pSend += sprintf(pSend,"SETHR"CR);
    memcpy(pSend,harmonic,sizeof(SET_HARMONIC_TYPE));
    pSend += sizeof(SET_HARMONIC_TYPE);
    UINT16 frameSize = pSend - (char*)sendBuf;
    retValue = readOneFrame(frameSize,(char*)"SETHR",NULL,(char*)"SETHRACK",1);

    return retValue;
}

UINT8 xl618::setFramerr(pSETFRAMERR_TYPE data)
{
        UINT8 retValue = ERR_UNIVERSAL;
        char *pSend = (char*)sendBuf;

        if(data)
        {
                strcpy(pSend,STR_SETFRAMERR CR);
                pSend += strlen(STR_SETFRAMERR CR);

                pSend += sprintf(pSend,"RATIO;%f"CR"MODE;%u"CR"INT;%u"CR"CALCUTYPE;%u"CR"ONOFF;%u"CR,
                        data->ratio,data->mode,data->intTimes,data->calcutype,data->onoff);

                UINT16 frameSize = pSend - (char*)sendBuf;

                retValue = readOneFrame(frameSize,(char*)"SETFRAMERR",NULL,(char*)"SETFRAMERRACK",0);
        }else
                retValue = ERR_BUFFER;

        return retValue;
}

UINT8 xl618::getHRN(UINT8 channel,pHARTYPE data,UINT32 H1,UINT32 H2)
{
     QString  str;
    UINT8 retValue = ERR_UNIVERSAL;
    char tempBuf[10];

    if(channel >= 1 && channel <= 6)
    {
        char *pSend = (char*)sendBuf;
        if(channel == 4)
        {
            pSend += sprintf(pSend,"HAR\n\rChannel;I" CR "H1;%d"CR"H2;%d"CR,H1,H2);
        }
        else
        {
           pSend += sprintf(pSend,"HAR\n\rChannel;U" CR "H1;%d"CR"H2;%d"CR,H1,H2);
        }

            char* temp;
            //UINT32 tempSam = data->samdot;

            UINT16 frameSize = pSend - (char*)sendBuf;

            if((retValue = readOneFrame(frameSize,(char*)"HAR",NULL,(char*)"HARACK",2500)) == ERR_RIGHT)
            {//解析帧

//                qDebug("retValue----%s",recvBuf);
                temp = (char*)recvBuf;
                for(UINT32 i = H1; i <= H2; i ++)
                {
#if 1
                    sprintf(tempBuf,"%d;",i);
                    temp = strstr(temp,tempBuf);
                    //qDebug()<<"------------->"<<QString(tempBuf);
                    if(temp)
                    {
                            data->order[i] = (FLOAT32)atof(&temp[strlen(tempBuf)]);
                            //qDebug("--->%d,%f",i,data->order[i]);

                    }else
                    {
                        break;
                    }
 #endif
                }

            }
     }

    return retValue;
}

pHARMONIC_TYPE xl618:: newHRNBuf(UINT16 cnt)
{
    pHARMONIC_TYPE retValue = new HARMONIC_TYPE;
    FloHandle FloHan = 0;

    FloHan = (FloHandle)(new FloHandle);
    *FloHan = (pFLOARR)(new INT32[cnt + 1]);
    (*FloHan)->dimSize = cnt;
    retValue->fN = FloHan;

    FloHan = (FloHandle)(new FloHandle);
    *FloHan = (pFLOARR)(new FLOAT32[cnt + 1]);
    (*FloHan)->dimSize = cnt;
    retValue->qN = FloHan;

    return retValue;
}

void xl618::  deleteHRNBuf(pHARMONIC_TYPE lkl)
{
    if(lkl)
    {
        if(lkl->fN && *(lkl->fN))
        {
            delete []((FLOAT32*)(*(lkl->fN)));
        }
        if(lkl->qN && *(lkl->qN))
        {
            delete []((FLOAT32*)(*(lkl->qN)));
        }

    delete lkl;
    }
}

UINT8 xl618::muTest(pMUTYPE mu)
{

        UINT8 retValue = ERR_UNIVERSAL;
        char *pSend = (char*)sendBuf;
        strcpy(pSend,STR_MU CR);
        pSend += strlen(STR_MU CR);

        UINT16 frameSize = pSend - (char*)sendBuf;

        if((retValue = readOneFrame(frameSize,(char*)"MU",NULL,(char*)"MUACK",0)) == ERR_RIGHT)
        {//解析帧
                char *temp = 0;

                temp = strstr((char*)recvBuf,"FRAME_DELAY;");
                if(temp)
                        mu->frame_delay = (FLOAT32)atof(&temp[strlen("FRAME_DELAY;")]);

                temp = strstr((char*)recvBuf,"CLOCLK_DELT;");
                if(temp)
                        mu->cloclk_delay = (FLOAT32)atof(&temp[strlen("CLOCLK_DELT;")]);

                temp = strstr((char*)recvBuf,"DELAYSHAKE;");
                if(temp)
                        mu->delayshake = (FLOAT32)atof(&temp[strlen("DELAYSHAKE;")]);
        }

        return retValue;
}

UINT8 xl618::getSyn(UINT8 *syn)
{
        UINT8 retValue = ERR_UNIVERSAL;
        char *pSend = (char*)sendBuf;

        pSend += sprintf(pSend,STR_GETSYN);

        UINT16 frameSize = pSend - (char*)sendBuf;
        char* temp;

        if((retValue = readOneFrame(frameSize,(char*)"GETSYN",NULL,(char*)"GETSYNACK",0)) == ERR_RIGHT)
        {
                temp = (char*)&recvBuf[6];

                temp = strstr(temp,"SYNTIME;");
                if(temp)
                        *syn = atoi(&temp[8]);
        }

        return retValue;
}

UINT8 xl618::setCR(FLOAT32 type)
{
    UINT8 retValue = ERR_UNIVERSAL;
    char *pSend = (char*)sendBuf;
    strcpy(pSend,STR_CR);
    pSend += strlen(STR_CR);

    if(type < 0.001)
            pSend += sprintf(pSend,"AUTO;");
    else
            pSend += sprintf(pSend,"%f;",type);

    UINT16 frameSize = pSend - (char*)sendBuf;

    retValue = readOneFrame(frameSize,(char*)"CR",NULL,(char*)"CRACK",0);


    return retValue;
}

 UINT8 xl618:: setSGP_part(pGSE_SPTYPE data,pISABLE_GSE_SP isAble)
{

    UINT8 retValue = ERR_UNIVERSAL;
    char *pSend = (char*)sendBuf;
    strcpy(pSend,STR_SGP CR);
    pSend += strlen(STR_SGP CR);
    pGSE_SPTYPE p = data;
    if(isAble->GOOSENUM)
        pSend += sprintf(pSend,"GOOSENUM;%d"CR,p->GOOSENUM);
    if(isAble->DADDR)
        pSend += sprintf(pSend,"DADDR0;%d,%d,%d,%d,%d,%d"CR,p->DADDR[0],p->DADDR[1],p->DADDR[2],p->DADDR[3],p->DADDR[4],p->DADDR[5]);
    if(isAble->TCIPRI)
        pSend += sprintf(pSend,"TCIPRI;%d"CR,p->TCIPRI);
    if(isAble->TCIVID)
            pSend += sprintf(pSend,"TCIVID;%d"CR,p->TCIVID);
    if(isAble->APPID)
            pSend += sprintf(pSend,"APPID;%d"CR,p->APPID);
    if(isAble->GOCBREF)
            pSend += sprintf(pSend,"GOCBREF0;%s"CR,p->GOCBREF);
    if(isAble->DATASET)
            pSend += sprintf(pSend,"DATASET0;%s"CR,p->DATASET);
    if(isAble->GOID)
            pSend += sprintf(pSend,"GOID0;%s"CR,p->GOID);
    if(isAble->TEST)
            pSend += sprintf(pSend,"TEST;%d"CR,p->TEST);
    if(isAble->CONFREV)
            pSend += sprintf(pSend,"CONFREV;%d"CR,p->CONFREV);
    if(isAble->CHANNLENUM)
            pSend += sprintf(pSend,"CHANNLENUM;%d"CR,p->CHANNLENUM);
    if(isAble->TIME0)
            pSend += sprintf(pSend,"TIME0;%d"CR,p->TIME0);
    if(isAble->TIME1)
            pSend += sprintf(pSend,"TIME1;%d"CR,p->TIME1);
    if(isAble->TIME2)
            pSend += sprintf(pSend,"TIME2;%d"CR,p->TIME2);
    if(isAble->TIME3)
            pSend += sprintf(pSend,"TIME3;%d"CR,p->TIME3);
    if(isAble->TINVERSE)
            pSend += sprintf(pSend,"TINVERSE;%d"CR,p->TINVERSE);
    if(isAble->INVEREN)
            pSend += sprintf(pSend,"INVEREN;%d"CR,p->INVEREN);

    if(isAble->DATAFORMAT)
                    pSend += sprintf(pSend,"DATAFORMAT0;");
#if 0
             pSend += sprintf(pSend,"DATAFORMAT0;%d",p->DATAFORMAT[0]);
            for(int i =1;i<p->CHANNLENUM;i++)
            {
              pSend += sprintf(pSend,"%d",p->DATAFORMAT[0]);
            }
      #endif
            for(int i=0;i<p->CHANNLENUM;i++)
            {
                 //  pSend += sprintf(pSend,"%s",temp);
                  memcpy(pSend, &(p->DATAFORMAT)[i], sizeof(UINT8));
                  //printf("DATAFORMAT == %d\n", *(&(p->DATAFORMAT)[i]) );
                  pSend += sizeof(UINT8);
                  //printf(" sizeof(&data[i])=%d", sizeof(data[i]));
            }
            memcpy(pSend, CR , sizeof(CR));
             // pSend += sprintf(pSend,"DATAFORMAT0;%02x"CR,charTemp);

    UINT16 frameSize = pSend - (char*)sendBuf;
    retValue = readOneFrame(frameSize,(char*)"SGP",NULL,(char*)"SPACK",0);

    return retValue;
}
#if 0
 UINT8 xl618::setHRVirtual(pSET_HARMONIC_TYPE harmonic)
 {

     UINT8 retValue = ERR_UNIVERSAL;
     char *pSend = (char*)sendBuf;
     pSend += sprintf(pSend,"SETHR"CR);
     memcpy(pSend,harmonic,sizeof(SET_HARMONIC_TYPE));
     pSend += sizeof(SET_HARMONIC_TYPE);
     UINT16 frameSize = pSend - (char*)sendBuf;
     retValue = readOneFrame(frameSize,(char*)"SETHR",NULL,(char*)"SETHRACK",1);

     return retValue;
 }
#endif

 UINT8 xl618:: setUPG(GSE_UPGTYPE data[],int rowCount)
{
    // char * temp="100000000000";

     UINT8 retValue = ERR_UNIVERSAL;
     char *pSend = (char*)sendBuf;
     strcpy(pSend,STR_UPG CR "ALLDATA0;");
     pSend += strlen(STR_UPG CR "ALLDATA0;");

    // printf("rowCount=%d\n",rowCount);

     for(int i=0;i<rowCount;i++)
     {
      //  pSend += sprintf(pSend,"%s",temp);
       memcpy(pSend, &data[i], sizeof(data[i]));
       //printf("GSE_UPGTYPE%d\n", data[i].DA1.stVal );
       pSend += sizeof(data[i]);
       //printf(" sizeof(&data[i])=%d", sizeof(data[i]));
     }
      memcpy(pSend, CR , sizeof(CR));

    UINT16 frameSize = pSend - (char*)sendBuf;
    retValue = readOneFrame(frameSize,(char*)"UPG",NULL,(char*)"CRACK",0);

     return retValue;

}

 UINT8 xl618::scanGSE(pGSE_SCANTYPE data) //get appid
 {

    UINT8 retValue = ERR_UNIVERSAL;
    char *pSend = (char*)sendBuf;
    UINT16 frameSize;
    char *temp;
    QString strTemp ;
    pSend += sprintf(pSend,"GOOSESCAN\r\nEN;1\r\n");

    frameSize = pSend - (char*)sendBuf;

     if((retValue = readOneFrame(frameSize,(char*)"GOOSESCAN",NULL,NULL,2)) == ERR_RIGHT)
     {
             temp = strstr((char*)recvBuf,"GNUM;");
             if(temp)
                     data->GNUM = atoi(&temp[strlen("GNUM;")]);
             else
                     data->GNUM = 0;

             for(int i =0;i<16;i++)
             {
                 strTemp =  QObject::trUtf8("ID%1;").arg(i+1);

                 temp = strstr((char*)recvBuf,(char *)strTemp.toLatin1().data());
                 if(temp)
                         data->APPID[i] = atoi(&temp[strlen((char *)strTemp.toLatin1().data())]);
                 else
                         data->APPID[i] = 0;

             }
     }


    return retValue;
 }

 UINT8 xl618:: setGSE_RGDATA(pGSE_RGDATATYPE data)
{

     UINT8 retValue = ERR_UNIVERSAL;
     char *pSend = (char*)sendBuf;
     UINT16 frameSize;
     char *temp;

     pSend += sprintf(pSend,"RGDATA\r\n");

     frameSize = pSend - (char*)sendBuf;

      if((retValue = readOneFrame(frameSize,(char*)"RGDATA",NULL,NULL,0)) == ERR_RIGHT)
      {
              temp = strstr((char*)recvBuf,"STNUM;");
              if(temp)
                      data->STNUM = atoi(&temp[strlen("STNUM;")]);
              else
                      data->STNUM = 0;

              temp = strstr((char*)recvBuf,"SQNUM;");
              if(temp)
                      data->SQNUM = atoi(&temp[strlen("SQNUM;")]);
              else
                      data->SQNUM = 0;

              temp = strstr((char*)recvBuf,"CHLNUM;");
              if(temp)
                      data->CHLNUM = atoi(&temp[strlen("CHLNUM;")]);
              else
                      data->CHLNUM = 0;

              temp = strstr((char*)recvBuf,"ALLDATA;");
              INT32 size = dataSize - (temp - (char*)recvBuf);//不要前面几个字节

               if(temp)
               {
                   if(size <= 3000) //判断是否够存放
                   {
                       memcpy(data->ALLDATA,&temp[strlen("ALLDATA;")],size-20);//去掉RGDATAACK字节
                       //qDebug("data->ALLDATA==%s\n",data->ALLDATA);
                      // data->frame_size=size-12;
                           //memcpy(data->frame,&temp[strlen("DATA;")],size);
                   }else

                           retValue = ERR_BUFFER;
               }
      }else
          retValue = ERR_BUFFER;

     return retValue;

}

 UINT8 xl618::GSEFrame(pGSE_FRAMETYPE data)
 {
     UINT8 retValue = ERR_UNIVERSAL;
     char *pSend = (char*)sendBuf;
     UINT16 frameSize;
     char *temp;

     pSend += sprintf(pSend,"GOOSEFRAME\r\n");

     frameSize = pSend - (char*)sendBuf;

    if((retValue = readOneFrame(frameSize,(char*)"GOOSEFRAME",NULL,NULL,1)) == ERR_RIGHT)
    {
        temp = strstr((char*)recvBuf,"DATA;");
        INT32 size = dataSize - (temp - (char*)recvBuf);//不要前面几个字节

         if(temp)
         {
                 if(size <= 3000) //判断是否够存放
                 {
                     memcpy(data->frame,&temp[strlen("DATA;")],size-20);//去掉GOOSEFRAME字节
                    // data->frame_size=size-12;
                         //memcpy(data->frame,&temp[strlen("DATA;")],size);
                 }else

                         retValue = ERR_BUFFER;
         }

    }else
         retValue = ERR_BUFFER;
     return retValue;
 }
 UINT8 xl618::RWAVEALL(UINT32 *samnum,UINT32 *chnum,INT32 *data,UINT32 dataSize)
 {
         UINT8 retValue = ERR_UNIVERSAL;
 #if 1
         char *pSend = (char*)sendBuf;

         strcpy(pSend,STR_RWAVEALL);
         pSend += strlen(STR_RWAVEALL);

         UINT16 frameSize = pSend - (char*)sendBuf;

         UINT32 iSize;
         char *temp;


         if((retValue = readOneFrame(frameSize,(char*)"RWAVEALL",NULL,(char*)"RWAVEALLACK",3)) == ERR_RIGHT)
         {//解析帧
                 temp = strstr((char*)&recvBuf[8],"SAMNUM;");
                 if(temp)
                         *samnum = atoi((char*)&temp[strlen("SAMNUM;")]);

                 temp = strstr((char*)&recvBuf[8],"CHNUM;");
                 if(temp)
                         *chnum = atoi((char*)&temp[strlen("CHNUM;")]);

                 temp = strstr(temp,CR);

                 iSize = (*samnum) * (*chnum) * 4;
                // iSize = *samnum * *chnum * 8;

                 if(dataSize >= iSize)
                 {
                         memcpy(data,&temp[strlen(CR)],iSize);
                 }else
                 {
                         retValue = ERR_BUFFER;
                 }
         }
 #endif

         return retValue;
 }
