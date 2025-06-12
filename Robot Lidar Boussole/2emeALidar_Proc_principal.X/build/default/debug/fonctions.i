# 1 "fonctions.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.46\\pic\\include\\language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "fonctions.c" 2
unsigned long Get_millis(void) {return millisecondes;}
void Set_millis (unsigned long t) {millisecondes=t;}
unsigned short Get_valeur_selection (void) {return consigne;}
void Set_valeur_selection (unsigned short val) {consigne=val;}
void Motors_Speed(signed char vitg, signed char vitd) {consigneg=vitg; consigned=vitd;}
void Motors_Enable(unsigned char enable) {ENL298=enable;}
unsigned char Get_BP_status (void) {return SELECTEURS;}

void SSP2_Write (unsigned char SSP2_ADD, unsigned char SSP2_REG, unsigned char SSP2_DATA)
{
    CSSP2=0;
    READ=0;
    WRITE=1;
    SSP2_ADD_ISR=SSP2_ADD;
    SSP2_REG_ISR=SSP2_REG;
    SSP2_DATA_ISR=SSP2_DATA;
    SSP2CON2bits.SEN=1;
}

void SSP2_Read (unsigned char SSP2_ADD, unsigned char SSP2_REG)
{
    CSSP2=0;
    READ=1;
    WRITE=0;
    SSP2_ADD_ISR=SSP2_ADD;
    SSP2_REG_ISR=SSP2_REG;
    SSP2CON2bits.SEN=1;
}

void Init_Compass(void)
{
    SSP2_Read(0x69,0x00);
    gotoLCD(1,9); lcdWrite((SSP2_DATA_ISR/100)+48,DATA);
    gotoLCD(1,10); lcdWrite(((SSP2_DATA_ISR%100)/10)+48,DATA);
    gotoLCD(1,11); lcdWrite((SSP2_DATA_ISR%10)+48,DATA);
    __delay_ms(500);
    SSP2_Write(0x69,0x06,0x01);
    __delay_ms(10);
    SSP2_Read(0x69,0x06);
    gotoLCD(1,9); lcdWrite((SSP2_DATA_ISR/100)+48,DATA);
    gotoLCD(1,10); lcdWrite(((SSP2_DATA_ISR%100)/10)+48,DATA);
    gotoLCD(1,11); lcdWrite((SSP2_DATA_ISR%10)+48,DATA);
    __delay_ms(500);
    SSP2_Write(0x69,0x0F,0b00000010);
    __delay_ms(10);
    SSP2_Write(0x69,0x03,0b00000000);
    __delay_ms(10);
    SSP2_Read(0x0C,0x01);
    gotoLCD(1,9); lcdWrite((SSP2_DATA_ISR/100)+48,DATA);
    gotoLCD(1,10); lcdWrite(((SSP2_DATA_ISR%100)/10)+48,DATA);
    gotoLCD(1,11); lcdWrite((SSP2_DATA_ISR%10)+48,DATA);
    __delay_ms(500);
    SSP2_Write(0x0C,0x31,0b00000000);
    __delay_ms(10);
    SSP2_Write(0x0C,0x31,0b00001000);
    __delay_ms(10);
}

