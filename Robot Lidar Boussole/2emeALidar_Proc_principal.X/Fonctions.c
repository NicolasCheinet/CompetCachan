unsigned long Get_Millis(void)      {return millisecondes;}
void Set_Millis (unsigned long t)   {millisecondes=t;}

unsigned short ENC_Get_Value (void) {return consigne;}
void ENC_Set_Value (unsigned short val) {consigne=val;}
void Motors_Speed(signed char vitg, signed char vitd) {consigneg=vitg; consigned=vitd;}
void Motors_Enable(unsigned char enable) {sigmaerreurg=0; sigmaerreurd=0; ENL298=enable;}
signed long Motors_Get_Enc_Left (void) {return CPT_ENCG;}
void Motors_Set_Enc_Left (signed long cpt) {CPT_ENCG=cpt;}
signed long Motors_Get_Enc_Right (void) {return CPT_ENCD;}
void Motors_Set_Enc_Right (signed long cpt) {CPT_ENCD=cpt;}
/*
void Motors_Turn_Angle (signed short angle)
{
    signed long encg=Motors_Get_Enc_Left();
    signed long encd=Motors_Get_Enc_Right();
    angle=(signed short)(angle*1.63);
    if (angle>0)
    {
        Motors_Speed(7,-7);
        while ((consigneg!=0)||(consigned!=0))
        {
            if (consigneg!=0) {if (Motors_Get_Enc_Left()==(encg+angle))   {consigneg=0;}}
            if (consigned!=0) {if (Motors_Get_Enc_Right()==(encd-angle))  {consigned=0;}}
        }
    }
    else if (angle<0)
    {
        Motors_Speed(-7,7);
        while ((consigneg!=0)||(consigned!=0))
        {
            if (consigneg!=0) {if (Motors_Get_Enc_Left()==(encg+angle))   {consigneg=0;}}
            if (consigned!=0) {if (Motors_Get_Enc_Right()==(encd-angle))  {consigned=0;}}
        }
    }
}
*/

void Motors_Turn_Angle (signed short angle)
{
    signed long encg=Motors_Get_Enc_Left();
    signed long encd=Motors_Get_Enc_Right();
    signed long erreurag=1;
    signed long erreurad=1;
    float anglefloat=0;
    float coeff=1.655;
    angle = -angle; //Ligne Jérôme
    anglefloat=((float)(angle)*coeff)+resteturna; // à virgule
    angle=(signed short)(anglefloat); // entier
    resteturna=anglefloat-angle; // reste
    while ((erreurag!=0)||(erreurad!=0))
    {
        erreurag=angle-Motors_Get_Enc_Left()+encg;
        erreurad=angle+Motors_Get_Enc_Right()-encd;
        if (erreurag>7)   {erreurag=7;} else if (erreurag<-7) {erreurag=-7;}
        if (erreurad>7)   {erreurad=7;} else if (erreurad<-7) {erreurad=-7;}
        Motors_Speed((signed char)erreurag,(signed char)-erreurad);
        __delay_us(200);
    }
}

unsigned char BP_Get_Status (void) {return SELECTEURS;}
unsigned char BP_Get_Rising_Edge (void)
{
    __delay_ms(10);
    BP_Status=SELECTEURS;
    if (BP_Status>Old_BP_Status)   {BP_Edge=1;}
    else                           {BP_Edge=0;}
    Old_BP_Status=BP_Status;
    return BP_Edge;
}
unsigned char BP_Get_Falling_Edge (void)
{
    __delay_ms(10);
    BP_Status=SELECTEURS;
    if (BP_Status<Old_BP_Status)   {BP_Edge=1;}
    else                           {BP_Edge=0;}
    Old_BP_Status=BP_Status;
    return BP_Edge;
}

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

