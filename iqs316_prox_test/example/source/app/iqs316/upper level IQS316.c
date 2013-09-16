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
//! 2013.7.31 增加 void CommsIQSxxx_Read_continuous(uint8_t read_addr ,uint8_t *data,uint8_t NoOfBytes) zozo
//! 2013.7.31 增加iqs316_debug调试监控 zozo
//---------------------------------------------------------------------------//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      revision   :  IQS316 I2C Firmware library rev 1.00
          date   :  2010-08-05
            by   :  Gerhard du Toit  Azoteq (Pty) Ltd 
   description   :  Basic functions to implement IQS316 via I2C (msp430f4619 was used)
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

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
#include "mb9bf506r.h"
#include "board.h"
#include "IQS316.h"
#include "iqs316_define.h"
#include "lower level mb9bfxxx.h"
#include "upper level IQS316.h"
  
//****************************************************************************
// 变量定义
//****************************************************************************/
IQS316_t IQS316;
/*
 max key number
*/
uint8_t Key;


IQS316_chan_info_t Iqs316ChanInfo[IQS316_KEY_NUMBER];

/*touch mode*/
static uint8_t time_up=0,prox_release=0;

/*test*/
static int8_t history_delta_buf[100],history_delta_ptr=0;


//---------------------------------------------------------------------------//
//
//! @brief 			IQS316_ATI
//! @author 		zozo
//! @note       
//! @param 			mode:0 prox ati 1 touch ait
//!             
//! @return  		void
//!
//!	@deprecated none
//! @since 2012.11.28
//! @see   
//---------------------------------------------------------------------------//
void IQS316_ATI(uint8_t mode)
{
	uint8_t temp;

	temp =  CommsIQSxxx_Read(PROX_SETTINGS_2);
	temp &=~ (BIT5|BIT4|BIT3);
  temp |= BIT1|BIT0;                   						//long term noise disabled still
	CommsIQSxxx_Write(PROX_SETTINGS_2, temp);       //disable watchdog, //skip conversions, //disable LTN (set bit)
	CommsIQSxxx_stop();	
	
	if(mode==1)
	{
    /***********TOUCH ATI**************/

		CommsIQSxxx_Write(AUTO_ATI_TARGET_HI, TOUCH_ATI_TARGET_HI);
		CommsIQSxxx_Write(AUTO_ATI_TARGET_LO, TOUCH_ATI_TARGET_LO);
		CommsIQSxxx_stop();
		
		temp = CommsIQSxxx_Read(UI_SETTINGS0);   
		CommsIQSxxx_Write(UI_SETTINGS0, temp | 0x40);
		CommsIQSxxx_stop();

	  
	}
	else if(mode==2)
	{
 
	}
	else
	{
    /***********PROX ATI**************/
		CommsIQSxxx_Write(AUTO_ATI_TARGET_HI, PROX_ATI_TARGET_HI);             //AUTO ATI LEVELS - 2000
		CommsIQSxxx_Write(AUTO_ATI_TARGET_LO, PROX_ATI_TARGET_LO);      
		CommsIQSxxx_stop();
		
		temp = CommsIQSxxx_Read(UI_SETTINGS0);
		CommsIQSxxx_Write(UI_SETTINGS0,temp & 0xBF);        //Prox auto ati - just clear bit 6
		CommsIQSxxx_stop();		

	}

  temp = CommsIQSxxx_Read(PROX_SETTINGS_1);   
  CommsIQSxxx_Write(PROX_SETTINGS_1,temp | 0x08);  //AUTO ATI 
  CommsIQSxxx_stop();
    
  temp = CommsIQSxxx_Read(UI_FLAGS0);   
  CommsIQSxxx_stop();
    
  while((temp & 0x04) != 0){
    temp = CommsIQSxxx_Read(UI_FLAGS0);
    CommsIQSxxx_stop();
  };

	CommsIQSxxx_Write(PROX_SETTINGS_2, PROX_SETTINGS_2_DEF);       //默认值 
	CommsIQSxxx_stop();		
}


