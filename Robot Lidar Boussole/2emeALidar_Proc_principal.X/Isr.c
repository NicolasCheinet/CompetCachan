void  __interrupt(low_priority) myLowIsr(void) //reste T0
{
    if (TMR3IE&&TMR3IF) //9ms + 5*400us pour I2C boussole
    {
        switch (CPTTMR3)
        {
            case 0 : TMR3=29536; if (CPTNBECHTMR3==0) {MAGN_X=0; MAGN_Y=0;}     SSP2_Write(0x0C,0x31,0x01);             CPTTMR3++;    break;
            case 1 : TMR3=63936;                         SSP2_Read(0x0C,0x12);                                          CPTTMR3++;    break;
            case 2 : TMR3=63936; MAGN_X_H=SSP2_DATA_ISR; SSP2_Read(0x0C,0x11);                                          CPTTMR3++;    break;
            case 3 : TMR3=63936; MAGN_X_L=SSP2_DATA_ISR; SSP2_Read(0x0C,0x14); MAGN_X=MAGN_X+(MAGN_X_H<<8)+MAGN_X_L;    CPTTMR3++;    break;
            case 4 : TMR3=63936; MAGN_Y_H=SSP2_DATA_ISR; SSP2_Read(0x0C,0x13);                                          CPTTMR3++;    break;
            case 5 : TMR3=63936; MAGN_Y_L=SSP2_DATA_ISR; SSP2_Read(0x0C,0x18); MAGN_Y=MAGN_Y+(MAGN_Y_H<<8)+MAGN_Y_L;
                     CPTTMR3=0; CPTNBECHTMR3++;
                     if (CPTNBECHTMR3==16)
                     {
                         MAGN_X=MAGN_X>>4; MAGN_X=MAGN_X-MAGN_X_ZERO;
                         MAGN_Y=MAGN_Y>>4; MAGN_Y=MAGN_Y-MAGN_Y_ZERO;
                         MAGN_Y=(signed short)(RAPPORT_X_Y*MAGN_Y);
                         ANGLE_MAGN=atan((float)((float)MAGN_Y/(float)MAGN_X));
                         ANGLE_MAGN=ANGLE_MAGN*(float)(180/M_PI);
                         if      (MAGN_X<0) {ANGLE_MAGN=180+ANGLE_MAGN;}
                         else if (MAGN_Y<0) {ANGLE_MAGN=360+ANGLE_MAGN;}
                         if (ANGLE_MAGN==450)   {ANGLE_MAGN=270;}
                         CPTNBECHTMR3=0;
                     }
                     break;
        }
        TMR3IF=0;
    }
    if (TMR6IF&&TMR6IE) // 1ms asservissement moteurs
    {
        if (CPT_ENC==1) {CPT_ENC=2;}
        else if (CPT_ENC==2)
        {
            if      (CENCA==0&&CENCB==0) {if (!SELECTEURA&& SELECTEURB)     {CENCA=1;}    else    {CENCA=0;}}
            else if (CENCA==1&&CENCB==0) {if (!SELECTEURA&&!SELECTEURB)     {CENCA=2;}    else    {CENCA=0;}}
            else if (CENCA==2&&CENCB==0) {if ( SELECTEURA&&!SELECTEURB)     {CENCA=3;}    else    {CENCA=0;}}
            else if (CENCA==3&&CENCB==0) {if ( SELECTEURA&& SELECTEURB)     {consigne++;}           CENCA=0;}
            if      (CENCB==0&&CENCA==0) {if ( SELECTEURA&&!SELECTEURB)     {CENCB=1;}    else    {CENCB=0;}}
            else if (CENCB==1&&CENCA==0) {if (!SELECTEURA&&!SELECTEURB)     {CENCB=2;}    else    {CENCB=0;}}
            else if (CENCB==2&&CENCA==0) {if (!SELECTEURA&& SELECTEURB)     {CENCB=3;}    else    {CENCB=0;}}
            else if (CENCB==3&&CENCA==0) {if ( SELECTEURA&& SELECTEURB)     {consigne--;}           CENCB=0;}
            CPT_ENC=0;
        }
        if (RC1STAbits.FERR||RC1STAbits.OERR)  {RC1STAbits.CREN=0; RC1STAbits.CREN=1; RC1REG; RC1REG;} //test et déplantage UART1
        if (RC2STAbits.FERR||RC2STAbits.OERR)  {RC2STAbits.CREN=0; RC2STAbits.CREN=1; RC2REG; RC2REG;} //test et déplantage UART2
        millisecondes++;
        erreurg=(signed short)(-consigneg-(87000/tempsg)); erreurd=(signed short)(-consigned-(87000/tempsd));
        if (consigneg==0)   {sigmaerreurg=0;}   else    {sigmaerreurg=sigmaerreurg+erreurg;}
        if (consigned==0)   {sigmaerreurd=0;}   else    {sigmaerreurd=sigmaerreurd+erreurd;}
        if (sigmaerreurg>sigmamax)  {sigmaerreurg=sigmamax;}    else if (sigmaerreurg<-sigmamax) {sigmaerreurg=-sigmamax;}
        if (sigmaerreurd>sigmamax)  {sigmaerreurd=sigmamax;}    else if (sigmaerreurd<-sigmamax) {sigmaerreurd=-sigmamax;}
        pwmgc=(signed short)(kpm*erreurg)+(signed short)(kim*sigmaerreurg);
        pwmdc=(signed short)(kpm*erreurd)+(signed short)(kim*sigmaerreurd);
        if (pwmgc>128)  {pwmgc=128;}    else if (pwmgc<-127) {pwmgc=-127;}
        if (pwmdc>128)  {pwmdc=128;}    else if (pwmdc<-127) {pwmdc=-127;}
        pwmgc=pwmgc+127; pwmdc=pwmdc+127;
        if (RC2IE)
        {
            CPT_BT++;
            if (CPT_BT>=100)    {BTC=0; PWMG=127;  PWMD=127; CPT_BT=100;}
            else                {BTC=1; PWMG=(unsigned char)pwmgc;  PWMD=(unsigned char)pwmdc;}
        }
        else
        {
            PWMG=(unsigned char)pwmgc;  PWMD=(unsigned char)pwmdc;
            BTC=0;
        }
        TMR6IF=0;
    }
}