void Compass_Init(void)
{
    LCD_Goto(0,0); LCD_Print(" INITIALISATION ");
    LCD_Goto(1,0); LCD_Print("      I2C       ");
    __delay_ms(500);
    LCD_Clear();
    /*
    SSP2_Read(0x69,0x00);
    LCD_Goto(1,7); LCD_Write_8bits(SSP2_DATA_ISR,3);
    LCD_Goto(0,0); LCD_Print("    WHO AM I ?  ");
    __delay_ms(100);
    
    SSP2_Read(0x69,0x06);
    LCD_Goto(1,7); LCD_Write_8bits(SSP2_DATA_ISR,3);
    LCD_Goto(0,0); LCD_Print("   Registre 0x06 "); // 65=sleep
    __delay_ms(100);
    */
    SSP2_Write(0x69,0x06,0x01); // unsleep
    __delay_ms(1);
    /*
    SSP2_Read(0x69,0x06);
    LCD_Goto(1,7); LCD_Write_8bits(SSP2_DATA_ISR,3);
    LCD_Goto(0,0); LCD_Print("Nv Registre 0x06 "); // 1=unsleep
    __delay_ms(100);
    
    SSP2_Read(0x69,0x0F);
    LCD_Goto(1,7); LCD_Write_8bits(SSP2_DATA_ISR,3);
    LCD_Goto(0,0); LCD_Print("   Registre 0x0F "); // 0=bypass off
    __delay_ms(100);
    */
    SSP2_Write(0x69,0x0F,0x02); // bypass on
    __delay_ms(1);
    /*
    SSP2_Read(0x69,0x0F);
    LCD_Goto(1,7); LCD_Write_8bits(SSP2_DATA_ISR,3);
    LCD_Goto(0,0); LCD_Print("Nv Registre 0x0F "); // 2=bypass on
    __delay_ms(100);
    */
    SSP2_Read(0x0C,0x01); // reponse 9
    //LCD_Goto(1,7); LCD_Write_8bits(SSP2_DATA_ISR,3);
    //LCD_Goto(0,0); LCD_Print("who am i boussole"); // 2=bypass on
    __delay_ms(1);
    
    if (SSP2_DATA_ISR==9) {LCD_Goto(0,0); LCD_Print(" INITIALISATION "); LCD_Goto(1,0); LCD_Print("    I2C  OK     "); __delay_ms(500);}
    else                  {LCD_Goto(0,0); LCD_Print("INIT I2C ECHOUEE"); LCD_Goto(1,0); LCD_Print(" REDEMARREZ-MOI "); while(1) {NOP();}}
    
    SSP2_Write(0x0C,0x0F,0x00); // mode=0, il attend
    __delay_ms(1);
    
    /*   LCD_Goto(0,0); LCD_Print(" INITIALISATION ");
    LCD_Goto(1,0); LCD_Print("       OK       ");
    __delay_ms(500);*/
}

void Compass_Calibre(void)
{
    unsigned char i=0;
    unsigned char j=0;
    LCD_Goto(0,0); LCD_Print(" CALIB. COMPAS  ");
    LCD_Goto(1,0); LCD_Print("    EN COURS    ");
    Motors_Enable(1);
    __delay_ms(500);
    for (i=0;i<12;i++)
    {
        for (j=0;j<32;j++)
        {
            SSP2_Write(0x0C,0x31,0x01); __delay_ms(9);
            SSP2_Read(0x0C,0x12); __delay_us(400); MAGN_X_H=SSP2_DATA_ISR;
            SSP2_Read(0x0C,0x11); __delay_us(400); MAGN_X_L=SSP2_DATA_ISR;
            MAGN_X=(MAGN_X_H<<8)+MAGN_X_L; MAGN_X_TOTAL=MAGN_X_TOTAL+MAGN_X;
            SSP2_Read(0x0C,0x14); __delay_us(400); MAGN_Y_H=SSP2_DATA_ISR;
            SSP2_Read(0x0C,0x13); __delay_us(400); MAGN_Y_L=SSP2_DATA_ISR;
            MAGN_Y=(MAGN_Y_H<<8)+MAGN_Y_L; MAGN_Y_TOTAL=MAGN_Y_TOTAL+MAGN_Y;
            SSP2_Read(0x0C,0x18); __delay_us(400);
            if (MAGN_X<MAGN_X_MIN)  {MAGN_X_MIN=MAGN_X;}
            if (MAGN_X>MAGN_X_MAX)  {MAGN_X_MAX=MAGN_X;}
            if (MAGN_Y<MAGN_Y_MIN)  {MAGN_Y_MIN=MAGN_Y;}
            if (MAGN_Y>MAGN_Y_MAX)  {MAGN_Y_MAX=MAGN_Y;}
        }
        Motors_Turn_Angle(30);
        __delay_ms(100);
    }
    MAGN_X_ZERO=(signed short)(MAGN_X_TOTAL/384);
    MAGN_Y_ZERO=(signed short)(MAGN_Y_TOTAL/384);
    RAPPORT_X_Y=(float)(((float)(MAGN_X_MAX-MAGN_X_MIN))/((float)(MAGN_Y_MAX-MAGN_Y_MIN))); 
    if (RAPPORT_X_Y<0)  {RAPPORT_X_Y=-RAPPORT_X_Y;}
    Motors_Speed(0,0);
    Motors_Enable(0);
    TMR3ON=1;
    LCD_Goto(0,0); LCD_Print("CALIB. BOUSSOLE ");
    LCD_Goto(1,0); LCD_Print("     FINIE      ");
    ENC_Set_Value(0);
}

