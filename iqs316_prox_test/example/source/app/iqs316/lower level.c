//---------------------------------------------------------------------------//
//
//! @file 		lower level.c
//! @brief    Azoteq I2C Firmware library 
//
//! @author 	zozo 
//! @version 	v0.1
//! @note
//!     
//! 
//! 修改日志  :
//! 2013.7.12    创建初试 zozo
//
//
//---------------------------------------------------------------------------//

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
// 头文件
//****************************************************************************/
#include "board.h"
#include "lower level mb9bfxxx.h"
#include "iqs316_define.h"



//---------------------------------------------------------------------------//
//
//! @brief 			CommsIQSxxx_WaitBusy
//! @author 		zozo
//! @note       
//! @param 			ms  延时判断时间 n*ms 
//! @return 		rc TRUE no busy; FALSE busy
//!
//!	@deprecated none
//! @since 2012.8.10
//! @see   
//---------------------------------------------------------------------------//
uint8_t CommsIQSxxx_WaitBusy(uint8_t ms)
{
  uint16_t rc = ms+1;
	I2C_RDY_DIR_INPUT;
	while ((!I2C_RDY_IN)&&(--rc))
	{
		delay_ms(1);//1000 cycles
	}; // 等待 IRDY 为高  或 超时 
  
  if(rc==0)
	{
		return FALSE; 	 
	}  
	else
		return TRUE;

}
/*
	Function name:	Comms_init
	Parameters:		none
	Return:			none
	Description:	Initializes the I2C module on the PIC16LF1827
					Note that the SSPADD acts as a counter to determine the I2C frequency. A smaller value will
					increase the frequency.
*/
void Comms_init(void)
{
	I2C_SCL_DIR_INPUT;
	I2C_SDA_DIR_INPUT;
	I2C_RDY_DIR_INPUT;
	MCLR_DIR_INPUT;
	
  /*reset chip*/ 
  MCLR_DIR_OUTPUT;
  MCLR_LO;
		delay_ms(50);
  MCLR_HI;
		delay_ms(500);
  MCLR_DIR_INPUT;
}

/*
	Function name:	CommsIQSxxx_send
	Parameters:		uint8_t send_data - data to be sent transmitted via the I2C
	Return:			ack 1 ok 0 fail
	Description:	Transmits the data byte given as parameter via the I2C of the PIC16LF1827
					Note that the I2C communication channel must already be active before calling this function,
					as no start bits are included in this function
*/
uint8_t CommsIQSxxx_send(uint8_t send_data)
{
	uint8_t  BitCnt;
	for (BitCnt=0; BitCnt<8; BitCnt++)  //要传送的数据长度为8位
	 {
		  if (send_data&0x80)    //判断发送位(从高位起发送)
		  {
				I2C_SDA_HI; 
		 		while (!I2C_SDA_IN);   
		  }
		  else
		  {				
				I2C_SDA_DIR_OUTPUT;
				I2C_SDA_LO;//SDA = 0;				
		  }
		  send_data = send_data<<1;
		 // _NOP();
		  nop();
		  
		  I2C_SCL_DIR_INPUT;//SCLM = 0;
	 	  while (!I2C_SCL_IN);      //置时钟线为高通知被控器开始接收数据位
	 		delay_us(4);//4 us     //保证时钟高电平周期大于 4 us
		  I2C_SCL_DIR_OUTPUT;//SCLM = 1;
		  I2C_SCL_LO;//SCL = 0;
			delay_us(2);//2 us
	}
		 nop();
		I2C_SDA_DIR_OUTPUT;// SDAM = 0; 
		 nop();
		 
	/*Check ack*/ 
	 uint8_t time1 = 10;
   uint8_t ack;
   I2C_SDA_DIR_INPUT; // sda输入 1128
	 I2C_SCL_DIR_INPUT;//SCLM = 0;             //ack
	 while (!I2C_SCL_IN);
	 nop();
	 while (I2C_SDA_IN)
	 {
	 	 time1--;
	 	 delay_us(10); 
	 	 if (time1==0)
	 	 {
	 		nop();
	 		I2C_SCL_DIR_OUTPUT;//SCLM=1;
	 		I2C_SCL_LO;//SCL=0;
	 		nop();
	   	ack = 0;
			I2C_SDA_DIR_OUTPUT;//SDAM = 1; 
		  I2C_SDA_HI;//SDA = 1;
			return ack;
 		 }
	 }
	 nop();
	 I2C_SCL_DIR_OUTPUT;//SCLM=1;
	 I2C_SCL_LO;//SCL=0;
	 nop();
	 ack=1;
	 I2C_SDA_DIR_OUTPUT;//SDAM = 1; 
	 I2C_SDA_HI;//SDA = 1;
	 return ack;
		 
}