void Calibre_Compass(void)
{
    unsigned short i=0;
    gotoLCD(0,0); lcdPrint(" CALIB.  COMPAS ");
    gotoLCD(1,0); lcdPrint("10TOURS RESTANTS");
    Motors_Enable(1);
    Motors_Speed(16,-16);
    for (i=0;i<NB_ECH;i++)
    {
        SSP2_Read(0x0C,0x12); __delay_ms(1);
        MAGN_X_H=SSP2_DATA_ISR;
        SSP2_Read(0x0C,0x11); __delay_ms(1);
        MAGN_X_L=SSP2_DATA_ISR;
        MAGN_X=(MAGN_X_H<<8)+MAGN_X_L;
        MAGN_X_TOTAL=MAGN_X_TOTAL+MAGN_X;
        SSP2_Read(0x0C,0x14); __delay_ms(1);
        MAGN_Y_H=SSP2_DATA_ISR;
        SSP2_Read(0x0C,0x13); __delay_ms(1);
        MAGN_Y_L=SSP2_DATA_ISR;
        MAGN_Y=(MAGN_Y_H<<8)+MAGN_Y_L;
        MAGN_Y_TOTAL=MAGN_Y_TOTAL+MAGN_Y;
        SSP2_Read(0x0C,0x18);
        __delay_ms(10);
        if (i==NB_ECH/10) {gotoLCD(1,0); lcdPrint("9 TOURS RESTANTS");}
        else if (i==NB_ECH/10*2) {gotoLCD(1,0); lcdPrint("8 TOURS RESTANTS");}
        else if (i==NB_ECH/10*3) {gotoLCD(1,0); lcdPrint("7 TOURS RESTANTS");}
        else if (i==NB_ECH/10*4) {gotoLCD(1,0); lcdPrint("6 TOURS RESTANTS");}
        else if (i==NB_ECH/10*5) {gotoLCD(1,0); lcdPrint("5 TOURS RESTANTS");}
        else if (i==NB_ECH/10*6) {gotoLCD(1,0); lcdPrint("4 TOURS RESTANTS");}
        else if (i==NB_ECH/10*7) {gotoLCD(1,0); lcdPrint("3 TOURS RESTANTS");}
        else if (i==NB_ECH/10*8) {gotoLCD(1,0); lcdPrint("2 TOURS RESTANTS");}
        else if (i==NB_ECH/10*9) {gotoLCD(1,0); lcdPrint("1 TOURS RESTANTS");}
        else if (i==(NB_ECH-1)) {gotoLCD(1,0); lcdPrint("      OK !!!    ");}
    }
    Motors_Speed(0,0);
    Motors_Enable(0);
    __delay_ms(500);
    MAGN_X_ZERO=MAGN_X_TOTAL/NB_ECH;
    MAGN_Y_ZERO=MAGN_Y_TOTAL/NB_ECH;
    TMR3ON=1;
}

void DATAEE_WriteByte(uint16_t bAdd, uint8_t bData)
{
    NVMADRH = (uint8_t)((bAdd >> 8) & 0x03);
    NVMADRL = (uint8_t)(bAdd & 0xFF);
    NVMDAT = bData;
    NVMCON1bits.NVMREG = 0;
    NVMCON1bits.WREN = 1;
    GIE = 0;
    NVMCON2 = 0x55;
    NVMCON2 = 0xAA;
    NVMCON1bits.WR = 1;



    GIE = 1;
}

uint8_t DATAEE_ReadByte(uint16_t bAdd)
{
    NVMADRH = (uint8_t)((bAdd >> 8) & 0x03);
    NVMADRL = (uint8_t)(bAdd & 0xFF);
    NVMCON1bits.NVMREG = 0;
    NVMCON1bits.RD = 1;
    NOP();
    NOP();
    return (NVMDAT);
}

void NOTE (unsigned char note, signed char alteration, unsigned char octave, unsigned short duree)
{

    float frequenceC2=130.81;
    unsigned char intervalle=0;
    unsigned short prediv=0;
    float frequencefloat=0;
    unsigned long frequence=0;
    unsigned long periode=0;
    float RDD2=1.0594631;
    unsigned short i=0;
    switch (note)
    {
        case 'C' : intervalle=0; break;
        case 'D' : intervalle=2; break;
        case 'E' : intervalle=4; break;
        case 'F' : intervalle=5; break;
        case 'G' : intervalle=7; break;
        case 'A' : intervalle=9; break;
        case 'B' : intervalle=11; break;
    }
    frequence=(unsigned long)frequenceC2;
    for (i=0;i<(octave-2);i++) {frequence=frequence*2;}
    frequencefloat=frequence;
    for (i=0;i<(intervalle+alteration);i++) {frequencefloat=frequencefloat*RDD2;}
    frequence=(unsigned long)frequencefloat;
    if ((frequence>126)&&(frequence<490))
    {
        T4CLKCON=0b00000101;
        T4CON=0b10000000;
        periode=31000/frequence;
        PR4=(unsigned char)periode;
        PWM3DCH=PR4>>1;
        PWM3CON=0b10000000;
        RA0PPS=0x07;
        for(i=0;i<duree;i++) {__delay_ms(1);}
    }
    else if ((frequence>=490)&&frequence<4200)
    {
        T4CLKCON=0b00000001;
        periode=16000000/frequence;
        prediv=(unsigned short)periode/255;
        if (prediv>64) {T4CON=0b11110000; prediv=128;}
        else if (prediv>32) {T4CON=0b11100000; prediv=64;}
        else if (prediv>16) {T4CON=0b11010000; prediv=32;}
        else if (prediv>8) {T4CON=0b11000000; prediv=16;}
        else if (prediv>4) {T4CON=0b10110000; prediv=8;}
        else if (prediv>2) {T4CON=0b10100000; prediv=4;}
        else if (prediv>1) {T4CON=0b10010000; prediv=2;}
        else if (prediv>0) {T4CON=0b10000000; prediv=1;}
        periode=periode/prediv;
        PR4=(unsigned char)periode;
        PWM3DCH=PR2>>1;
        PWM3CON=0b10000000;
        RA0PPS=0x07;
        for(i=0;i<duree;i++) {__delay_ms(1);}
    }
    RA0PPS=0;
    BUZZER=0;
}

unsigned char Get_ADC_CNYG (void) {ALIMCNYG=1; ADPCH=0b000010; __delay_us(100); ADGO=1; while (ADGO) {NOP();} ALIMCNYG=0; return ADRESH;}
unsigned char Get_ADC_CNYD (void) {ALIMCNYD=1; ADPCH=0b100000; __delay_us(100); ADGO=1; while (ADGO) {NOP();} ALIMCNYD=0; return ADRESH;}
unsigned char Get_ADC_BATT (void) { ADPCH=0b000111; __delay_us(100); ADGO=1; while (ADGO) {NOP();} return ADRESH;}

void LowIsr(void)
{
 if (RC2IE&&RC2IF)
    {
        RC2REG_DATA=RC2REG;
    }
    if (TMR3IE&&TMR3IF)
    {
        TMR3=49536;
        if (CPTTMR3==0) {MAGN_X=0; MAGN_Y=0;}
        else if (CPTTMR3%11==7) {SSP2_Read(0x0C,0x12);}
        else if (CPTTMR3%11==8) {MAGN_X_H=SSP2_DATA_ISR; SSP2_Read(0x0C,0x11);}
        else if (CPTTMR3%11==9) {MAGN_X_L=SSP2_DATA_ISR; MAGN_X=MAGN_X+(MAGN_X_H<<8)+MAGN_X_L; SSP2_Read(0x0C,0x14);}
        else if (CPTTMR3%11==10) {MAGN_Y_H=SSP2_DATA_ISR; SSP2_Read(0x0C,0x13);}
        else if (CPTTMR3%11==0) {MAGN_Y_L=SSP2_DATA_ISR; MAGN_Y=MAGN_Y+(MAGN_Y_H<<8)+MAGN_Y_L; SSP2_Read(0x0C,0x18);}
        if (CPTTMR3==353)
        {
            MAGN_X=MAGN_X>>5;
            MAGN_Y=MAGN_Y>>5;
            MAGN_X=MAGN_X-MAGN_X_ZERO;
            MAGN_Y=MAGN_Y-MAGN_Y_ZERO;
            ANGLE_MAGN=atan((float)((float)MAGN_Y/(float)MAGN_X));
            ANGLE_MAGN=ANGLE_MAGN*(float)(180/M_PI);
            if (MAGN_X<0) {ANGLE_MAGN=180+ANGLE_MAGN;}
            else if (MAGN_Y<0) {ANGLE_MAGN=360+ANGLE_MAGN;}
            CPTTMR3=0;
        }
        CPTTMR3++;
        TMR3IF=0;
    }
}

