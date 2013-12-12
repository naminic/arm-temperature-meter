
#ifndef lcd_INCLUDED_
#define lcd_INCLUDED_
#include <stdio.h>
#include "lcd4bit.h"
#include <board.h>
#include <pio/pio.h>
#include <ssc/ssc.h>

//int rs,en,db4,db5,db6,db7,port_add;

//void lcd_pin(int port,int ain,int bin,int cin,int din,int gin,int fin);
void lcd_init(void);
void lcd_putchar(char ch);
void lcd_command(char cmd);
void lcd_gotoxy(char x, char y);
void lcd_clear(void);
void lcd_clear_line(char y);
void lcd_shift_left(char n);
void lcd_shift_right(char n);
void lcd_puts(int num);
void lcd_putsf(char* data);

void lcd_define_char(const char *pc,char char_code);
//***********************************************
//you muste dfine these parameters



//***********************************************

//***********************************************

//***********************************************

//***********************************************


#define F_CPU	18432000
#define clcd_minDelay()	cwait(F_CPU/80000)
#define clcd_Delay()	cwait(F_CPU/8000)

static const Pin Lcdpins[] = {LCD_PINS};
static const Pin sscPins[] = {PIN_SSC_TD, PIN_SSC_TK,PIN_SSC_TF};

//=========================================================
static void cwait (volatile int t) {
  for (;t; t--);
}
// ============================================================================
static void setD4567(char val)
{
	SSC_Write(AT91C_BASE_SSC,0);
	SSC_Write(AT91C_BASE_SSC,val);
}
// ============================================================================
void lcd_putchar(char ch)
{
        PIO_Set(&Lcdpins[0]);
	setD4567(ch);
        PIO_Set(&Lcdpins[1]);
	clcd_minDelay();
        PIO_Clear(&Lcdpins[1]);
	clcd_minDelay();
	setD4567(ch<<4);
        PIO_Set(&Lcdpins[1]);
	clcd_minDelay();
        PIO_Clear(&Lcdpins[1]);
	clcd_Delay();
}
// ============================================================================
void lcd_command(char cmd)	//Sends Command to LCD
{
        PIO_Clear(&Lcdpins[0]);
	setD4567(cmd);
        PIO_Set(&Lcdpins[1]);
	clcd_minDelay();
        PIO_Clear(&Lcdpins[1]);
	clcd_minDelay();
	setD4567(cmd<<4);
        PIO_Set(&Lcdpins[1]);
	clcd_minDelay();
        PIO_Clear(&Lcdpins[1]);
	clcd_Delay();
}
// ============================================================================
void lcd_init()
{
	unsigned int tcmr,tfmr;
  
  PIO_Configure(Lcdpins,PIO_LISTSIZE(Lcdpins));
	PIO_Configure(sscPins, PIO_LISTSIZE(sscPins));
	
  SSC_Configure(AT91C_BASE_SSC,AT91C_ID_SSC,500000,BOARD_MCK);
    
  tcmr=AT91C_SSC_CKS_TK|AT91C_SSC_CKO_DATA_TX|AT91C_SSC_START_CONTINOUS;
  tfmr=SSC_DATLEN(8)|SSC_DATNB(15)|SSC_FSLEN(16)|AT91C_SSC_FSOS_LOW|AT91C_SSC_FSDEN ;
    
  SSC_ConfigureTransmitter(AT91C_BASE_SSC,tcmr,tfmr);
  SSC_EnableTransmitter(AT91C_BASE_SSC);
	
	
        
	// disable peripheral mode of this pins
	clcd_Delay();
	setD4567(0);
	//---------one------
	setD4567((1<<5)|(1<<4));
        PIO_Set(&Lcdpins[1]);
	clcd_minDelay();
        PIO_Clear(&Lcdpins[1]);
	clcd_Delay();
	//-----------two-----------
	setD4567((1<<5)|(1<<4));
        PIO_Set(&Lcdpins[1]);
	clcd_minDelay();
        PIO_Clear(&Lcdpins[1]);
	clcd_Delay();
	//-------three-------------
	setD4567(1<<5);
        PIO_Set(&Lcdpins[1]);
	clcd_minDelay();
        PIO_Clear(&Lcdpins[1]);
	clcd_Delay();
	//--------4 _BV--dual line---------------
	lcd_command(0x28);
	
	//-----increment address, invisible cursor shift------
	lcd_command(0x0c);

	clcd_Delay();
	
	
		// disable peripheral mode of this pins
	clcd_Delay();
	setD4567(0);
	//---------one------
	setD4567((1<<5)|(1<<4));
        PIO_Set(&Lcdpins[1]);
	clcd_minDelay();
        PIO_Clear(&Lcdpins[1]);
	clcd_Delay();
	//-----------two-----------
	setD4567((1<<5)|(1<<4));
        PIO_Set(&Lcdpins[1]);
	clcd_minDelay();
        PIO_Clear(&Lcdpins[1]);
	clcd_Delay();
	//-------three-------------
	setD4567(1<<5);
        PIO_Set(&Lcdpins[1]);
	clcd_minDelay();
        PIO_Clear(&Lcdpins[1]);
	clcd_Delay();
	//--------4 _BV--dual line---------------
	lcd_command(0x28);
	
	//-----increment address, invisible cursor shift------
	lcd_command(0x0c);

	clcd_Delay();
	
	

  PIO_Configure(Lcdpins,PIO_LISTSIZE(Lcdpins));
	PIO_Configure(sscPins, PIO_LISTSIZE(sscPins));
	
	
		// disable peripheral mode of this pins
	clcd_Delay();
	setD4567(0);
	//---------one------
	setD4567((1<<5)|(1<<4));
        PIO_Set(&Lcdpins[1]);
	clcd_minDelay();
        PIO_Clear(&Lcdpins[1]);
	clcd_Delay();
	//-----------two-----------
	setD4567((1<<5)|(1<<4));
        PIO_Set(&Lcdpins[1]);
	clcd_minDelay();
        PIO_Clear(&Lcdpins[1]);
	clcd_Delay();
	//-------three-------------
	setD4567(1<<5);
        PIO_Set(&Lcdpins[1]);
	clcd_minDelay();
        PIO_Clear(&Lcdpins[1]);
	clcd_Delay();
	//--------4 _BV--dual line---------------
	lcd_command(0x28);
	
	//-----increment address, invisible cursor shift------
	lcd_command(0x0c);

	clcd_Delay();

}
// ============================================================================
void lcd_gotoxy(char y, char x)	//Cursor to X Y position
{
	char DDRAMAddr;
x=x-1;

	// remap lines into proper order
	switch(y)
	{
		case 1: DDRAMAddr = 0x00+x; break;
		case 2: DDRAMAddr = 0x40+x; break;
		case 3: DDRAMAddr = 0x14+x; break;
		case 4: DDRAMAddr = 0x54+x; break;
		default: DDRAMAddr = 0x00+x;
	}
	// set data address
	lcd_command(1<<7 | DDRAMAddr);
}