void  __interrupt(high_priority) myHighIsr(void) //reste T0
{
    if (IOCIF&&IOCIE)
    {
        if (IOCBF0==1) {tempsg=TMR1; TMR1=0; if ((!CODEURAG&&CODEURBG)||(CODEURAG&&!CODEURBG)) {tempsg=-tempsg; CPT_ENCG++;} else {CPT_ENCG--;} IOCBF0=0;} //codeur B gauche
        if (IOCCF5==1) {tempsd=TMR5; TMR5=0; if ((CODEURAD&&!CODEURBD)||(!CODEURAD&&CODEURBD)) {tempsd=-tempsd; CPT_ENCD++;} else {CPT_ENCD--;} IOCCF5=0;} //codeur A droite
        if (IOCAF6||IOCCF2) //SELECTEUR A ou B
        {
            CPT_ENC=1;
            IOCAF6=0; IOCCF2=0;
        }
        IOCIF=0;
    }
    if (TMR1IF&&TMR1IE) {tempsg=87001; TMR1IF=0; TMR1=0;} //mesure temps moteur gauche
    if (TMR5IF&&TMR5IE) {tempsd=87001; TMR5IF=0; TMR5=0;} //mesure temps moteur droite
    if (SSP2IE&&SSP2IF)
    {
        if (WRITE&&!READ)
        {
            switch (CSSP2)
            {
                case 0 : SSP2BUF=(unsigned char)(SSP2_ADD_ISR<<1);  CSSP2=1; break;
                case 1 : SSP2BUF=SSP2_REG_ISR;                      CSSP2=2; break;
                case 2 : SSP2BUF=SSP2_DATA_ISR;                     CSSP2=3; break;
                case 3 : SSP2CON2bits.PEN=1;                        CSSP2=0; break;
            }
        }
        if (!WRITE&&READ)
        {
            switch (CSSP2)
            {
                case 0 : SSP2BUF=(unsigned char)(SSP2_ADD_ISR<<1);      CSSP2=1; break;
                case 1 : SSP2BUF=SSP2_REG_ISR;                          CSSP2=2; break;
                case 2 : SSP2CON2bits.SEN=1;                            CSSP2=3; break;
                case 3 : SSP2BUF=(unsigned char)(SSP2_ADD_ISR<<1)+1;    CSSP2=4; break;
                case 4 : SSP2CON2bits.RCEN=1;                           CSSP2=5; break;
                case 5 : SSP2CON2bits.PEN=1; SSP2_DATA_ISR=SSP2BUF;     CSSP2=0; break;
            }
        }
        SSP2IF=0;
    }
    if (RC1IE&&RC1IF) //UART avec l'autre proc (lidar), 115200 aussi ??
    {
        if (!RC1STAbits.FERR&&!RC1STAbits.OERR)
        {
            switch (CPT_RC1)
            {
                case 0 : if (RC1REG==255)                                                 {CPT_RC1=1;}  else {CPT_RC1=0;}  break;
                case 1 : if (RC1REG==254)                                                 {CPT_RC1=2;}  else {CPT_RC1=0;}  break;
                case 2 : angle_min=RC1REG; angle_min=angle_min<<8;                         CPT_RC1=3;                      break;
                case 3 : angle_min=angle_min+RC1REG; angle_min_r=angle_min;                CPT_RC1=4;                      break;
                case 4 : distance_min=RC1REG; distance_min=distance_min<<8;                CPT_RC1=5;                      break;
                case 5 : distance_min=distance_min+RC1REG; distance_min_r=distance_min;    CPT_RC1=6;                      break;
                case 6 : angle_max=RC1REG; angle_max=angle_max<<8;                         CPT_RC1=7;                      break;
                case 7 : angle_max=angle_max+RC1REG; angle_max_r=angle_max;                CPT_RC1=8;                      break;
                case 8 : distance_max=RC1REG; distance_max=distance_max<<8;                CPT_RC1=9;                      break;
                case 9 : distance_max=distance_max+RC1REG; distance_max_r=distance_max;    CPT_RC1=0;                      break;
            }
        }
        else
        {
            RC1STAbits.CREN=0; RC1STAbits.CREN=1; RC1REG; RC1REG;
        }
        RC1IF=0;
    }
    if (RC2IE&&RC2IF) //UART du bluetooth 115200 bauds, j'arrive pas Ã  lui envoyer de commandes AT
    {
        if (!RC2STAbits.FERR&&!RC2STAbits.OERR)
        {
            RC2REG_DATA=RC2REG;
            switch (CPT_RC2)
            {
                case 0 :
                    if      (RC2REG_DATA==241) {sens=1;           CPT_RC2=1;} //avant
                    else if (RC2REG_DATA==242) {sens=-1;          CPT_RC2=1;} //arrière
                    else if (RC2REG_DATA==243) {consignevitrc=0;  CPT_RC2=1;} //stop
                    else if (RC2REG_DATA==244) {consignevitrc=0;  CPT_RC2=0;} //mode levage
                    else if (RC2REG_DATA==245) {consignevitrc=0;  CPT_RC2=0;} //tout à l'arret
                    else                       {consignevitrc=0;  CPT_RC2=0;} //erreur
                    break;
               case 1 :
                    if (RC2REG_DATA<=90) {consignevitrc=RC2REG_DATA; CPT_RC2=2;}
                    else                 {CPT_RC2=0;}
                    break;
               case 2 :
                    if (RC2REG_DATA<=180)
                    {
                        if (consignevitrc!=0)   {consigneg=sens*(consignevitrc-((RC2REG_DATA-90)>>1)*consignevitrc/90); consigned=sens*(consignevitrc+((RC2REG_DATA-90)>>1)*consignevitrc/90);}
                        else                    {consigneg=(90-RC2REG_DATA)>>1;                                         consigned=(RC2REG_DATA-90)>>1;}
                        CPT_RC2=3;
                    }
                    else    {CPT_RC2=0;}
                    break;
               case 3 :
                    if ((RC2REG_DATA&1)==1)     {} //E
                    if ((RC2REG_DATA&2)==2)     {} //D
                    if ((RC2REG_DATA&4)==4)     {} //C
                    if ((RC2REG_DATA&8)==8)     {} //B
                    if ((RC2REG_DATA&16)==16)   {} //A
                    if ((RC2REG_DATA&32)==32)   {} //horn
                    if ((RC2REG_DATA&64)==64)   {} //back
                    if ((RC2REG_DATA&128)==128) {} //front
                    CPT_RC2=0;
                    CPT_BT=0;
                    break;
            }
        }
        else
        {
            RC2STAbits.CREN=0; RC2STAbits.CREN=1; RC2REG; RC2REG;
        }
        RC2IF=0;
    }
}