void HighIsr(void)
{
 if (RC1IE&&RC1IF)
    {
        switch (CPT_RC1)
        {
            case 0 : if (RC1REG=='A') {CPT_RC1=1;} else {CPT_RC1=0;} break;
            case 1 : if (RC1REG=='-') {CPT_RC1=2;} else {CPT_RC1=0;} break;
            case 2 : angle_min=RC1REG; angle_min=angle_min<<8; CPT_RC1=3; break;
            case 3 : angle_min=angle_min+RC1REG; CPT_RC1=4; break;
            case 4 : if (RC1REG=='D') {CPT_RC1=5;} else {CPT_RC1=0;} break;
            case 5 : if (RC1REG=='-') {CPT_RC1=6;} else {CPT_RC1=0;} break;
            case 6 : distance_min=RC1REG; distance_min=distance_min<<8; CPT_RC1=7; break;
            case 7 : distance_min=distance_min+RC1REG; CPT_RC1=8; break;
            case 8 : if (RC1REG=='A') {CPT_RC1=9;} else {CPT_RC1=0;} break;
            case 9 : if (RC1REG=='+') {CPT_RC1=10;} else {CPT_RC1=0;} break;
            case 10: angle_max=RC1REG; angle_max=angle_max<<8; CPT_RC1=11; break;
            case 11: angle_max=angle_max+RC1REG; CPT_RC1=12; break;
            case 12: if (RC1REG=='D') {CPT_RC1=13;} else {CPT_RC1=0;} break;
            case 13: if (RC1REG=='+') {CPT_RC1=14;} else {CPT_RC1=0;} break;
            case 14: distance_max=RC1REG; distance_max=distance_max<<8; CPT_RC1=15; break;
            case 15: distance_max=distance_max+RC1REG; CPT_RC1=0; break;
        }
        RC1IF=0;
    }
    if (IOCIF&&IOCIE)
    {
        if (IOCAF6||IOCCF2)
        {
            __delay_ms(1);
            if (CENCA==0&&CENCB==0) {if (!SELECTEURA&& SELECTEURB) {CENCA=1;} else {CENCA=0;}}
            else if (CENCA==1&&CENCB==0) {if (!SELECTEURA&&!SELECTEURB) {CENCA=2;} else {CENCA=0;}}
            else if (CENCA==2&&CENCB==0) {if ( SELECTEURA&&!SELECTEURB) {CENCA=3;} else {CENCA=0;}}
            else if (CENCA==3&&CENCB==0) {if ( SELECTEURA&& SELECTEURB) {consigne++;} CENCA=0;}
            if (CENCB==0&&CENCA==0) {if ( SELECTEURA&&!SELECTEURB) {CENCB=1;} else {CENCB=0;}}
            else if (CENCB==1&&CENCA==0) {if (!SELECTEURA&&!SELECTEURB) {CENCB=2;} else {CENCB=0;}}
            else if (CENCB==2&&CENCA==0) {if (!SELECTEURA&& SELECTEURB) {CENCB=3;} else {CENCB=0;}}
            else if (CENCB==3&&CENCA==0) {if ( SELECTEURA&& SELECTEURB) {consigne--;} CENCB=0;}
            IOCAF6=0; IOCCF2=0;
        }
        if (IOCCF3==1)
        {
           if (!SELECTEURS)
           {





           }
           IOCCF3=0;
        }
        if (IOCBF0==1) {tempsg=TMR1; TMR1=0; if ((CODEURAG&&CODEURBG)||(!CODEURAG&&!CODEURBG)) {tempsg=-tempsg;} IOCBF0=0;}
        if (IOCCF5==1) {tempsd=TMR5; TMR5=0; if ((CODEURAD&&!CODEURBD)||(!CODEURAD&&CODEURBD)) {tempsd=-tempsd;} IOCCF5=0;}
        IOCIF=0;
    }
    if (TMR1IF&&TMR1IE) {tempsg=87001; TMR1IF=0; TMR1=0;}
    if (TMR5IF&&TMR5IE) {tempsd=87001; TMR5IF=0; TMR5=0;}
    if (TMR6IF&&TMR6IE)
    {
       if (RC1STAbits.FERR||RC1STAbits.OERR) {RC1STAbits.CREN=0; RC1STAbits.CREN=1; RC1REG; RC1REG;}
       if (RC2STAbits.FERR||RC2STAbits.OERR) {RC2STAbits.CREN=0; RC2STAbits.CREN=1; RC2REG; RC2REG;}
       millisecondes++;
       erreurg=(signed short)(consigneg-(87000/tempsg));
       sigmaerreurg=sigmaerreurg+erreurg;
       if (sigmaerreurg>255) {sigmaerreurg=255;}
       if (sigmaerreurg<-255) {sigmaerreurg=-255;}
       pwmgc=(kpm*erreurg/10)+(kim*sigmaerreurg/100);
       if (pwmgc>128) {pwmgc=128;}
       if (pwmgc<-127) {pwmgc=-127;}
       pwmgc=pwmgc+127;
       PWMG=(unsigned char)pwmgc;
       erreurd=(signed short)(-consigned-(87000/tempsd));
       sigmaerreurd=sigmaerreurd+erreurd;
       if (sigmaerreurd>255) {sigmaerreurd=255;}
       if (sigmaerreurd<-255) {sigmaerreurd=-255;}
       pwmdc=(kpm*erreurd/10)+(kim*sigmaerreurd/100);
       if (pwmdc>128) {pwmdc=128;}
       if (pwmdc<-127) {pwmdc=-127;}
       pwmdc=pwmdc+127;
       PWMD=(unsigned char)pwmdc;
       TMR6IF=0;
    }
    if (SSP2IE&&SSP2IF)
    {
        if (WRITE&&!READ)
        {
            switch (CSSP2)
            {
                case 0 : SSP2BUF=(unsigned char)(SSP2_ADD_ISR<<1); CSSP2=1; break;
                case 1 : SSP2BUF=SSP2_REG_ISR; CSSP2=2; break;
                case 2 : SSP2BUF=SSP2_DATA_ISR; CSSP2=3; break;
                case 3 : SSP2CON2bits.PEN=1; CSSP2=0; break;
            }
        }
        if (!WRITE&&READ)
        {
            switch (CSSP2)
            {
                case 0 : SSP2BUF=(unsigned char)(SSP2_ADD_ISR<<1); CSSP2=1; break;
                case 1 : SSP2BUF=SSP2_REG_ISR; CSSP2=2; break;
                case 2 : SSP2CON2bits.SEN=1; CSSP2=3; break;
                case 3 : SSP2BUF=(unsigned char)(SSP2_ADD_ISR<<1)+1; CSSP2=4; break;
                case 4 : SSP2CON2bits.RCEN=1; CSSP2=5; break;
                case 5 : SSP2CON2bits.PEN=1; SSP2_DATA_ISR=SSP2BUF; CSSP2=0; break;
            }
        }
        SSP2IF=0;
    }
}

