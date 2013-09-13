//---------------------------------------------------------------------------//
//
//! @file 		iqs316_define.h
//! @brief      IQS316 user defined. 
//
//! @author 	zozo 
//! @version 	v0.1
//! @note
//!     
//! CPU内核   : MSP430 
//! 使用芯片  : MSP430 全系列
//! 指令集 		: RSIC
//! 开发环境	: IAR Embedded Workbench Evaluation for MSP430
//! 版权所有(C)2012-2017
//! 
//! 修改日志  :
//! 2012.12.25    创建初试 zozo
//
//
//---------------------------------------------------------------------------//
#ifndef __IQS316_DEFINE_H__ 
#define __IQS316_DEFINE_H__

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif


//****************************************************************************
// 宏定义 
//****************************************************************************/
//----------------------------------------------------
//器件地址 */
#define IQS316_ADDR 						0x74	
  
#define IQS316_DEBUG

#define IQS316_KEY_NUMBER       16+4

//#define IQS316_RAM_PROG					//Enable minimum mode

/*
	Prox check	increased threshold
*/
#define PM_INC_THRESHOLD					 0	
/*
	Prox check coutinuous count times
*/
#define PM_CHK_CNT								 5
/*
	touch check 	if no touch wait memont to back prox mode  suggest 4 second
*/
#define TOUCH_CHK_CNT 						 40
/*
	Prox check	 
*/
#define PM_CHK_MAX_THRESHOLD			 10*2

//----------------------------------------------------
//设置寄存器默认值 */


/*																																				
	Touch Threshold 		
*/
/*
	0 = Lower range threshold set (value:1/32 1/16  2/16  3/16)
	1 = Higher range threshold set (default:4/16 6/16  8/16  10/16)
		*/
#define TOUCH_RANGE								 1

#define TOUCH_THRESHOLD_CH4 			 (2<<6)
#define TOUCH_THRESHOLD_CH5 			 (2<<6)
#define TOUCH_THRESHOLD_CH6 			 (1<<6)
#define TOUCH_THRESHOLD_CH7 			 (1<<6)
#define TOUCH_THRESHOLD_CH8 			 (2<<6)
#define TOUCH_THRESHOLD_CH9 			 (2<<6)
#define TOUCH_THRESHOLD_CH10			 (2<<6)
#define TOUCH_THRESHOLD_CH11			 (3<<6)
#define TOUCH_THRESHOLD_CH12			 (2<<6)
#define TOUCH_THRESHOLD_CH13			 (2<<6)
#define TOUCH_THRESHOLD_CH14			 (2<<6)
#define TOUCH_THRESHOLD_CH15			 (2<<6)
#define TOUCH_THRESHOLD_CH16			 (2<<6)
#define TOUCH_THRESHOLD_CH17			 (2<<6)
#define TOUCH_THRESHOLD_CH18			 (2<<6)
#define TOUCH_THRESHOLD_CH19			 (2<<6)


/*
	Prox Threshold	 
*/

/*
		0 = Lower range threshold set (value:2 3 4 6)
		1 = Higher range threshold set (value:8 16 20 30)
		*/
#define PROX_RANGE 							 	0

#define PROX_THRESHOLD_CH0				(2<<4)
#define PROX_THRESHOLD_CH1				(2<<4)
#define PROX_THRESHOLD_CH2				(2<<4)
#define PROX_THRESHOLD_CH3				(2<<4)


/*
#define PROX_THRESHOLD_CH0				(2<<4)
#define PROX_THRESHOLD_CH1				(2<<4)
#define PROX_THRESHOLD_CH2				(2<<4)
#define PROX_THRESHOLD_CH3				(2<<4)
*/