/*
	Function name:	setChanReseed
	Parameters: 		ch 
	Parameters: 		bit
	Return: 				None
	Description:	
*/
void setChanReseed(uint8_t ch, uint8_t bit)
{
	CommsIQSxxx_Write(ch, bit);
}

/*
	Function name:	setProxMode
	Parameters:		None
	Return:				None
	Description:	
*/
void setProxMode(void)
{
	uint8_t temp;
	temp = CommsIQSxxx_Read(UI_SETTINGS0);
	temp &=~ BIT2;
	temp |= BIT3;
	CommsIQSxxx_Write(UI_SETTINGS0, temp);
	
}
/*
	Function name:	setTouchMode
	Parameters:		None
	Return:				None
	Description:	
*/
void setTouchMode(void)
{
	uint8_t temp;
	temp = CommsIQSxxx_Read(UI_SETTINGS0);
	temp |= BIT2;
	temp &=~ BIT3;
	CommsIQSxxx_Write(UI_SETTINGS0, temp);
	time_up =0;
	
}
/*
Function name:	ScanProxChan
Parameters: 	num_chan
Return: 			group number
Description:	
*/
uint8_t ReadOutChanInfo(uint8_t num_chan)
{
	uint8_t temp_num,ptr=0;
	uint8_t i=0,j=0;

	uint8_t buf[20];
  struct{
    union
    {
      uint16_t lta_word[4];
      uint8_t lta_byte[8];
    };	
    union
    {
      uint16_t cs_word[4];
      uint8_t cs_byte[8];
    };
    int16_t temp_delta[4];

    }temp_info;
	



	temp_num = CommsIQSxxx_Read(GROUP_NUM);

	CommsIQSxxx_Read_continuous(LTA_04_HI,&buf[0],2*num_chan);  //lta
	CommsIQSxxx_Read_continuous(CUR_SAM_04_HI,&buf[2*num_chan],2*num_chan);  //current sample
	
	for(i=0,j=0;i<4;i++)
	{
		if(i<num_chan)
			{
				temp_info.lta_byte[j] = buf[j+1];
				temp_info.lta_byte[j+1] = buf[j];
				temp_info.lta_word[i] &= 0x0fff;
				
				temp_info.cs_byte[j] = buf[j+1+2*num_chan];
				temp_info.cs_byte[j+1] = buf[j+2*num_chan];
				j+=2;
				
				temp_info.temp_delta[i] = temp_info.lta_word[i] - temp_info.cs_word[i];

			}
		else
			{
				temp_info.lta_word[i]=0;
				temp_info.cs_word[i]=0;
				temp_info.temp_delta[i]=0;
			}

	}
	switch(temp_num)
	{
		case 0:
			ptr = 0;
			break;
		case 1:
			ptr = 4;
			break;
		case 2:
			ptr = 8;
			break;
		case 3:
			ptr = 12;
			break;
		case 4:
			ptr = 16;
			break;
			
		default:		
			break;
	}
  if(temp_num <=4)
  {
    Iqs316ChanInfo[0+ptr].Lta = temp_info.lta_word[0];
    Iqs316ChanInfo[1+ptr].Lta = temp_info.lta_word[1];
    Iqs316ChanInfo[2+ptr].Lta = temp_info.lta_word[2];
    Iqs316ChanInfo[3+ptr].Lta = temp_info.lta_word[3];
    Iqs316ChanInfo[0+ptr].CurtSample = temp_info.cs_word[0];
    Iqs316ChanInfo[1+ptr].CurtSample = temp_info.cs_word[1];
    Iqs316ChanInfo[2+ptr].CurtSample = temp_info.cs_word[2];
    Iqs316ChanInfo[3+ptr].CurtSample = temp_info.cs_word[3];
    Iqs316ChanInfo[0+ptr].Delta= temp_info.temp_delta[0];
    Iqs316ChanInfo[1+ptr].Delta = temp_info.temp_delta[1];
    Iqs316ChanInfo[2+ptr].Delta = temp_info.temp_delta[2];
    Iqs316ChanInfo[3+ptr].Delta = temp_info.temp_delta[3];	
  }


	return temp_num;

}



