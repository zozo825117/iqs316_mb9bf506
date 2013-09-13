/*******************************************************************************
* File Name          : main.c
* Version            : V1.0.0
* Date               : 11/13/2010
* Description        : Define main module.
											 This example project shows how to use the
											 FSSDC-9B506-EK board. It shows basic use of joy-stick,
											 ADC and PWM.
*******************************************************************************/
#include "mb9bf506r.h"
#include "system_mb9bf50x.h"
#include "board.h"
#include "upper level IQS316.h"
#include "lower level mb9bfxxx.h"

#define TIMER0_TICK_PER_SEC   40

unsigned long CriticalSecCntr;
char disp[8] = {'\0'};
unsigned short AD_Data;
unsigned char LcdFlag;
unsigned short FlagCounter,Vaule;
unsigned char led_index = 0;
/*************************************************************************
 * Function Name: NVIC_IntEnable
 * Parameters: IntNumber - Interrup number
 * Return: void
 *
 * Description: Enable interrup at NVIC
 *
 *
 *************************************************************************/
void NVIC_IntEnable(unsigned long IntNumber)
{
volatile unsigned long * pNVIC_SetEn = (unsigned long *)0xE000E100;

 // assert((CSV_IRQn <= IntNumber) && (DMAC7_IRQn >= IntNumber));
  IntNumber -= CSV_IRQn;

  pNVIC_SetEn += IntNumber/32;
  *pNVIC_SetEn = (1UL<<(IntNumber%32));

}

/*************************************************************************
 * Function Name: NVIC_IntDisable
 * Parameters: IntNumber - Interrup number
 * Return: void
 *
 * Description: Disables interrup at NVIC
 *
 *
 *************************************************************************/
void NVIC_IntDisable(unsigned long IntNumber)
{
volatile unsigned long * pNVIC_ClrEn = (unsigned long *)0xE000E180;

//  assert((CSV_IRQn <= IntNumber) && (DMAC7_IRQn >= IntNumber));
  IntNumber -= CSV_IRQn;

  pNVIC_ClrEn += IntNumber/32;
  *pNVIC_ClrEn = (1UL<<(IntNumber%32));

}

/*************************************************************************
 * Function Name: NVIC_ClrPend
 * Parameters: IntNumber - Interrup number
 * Return: void
 *
 * Description:Clear pending interrupt at NVIC
 *
 *
 *************************************************************************/
void NVIC_ClrPend(unsigned long IntNumber)
{
volatile unsigned long * pNVIC_ClrPend = (unsigned long *)0xE000E280;

 // assert((CSV_IRQn <= IntNumber) && (DMAC7_IRQn >= IntNumber));
  IntNumber -= CSV_IRQn;

  pNVIC_ClrPend += IntNumber/32;
  *pNVIC_ClrPend = (1UL<<(IntNumber%32));

}

/*************************************************************************
 * Function Name: NVIC_ClrPend
 * Parameters: IntNumber - Interrup number, Interrupt Priority
 * Return: void
 *
 * Description:Sets Interrupt priority
 *
 *
 *************************************************************************/
void NVIC_IntPri(unsigned long IntNumber, unsigned char Priority)
{
volatile unsigned char * pNVIC_IntPri = (unsigned char *)0xE000E400;

//  assert((CSV_IRQn <= IntNumber) && (DMAC7_IRQn >= IntNumber));
  IntNumber -= CSV_IRQn;
  pNVIC_IntPri += IntNumber;
  *pNVIC_IntPri = Priority;
}



int Tmr1Tick;

/*************************************************************************
 * Function Name: DT_QDU_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 1 interrupt handler
 *
 *************************************************************************/
void DT_QDU_IRQHandler(void)
{
    Tmr1Tick = 1;
    Vaule++;
    if(Vaule == 1000)
    {
        Vaule = 0;
        //USER_LED_PDOR |= USER_LED_MASK;
        //USER_LED_PDOR &= ~(0x1<<(led_index + 9));
        //led_index++;
        //if(led_index == 3) led_index = 0;
        USER_LED_PDOR ^= USER_LED3;
    }
    FM3_DTIM->TIMER1INTCLR = 1;
}

/*************************************************************************
 * Function Name: Init_Joy_IO
 * Parameters: void
 * Return: void
 *
 * Description: Init Button pins as inputs.
 *
 *************************************************************************/
void Init_Joy_IO(void)
{
    /*Select CPIO function*/
    JOY_PFR &= ~JOY_MASK;
    /*Set CPIO Pull-Up function*/
    JOY_PCR |= JOY_MASK;
    /*Make button pins inputs*/
    JOY_DDR &= ~JOY_MASK;
}


/*************************************************************************
 * Function Name: Init_LED
 * Parameters: void
 * Return: void
 *
 * Description: Init Button pins as inputs.
 *
 *************************************************************************/
void Init_LED(void)
{
    /*Select CPIO function*/
    USER_LED_PFR &= ~USER_LED_MASK;
    /* disable analog input */
    FM3_GPIO->ADE &= ~USER_LED_MASK;
    /*Set CPIO Pull-Up function*/
    USER_LED_PCR |= USER_LED_MASK;
    /*Make led pins outputs*/
    USER_LED_DDR |= USER_LED_MASK;
    USER_LED_PDOR |= USER_LED_MASK;
}


/*************************************************************************
 * Function Name: Init_LCD_IO
 * Parameters: void
 * Return: void
 *
 * Description: Init LCD Pins
 *		
 *************************************************************************/