/*
	Function name:	CommsIQSxxx_read_nack
	Parameters:		none
	Return:			uint8_t - the data received via the I2C
	Description:	Enables the Master Receive Mode of the I2C module on the PIC16LF1827. The data received is
					returned and a NACK acknowledge is sent to the IQSxxx to indicate that this was the final read
					of the current continuous read block. 
					A stop or repeated start command has be called next.
*/
uint8_t CommsIQSxxx_read_nack(void)
{
	 uint8_t BitCnt,databuf = 0;     //位
	 
	 I2C_SDA_DIR_INPUT;//SDAM = 0;         //SDA位为输入模式    
            
	 for(BitCnt=0;BitCnt<8;BitCnt++)
	 {   
		 	nop();  
		  I2C_SCL_DIR_INPUT;//SCLM = 0;             //SCL为输入模式
		 	nop();  
		 	while (!I2C_SCL_IN);
		 	databuf=databuf<<1;
		  delay_us(1);//1 us
	  	if(I2C_SDA_IN)
		 	{
		   		databuf=databuf+1;     //读数据位，接收的数据放入cct中
		  }
	  	delay_us(1);//1 us
			I2C_SCL_DIR_OUTPUT;//SCLM = 1; 
		  I2C_SCL_LO;//SCL = 0;       
		 	delay_us(1);//1us
	  }             
	 
	  I2C_SDA_DIR_OUTPUT;//SDAM = 1;         //SDA位为输入模式
	  I2C_SDA_HI;//SDA = 1;
	  while (!I2C_SDA_IN);  
	  nop();
	  nop();
	  I2C_SCL_DIR_INPUT;//SCLM = 0;
	  while (!I2C_SCL_IN);        //ACK
	 	delay_us(2);//2us
	  I2C_SCL_DIR_OUTPUT;//SCLM = 1; 
	  I2C_SCL_LO;//SCL = 0;
	  nop();
	  nop();
	  nop();
	  nop();
	  
	return databuf;
}

/*
	Function name:	CommsIQSxxx_read_ack
	Parameters:		none
	Return:			uint8_t - the data received via the I2C
	Description:	Enables the Master Receive Mode of the I2C module on the PIC16LF1827. The data received is
					returned and a ACK acknowledge is sent to the IQSxxx to indicate that another read command will follow this one.
*/
uint8_t CommsIQSxxx_read_ack(void)
{
	 uint8_t BitCnt,databuf = 0;     //位
	 
	 I2C_SDA_DIR_INPUT;//SDAM = 0;         //SDA位为输入模式    
            
	 for(BitCnt=0;BitCnt<8;BitCnt++)
	 {   
		 	nop();  
		  I2C_SCL_DIR_INPUT;//SCLM = 0;             //SCL为输入模式
		 	nop();  
		 	while (!I2C_SCL_IN);
		 	databuf=databuf<<1;
		  delay_us(1);//1us
	  	if(I2C_SDA_IN)
		 	{
		   		databuf=databuf+1;     //读数据位，接收的数据放入cct中
		  }
	  	delay_us(1);//1us
			I2C_SCL_DIR_OUTPUT;//SCLM = 1; 
		  I2C_SCL_LO;//SCL = 0;       
		 	delay_us(1);//1us
	  }             
	 
	  I2C_SDA_DIR_OUTPUT;//SDAM = 1;         //SDA位为输入模式
	  I2C_SDA_LO;//SDA = 0;
	  while (I2C_SDA_IN);  
	  nop();
	  nop();
	  I2C_SCL_DIR_INPUT;//SCLM = 0;
	  while (!I2C_SCL_IN);        //ACK
	 	delay_us(2);//2us
	  I2C_SCL_DIR_OUTPUT;//SCLM = 1; 
	  I2C_SCL_LO;//SCL = 0;
	  nop();
	  nop();
	  nop();
	  nop();
	  
	return databuf;
}
/*
	Function name:		CommsIQS5xx_start_force
	Parameters:			none
	Return:				none
	Description:		A start condition is created on the I2C.
*/
void CommsIQSxxx_start_force(void)
{
		I2C_SCL_DIR_INPUT;
		while(!I2C_SCL_IN);
		I2C_SDA_HI;		
		I2C_SDA_DIR_OUTPUT;
		while(!I2C_SDA_IN);
		delay_us(4);//4us
		I2C_SDA_LO;		
		delay_us(4);//4us
		I2C_SCL_DIR_OUTPUT;
		I2C_SCL_LO;
}
/*
	Function name:		CommsIQSxxx_start
	Parameters:			none
	Return:				none
	Description:		A start condition is created on the I2C.
*/
void CommsIQSxxx_start(void)
{
	  while(!I2C_RDY_IN); //等待ready 
		I2C_SCL_DIR_INPUT;
		while(!I2C_SCL_IN);
		I2C_SDA_DIR_OUTPUT;
		I2C_SDA_HI;    
		while(!I2C_SDA_IN);
		delay_us(4);//4us
		I2C_SDA_LO;		
		delay_us(4);//4us
		I2C_SCL_DIR_OUTPUT;
		I2C_SCL_LO;
}

/*
	Function name:		CommsIQSxxx_repeat_start
	Parameters:			none
	Return:				none
	Description:		A repeated start condition is created on the I2C.
*/
void CommsIQSxxx_repeat_start(void)
{
	  while(!I2C_RDY_IN); //等待ready 
		I2C_SCL_DIR_INPUT;
		while(!I2C_SCL_IN);
		I2C_SDA_DIR_OUTPUT;
		I2C_SDA_HI;
		while(!I2C_SDA_IN);
		delay_us(4);//4us
		I2C_SDA_LO;		
		delay_us(4);//4us
		I2C_SCL_DIR_OUTPUT;
		I2C_SCL_LO;
}

/*
	Function name:		CommsIQSxxx_stop
	Parameters:			none
	Return:				none
	Description:		A stop condition is created on the I2C.
*/
void CommsIQSxxx_stop(void)
{
		I2C_SDA_DIR_OUTPUT;
		I2C_SDA_LO;
		delay_us(1);//1us
 		I2C_SCL_DIR_INPUT;
 		while (!I2C_SCL_IN);
		delay_us(1);//1us  
		I2C_SDA_HI;  //发送结束条件的时钟信号
		while (!I2C_SDA_IN);
 		nop();   
 		delay_ms(1);
}
//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif