/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support 
 * ----------------------------------------------------------------------------
 * Copyright (c) 2013, Naminic.com
 * info@naminic.com
 */

#include <board.h>
#include <pio/pio.h>
#include <dbgu/dbgu.h>
#include <aic/aic.h>
#include <utility/trace.h>
#include <adc/adc.h>
#include <pmc/pmc.h>
#include <ssc/ssc.h>
#include <utility/lcd4bit.h>
#include <stdio.h>


#define BOARD_ADC_FREQ 6000000
#define ADC_VREF       3300  // 3.3v * 1000

/// Pio pins to configure.
#ifdef PINS_ADC	//PINS_ADC is array of ADC pins as PIN structure (defined in board.h)
static const Pin pinsADC[] = {PINS_ADC};
#endif


/// Indicates that the conversion is finished.
static volatile unsigned char conversionDone;



//-----------------------------------------------------------------------------
/// Convert a digital value in millivolt
/// /param valueToconvert Value to convert in milivolt
//-----------------------------------------------------------------------------
static float ConvHex2mV( unsigned int valueToConvert )
{
    return ( (ADC_VREF * valueToConvert)/0x3FF);
}


//---------------------------------------------------------------------------
/// blocks execution for the b in milliseconds
/// /param b time in milliseconds
void delay_ms(unsigned long b)
{
	unsigned long y = 0;	
	unsigned long e = 0;
	do
	{
		for (e=0; e<3686; e++);
			y++;
	} while (y < b);
	y=0;
}

//------------------------------------------------------------------------------
/// Interrupt handler for the ADC. Signals that the conversion is finished by
/// setting a flag variable.
//------------------------------------------------------------------------------
static void ISR_Adc(void)
{
    unsigned int status;

    status = ADC_GetStatus(AT91C_BASE_ADC);

	if (ADC_IsChannelInterruptStatusSet(status, ADC_CHANNEL_7))
	{
		ADC_DisableIt(AT91C_BASE_ADC, ADC_CHANNEL_7);
		conversionDone |= 1<<ADC_CHANNEL_7;
	}    
}


static float readTemp()
{
	float temp=0;
	conversionDone = 0;	//global variable, will set whenever ADC is done
	ADC_EnableIt(AT91C_BASE_ADC, ADC_CHANNEL_7);
	// Start measurement
	ADC_StartConversion(AT91C_BASE_ADC);
	//wait for ADC to finish
	while ( conversionDone != ((1<<ADC_CHANNEL_7)));
		
	//read ADC data
	temp = ConvHex2mV(ADC_GetConvertedData(AT91C_BASE_ADC, ADC_CHANNEL_7));
	temp = temp /10; //10mv -> 1 celsius
	
	return temp;
}


int main(void)
{
	unsigned int id_channel;	

	TRACE_CONFIGURE(DBGU_STANDARD, 9600, BOARD_MCK);	

	//LCD configuration
	lcd_init();
	lcd_clear();
	lcd_gotoxy(1,1);
	lcd_putsf("WWW.NAMINIC.COM");
	lcd_gotoxy(2,1);
	lcd_putsf("Temp: ");

	#ifdef PINS_ADC //PINS_ADC is array of ADC pins as PIN structure (defined in board.h)
	PIO_Configure(pinsADC, PIO_LISTSIZE(pinsADC));
	#endif

	//initialize ADC
	ADC_Initialize( AT91C_BASE_ADC,
					AT91C_ID_ADC,
					AT91C_ADC_TRGEN_DIS,
					0,
					AT91C_ADC_SLEEP_NORMAL_MODE,
					AT91C_ADC_LOWRES_10_BIT,
					BOARD_MCK,
					BOARD_ADC_FREQ,
					10,
					1200);

	//using ADC channel4
	ADC_EnableChannel(AT91C_BASE_ADC, ADC_CHANNEL_7);

	//enable interrupt for ADC
	AIC_ConfigureIT(AT91C_ID_ADC, 0, ISR_Adc);
	AIC_EnableIT(AT91C_ID_ADC);

	char tempStr[10];
	float temp1=0,temp2=0;

	// Infinite loop
	while (1)
	{
		temp1 = readTemp();
		delay_ms(500);			
		temp2 = readTemp();
		
		//average temp.
		temp1 = (temp1+temp2)/2;

		//write temperature in serial port
		printf("Temperature: %4.2f C\n", temp1);
		lcd_gotoxy(2,6);	//set cursor position on LCD
		sprintf(tempStr,"%4.2f ", temp1);	//write temp. in buffer for writing to LCD
		lcd_putsf(&tempStr);	//write temp in LCD

		//just delay
		delay_ms(1000);			
	}
}
