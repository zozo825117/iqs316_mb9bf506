//---------------------------------------------------------------------------//
//
//! @file 		lower level msp430.h
//! @brief    IQS5xx I2C Firmware library 
//
//! @author 	zozo 
//! @version 	v0.1
//! @note
//!     
//! CPU�ں�   : MSP430 
//! ʹ��оƬ  : MSP430 ȫϵ��
//! ָ� 		: RSIC
//! ��������	: IAR Embedded Workbench Evaluation for MSP430
//! ��Ȩ����(C)2012-2017
//! 
//! �޸���־  :
//! 2012.12.10    �������� zozo
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
// �궨�� 
//****************************************************************************/
//----------------------------------------------------
//iic define */
/*sda & scl define */
#define I2C_SDA_DIR_INPUT       I2C_SDA_REG_DIR &=~ I2C_SDA_BIT;\
																I2C_SDA_REG_RES |= I2C_SDA_BIT
	
#define I2C_SCL_DIR_INPUT       I2C_SCL_REG_DIR &=~ I2C_SCL_BIT;\
																I2C_SCL_REG_RES |= I2C_SCL_BIT

#define I2C_SDA_DIR_OUTPUT      (I2C_SDA_REG_DIR |= I2C_SDA_BIT)
#define I2C_SCL_DIR_OUTPUT      (I2C_SCL_REG_DIR |= I2C_SCL_BIT)

#define I2C_SDA_IN              ((I2C_SDA_REG_IN&I2C_SDA_BIT)>>I2C_SDA_BIT_CNT)
#define I2C_SCL_IN              ((I2C_SCL_REG_IN&I2C_SCL_BIT)>>I2C_SCL_BIT_CNT)


#define I2C_SDA_LO              (I2C_SDA_REG_OUT &=~ I2C_SDA_BIT)
#define I2C_SCL_LO              (I2C_SCL_REG_OUT &=~ I2C_SCL_BIT)
#define I2C_SDA_HI              (I2C_SDA_REG_OUT |= I2C_SDA_BIT)
#define I2C_SCL_HI              (I2C_SCL_REG_OUT |= I2C_SCL_BIT)

/*IRDY define */
#define I2C_RDY_DIR_INPUT       (I2C_RDY_REG_DIR &=~ I2C_RDY_BIT)  
#define I2C_RDY_DIR_OUTPUT      (I2C_RDY_REG_DIR |= I2C_RDY_BIT)

#define I2C_RDY_IN              ((I2C_RDY_REG_IN&I2C_RDY_BIT)>>I2C_RDY_BIT_CNT)

#define I2C_RDY_HI              (I2C_RDY_REG_OUT |= I2C_RDY_BIT)
#define I2C_RDY_LO              (I2C_RDY_REG_OUT &=~ I2C_RDY_BIT)

/*MCLR define */

#define  MCLR_DIR_INPUT 				(MCLR_REG_DIR &=~ MCLR_BIT)
#define  MCLR_DIR_OUTPUT				(MCLR_REG_DIR |= MCLR_BIT)
#define  MCLR_LO								(MCLR_REG_OUT &=~ I2C_RDY_BIT)
#define  MCLR_HI								(MCLR_REG_OUT |= I2C_RDY_BIT)


/*delay*/
void DELAY_US(uint32_t cycles);
void DELAY_MS(uint32_t cycles);

#define delay_us(x)							DELAY_US(MCLK*x);
#define delay_ms(x) 						DELAY_MS(x);
#define nop()                  { __ASM volatile ("nop"); }




//****************************************************************************
// �������� 
//****************************************************************************/
void Comms_init(void);
uint8_t CommsIQSxxx_WaitBusy(uint8_t ms);
uint8_t CommsIQSxxx_send(uint8_t send_data);
uint8_t CommsIQSxxx_Read(uint8_t read_addr);
uint8_t CommsIQSxxx_read_nack(void);
uint8_t CommsIQSxxx_read_ack(void);
void CommsIQSxxx_start(void);
void CommsIQSxxx_start_force(void);
void CommsIQSxxx_repeat_start(void);
void CommsIQSxxx_stop(void);



//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __LOWER_LEVEL_MSP430_H__