/*
Function name:	ScanProxChan
Parameters: 	none
Return: 			rc:1 detected 0 no
Description:	

*/
uint8_t ScanProxChan(void)
{
	uint8_t temp,rc;
        
  CommsIQSxxx_stop();
	/*
		disable stop comm
	*/
	temp = CommsIQSxxx_Read(PROX_SETTINGS_2);
	temp &=~ BIT5;
	CommsIQSxxx_Write(PROX_SETTINGS_2, temp); 
	/*
		look at mode indicator bit
	*/
	if(0==ReadOutChanInfo(CHAN_NUM))
		{	
			rc = 1;
		}
	return rc;
}


/*
	Function name:	ProxDetect
	Parameters:		uint8_t prox_state
	Return:				rc:1 detected 0 no
	Description:	
*/
uint8_t ProxDetect(uint8_t prox_state)
{
	uint8_t rc = 0,i,continus,cnt=0;
	int16_t delta=0; 
	static int16_t filter_buf[10],filter_ptr=0;
	uint8_t index;
	
	do{
	/*sum use group0 delta*/ 
		for(i=0;i<4;i++)
			{
				if(PM_CX_SELECT_DEF & (1<<i))
					{
						delta += Iqs316ChanInfo[i].Delta;
					}
			}
		//test
		history_delta_buf[history_delta_ptr++]=delta;
		if(history_delta_ptr>=100)
			{
				history_delta_ptr=0;
			}
		
		/*iir filter*/
		filter_buf[filter_ptr] = delta;
		delta=0;
		index = filter_ptr;
		if(cnt>=5)
			{
				for(i=0;i<5;i++)
					{
						delta += filter_buf[index];
						if(index)
							{
								index--;
							}
						else
							{
							 index = 9;
							}
					}
				delta /= 5;
				if(delta >= PM_CHK_MAX_THRESHOLD )	
					{
						rc = 1;
						break;
					}

			}
		else
			{

			}

    filter_ptr++;
		if(filter_ptr>=10)
			{
				filter_ptr=0;
			}
    



		if(prox_state)
			{
				continus = 1;
				ScanProxChan();
			}
		else
			{
				break;
			}

cnt++;
	}while(continus && cnt<10);

	


		return rc;
}
/*
	Function name:	TouchDetect
	Parameters: 		none
	Return: 				key number
	Description:	
*/
uint8_t TouchDetect(void)
{

		int16_t min = 32767;
		int16_t max = -32768; 	
		uint8_t i,max_key;
		
		for (i = IQS316_PROX_NUMBER;  i < IQS316_KEY_NUMBER;	i++)
		{
		
				if (Iqs316ChanInfo[i].Delta< min)
				{
					min = Iqs316ChanInfo[i].Delta;
					//min_key =i;
				}
						
				if (Iqs316ChanInfo[i].Delta > max)
				{
						max = Iqs316ChanInfo[i].Delta;
						max_key = i;
				}

		}
		
		if( max > TOUCH_MAX_THRESHOLD ) 
			return max_key;
		else
			return NONE_KEY;
}

