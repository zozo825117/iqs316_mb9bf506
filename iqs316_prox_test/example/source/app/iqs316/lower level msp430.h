//---------------------------------------------------------------------------//
//
//! @file 		lower level msp430.h
//! @brief    specific functions for IQS316 I2C Firmware library
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
//! 2012.12.10    创建初试 zozo
//
//
//---------------------------------------------------------------------------//
#ifndef __LOWER_LEVEL_MSP430_H__ 
#define __LOWER_LEVEL_MSP430_H__


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
//iic define */
//sda & scl & IRDY & MCLR
#define I2C_SDA_DIR_INPUT       HWREG_BIT_CLR(I2C_SDA_REG_DIR,I2C_SDA_BIT)
#define I2C_SCL_DIR_INPUT       HWREG_BIT_CLR(I2C_SCL_REG_DIR,I2C_SCL_BIT)

#define I2C_SDA_DIR_OUTPUT       HWREG_BIT_SET(I2C_SDA_REG_DIR,I2C_SDA_BIT)
#define I2C_SCL_DIR_OUTPUT       HWREG_BIT_SET(I2C_SCL_REG_DIR,I2C_SCL_BIT)

#define I2C_SDA_IN              HWREG_BIT_AND(I2C_SDA_REG_IN ,I2C_SDA_BIT)
#define I2C_SCL_IN              HWREG_BIT_AND(I2C_SCL_REG_IN ,I2C_SCL_BIT)


#define I2C_SDA_LO              HWREG_BIT_CLR(I2C_SDA_REG_OUT,I2C_SDA_BIT)
#define I2C_SCL_LO              HWREG_BIT_CLR(I2C_SCL_REG_OUT,I2C_SCL_BIT)
#define I2C_SDA_HI              HWREG_BIT_SET(I2C_SDA_REG_OUT,I2C_SDA_BIT)
#define I2C_SCL_HI              HWREG_BIT_SET(I2C_SCL_REG_OUT,I2C_SCL_BIT)

/*
#define I2C_SDA_OUT_LOW         HWREG_BIT_CLR(I2C_SDA_REG_SEL,I2C_SDA_BIT);\
																HWREG_BIT_CLR(I2C_SDA_REG_OUT,I2C_SDA_BIT)
 																
#define I2C_SCL_OUT_LOW         HWREG_BIT_CLR(I2C_SCL_REG_SEL,I2C_SCL_BIT);\
																HWREG_BIT_CLR(I2C_SCL_REG_OUT,I2C_SCL_BIT)*/

//---------- */
#define I2C_RDY_DIR_INPUT       HWREG_BIT_CLR(I2C_RDY_REG_DIR,I2C_RDY_BIT)  
#define I2C_RDY_DIR_OUTPUT       HWREG_BIT_SET(I2C_RDY_REG_DIR,I2C_RDY_BIT)

#define I2C_RDY_IN              HWREG_BIT_AND(I2C_RDY_REG_IN ,I2C_RDY_BIT)

#define I2C_RDY_HI              HWREG_BIT_SET(I2C_RDY_REG_OUT,I2C_RDY_BIT)
#define I2C_RDY_LO              HWREG_BIT_CLR(I2C_RDY_REG_OUT,I2C_RDY_BIT)

//---------- */
#define  MCLR_DIR_INPUT         HWREG_BIT_CLR(MCLR_REG_DIR,MCLR_BIT)
#define  MCLR_DIR_OUTPUT        HWREG_BIT_SET(MCLR_REG_DIR,MCLR_BIT)
#define  MCLR_LO                HWREG_BIT_CLR(MCLR_REG_OUT,MCLR_BIT)
#define  MCLR_HI                HWREG_BIT_SET(MCLR_REG_OUT,MCLR_BIT)  


//****************************************************************************
// 函数申明 
//****************************************************************************/
void Comms_init(void);
uint8_t CommsIQS316_WaitBusy(uint8_t ms);
uint8_t CommsIQS316_send(uint8_t send_data);
uint8_t CommsIQS316_read_nack(void);
uint8_t CommsIQS316_read_ack(void);
void CommsIQS316_start(void);
void CommsIQS316_repeat_start(void);
void CommsIQS316_stop(void);



//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif
#endif // __LOWER_LEVEL_MSP430_H__
