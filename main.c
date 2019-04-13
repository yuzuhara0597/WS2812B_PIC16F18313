// CONFIG1
#pragma config FEXTOSC = OFF    // FEXTOSC External Oscillator mode Selection bits (Oscillator not enabled)
#pragma config RSTOSC = HFINT32 // Power-up default value for COSC bits (HFINTOSC with 2x PLL (32MHz))
#pragma config CLKOUTEN = OFF   // Clock Out Enable bit (CLKOUT function is disabled; I/O or oscillator function on OSC2)
#pragma config CSWEN = OFF      // Clock Switch Enable bit (The NOSC and NDIV bits cannot be changed by user software)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config MCLRE = ON       // Master Clear Enable bit (MCLR/VPP pin function is MCLR; Weak pull-up enabled )
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config WDTE = OFF       // Watchdog Timer Enable bits (WDT disabled; SWDTEN is ignored)
#pragma config LPBOREN = OFF    // Low-power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out Reset disabled)
#pragma config BORV = LOW       // Brown-out Reset Voltage selection bit (Brown-out voltage (Vbor) set to 2.45V)
#pragma config PPS1WAY = OFF    // PPSLOCK bit One-Way Set Enable bit (The PPSLOCK bit can be set and cleared repeatedly (subject to the unlock sequence))
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset Enable bit (Stack Overflow or Underflow will not cause a Reset)
#pragma config DEBUG = OFF      // Debugger enable bit (Background debugger disabled)

// CONFIG3
#pragma config WRT = OFF        // User NVM self-write protection bits (Write protection off)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (HV on MCLR/VPP must be used for programming.)

// CONFIG4
#pragma config CP = OFF         // User NVM Program Memory Code Protection bit (User NVM code protection disabled)
#pragma config CPD = OFF        // Data NVM Memory Code Protection bit (Data NVM code protection disabled)

#include <xc.h>
#define _XTAL_FREQ 32000000 //32MHz

void ws2812b_init(void);
void ws2812b_reset(unsigned int led_number);
void ws2812b_flash(unsigned char r,unsigned char g,unsigned char b);
void ws2812b_end(void);

void ws2812b_demo(void);

void main(void) {
    ws2812b_init();
    
    ws2812b_reset(7);
    while(1){
        ws2812b_demo();
    }
    
    return;
}
void ws2812b_init(void){
    //ピン設定
    ANSELA = 0;
    TRISA = 0;
    PORTA = 0;
    RA5PPS = 0x04;   //RA5->CLC1:CLC1OUT;    
    
    //Timer2設定
    PR2 = 0x09;
    TMR2 = 0x00;
    PIR1bits.TMR2IF = 0;
    T2CON = 0x04;
    
    //PWM5設定
    PWM5CON = 0x80;    
    PWM5DCH = 0x02;
    PWM5DCL = 0x40; 
    
    //SPI1設定
    SSP1STAT = 0x00;
    SSP1CON1 = 0x23;
    SSP1ADD = 0x00;
    
    //CLC1設定
    CLC1POL = 0x01;
    CLC1SEL0 = 0x10;
    CLC1SEL1 = 0x12;
    CLC1SEL2 = 0x13;
    CLC1SEL3 = 0x00;
    CLC1GLS0 = 0x05;
    CLC1GLS1 = 0x10; 
    CLC1GLS2 = 0x08;
    CLC1GLS3 = 0x20; 
    CLC1CON = 0x80;
}
void ws2812b_reset(unsigned int led_number){
    for(unsigned int i=0;i<led_number;i++){
        ws2812b_flash(0,0,0);
    }
    __delay_us(100);
}
void ws2812b_flash(unsigned char r,unsigned char g,unsigned char b){
    SSP1CON1bits.WCOL = 0;
    SSPBUF = g;
    while(SSP1STATbits.BF == 0);
    
    SSP1CON1bits.WCOL = 0;
    SSPBUF = r;
    while(SSP1STATbits.BF == 0);
    
    SSP1CON1bits.WCOL = 0;
    SSPBUF = b;
    while(SSP1STATbits.BF == 0);
}
void ws2812b_end(void){
    __delay_us(100);
}

void ws2812b_demo(void){
    for(unsigned char i=0;i<255;i++){
        ws2812b_flash(0,0,i);
        ws2812b_flash(0,i,0);
        ws2812b_flash(i,0,0);
        ws2812b_flash(i,0,i);
        ws2812b_flash(i,i,0);
        ws2812b_flash(0,i,i);
        ws2812b_flash(i,i,i);
        ws2812b_end();
        __delay_ms(10);
    }
}
