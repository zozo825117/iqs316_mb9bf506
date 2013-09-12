//---------------------------------------------------------------------------//
//
//! @file 		IQS316.h
//! @brief    IQS316 Module specification 
//
//! @author 	zozo 
//! @version 	v0.1
//! @note
//!     
//! 
//! 修改日志  :
//! 2013.7.16    创建初试 zozo
//
//
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
#ifndef __IQS316_H__ 
#define __IQS316_H__

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

/*
Name             :  IQS316.h
Description      :  Header file of IQS316 memory map addresses
*/

#define PROD_NUM		0x00
#define VERSION_NUM		0x01

#define UI_FLAGS0		0x10

#define PROX_STAT		0x31
#define TOUCH_STAT 		0x35
#define HALT_STAT		0x39
#define GROUP_NUM		0x3D

#define CUR_SAM_04_HI	0x42
#define CUR_SAM_04_LO	0x43
#define CUR_SAM_15_HI	0x44
#define CUR_SAM_15_LO	0x45
#define CUR_SAM_26_HI	0x46
#define CUR_SAM_26_LO	0x47
#define CUR_SAM_37_HI	0x48
#define CUR_SAM_37_LO	0x49

#define LTA_04_HI		0x83
#define LTA_04_LO		0x84
#define LTA_15_HI		0x85
#define LTA_15_LO		0x86
#define LTA_26_HI		0x87
#define LTA_26_LO		0x88
#define LTA_37_HI		0x89
#define LTA_37_LO		0x8A

#define UI_SETTINGS0 		0xC4
#define POWER_SETTINGS		0xC5
#define PROX_SETTINGS_1		0xC6
#define PROX_SETTINGS_2		0xC7
#define ATI_MULT1			0xC8
#define CMT_SETTINGS		0xC9
#define PCC0			0xCA
#define PCC1			0xCB
#define PCC2			0xCC
#define PCC3			0xCD
#define SHLD_SETTINGS	0xCE
#define INT_CAL_SETTINGS	0xCF
#define PM_CX_SELECT	0xD0
#define DEFAULT_COMMS_PTR	0xD1
#define CHAN_ACTIVE0	0xD2
#define CHAN_ACTIVE1	0xD3
#define CHAN_ACTIVE2	0xD4
#define CHAN_ACTIVE3	0xD5
#define CHAN_ACTIVE4	0xD6
#define CHAN_RESEED0	0xD7
#define CHAN_RESEED1	0xD8
#define CHAN_RESEED2	0xD9
#define CHAN_RESEED3	0xDA
#define CHAN_RESEED4	0xDB
#define AUTO_ATI_TARGET_HI	0xDC
#define AUTO_ATI_TARGET_LO	0xDD

#define IO_PORT 0xDE
#define IO_TRIS 0xDF

#define DIRECT_ADDR_RW		0xFC
#define DIRECT_DATA_RW		0xFD

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif
#endif // __IQS316_H__

