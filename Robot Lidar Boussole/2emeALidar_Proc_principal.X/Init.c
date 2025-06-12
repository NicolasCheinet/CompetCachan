void Initialisation(void)
{
    BUZZER=0;
    ENL298=0;
    ALIMCNYD=0;
    ALIMCNYG=0;
    
    //Modules Diable
    PMD0bits.FVRMD=1;
    PMD0bits.HLVDMD=1;
    PMD0bits.CRCMD=1;
    PMD0bits.NVMMD=1;
    PMD0bits.CLKRMD=1;
    PMD2bits.DACMD=1;
    PMD2bits.CMP1MD=1;
    PMD2bits.CMP2MD=1;
    PMD2bits.ZCDMD=1;
    PMD3bits.PWM4MD=1;
    PMD4bits.MSSP1MD=1;
    PMD4bits.CWG1MD=1;
    PMD5bits.DSMMD=1;
    
    //ANSEL
    ANSELA=0b10000100;
    ANSELB=0b00000000;
    ANSELC=0b00000000;
    ANSELD=0b00000000;
    ANSELE=0b00000001;
    
    //TRIS
    TRISA=0b11110110;
    TRISB=0b11000001;    
    TRISC=0b00111101;
    TRISD=0b01001101;
    TRISE=0b00001001;
            
    //PULL UP
    WPUA=0b01000000;
    WPUB=0b11000000;
    WPUC=0b00001100;
    WPUD=0b00000000;
    WPUE=0b00000000;
        
    //ADC 
    ADCON0=0b10000000;
    ADCON1=0b00000000;
    ADCON2=0b00000000;
    ADCON3=0b00000000;
    ADCLK=0b00011111;
    ADREF=0b00000000;
        
    //I2C
    
    SSP2STAT=0b00000000;
    SSP2ADD=0x22;
    SSP2CON1=0b00101000;
    SSP2CON3=0b00000000;
    
    
   //UART1 proc lidar
    TXSTA1bits.SYNC=0; TXSTA1bits.TX9=0; TXSTA1bits.TXEN=1;
    RCSTA1bits.RX9=0; RCSTA1bits.CREN=1; RCSTA1bits.SPEN=1;
    BAUDCON1bits.BRG16=1; TXSTA1bits.BRGH=0; SPBRG1=34; //115200 bauds
    
    //UART2 Bluetooth
    TXSTA2bits.SYNC=0; TXSTA2bits.TX9=0; TXSTA2bits.TXEN=1;
    RCSTA2bits.RX9=0; RCSTA2bits.CREN=1; RCSTA2bits.SPEN=1;
    BAUDCON2bits.BRG16=1; TXSTA2bits.BRGH=0; SPBRG2=34; //115200 bauds
 
    //PWMs
    
    T2PR=255;
    CCPTMRS=0b10100101; //TMR2 pour CCP1&2 et TM4 pour PWM 3&4
    
    CCP1CON=0b10011100;
    CCPR1H=127;
    CCPR1L=0;
        
    CCP2CON=0b10011100;
    CCPR2H=127;
    CCPR2L=0;
    
    //PPS
        
    RC6PPS=0x05;        // moteur gauche CCP1
    RC7PPS=0x06;        // moteur droite CCP2
    //RA0PPS=0x07;        // Buzzer PWM3
    RD2PPS=0x11;        // I2C SCL MSSP2
    RD3PPS=0x12;        // I2C SDA MSSP2
    SSP2CLKPPS=0b00011010; // I2C SCL MSSP2
    SSP2DATPPS=0b00011011; // I2C SDA MSSP2
    RX2PPS=0b00011000;  // RX2 Bluetooth sur RD0
    RD1PPS=0x0B;        // TX2 Bluetooth sur RD1
    RX1PPS=0b00010000;  // RX1 autre proc sur RC0
    RC1PPS=0x09;        // TX1 autre proc sur RC1
        
    //TIMERS
    T1CON=0b00110111; //pour asserv moteur gauche
    TMR1CLK=0b00000001;
            
    T2CON=0b10010000; //pour PWM
    T2CLKCON=0b00000001;
    
    // TIMER 4 est utilisé pour la PWM3 musique
    
    
    T3CON=0b00100110; // lecture boussole, 9ms apr�s envoi commande one shoot, puis 400us pour lecture, OFF pour l'instant, pré = 1/4
    TMR3CLK=0b00000001; //Fosc/4
    TMR3=29536; // 9000us / 0.0625 = 144000 --> /4 = 36000, 65536-36000=29536
    //TMR3=63936; // 400us  / 0.0625 = 6400   --> /4 = 1600,  65536-1600=63936
           
    T5CON=0b00110111; //pour asserv moteur droite
    TMR5CLK=0b00000001;
    
    T6CON=0b11100000; // pour asservissement moteurs //1ms
    T6CLKCON=0b00000001;
    T6PR=250;
    
    //INTERRUPTIONS
    GIE=1; GIEH=1; GIEL=1; IPEN=1;
    PEIE=1;
    IOCIE=1;  IOCIP=1;
    
    TMR0IE=0; TMR0IP=0;
    TMR1IE=1; TMR1IP=1;
    TMR2IE=0; TMR2IP=0;
    TMR3IE=1; TMR3IP=0;
    TMR4IE=0; TMR4IP=0;
    TMR5IE=1; TMR5IP=1;
    TMR6IE=1; TMR6IP=0;
    
    SSP2IF=0;
    SSP2IE=1;
    SSP2IP=1;
    
    RC1IP=1; RC1IE=1;
    RC2IP=1; RC2IE=0;
    
    IOCCN2=1;   //SELECTEUR B
    IOCCP2=1;   //SELECTEUR B
    IOCAN6=1;   //SELECTEUR A
    IOCAP6=1;   //SELECTEUR A
    IOCBN0=1;   //CODEUR A GAUCHE
    IOCBP0=1;   //CODEUR A GAUCHE
    IOCCN5=1;   //CODEUR A DROITE
    IOCCP5=1;   //CODEUR A DROITE
            
    LIDAR_Motor_Enable(0);
    LCD_Retro(1);
    LCD_Init();
    LCD_Clear();
    ALIMCNYG=1;
    ALIMCNYD=1;
    LCD_Goto(0,0); LCD_Print("ROBOT CACHAN BUT");
    LCD_Goto(1,0); LCD_Print("LIDAR COMPAS   2");
    __delay_ms(500);
    LCD_Goto(0,0); LCD_Print("ALEXANDRE ULRICH");
    LCD_Goto(1,0); LCD_Print(" IUT DE HAGUENAU");
    __delay_ms(500);
    Compass_Init();
    LCD_Goto(0,0); LCD_Print(" ACTIVER LIDAR ?");
    ENC_Set_Value(0);
    while (BP_Get_Falling_Edge()==0)
    {
        if (ENC_Get_Value()>65000)  {ENC_Set_Value(0);}
        if (ENC_Get_Value()>1)      {ENC_Set_Value(1);}
        switch (ENC_Get_Value())
        {
            case 0 : LCD_Goto(1,0); LCD_Print("  >OUI<   NON   "); break;
            case 1 : LCD_Goto(1,0); LCD_Print("   OUI   >NON<  "); break;
        }
    }
    if (ENC_Get_Value()==0) {LIDAR_Motor_Enable(1);}
    __delay_ms(200);
    LCD_Goto(0,0); LCD_Print("CALIBRER COMPAS?");
    ENC_Set_Value(0);
    while (BP_Get_Falling_Edge()==0)
    {
        if (ENC_Get_Value()>65000)  {ENC_Set_Value(0);}
        if (ENC_Get_Value()>1)      {ENC_Set_Value(1);}
        switch (ENC_Get_Value())
        {
            case 0 : LCD_Goto(1,0); LCD_Print("  >OUI<   NON   "); break;
            case 1 : LCD_Goto(1,0); LCD_Print("   OUI   >NON<  "); break;
        }
    }
    if (ENC_Get_Value()==0)
    {
        __delay_ms(500);
        Compass_Calibre();
    }
    __delay_ms(400);
    LCD_Goto(0,0); LCD_Print("ACTIVER RC BT ? ");
    ENC_Set_Value(1);
    while (BP_Get_Falling_Edge()==0)
    {
        if (ENC_Get_Value()>65000)  {ENC_Set_Value(0);}
        if (ENC_Get_Value()>1)      {ENC_Set_Value(1);}
        switch (ENC_Get_Value())
        {
            case 0 : LCD_Goto(1,0); LCD_Print("  >OUI<   NON   "); break;
            case 1 : LCD_Goto(1,0); LCD_Print("   OUI   >NON<  "); break;
        }
    }
    if (ENC_Get_Value()==0) {BT_Enable(1);}
    ENC_Set_Value(0);
    __delay_ms(500);
    LCD_Clear();
}