/*
	Function name:	IQS316_Settings
	Parameters:		None
	Return:			None
	Description:	Sends commands to set thresholds of the IQS316 via the comms terminal.
					This is done, by disabling the conversion mode of the IQS316 (by setting
					the SKIP_CONV bit, PROX_SETTINGS_2[3])
					Each group is now accessed by initialising new conversions and in each
					group the respective thresholds are written as desired.
					Remember to set the Prox Range and Touch Range bits (UI_SETTINGS[5,4])
					to access either high range or low range thresholds.
*/
void IQS316_Settings(void)
{
	uint8_t start_num, temp;
	int i = 0;
	uint8_t temp_lta;
	//uint8_t cs_h, cs_l, group_num, ui_settings, auto_ati_h, auto_ati_l,pcc;
	
	CommsIQSxxx_Init_Conversion();

  /*
  		setting register start  will use about 2~3 second
  		*/ 
  temp =  CommsIQSxxx_Read(PROD_NUM);
  if(temp!=27)
  {
    nop(); //通讯异常
  }

  CommsIQSxxx_Write(UI_SETTINGS0,UI_SETTINGS0_DEF); 
  CommsIQSxxx_Write(PROX_SETTINGS_1,PROX_SETTINGS_1_DEF);  
  CommsIQSxxx_Write(PROX_SETTINGS_2,PROX_SETTINGS_2_DEF);  

	
  
	//set prox mode
	CommsIQSxxx_Write(PM_CX_SELECT, PM_CX_SELECT_DEF);			//all channel groups for prox mode
	CommsIQSxxx_Write(CHAN_ACTIVE0, CHAN_ACTIVE0_DEF);			
	/*
	 set touch channal
	*/
	CommsIQSxxx_Write(CHAN_ACTIVE1, CHAN_ACTIVE1_DEF);			    
	CommsIQSxxx_Write(CHAN_ACTIVE2, CHAN_ACTIVE2_DEF);			    
	CommsIQSxxx_Write(CHAN_ACTIVE3, CHAN_ACTIVE3_DEF);			    
	CommsIQSxxx_Write(CHAN_ACTIVE4, CHAN_ACTIVE4_DEF);			    
	CommsIQSxxx_stop();


 	/*
 		 initial channl   Threshold & ATI Multiplier C
	*/ 
	temp =  CommsIQSxxx_Read(PROX_SETTINGS_2);
	temp |= BIT3|BIT1|BIT0;                   					//Skip Conv  long term noise disabled still
	CommsIQSxxx_Write(PROX_SETTINGS_2, temp);           //disable watchdog, //skip conversions, //disable LTN (set bit)
	CommsIQSxxx_stop();
	            
	start_num = CommsIQSxxx_Read(GROUP_NUM);
	temp = start_num;

	do{
		switch (temp)
		{
			case 0:
					temp_lta = CommsIQSxxx_Read(LTA_04_HI);
					CommsIQSxxx_Write(LTA_04_HI, temp_lta | PROX_THRESHOLD_CH0);
					temp_lta = CommsIQSxxx_Read(LTA_15_HI);
					CommsIQSxxx_Write(LTA_15_HI, temp_lta | PROX_THRESHOLD_CH1);
					temp_lta = CommsIQSxxx_Read(LTA_26_HI);
					CommsIQSxxx_Write(LTA_26_HI, temp_lta | PROX_THRESHOLD_CH2);
					temp_lta = CommsIQSxxx_Read(LTA_37_HI);
					CommsIQSxxx_Write(LTA_37_HI, temp_lta | PROX_THRESHOLD_CH3);
					CommsIQSxxx_Write(ATI_MULT1, ATI_MULT1_GRP0);  
				break;
				
			case 1:
					temp_lta = CommsIQSxxx_Read(LTA_04_HI);
					CommsIQSxxx_Write(LTA_04_HI, temp_lta | TOUCH_THRESHOLD_CH4);
					temp_lta = CommsIQSxxx_Read(LTA_15_HI);
					CommsIQSxxx_Write(LTA_15_HI, temp_lta | TOUCH_THRESHOLD_CH5);
					temp_lta = CommsIQSxxx_Read(LTA_26_HI);
					CommsIQSxxx_Write(LTA_26_HI, temp_lta | TOUCH_THRESHOLD_CH6);
					temp_lta = CommsIQSxxx_Read(LTA_37_HI);
					CommsIQSxxx_Write(LTA_37_HI, temp_lta | TOUCH_THRESHOLD_CH7);
					CommsIQSxxx_Write(ATI_MULT1, ATI_MULT1_GRP1);  
				break;
			case 2:		
					temp_lta = CommsIQSxxx_Read(LTA_04_HI);
					CommsIQSxxx_Write(LTA_04_HI, temp_lta | TOUCH_THRESHOLD_CH8);
					temp_lta = CommsIQSxxx_Read(LTA_15_HI);
					CommsIQSxxx_Write(LTA_15_HI, temp_lta | TOUCH_THRESHOLD_CH9);
					temp_lta = CommsIQSxxx_Read(LTA_26_HI);
					CommsIQSxxx_Write(LTA_26_HI, temp_lta | TOUCH_THRESHOLD_CH10);
					temp_lta = CommsIQSxxx_Read(LTA_37_HI);
					CommsIQSxxx_Write(LTA_37_HI, temp_lta | TOUCH_THRESHOLD_CH11);
					CommsIQSxxx_Write(ATI_MULT1, ATI_MULT1_GRP2);  
				break;
			case 3:
					temp_lta = CommsIQSxxx_Read(LTA_04_HI);
					CommsIQSxxx_Write(LTA_04_HI, temp_lta | TOUCH_THRESHOLD_CH12);
					temp_lta = CommsIQSxxx_Read(LTA_15_HI);
					CommsIQSxxx_Write(LTA_15_HI, temp_lta | TOUCH_THRESHOLD_CH13);
					temp_lta = CommsIQSxxx_Read(LTA_26_HI);
					CommsIQSxxx_Write(LTA_26_HI, temp_lta | TOUCH_THRESHOLD_CH14);
					temp_lta = CommsIQSxxx_Read(LTA_37_HI);
					CommsIQSxxx_Write(LTA_37_HI, temp_lta | TOUCH_THRESHOLD_CH15);
					CommsIQSxxx_Write(ATI_MULT1, ATI_MULT1_GRP3);  
				break;
				
			case 4:
					temp_lta = CommsIQSxxx_Read(LTA_04_HI);
					CommsIQSxxx_Write(LTA_04_HI, temp_lta | TOUCH_THRESHOLD_CH16);
					temp_lta = CommsIQSxxx_Read(LTA_15_HI);
					CommsIQSxxx_Write(LTA_15_HI, temp_lta | TOUCH_THRESHOLD_CH17);
					temp_lta = CommsIQSxxx_Read(LTA_26_HI);
					CommsIQSxxx_Write(LTA_26_HI, temp_lta | TOUCH_THRESHOLD_CH18);
					temp_lta = CommsIQSxxx_Read(LTA_37_HI);
					CommsIQSxxx_Write(LTA_37_HI, temp_lta | TOUCH_THRESHOLD_CH19);				
					CommsIQSxxx_Write(ATI_MULT1, ATI_MULT1_GRP4);    			
				break;
		}
		CommsIQSxxx_stop();	
 		
    for (i = 0; i < 10; i++)           //time to settle
        delay_ms(1); 

		temp = CommsIQSxxx_Read(GROUP_NUM);
	} while (temp != start_num);
	
	CommsIQSxxx_stop();	
	
	/*ATI*/ 
  IQS316_ATI(1);
  IQS316_ATI(0);
	
	/*prox mode*/ 
	setProxMode();
	
	/*
		set stop comm
	*/ 
	CommsIQSxxx_Write(PROX_SETTINGS_2, PROX_SETTINGS_2_DEF | BIT5);  
	CommsIQSxxx_stop();			

			
}


