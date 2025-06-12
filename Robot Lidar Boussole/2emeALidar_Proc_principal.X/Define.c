#define _XTAL_FREQ 64000000

//RA1 et RA4 pas cablés
#define BUZZER      LATAbits.LATA0
#define CNYG        PORTAbits.RA2
#define ALIMCNYG    LATAbits.LATA3
#define BTCONNECT   PORTAbits.RA5
#define SELECTEURA  PORTAbits.RA6
#define VBAT        PORTAbits.RA7

// RB1-->5 LCD RB6 et 7 prog
#define CODEURBG    PORTBbits.RB0
#define STARTSW     PORTBbits.RB7

#define RXPROC2     PORTCbits.RC0
#define TXPROC2     PORTCbits.RC1
#define SELECTEURB  PORTCbits.RC2
#define SELECTEURS  PORTCbits.RC3
#define CODEURBD    PORTCbits.RC4
#define CODEURAD    PORTCbits.RC5
#define PWMMOTEURG  LATCbits.LATC6
#define PWMMOTEURD  LATCbits.LATC7

//RD5(RE) et RD7 LCD
#define RXBT        PORTDbits.RD0
#define TXBT        PORTDbits.RD1
#define SCL         PORTDbits.RD2
#define SDA         PORTDbits.RD3
#define ENL298      LATDbits.LATD4
#define RE          LATDbits.LATD5
#define CODEURAG    PORTDbits.RD6

//RE2 pas cablé
#define CNYD        PORTEbits.RE0
#define ALIMCNYD    LATEbits.LATE1
#define MCLR        PORTEbits.RE3

#define PWMD        CCPR1H
#define PWMG        CCPR2H

#define LOW_32(var) ((char *)&var)[0]
#define MEL_32(var) ((char *)&var)[1]
#define MEH_32(var) ((char *)&var)[2]
#define HIG_32(var) ((char *)&var)[3]