void Init_LCD_IO()
{
	/* Release the analog input function*/
	FM3_GPIO->ADE =0x03;
	/*Select CPIO function*/
  LCD_CS_PFR &= ~LCD_CS;
  /*Make pin output*/
  LCD_CS_DDR |= LCD_CS;
  /*Select CPIO function*/
  LCD_CD_PFR &= ~LCD_CD;
  /*Make pin output*/
  LCD_CD_DDR |= LCD_CD;
  /*Select CPIO function*/
  LCD_CLK_PFR &= ~LCD_CLK;
  /*Make pin output*/
  LCD_CLK_DDR |= LCD_CLK;
  /*Select CPIO function*/
  LCD_DATA_PFR &= ~LCD_DATA;
  /*Make pin output*/
  LCD_DATA_DDR |= LCD_DATA;
}


/*************************************************************************
 * Function Name: DelayResolution100us
 * Parameters: Int32U Dly
 * Return: void
 *
 * Description: Delay Dly * [100us]
 *		
 *************************************************************************/
void DelayResolution100us(unsigned long Dly)
{
volatile unsigned long  loop_100us;
  for(;Dly;Dly--)
  {
    for(loop_100us = 550; loop_100us; loop_100us--)
    {
    }
  }
}
//---------------------------------------------------------------------------//
//
//! @brief			ÑÓÊ±DelayUs
//! @author 		zozo
//! @note
//! @param			cycles
//! @return 		void
//£¡@deprecated  # of CPU cycles delayed is similar to "cycles". Specifically,\n 
//! 						it's ((cycles-15) % 6) + 15.	Not exact, but gives a sense of \n
//! 						the real-time delay.	Also, if MCLK ~1MHz, "cycles" is similar\n 
//! 						to # of useconds delayed.
//! @since			2010.07.01
//! @see	 
//---------------------------------------------------------------------------//
void DELAY_US (uint32_t cycles){ 
	while(cycles>15)													// 15 cycles consumed by overhead
		cycles = cycles - 4;										// 6 cycles consumed each iteration
}
//---------------------------------------------------------------------------//
//
//! @brief			ÑÓÊ±º¯Êý 
//! @author 		zozo
//! @note
//! @param			uint16_t cycles
//! @return 		void
//!
//! @deprecated none
//! @since 2010.10.10
//! @see	 
//---------------------------------------------------------------------------//
void DELAY_MS (uint32_t cycles){ 
	uint32_t cnt;
	while(cycles)
	{
		cycles = cycles - 1;
		cnt = MCLK;
		while(cnt)
		{
			DELAY_US(1000);
			cnt--;
		}
	}
}

/*
Process the conversion data
First check if a channel as been pressed, then check if theres a proximity
*/

void Process_Touch_Data(void){
		uint8_t button_count = 0;
		uint8_t button_pressed = 0x00;
		uint8_t i,bit_buf = 0x01;
		static uint8_t last_min_key;

		for(i = 1;i<=8;i++)
		{
				if (IQS316.touch4_11 & bit_buf )
				{
					button_pressed = i;
					button_count++; 
				}
				bit_buf<<=1;
		}
		bit_buf = 0x01;
		for(i = 9;i <=16;i++)
		{
				if (IQS316.touch12_19 & bit_buf )
				{
					button_pressed = i;
					button_count++; 
				}
				bit_buf<<=1;
		}
	 
		
		if (IQS316.touch_detected == 1)
		{ 								

			USER_LED_PDOR &=~ USER_LED2;  // on
		} 	 
		else
		{
			USER_LED_PDOR |= USER_LED2;	// off

		} 
		
		static uint8_t last_button;
		if(last_button != button_pressed) 	
		{ 	

		}

		last_button = button_pressed;


		
		if(last_min_key != IQS316.min_key)	 
		{ 	

		}

		last_min_key = IQS316.min_key;
				
		if (IQS316.prox_detected)
		{ 	 
			USER_LED_PDOR &=~ USER_LED1;	// on
		} 	 
		else
		{
			USER_LED_PDOR |= USER_LED1; // off
		} 			
		
} 	 

/*************************************************************************
 * Function Name: DelayResolution100us
 * Parameters: Int32U Dly
 * Return: void
 *
 * Description: Delay Dly * [100us]
 *		
 *************************************************************************/



int main(void)
{

  //SystemInit();

  /* Delay */
  DelayResolution100us(10);
  Init_Joy_IO();
  Init_LED();
	//DelayResolution100us(30000);
 
	/*Set Timer 1 Load Value*/
	FM3_DTIM->TIMER1LOAD= 80000000 /* Base Clock */ 
	            /2             /* APB Devider */
	            /256           /* Timer devider */
	            /1000;         /* 1000 times per second */
	/*Set Timer 1 Mode*/
	FM3_DTIM->TIMER1CONTROL = (0<<0) |     /*wrapping mode*/
	              (1<<1) |     /*1 = 32-bit counter*/
	              (2<<2) |     /*8 stages of prescale, clock is divided by 256*/
	              (1<<5) |     /*Interrupt Enable*/
	              (1<<6) |     /*Timer is in periodic mode*/
	              (1<<7) ;     /*Timer enabled*/  
	/*Enable Dual Timer Interrup*/
	NVIC_IntEnable(DTIM_QDU_IRQn);

	/*initial touch */
	Init_iqs316();

    /*Main Loop*/
   while(1)
  {

		if(CommsIQSxxx_WaitBusy(5))
		{
		 IQS316_New_Conversion(); 					 //do new conversion and check data
		}
		 
		 Process_Touch_Data();		
		 delay_ms(20);

  }

}