/*
	Function name:	init
	Parameters:		none
	Return:			none
	Description: 	Responsible for initializing the PIC16LF1827 ports and calling other 
					initialization functions.
*/
void Init_iqs316(void)
{

/*
	Needed if port initialization is done here
*/		
	//ready line input
	Comms_init();	
/*
	Place other functions responsible for hardware initialization here.
*/	
	IQS316_Settings();
}

/*
	Function name:	IQS316_New_Conversion
	Parameters:		None
	Return:			None
	Description:	Sends command to IQS316 to initiate a new conversion (0xFE). The group number is read to determine
					whether the IQS316 is in PROX mode or TOUCH mode. If the group number is 0, PROX mode, the prox_detected
					flag is cleared and the routine finishes.
					If the group number is not 0, then conversions are called to access each of the four groups. The PROX and
					TOUCH status registers for each group is read and stored in the global data structure. The prox_detected
					flag is set and the routine finishes.
	Global implication:		After calling this function, the data of the IQS316 can be accessed in the struct IQS316,
							which has the following members.
							prox_detected (flag to indicate prox mode or touch mode)
							prox4_11 (prox status for channels 4-11)
							prox12_19 (prox status for channels 12-19)
							touch4_11 (touch status for channels 4-11)
							touch12_19 (touch status for channels 12-19)
*/
void IQS316_New_Conversion(void)
{
  uint8_t temp,temp_num, temp_flag,start_num=0, temp_touch, temp_prox;
	uint8_t touch_detected=0,prox_detected=0;

  
		/*
			disable stop comm
		*/
		temp = CommsIQSxxx_Read(PROX_SETTINGS_2);
		temp &=~ BIT5;
		CommsIQSxxx_Write(PROX_SETTINGS_2, temp); 
		/*
			look at mode indicator bit
		*/
		temp_flag = CommsIQSxxx_Read(UI_FLAGS0); 

		temp_num = CommsIQSxxx_Read(GROUP_NUM);

		do
		{

			temp_touch = CommsIQSxxx_Read(TOUCH_STAT);
			temp_prox = CommsIQSxxx_Read(PROX_STAT);
			
			ReadOutChanInfo(4);

      if(temp_touch)
      {
      	touch_detected = 1;
      }
      if(temp_prox)
      {
      	prox_detected = 1;
      }
	       // x++;
			switch(temp_num)
			{
				case 0:
					goto skip_stop;
					break;
					
				case 1:
					IQS316.prox4_11 = IQS316.prox4_11 & 0xF0;
					IQS316.touch4_11 = IQS316.touch4_11 & 0xF0;
					IQS316.prox4_11 = IQS316.prox4_11 | (temp_prox & 0x0F);
					IQS316.touch4_11 = IQS316.touch4_11 | (temp_touch & 0x0F);
					break;
				case 2:
					IQS316.prox4_11 = IQS316.prox4_11 & 0x0F;
					IQS316.touch4_11 = IQS316.touch4_11 & 0x0F;
					IQS316.prox4_11 = IQS316.prox4_11 | ((temp_prox & 0x0F) << 4);
					IQS316.touch4_11 = IQS316.touch4_11 | ((temp_touch & 0x0F) << 4);
					break;
				case 3:
					IQS316.prox12_19 = IQS316.prox12_19 & 0xF0;
					IQS316.touch12_19 = IQS316.touch12_19 & 0xF0;
					IQS316.prox12_19 = IQS316.prox12_19 | (temp_prox & 0x0F);
					IQS316.touch12_19 = IQS316.touch12_19 | (temp_touch & 0x0F);
					break;
				case 4:
					IQS316.prox12_19 = IQS316.prox12_19 & 0x0F;
					IQS316.touch12_19 = IQS316.touch12_19 & 0x0F;
					IQS316.prox12_19 = IQS316.prox12_19 | ((temp_prox & 0x0F) << 4);
					IQS316.touch12_19 = IQS316.touch12_19 | ((temp_touch & 0x0F) << 4);
					break;
			    default:    
		             
			    break;
			}
		
			CommsIQSxxx_stop();
skip_stop:			

			
			/*
				proximity mode scan once break
			*/
			if((temp_num ==0) && !(temp_flag&0x40))
				{
				 	break;
				}
			/*
				touch mode scan start
			*/
			else if((0!=temp_num) && (temp_flag&0x40) && (0==start_num))
				{
					start_num = temp_num;

				}
			/*
				next group
			*/
			temp_num = CommsIQSxxx_Read(GROUP_NUM);		

		} while ((temp_num != 0) && (temp_num != start_num));

		

		/*
		 proximity detect
		*/   
		if((temp_num ==0) && !(temp_flag&0x40))
			{
			  if(0==prox_release)
			  	{
						if(ProxDetect(prox_detected))
							{
								IQS316.prox_detected = 1;
								setTouchMode();
							}
						else
							{
								IQS316.prox_detected = 0;
							}

			  	}
				else
					{
						prox_release--;
						setChanReseed(CHAN_RESEED0,BIT1|BIT0);
					}

					
			}
		else if((0!=temp_num) && (temp_flag&0x40))
			{
				/*
				 touch detect
				*/
				Key = TouchDetect();
				if(Key != NONE_KEY)
					{
						IQS316.touch_detected = 1;
						time_up =0;
					}
				else
					{
						IQS316.touch_detected = 0;
						time_up++;
						if( time_up > TOUCH_CHK_CNT)
							{
								setProxMode();
                setChanReseed(CHAN_RESEED0,BIT1|BIT0);
								IQS316.prox_detected = 0;
								prox_release = 1;
							}
					}
			}

		CommsIQSxxx_Write(PROX_SETTINGS_2, PROX_SETTINGS_2_DEF | BIT5);       //stop comm 		
		CommsIQSxxx_stop();
		delay_ms(1);
}