/*
UI Settings 0 (UI_SETTINGS0) C4H
Bit 7: RESEED
Bit 6: ATI_MODE
		0 : Automated ATI will apply to Prox-Mode channels 
		1 : Automated ATI will apply to Touch-Mode channels 
Bit 5: PROX RANGE
		0 = Lower range threshold set (ranger:2-6)
		1 = Higher range threshold set (default:8-30)

Bit 4: TOUCH  RANGE
		0 = Lower range threshold set (default:1/32)
		1 = Higher range threshold set (default:4/16)
Bit 3: FORCE PROX MODE
Bit 2: FORCE  TOUCH  MODE
Bit 1: ND
Bit 0: Internal
*/
#define UI_SETTINGS0_DEF        ((PROX_RANGE<<5)|(TOUCH_RANGE<<4)|BIT1 ) 


/*
Bit 7: CXVSS
Bit 6: ZC_EN
Bit 5-4:  HALT
Bit 3: AUTO_ATI
Bit 2-0:  CXDIV[2:0]
*/
#define PROX_SETTINGS_1_DEF     (BIT7|BIT4|BIT1)

/*
ProxSense Module Settings 2 (PROX_SETTINGS_2) C7H
Bit 7: Unused 
Bit 6: SHIELD_EN:  
Bit 5: STOP_COMMS
Bit 4: ACK_RESET
Bit 3: SKIP_CONV
Bit 2: ACF_DISABLE
Bit 1: LTN_DISABLE
Bit 0: WDT_DISABLE:
*/
#define PROX_SETTINGS_2_DEF     (BIT4|BIT0)


/*
	AUTO ATI LEVELS  Prox
*/
#define PROX_ATI_TARGET              2000
#define PROX_ATI_TARGET_HI           ((PROX_ATI_TARGET>>8)&0x00ff)
#define PROX_ATI_TARGET_LO           (PROX_ATI_TARGET&0x00ff)
/*
	AUTO ATI LEVELS  touch
*/
#define TOUCH_ATI_TARGET 						 1000
#define TOUCH_ATI_TARGET_HI          ((TOUCH_ATI_TARGET>>8)&0x00ff)
#define TOUCH_ATI_TARGET_LO          (TOUCH_ATI_TARGET&0x00ff)

/*
	Prox Mode Group Selection
*/
#define PM_CX_SELECT_DEF						(BIT1|BIT0)
/*
	Individual Channel Disable (CHAN_ACTIVE0) 
*/
#define CHAN_ACTIVE0_DEF						0x03 //enable CH0-CH1
/*
	Individual Channel Disable (CHAN_ACTIVE1) 
*/
#define CHAN_ACTIVE1_DEF						0x0d // ch4, ch6, ch7 enabled
/*
	Individual Channel Disable (CHAN_ACTIVE2) 
*/
#define CHAN_ACTIVE2_DEF						0x0d //ch8, ch10, ch11 enabled
/*
	Individual Channel Disable (CHAN_ACTIVE3) 
*/
#define CHAN_ACTIVE3_DEF						0x00 //ch12, ch13, ch15 enabled
/*
	Individual Channel Disable (CHAN_ACTIVE4) 
*/
#define CHAN_ACTIVE4_DEF						0x00 //ch16, ch17, ch19 enabled


/*
	ATI Multiplier C (ATI_MULT1) 
*/
#define ATI_MULT1_CH0                		3//   0
#define ATI_MULT1_CH1                		3//  1
#define ATI_MULT1_CH2                		0
#define ATI_MULT1_CH3                		0
#define ATI_MULT1_CH4                		0
#define ATI_MULT1_CH5                		0
#define ATI_MULT1_CH6                		3
#define ATI_MULT1_CH7                		3
#define ATI_MULT1_CH8                		1
#define ATI_MULT1_CH9                		0
#define ATI_MULT1_CH10               		0
#define ATI_MULT1_CH11               		0
#define ATI_MULT1_CH12               		0
#define ATI_MULT1_CH13               		0
#define ATI_MULT1_CH14               		0
#define ATI_MULT1_CH15               		0
#define ATI_MULT1_CH16               		0
#define ATI_MULT1_CH17               		0
#define ATI_MULT1_CH18               		0
#define ATI_MULT1_CH19               		0



#define ATI_MULT1_GRP0						 ((ATI_MULT1_CH3<<6)|(ATI_MULT1_CH2<<4)|(ATI_MULT1_CH1<<2)|(ATI_MULT1_CH0<<0)) 
#define ATI_MULT1_GRP1						 ((ATI_MULT1_CH7<<6)|(ATI_MULT1_CH6<<4)|(ATI_MULT1_CH5<<2)|(ATI_MULT1_CH4<<0)) 
#define ATI_MULT1_GRP2						 ((ATI_MULT1_CH11<<6)|(ATI_MULT1_CH10<<4)|(ATI_MULT1_CH9<<2)|(ATI_MULT1_CH8<<0)) 
#define ATI_MULT1_GRP3						 ((ATI_MULT1_CH15<<6)|(ATI_MULT1_CH14<<4)|(ATI_MULT1_CH13<<2)|(ATI_MULT1_CH12<<0))
#define ATI_MULT1_GRP4						 ((ATI_MULT1_CH19<<6)|(ATI_MULT1_CH18<<4)|(ATI_MULT1_CH17<<2)|(ATI_MULT1_CH16<<0))







//----------------------------------------------------
//hardware define */
//sda P37
#define  I2C_SDA_BIT_CNT					  7
#define  I2C_SDA_BIT								(1<<I2C_SDA_BIT_CNT)
#define  I2C_SDA_MASK								I2C_SDA_BIT
#define  I2C_SDA_PFR 								FM3_GPIO->PFR3
#define  I2C_SDA_REG_DIR						FM3_GPIO->DDR3
#define  I2C_SDA_REG_RES            FM3_GPIO->PCR3
#define  I2C_SDA_REG_OUT            FM3_GPIO->PDOR3
#define  I2C_SDA_REG_IN             FM3_GPIO->PDIR3



//scl P38 
#define  I2C_SCL_BIT_CNT						8
#define  I2C_SCL_BIT								(1<<I2C_SCL_BIT_CNT)
#define  I2C_SCL_MASK 							I2C_SCL_BIT
#define  I2C_SCL_PFR								FM3_GPIO->PFR3
#define  I2C_SCL_REG_DIR						FM3_GPIO->DDR3
#define  I2C_SCL_REG_RES						FM3_GPIO->PCR3


#define  I2C_SCL_REG_OUT						FM3_GPIO->PDOR3
#define  I2C_SCL_REG_IN 						FM3_GPIO->PDIR3


//IRDY P39
#define  I2C_RDY_BIT_CNT						9
#define  I2C_RDY_BIT								(1<<I2C_RDY_BIT_CNT)
#define  I2C_RDY_MASK 							I2C_RDY_BIT
#define  I2C_RDY_PFR								FM3_GPIO->PFR3
#define  I2C_RDY_REG_DIR						FM3_GPIO->DDR3
#define  I2C_RDY_REG_RES						FM3_GPIO->PCR3
#define  I2C_RDY_REG_OUT						FM3_GPIO->PDOR3
#define  I2C_RDY_REG_IN 						FM3_GPIO->PDIR3

//MCLR P3B
#define  MCLR_BIT_CNT								0x0B
#define  MCLR_BIT										(1<<MCLR_BIT_CNT)
#define  MCLR_MASK 									MCLR_BIT
#define  MCLR_PFR										FM3_GPIO->PFR3
#define  MCLR_REG_DIR								FM3_GPIO->DDR3
#define  MCLR_REG_RES								FM3_GPIO->PCR3
#define  MCLR_REG_OUT								FM3_GPIO->PDOR3
#define  MCLR_REG_IN 								FM3_GPIO->PDIR3



//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif
#endif // __IQS316_DEFINE_H__