void DATAEE_WriteByte(unsigned short bAdd, unsigned char bData)
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
    while (NVMCON1bits.WR);                        // Wait for write to complete
    NVMCON1bits.WREN = 0;
    GIE = 1;               
}

unsigned char DATAEE_ReadByte(unsigned short bAdd)
{
    NVMADRH = (uint8_t)((bAdd >> 8) & 0x03);
    NVMADRL = (uint8_t)(bAdd & 0xFF);
    NVMCON1bits.NVMREG = 0;
    NVMCON1bits.RD = 1;
    NOP();                    // NOPs maybe required for latency at high freq
    NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP();
    return (NVMDAT);
}

void NOTE (unsigned char note, signed char alteration, unsigned char octave, unsigned short duree)
{
    // de C2 Ã  C6
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
    for (i=0;i<(octave-2);i++)  {frequence=frequence*2;}
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
        RA0PPS=0x07; //PWM3
        for(i=0;i<duree;i++) {__delay_ms(1);}
    }
    else if ((frequence>=490)&&frequence<4200)
    {
        T4CLKCON=0b00000001;
        periode=16000000/frequence;
        prediv=(unsigned short)periode/255;
        if      (prediv>64)  {T4CON=0b11110000; prediv=128;}
        else if (prediv>32)  {T4CON=0b11100000; prediv=64;}
        else if (prediv>16)  {T4CON=0b11010000; prediv=32;}
        else if (prediv>8)   {T4CON=0b11000000; prediv=16;}
        else if (prediv>4)   {T4CON=0b10110000; prediv=8;}
        else if (prediv>2)   {T4CON=0b10100000; prediv=4;}
        else if (prediv>1)   {T4CON=0b10010000; prediv=2;}
        else if (prediv>0)   {T4CON=0b10000000; prediv=1;}
        periode=periode/prediv;
        PR4=(unsigned char)periode;
        PWM3DCH=PR2>>1;
        PWM3CON=0b10000000;
        RA0PPS=0x07; //PWM3
        for(i=0;i<duree;i++) {__delay_ms(1);}
    }
    RA0PPS=0;
    BUZZER=0;
}
void NOTE_Start (unsigned char note, signed char alteration, unsigned char octave)
{
    // de C2 Ã  C6
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
    for (i=0;i<(octave-2);i++)  {frequence=frequence*2;}
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
        RA0PPS=0x07; //PWM3
    }
    else if ((frequence>=490)&&frequence<4200)
    {
        T4CLKCON=0b00000001;
        periode=16000000/frequence;
        prediv=(unsigned short)periode/255;
        if      (prediv>64)  {T4CON=0b11110000; prediv=128;}
        else if (prediv>32)  {T4CON=0b11100000; prediv=64;}
        else if (prediv>16)  {T4CON=0b11010000; prediv=32;}
        else if (prediv>8)   {T4CON=0b11000000; prediv=16;}
        else if (prediv>4)   {T4CON=0b10110000; prediv=8;}
        else if (prediv>2)   {T4CON=0b10100000; prediv=4;}
        else if (prediv>1)   {T4CON=0b10010000; prediv=2;}
        else if (prediv>0)   {T4CON=0b10000000; prediv=1;}
        periode=periode/prediv;
        PR4=(unsigned char)periode;
        PWM3DCH=PR2>>1;
        PWM3CON=0b10000000;
        RA0PPS=0x07; //PWM3
    }
}