/*
	Function name:	IQS316_Process_Conversions
	Parameters:		None
	Return:			None
	Description:	Function to be called after calling for a new conversion, if the prox_detected flag is set.
					This function processes the data contained in the IQS316 struct.
*/
void IQS316_Process_Conversion (void)
{
/*
	Place code here to process data available in the IQS316 structure.
*/

}




/*
	Function name:	CommsIQSxxx_Write
	Parameters:		uint8_t write_addr - the address to be written to
					uint8_t data - the data to be written to the specified address.
	Return:			None
	Description:	Sends the first parameter to the the IQS440, followed by the second.
					The second byte is therefore written in the address of the first given that the IQS440 was
					already in write mode before calling the function.
*/
void CommsIQSxxx_Write(uint8_t write_addr, uint8_t data)
{
	CommsIQSxxx_start();
	CommsIQSxxx_send((IQS316_ADDR << 1) + 0x00);	//initiate I2C Comms in write mode
	CommsIQSxxx_send(write_addr);					//write address
	CommsIQSxxx_send(data);							//write data
}
/*
	Function name:	CommsIQSxxx_Read
	Parameters:		uint8_t read_addr - the address to be read
	Return:			uint8_t - the data that was read from the address on the IQS440
	Description:	Rerturns data read from the IQS440, at a specific address.
					There has to be an active communication window to the IQS440 prior to calling this function.
					I.e. either a start or a repeated start must be called before calling this function.
					This function is specifically for reading one data byte from a specific value. After calling
					this function, either a stop or a repeated start has to be called.
					- Please use CommsIQS440_Cont_Read for reading multiple bytes from the IQS440
*/
uint8_t CommsIQSxxx_Read(uint8_t read_addr)
{
	uint8_t temp;
	
	CommsIQSxxx_start();
	CommsIQSxxx_send((IQS316_ADDR << 1) + 0x00);	//initiates I2C Comms in write mode
	CommsIQSxxx_send(read_addr);					//write address to be read from
	CommsIQSxxx_repeat_start();
	CommsIQSxxx_send((IQS316_ADDR << 1) + 0x01);	//initiates I2C Comms in read mode
	temp = CommsIQSxxx_read_nack();					//NACK indicates that no more reads will follow after this one.
	return temp;
}
/*
	Function name:	CommsIQSxxx_Read_continuous
	Parameters:			uint8_t read_addr ,uint8_t *data,uint8_t NoOfBytes
	Return:					uint8_t - the data that was read from the address on the IQS440
	Description:	

*/
void CommsIQSxxx_Read_continuous(uint8_t read_addr ,uint8_t *data,uint8_t NoOfBytes)
{
	uint8_t i;
	
	CommsIQSxxx_start();
	CommsIQSxxx_send((IQS316_ADDR << 1) + 0x00);	//initiates I2C Comms in write mode
	CommsIQSxxx_send(read_addr);					//write address to be read from
	CommsIQSxxx_repeat_start();
	CommsIQSxxx_send((IQS316_ADDR << 1) + 0x01);	//initiates I2C Comms in read mode
	if (NoOfBytes > 1)
	{
		for (i = 0; i < NoOfBytes - 1; i++)
			data[i] = CommsIQSxxx_read_ack();				// all bytes except last must be followed by an ACK
	}
	
	data[NoOfBytes-1] = CommsIQSxxx_read_nack();			// last byte read must be followed by a NACK
}