void LCD_Write_8bits(unsigned char var, unsigned char digit)
{
    switch (digit)
    {
        case 1 :
            lcdWrite((var%10)+48,DATA);
            break;
        case 2 :
            lcdWrite(((var%100)/10)+48,DATA);
            lcdWrite((var%10)+48,DATA);
            break;
        case 3 :
            lcdWrite((var/100)+48,DATA);
            lcdWrite(((var%100)/10)+48,DATA);
            lcdWrite((var%10)+48,DATA);
            break;
    }
}

void LCD_Write_16bits(unsigned short var, unsigned char digit)
{
    switch (digit)
    {
        case 1 :
            lcdWrite((var%10)+48,DATA);
            break;
        case 2 :
            lcdWrite(((var%100)/10)+48,DATA);
            lcdWrite((var%10)+48,DATA);
            break;
        case 3 :
            lcdWrite(((var%1000)/100)+48,DATA);
            lcdWrite(((var%100)/10)+48,DATA);
            lcdWrite((var%10)+48,DATA);
            break;
        case 4 :
            lcdWrite(((var%10000)/1000)+48,DATA);
            lcdWrite(((var%1000)/100)+48,DATA);
            lcdWrite(((var%100)/10)+48,DATA);
            lcdWrite((var%10)+48,DATA);
            break;
        case 5 :
            lcdWrite((var/10000)+48,DATA);
            lcdWrite(((var%10000)/1000)+48,DATA);
            lcdWrite(((var%1000)/100)+48,DATA);
            lcdWrite(((var%100)/10)+48,DATA);
            lcdWrite((var%10)+48,DATA);
            break;
    }
}

unsigned short Get_Angle_Magn(void) {return (unsigned short)ANGLE_MAGN;}
void Set_Search_Angle (unsigned short angle) {ADR=angle;}

void Send_Search_Angle (unsigned short angle)
{
    TX1REG='A'; while (!TX1STAbits.TRMT) {NOP();} __delay_us(10);
    TX1REG=(unsigned char)(ADR>>8); while (!TX1STAbits.TRMT) {NOP();} __delay_us(10);
    TX1REG=(unsigned char)ADR; while (!TX1STAbits.TRMT) {NOP();} __delay_us(10);
}
unsigned short Get_Search_Angle (void) {return ADR;}
unsigned short Get_Angle_min (void) {return angle_min;}
unsigned short Get_Angle_max (void) {return angle_max;}
unsigned short Get_Distance_min (void) {return distance_min;}
unsigned short Get_Distance_max (void) {return distance_max;}