// ============================================================================
void lcd_define_char(const char *pc,char char_code)
{
	char a , i;
	a = ((char_code<<3)|0x40) & 0xff;
	for (i = 0; i < 8 ;i++)
	{
		lcd_command(a++);
		clcd_Delay();
		lcd_putchar(pc[i]);
		clcd_Delay();
	}
}
// ============================================================================
void lcd_clear(void)				//Clears LCD
{
	lcd_command(0x01);
lcd_command(0x02);
	clcd_Delay();
	clcd_Delay();
}

//============================================================
void lcd_shift_left(char n)	//Scrol n of characters Right
{
	char i;
	for (i = 0 ; i < n ; i++)
	{
		lcd_command(0x1E);
		clcd_Delay();
	}
}
//========================================================
void lcd_shift_right(char n)	//Scrol n of characters Left
{
	char i;
	for (i = 0 ; i < n ; i++)
	{
		lcd_command(0x18);
		clcd_Delay();
	}
}
//============================================================================
 void lcd_puts(int num)
{
   int i,j;
   int p,f=0;
   char ch[5];
 for(i=0;i<5;i++)
   { 
     p=1;
     for(j=4-i;j>0;j--)
       p = p*10;
     ch[i] = (num/p);
	 if (num>=p && !f){
	  f=1;
	  
	 }
     num =num - ch[i] *p ;
	 ch[i] = ch[i] +48;
     if(f) lcd_putchar(ch[i]);
  	
   }
}
// ============================================================================
void lcd_putsf(char* data)	//Outputs string to LCD
{
	char *p;
	p = data;

	// check to make sure we have a good pointer
	if (!p)
		return;

	// print data
	for(; *p ; )
	{
		lcd_putchar(*p);
		p++;
	}
}
//=============================================================
static void Display_off(void)
{
	lcd_command(0x08);
	clcd_Delay();
}
//=============================================================
static void Display_on(void)
{
	lcd_command(0x0C);
	clcd_Delay();
}
//=============================================================
static void cursor_off(void)
{
	lcd_command(0x0C);
	clcd_Delay();
}
//=============================================================
static void cursor_on(void)
{
	lcd_command(0x0E);
	clcd_Delay();
}
//=============================================================
static void cursor_blink(void)
{
	lcd_command(0x0F);
	clcd_Delay();
}
#endif