/*
	Function name:	CommsIQSxxx_Cont_Read
	Parameters:		uint8_t start_addr - the address where the first byte will be read from
	Return:			uint8_t - the data that was read from the address on the IQS316
	Description:	This function is similiar to the previous one (CommsIQSxxx_Read) and only differs in sending an
					ACK to the IQS316, indicating that another read command will follow this. The data pointer of the
					IQS316 will be incremented for the next read.
*/
uint8_t CommsIQSxxx_Cont_Read(uint8_t start_addr)
{
	uint8_t temp;
	CommsIQSxxx_start();
	CommsIQSxxx_send((IQS316_ADDR << 1) + 0x00);
	CommsIQSxxx_send(start_addr);
	CommsIQSxxx_repeat_start();
	CommsIQSxxx_send((IQS316_ADDR << 1) + 0x01);
	temp = CommsIQSxxx_read_ack();
	return temp;
}




/*
	Function name:	CommsIQSxxx_Read_Next_Cont
	Parameters:		none
	Return:			uint8_t - the data that was read from the IQS316
	Description:	This function has to be used in conjunction with the CommsIQSxxx_Cont_Read function for reading multiple
					bytes from the IQS316. This specific function sends another ACK to the IQS316 and another read command must
					be sent after it. The data pointer will be incremented after executing this command.
*/
uint8_t CommsIQSxxx_Read_Next_Cont(void)
{
	uint8_t temp;
	temp = CommsIQSxxx_read_ack();
	return temp;
}