void NOTE_Stop (void)
{
    RA0PPS=0;
    BUZZER=0;
}

unsigned char ADC_Get_CNYG (void)   {ADPCH=0b000010; __delay_us(100); ADGO=1; while (ADGO) {NOP();} return ADRESH;}
unsigned char ADC_Get_CNYD (void)   {ADPCH=0b100000; __delay_us(100); ADGO=1; while (ADGO) {NOP();} return ADRESH;}
unsigned char ADC_Get_BATT (void)   {ADPCH=0b000111; __delay_us(100); ADGO=1; while (ADGO) {NOP();} return ADRESH;}

void LCD_Write_u8bits(unsigned char var, unsigned char digit)
{
    if (digit>2) {LCD_Write((var/100)+48,DATA);}
    if (digit>1) {LCD_Write(((var%100)/10)+48,DATA);}
    if (digit>0) {LCD_Write((var%10)+48,DATA);}
}

void LCD_Write_s8bits(signed char var, unsigned char digit)
{
    if (var<0)      {LCD_Print("-");}
    else            {LCD_Print("+");}
    LCD_Write_u8bits((unsigned char)(abs(var)), digit);
}

void LCD_Write_u16bits(unsigned short var, unsigned char digit)
{
    if (digit>4) {LCD_Write((var/10000)+48,DATA);}
    if (digit>3) {LCD_Write(((var%10000)/1000)+48,DATA);}
    if (digit>2) {LCD_Write(((var%1000)/100)+48,DATA);}
    if (digit>1) {LCD_Write(((var%100)/10)+48,DATA);}
    if (digit>0) {LCD_Write((var%10)+48,DATA);}
}

void LCD_Write_s16bits(signed short var, unsigned char digit)
{
    if (var<0)      {LCD_Print("-");}
    else            {LCD_Print("+");}
    LCD_Write_u16bits((unsigned short)(abs(var)), digit);
}

void LCD_Write_u32bits(unsigned long var, unsigned char digit)
{
    if (digit>9) {LCD_Write((var/1000000000)+48,DATA);}
    if (digit>8) {LCD_Write(((var%1000000000)/100000000)+48,DATA);}
    if (digit>7) {LCD_Write(((var%100000000) /10000000)+48,DATA);}
    if (digit>6) {LCD_Write(((var%10000000)  /1000000)+48,DATA);}
    if (digit>5) {LCD_Write(((var%1000000)   /100000)+48,DATA);}
    if (digit>4) {LCD_Write(((var%100000)    /10000)+48,DATA);}
    if (digit>3) {LCD_Write(((var%10000)     /1000)+48,DATA);}
    if (digit>2) {LCD_Write(((var%1000)      /100)+48,DATA);}
    if (digit>1) {LCD_Write(((var%100)       /10)+48,DATA);}
    if (digit>0) {LCD_Write((var%10)+48,DATA);}
}

void LCD_Write_s32bits(signed long var, unsigned char digit)
{
    if (var<0)      {LCD_Print("-"); LCD_Write_u32bits((unsigned long)(var), digit);}
    else            {LCD_Print("+"); LCD_Write_u32bits((unsigned long)(-var), digit);}
}

void LCD_Retro (unsigned char retro) {RE=retro;}
unsigned short Compass_Get_Angle_Magn(void) {return (unsigned short)ANGLE_MAGN;}

void LIDAR_Set_Search_Angle (unsigned short ADR)
{
    TX1REG=255;                     while (!TX1STAbits.TRMT) {NOP();} __delay_us(10);
    TX1REG=254;                     while (!TX1STAbits.TRMT) {NOP();} __delay_us(10);
    TX1REG=(unsigned char)(ADR>>8); while (!TX1STAbits.TRMT) {NOP();} __delay_us(10);
    TX1REG=(unsigned char)ADR;      while (!TX1STAbits.TRMT) {NOP();} __delay_us(10);
    TX1REG=255;                     while (!TX1STAbits.TRMT) {NOP();} __delay_us(10);
    TX1REG=254;                     while (!TX1STAbits.TRMT) {NOP();} __delay_us(10);
    TX1REG=(unsigned char)(ADR>>8); while (!TX1STAbits.TRMT) {NOP();} __delay_us(10);
    TX1REG=(unsigned char)ADR;      while (!TX1STAbits.TRMT) {NOP();} __delay_us(10);
}

void LIDAR_Motor_Enable (unsigned char motor_en)
{
    TX1REG=255;                     while (!TX1STAbits.TRMT) {NOP();} __delay_us(10);
    TX1REG=253;                     while (!TX1STAbits.TRMT) {NOP();} __delay_us(10);
    TX1REG=motor_en;                while (!TX1STAbits.TRMT) {NOP();} __delay_us(10);
    TX1REG=255;                     while (!TX1STAbits.TRMT) {NOP();} __delay_us(10);
    TX1REG=253;                     while (!TX1STAbits.TRMT) {NOP();} __delay_us(10);
    TX1REG=motor_en;                while (!TX1STAbits.TRMT) {NOP();} __delay_us(10);
}

unsigned short LIDAR_Get_Angle_min (void) {return angle_min_r;}
unsigned short LIDAR_Get_Angle_max (void) {return angle_max_r;}
unsigned short LIDAR_Get_Distance_min (void) {return distance_min_r;}
unsigned short LIDAR_Get_Distance_max (void) {return distance_max_r;}

void BT_Enable (unsigned char BT)  {RC2IE=(__bit)BT;}
void BT_Send_Char (unsigned char var)   {TX2REG=var; while (!TX2STAbits.TRMT) {NOP();}}
void BT_Send_String (unsigned char *str) {while(*str != '\0') {BT_Send_Char(*str++);}}
unsigned char BT_RC_Connected (void)  {return BTC;}
unsigned char BT_RC_Enabled (void) {return RC2IE;}

void FLAG_Set (void)
{
    TX1REG=255;                     while (!TX1STAbits.TRMT) {NOP();} __delay_us(10);
    TX1REG=252;                     while (!TX1STAbits.TRMT) {NOP();} __delay_us(10);
    TX1REG=1;                       while (!TX1STAbits.TRMT) {NOP();} __delay_us(10);
    GIE=0; GIEH=0; GIEL=0; Motors_Enable(0); while(1) {NOP();}
}

unsigned char Get_Start_Switch (void)
{
    return STARTSW;
}

void Mon_Prog (void)
{
    Motors_Enable(1);
    while(1)
    {
        if (ENC_Get_Value()>65000)     {ENC_Set_Value(359);}
        if (ENC_Get_Value()>359)       {ENC_Set_Value(0);}
        if (BP_Get_Falling_Edge()==1)  {LIDAR_Set_Search_Angle(ENC_Get_Value());}
        LCD_Goto(1,13); LCD_Write_u16bits(Compass_Get_Angle_Magn(),3);
        LCD_Goto(0,13); LCD_Write_u16bits(ENC_Get_Value(),3);
        LCD_Goto(0,0);  LCD_Write_u16bits(LIDAR_Get_Angle_min(),3);
        LCD_Goto(0,6);  LCD_Write_u16bits(LIDAR_Get_Angle_max(),3);
        LCD_Goto(1,0);  LCD_Write_u16bits(LIDAR_Get_Distance_min(),5);
        LCD_Goto(1,6);  LCD_Write_u16bits(LIDAR_Get_Distance_max(),5);
        while ((BT_RC_Connected()==0)&&(BT_RC_Enabled()==1))
        {
            LCD_Goto(0,0); LCD_Print("EN ATTENTE D'UNE");
            LCD_Goto(1,0); LCD_Print("  CONNECTION BT ");
            while (BT_RC_Connected()==0) {NOP();}
            LCD_Clear();
        }   
        /*if (Lidar_Get_Distance_min()<500)
        {
            if (Lidar_Get_Angle_min()>=315) {Motors_Turn_Angle(60,10);}
            if (Lidar_Get_Angle_min()<=45)  {Motors_Turn_Angle(-60,10);}
        }
        else
        {
            Motors_Speed(15,15);
        }*/
    }
}