//---------------------------------------------------------------------------//
//
//! @file 		upper level IQS316.c
//! @brief    IQS316 specific functions for I2C Firmware library
//
//! @author 	zozo 
//! @version 	v0.1
//! @note
//!     
//! 
//! 修改日志  :
//! 2012.12.10    创建初试 zozo
//
//
//---------------------------------------------------------------------------//
#ifndef __UPPER_LEVEL_IQS316_H__ 
#define __UPPER LEVEL IQS316_H__


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
// Global Variables  
//****************************************************************************/

/*----------------------------------------------------
//标志*/
typedef struct { 
	//IQS316_flag_u IQS316_flag;    //flag to indicate whether IQS316 is in prox mode or touch mode 
  uint8_t prox_detected;
  uint8_t touch_detected;
	uint8_t prox4_11; //prox status of channels 4-11  
	uint8_t prox12_19;          //prox status of channels 12-19  
	uint8_t touch4_11;           //touch status of channels 4-11 
	uint8_t touch12_19;         //touch status of channels 12-19 
	uint8_t min_key;
} IQS316_t; 

typedef struct { 
  uint16_t Lta;
  uint16_t CurtSample;
  int16_t Delta;  
} IQS316_chan_info_t; 


//****************************************************************************
// 函数申明 
//****************************************************************************/
void Init_iqs316(void);
void IQS316_New_Conversion(void);
void CommsIQSxxx_Write(uint8_t write_addr, uint8_t data);
void CommsIQSxxx_Write_Restart(uint8_t write_addr, uint8_t data);
uint8_t CommsIQSxxx_Read(uint8_t read_addr);
uint8_t CommsIQSxxx_Cont_Read(uint8_t start_addr);
uint8_t CommsIQSxxx_Read_Next_Cont(void);
uint8_t CommsIQSxxx_Read_Next_Stop(void);
void CommsIQSxxx_Init_Conversion(void);
void Comms_Error(void);
void CommsIQSxxx_Read_continuous(uint8_t read_addr ,uint8_t *data,uint8_t NoOfBytes);
//****************************************************************************
// 变量申明 
//****************************************************************************/
extern IQS316_t IQS316;

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif
#endif // __UPPER LEVEL IQS316_H__