/*
	Function name:	CommsIQSxxx_Read_Next_Stop
	Parameters:		none
	Return:			uint8_t - the data that was read from the IQS316
	Description:	This function has to be used in conjunction with the CommsIQSxxx_Cont_Read function for reading multiple
					bytes from the IQS316. This specific function sends a NACK to the IQS316 to indicate that this is the last read
					command.
					Either a stop command or a repeated start command has to be sent after this.
*/
uint8_t CommsIQSxxx_Read_Next_Stop(void)
{
	uint8_t temp;
	temp = CommsIQSxxx_read_nack();
	return temp;
}




/*
	Function name:	CommsIQSxxx_Initiate_Conversion
	Parameters:		none
	Return:			none
	Description:	This function initiates a communication window with the IQS316 and then closes it again. 
					In I2C mode a new conversion will automatically be started once the stop bit is received.
					This function should therefore be used if more than one conversion is required between
					read and write operations. (Such as the initial conversions before reseeding and calling 
					the IPA routine.)
*/
void CommsIQSxxx_Init_Conversion(void)
{
	CommsIQSxxx_start();
	CommsIQSxxx_send((IQS316_ADDR << 1) + 0x00);
	CommsIQSxxx_stop();
}





/*
	Function name:	Comms_Error
	Parameters:		none
	Return:			none
	Description:	Function called if an unexpected return value is received communicating
					with the IQS316.
*/
void Comms_Error(void)
{
	/*
		Place error routine code here
	*/
	while (1)
	{}
}

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif
