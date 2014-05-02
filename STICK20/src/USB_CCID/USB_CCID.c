/*
* Author: Copyright (C) Rudolf Boeddeker  Date: 24.11.2010
*
* This file is part of GPF Crypto Stick 2
*
* GPF Crypto Stick 2  is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* GPF Crypto Stick is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with GPF Crypto Stick. If not, see <http://www.gnu.org/licenses/>.
*/


/*
 * USB_CCID.c
 *
 *  Created on: 24.11.2010
 *      Author: RB
 */

#include "global.h"
#include "tools.h"
#include "usart.h"
#include "CCID\\USART\\ISO7816_USART.h"
#include "CCID\\USART\\ISO7816_ADPU.h"
#include "CCID\\USART\\ISO7816_Prot_T1.h"
#include "USB_CCID.h"
#include "USB_CCID_task.h"
#include "CCID/Local_ACCESS/Debug_T1.h"

/*******************************************************************************

 Local defines

*******************************************************************************/

//#define DEBUG_USB_CCID_IO

#ifdef DEBUG_USB_CCID_IO
  int CI_LocalPrintf (char *szFormat,...);
  int CI_TickLocalPrintf (char *szFormat,...);
#else
  #define CI_LocalPrintf(...)
  #define CI_TickLocalPrintf(...)
#endif

#define DEBUG_LOG_CCID_DETAIL


/*******************************************************************************

 Global declarations

*******************************************************************************/

/*******************************************************************************

 External declarations

*******************************************************************************/

/*******************************************************************************

 Local declarations

*******************************************************************************/

t_USB_CCID_data_st g_USB_CCID_data_st;


static u8 CCID_SlotStatus_u8  = CCID_SLOT_STATUS_NOT_PRESENT; 
static u8 CCID_ClockStatus_u8 = CCID_SLOT_STATUS_CLOCK_UNKNOWN;

/*******************************************************************************

  CCID_RestartSmartcard_u8

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

u8 CCID_RestartSmartcard_u8 (void)
{
	int nRet;

	nRet = ISO7816_InitSC ();
	if (TRUE == nRet)
	{
	  CI_TickLocalPrintf ("*** Smartcard ON ***\n");
		CCID_SlotStatus_u8 = CCID_SLOT_STATUS_PRESENT_ACTIVE;
	}
	else
	{
	  CI_TickLocalPrintf ("*** ERROR Smartcard is not ON ***\n");
		CCID_SlotStatus_u8 = CCID_SLOT_STATUS_NOT_PRESENT;
	}

	return (nRet);
}

/*******************************************************************************

  CCID_SmartcardOff_u8

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

u8 CCID_SmartcardOff_u8 (void)
{
  CI_TickLocalPrintf ("*** Smartcard off ***\n");
	Smartcard_Reset_off ();		// Disable SC
  CCID_SlotStatus_u8 = CCID_SLOT_STATUS_NOT_PRESENT;
	return (TRUE);
}

/*******************************************************************************

  CCID_GetHwError_u8

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

u8 CCID_GetHwError_u8 (u8 *SC_ErrorCode_u8)
{
	return (CCID_NO_ERROR);
}

/*******************************************************************************

  CCID_SetCardState_v

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

void CCID_SetCardState_v (unsigned char nState)
{
//	cCRD_CardPresent = nState;	
}
/*******************************************************************************

  CCID_GetCardState_u8

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

u8 CCID_GetCardState_u8 (void)
{
	return (TRUE);	
}
/*******************************************************************************

  CCID_GetSlotStatus_u8

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

u8 CCID_GetSlotStatus_u8 (void)
{
	return (CCID_SlotStatus_u8); // RB TODO CCID_SlotStatus_u8);
}
/*******************************************************************************

  CCID_SetSlotStatus_u8

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

void CCID_SetSlotStatus_u8 (u8 SlotStatus_u8)
{
	CCID_SlotStatus_u8 = SlotStatus_u8;	
}

/*******************************************************************************

  CCID_GetClockStatus_u8

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

u8 CCID_GetClockStatus_u8 (void)
{
	return (CCID_ClockStatus_u8);	
}

/*******************************************************************************

  CCID_SetClockStatus_u8

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

void CCID_SetClockStatus_u8 (u8 ClockStatus_u8)
{
	CCID_ClockStatus_u8 = ClockStatus_u8;	
}
/*******************************************************************************

  CCID_SetATRData_u8

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

u8 CCID_SetATRData_u8 (t_USB_CCID_data_st *USB_CCID_data_pst)
{
	USB_CCID_data_pst->CCID_datalen = 0; //CCID_OFFSET_XFR_BLOCK_DATA;

	USB_CCID_data_pst->USB_data[CCID_OFFSET_LENGTH] = ISO7816_CopyATR ((u8*)&USB_CCID_data_pst->USB_data[CCID_OFFSET_XFR_BLOCK_DATA],USB_CCID_MAX_LENGTH);
	USB_CCID_data_pst->CCID_datalen += USB_CCID_data_pst->USB_data[CCID_OFFSET_LENGTH];

	USB_CCID_data_pst->USB_data[12] = 0x18; // Hack for slow SC baudrate

	return (TRUE);	
}
/*******************************************************************************

  CCID_XfrBlock_u8

  Erh�ht den Eventz�hler eines Z�hlers

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

u8 CCID_XfrBlock_u8 (t_USB_CCID_data_st *USB_CCID_data_pst,u16 *CCID_AnswerSize_pu16,u16 CCID_LevelParameter_u16)
{
	s32 Ret_s32;
	s32 XfrLenght_s32;
//	u32 TickStart_u32;


	XfrLenght_s32 = USB_CCID_data_pst->USB_data[CCID_OFFSET_LENGTH];
/*
TickStart_u32 =	xTaskGetTickCount();
CI_LocalPrintf ("%7d : CCID_XfrBlock - Max len %3d - ",TickStart_u32,XfrLenght_s32);

Print_T1_Block (USB_CCID_data_pst->USB_data[CCID_OFFSET_LENGTH],&USB_CCID_data_pst->USB_data[CCID_OFFSET_XFR_BLOCK_DATA]);
*/
#ifdef DEBUG_LOG_CCID_DETAIL
	LogStart_T1_Block (USB_CCID_data_pst->USB_data[CCID_OFFSET_LENGTH],(u8*)&USB_CCID_data_pst->USB_data[CCID_OFFSET_XFR_BLOCK_DATA]);
#endif

	Ret_s32 = ISO7816_T1_DirectXfr (&XfrLenght_s32,(u8*)&USB_CCID_data_pst->USB_data[CCID_OFFSET_XFR_BLOCK_DATA],CCID_MAX_XFER_LENGTH);

	if (USART_SUCCESS == Ret_s32)
	{
#ifdef DEBUG_LOG_CCID_DETAIL
	  LogEnd_T1_Block (XfrLenght_s32,(u8*)&USB_CCID_data_pst->USB_data[CCID_OFFSET_XFR_BLOCK_DATA]);
#endif
//CI_LocalPrintf (" Answer len %3d - %5d ms\n",XfrLenght_s32,xTaskGetTickCount()-TickStart_u32);
//HexPrint (XfrLenght_s32,&USB_CCID_data_pst->USB_data[CCID_OFFSET_XFR_BLOCK_DATA]);

		*CCID_AnswerSize_pu16 = (u16)XfrLenght_s32;
		Ret_s32 = CCID_NO_ERROR;
	}
	else
	{
		USB_CCID_data_pst->USB_data[CCID_OFFSET_LENGTH] = 0;
		Ret_s32 = CCID_ERROR_HW_ERROR;
CI_TickLocalPrintf ("CCID ERROR\n");
	}

	return (Ret_s32);
}
/*******************************************************************************

  CCID_CheckAbortRequest_u8

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

u8 CCID_CheckAbortRequest_u8 (void)
{
	return (FALSE);	
}
/*******************************************************************************

  RDR_to_PC_DataBlock_u8

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

u8 RDR_to_PC_DataBlock_u8 (t_USB_CCID_data_st *USB_CCID_data_pst)
{

	USB_CCID_data_pst->USB_data[CCID_OFFSET_MESSAGE_TYPE] = RDR_TO_PC_DATA_BLOCK;
	return (TRUE);
}
/*******************************************************************************

  RDR_to_PC_SlotStatus_CardStopped_u8

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

u8 RDR_to_PC_SlotStatus_CardStopped_u8 (u8 ErrorCode_u8)
{
	return (TRUE);
}

/*******************************************************************************

  PC_to_RDR_IccPowerOn_u8

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

u8 PC_to_RDR_IccPowerOn_u8 (t_USB_CCID_data_st *USB_CCID_data_pst)
{
	if(0 != USB_CCID_data_pst->USB_data[CCID_OFFSET_SLOT])
	{
		return (CCID_ERROR_BAD_SLOT);
	}

	if (0 != USB_CCID_data_pst->CCID_datalen)
	{
		return (CCID_ERROR_BAD_LENTGH);
	}

	if(TRUE == USB_CCID_data_pst->CCID_CMD_aborted)
	{
		return (CCID_ERROR_CMD_ABORTED);
	}

// We used only one voltage
	if (FALSE == CCID_RestartSmartcard_u8 ())
	{
		return (CCID_ERROR_HW_ERROR);
	}

	return (CCID_NO_ERROR);
}

/*******************************************************************************

  PC_to_RDR_IccPowerOff_u8

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

u8 PC_to_RDR_IccPowerOff_u8 (t_USB_CCID_data_st *USB_CCID_data_pst)
{
	if(0 != USB_CCID_data_pst->USB_data[CCID_OFFSET_SLOT])
	{
		return (CCID_ERROR_BAD_SLOT);
	}

	if (0 != USB_CCID_data_pst->CCID_datalen)
	{
		return (CCID_ERROR_BAD_LENTGH);
	}

// Do nothing, restart card at SM on
    CCID_SmartcardOff_u8 ();

	return (CCID_NO_ERROR);
}
/*******************************************************************************

  PC_to_RDR_GetSlotStatus_u8

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

u8 PC_to_RDR_GetSlotStatus_u8 (t_USB_CCID_data_st *USB_CCID_data_pst)
{
	u8 ErrorCode_u8;
	u8 SC_ErrorCode_u8;

	if(0 != USB_CCID_data_pst->USB_data[CCID_OFFSET_SLOT])
	{
		return (CCID_ERROR_BAD_SLOT);
	}

	if (0 != USB_CCID_data_pst->CCID_datalen)
	{
		return (CCID_ERROR_BAD_LENTGH);
	}

	ErrorCode_u8 = CCID_GetHwError_u8 (&SC_ErrorCode_u8);

	return (ErrorCode_u8);
}
/*******************************************************************************

  PC_to_RDR_XfrBlock_u8

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

u8 PC_to_RDR_XfrBlock_u8 (t_USB_CCID_data_st *USB_CCID_data_pst)
{
	u16 CCID_AnswerBlockSize_u16;
	u16 CCID_LevelParameter_u16;
	u32 UsbMessageLength_u32;

	u8 ErrorCode_u8;
	u8 SC_ErrorCode_u8;

	if(0 != USB_CCID_data_pst->USB_data[CCID_OFFSET_SLOT])
	{
		return (CCID_ERROR_BAD_SLOT);
	}

 	if (0 == USB_CCID_data_pst->CCID_datalen)
	{
		return (CCID_ERROR_BAD_LENTGH);
	}

	if(TRUE == USB_CCID_data_pst->CCID_CMD_aborted)
	{
		return (CCID_ERROR_CMD_ABORTED);
	}

	ErrorCode_u8 = CCID_GetHwError_u8(&SC_ErrorCode_u8);

	if(CCID_NO_ERROR != ErrorCode_u8)
	{
		return (ErrorCode_u8);
	}

// Check for size command
	if ((0xC1 == USB_CCID_data_pst->USB_data[CCID_OFFSET_XFR_BLOCK_DATA+1]) &&
		(0x01 == USB_CCID_data_pst->USB_data[CCID_OFFSET_XFR_BLOCK_DATA+2]) &&
		(0xFE == USB_CCID_data_pst->USB_data[CCID_OFFSET_XFR_BLOCK_DATA+3]) &&
		(0x3E == USB_CCID_data_pst->USB_data[CCID_OFFSET_XFR_BLOCK_DATA+4]))
	{
		USB_CCID_data_pst->USB_data[CCID_OFFSET_XFR_BLOCK_DATA+1] = 0xE1;
		USB_CCID_data_pst->USB_data[CCID_OFFSET_XFR_BLOCK_DATA+2] = 0x01;
		USB_CCID_data_pst->USB_data[CCID_OFFSET_XFR_BLOCK_DATA+3] = 0x20;
		USB_CCID_data_pst->USB_data[CCID_OFFSET_XFR_BLOCK_DATA+4] = 0xC0;
		CCID_AnswerBlockSize_u16 = 5;
		return (CCID_NO_ERROR);
	}


// This parameter did not define the answer size RB ???
	CCID_LevelParameter_u16 = USB_CCID_data_pst->USB_data[CCID_OFFSET_XFR_BLOCK_LEVEL_PARAMETER+1] * 256 +
							  USB_CCID_data_pst->USB_data[CCID_OFFSET_XFR_BLOCK_LEVEL_PARAMETER];

	ErrorCode_u8 = CCID_XfrBlock_u8 ( USB_CCID_data_pst, 
		                              &CCID_AnswerBlockSize_u16,
									   CCID_LevelParameter_u16);

	if(TRUE ==  CCID_CheckAbortRequest_u8 ())
	{
		return (CCID_ERROR_CMD_ABORTED);
	}

	if(CCID_NO_ERROR == ErrorCode_u8)
	{
		UsbMessageLength_u32 = CCID_AnswerBlockSize_u16;
		USB_CCID_data_pst->CCID_datalen                   = CCID_AnswerBlockSize_u16;
		USB_CCID_data_pst->USB_data[CCID_OFFSET_LENGTH]   = (u8)  UsbMessageLength_u32;
		USB_CCID_data_pst->USB_data[CCID_OFFSET_LENGTH+1] = (u8) (UsbMessageLength_u32 >> 8);
		USB_CCID_data_pst->USB_data[CCID_OFFSET_LENGTH+2] = 0x00;
		USB_CCID_data_pst->USB_data[CCID_OFFSET_LENGTH+3] = 0x00;
	}

	return (ErrorCode_u8);
}
/*******************************************************************************

  PC_to_RDR_GetParameters_u8

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

u8 PC_to_RDR_GetParameters_u8 (t_USB_CCID_data_st *USB_CCID_data_pst)
{
	u8 ErrorCode_u8 = CCID_NO_ERROR;

	if(0 != USB_CCID_data_pst->USB_data[CCID_OFFSET_SLOT])
	{
		return (CCID_ERROR_BAD_SLOT);
	}

	if (0 != USB_CCID_data_pst->CCID_datalen)
	{
		return (CCID_ERROR_BAD_LENTGH);
	}

// Return default parameter 

	return (ErrorCode_u8);
}
/*******************************************************************************

  PC_to_RDR_ResetParameters_u8

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

#define		CCID_PARAMETER_CLOCK_NOTSTOPPED					 	0x00
#define		CCID_PARAMETER_CLOCK_STOPLOW					    0x01
#define		CCID_PARAMETER_CLOCK_STOPHIGH					    0x02
#define		CCID_PARAMETER_CLOCK_STOPHIGHORLOW				0x03

#define		CCID_DEFAULT_PARAMETER_FIDI								0x11
#define		CCID_DEFAULT_PARAMETER_T01CONVCHECKSUM		0x00				// No value ??
#define		CCID_DEFAULT_PARAMETER_GUARDTIME					0x00
#define		CCID_DEFAULT_PARAMETER_WAITING_INTEGER		0x0A
#define		CCID_DEFAULT_PARAMETER_CLOCK_STOP					CCID_PARAMETER_CLOCK_STOPHIGHORLOW
#define		CCID_DEFAULT_PARAMETER_IFSC								0x20
#define		CCID_DEFAULT_PARAMETER_NAD								0x00

u8 PC_to_RDR_ResetParameters_u8 (t_USB_CCID_data_st *USB_CCID_data_pst)
{
	u8 ErrorCode_u8 = CCID_NO_ERROR;
	u8 SC_ErrorCode_u8;

	if(0 != USB_CCID_data_pst->USB_data[CCID_OFFSET_SLOT])
	{
		return (CCID_ERROR_BAD_SLOT);
	}

	if (0 != USB_CCID_data_pst->CCID_datalen)
	{
		return (CCID_ERROR_BAD_LENTGH);
	}

// Todo: Send default parameter
	USB_CCID_data_pst->USB_data[CCID_OFFSET_SET_PARAMS_DATA]   = CCID_DEFAULT_PARAMETER_FIDI;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_SET_PARAMS_DATA+1] = CCID_DEFAULT_PARAMETER_T01CONVCHECKSUM;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_SET_PARAMS_DATA+2] = CCID_DEFAULT_PARAMETER_GUARDTIME;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_SET_PARAMS_DATA+3] = CCID_DEFAULT_PARAMETER_WAITING_INTEGER;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_SET_PARAMS_DATA+4] = CCID_DEFAULT_PARAMETER_CLOCK_STOP;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_SET_PARAMS_DATA+5] = 0x00;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_SET_PARAMS_DATA+6] = 0x00;

//	ErrorCode_u8 = IFD_SetParameters(&USB_CCID_data_pst->USB_data[CCID_OFFSET_SET_PARAMS_DATA], 0x00);

	if(CCID_NO_ERROR != ErrorCode_u8)
	{
		return (ErrorCode_u8);
	}

	ErrorCode_u8 = CCID_GetHwError_u8(&SC_ErrorCode_u8);

	return (ErrorCode_u8);
}

/*******************************************************************************

  PC_to_RDR_SetParameters_u8

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

u8 PC_to_RDR_SetParameters_u8 (t_USB_CCID_data_st *USB_CCID_data_pst)
{
	u8 ErrorCode_u8;
	u8 SC_ErrorCode_u8;

	if(0 != USB_CCID_data_pst->USB_data[CCID_OFFSET_SLOT])
	{
		return (CCID_ERROR_BAD_SLOT);
	}

	if ((5 != USB_CCID_data_pst->CCID_datalen) && (7 != USB_CCID_data_pst->CCID_datalen))
	{
		return (CCID_ERROR_BAD_LENTGH);
	}

	ErrorCode_u8 = CCID_NO_ERROR;

	if(CCID_NO_ERROR != ErrorCode_u8)
	{
		return (ErrorCode_u8);
	}

// Answer of SetParameters	Test only for OpenPGG cards
	USB_CCID_data_pst->USB_data[CCID_OFFSET_MESSAGE_TYPE]    = RDR_TO_PC_PARAMETERS;

	USB_CCID_data_pst->USB_data[CCID_OFFSET_LENGTH]          = (unsigned char) 7;				// Protocol T=1
	USB_CCID_data_pst->USB_data[CCID_OFFSET_LENGTH+1]        = (unsigned char) (7>>8);
	USB_CCID_data_pst->USB_data[CCID_OFFSET_LENGTH+2]        = 0x00;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_LENGTH+3]        = 0x00;

	USB_CCID_data_pst->USB_data[CCID_OFFSET_STATUS] 		     = CCID_GetSlotStatus_u8 ();
	USB_CCID_data_pst->USB_data[CCID_OFFSET_ERROR] 		       = 0x00;

// Todo: Send default parameter
// Take the send parameter
/*
	USB_CCID_data_pst->USB_data[CCID_OFFSET_SET_PARAMS_DATA]   = CCID_DEFAULT_PARAMETER_FIDI;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_SET_PARAMS_DATA+1] = CCID_DEFAULT_PARAMETER_T01CONVCHECKSUM;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_SET_PARAMS_DATA+2] = CCID_DEFAULT_PARAMETER_GUARDTIME;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_SET_PARAMS_DATA+3] = CCID_DEFAULT_PARAMETER_WAITING_INTEGER;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_SET_PARAMS_DATA+4] = CCID_DEFAULT_PARAMETER_CLOCK_STOP;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_SET_PARAMS_DATA+5] = 0x00;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_SET_PARAMS_DATA+6] = 0x00;
*/

	ErrorCode_u8 = CCID_GetHwError_u8(&SC_ErrorCode_u8);

	return (ErrorCode_u8);

}
/*******************************************************************************

  PC_to_RDR_Escape_u8

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

u8 PC_to_RDR_Escape_u8 (t_USB_CCID_data_st *USB_CCID_data_pst)
{


	u8 ErrorCode_u8;
	u8 SC_ErrorCode_u8;

	if(0 != USB_CCID_data_pst->USB_data[CCID_OFFSET_SLOT])
	{
		return (CCID_ERROR_BAD_SLOT);
	}

	ErrorCode_u8 = CCID_GetHwError_u8(&SC_ErrorCode_u8);

	return (ErrorCode_u8);

}
/*******************************************************************************

  PC_to_RDR_IccClock_u8

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

u8 PC_to_RDR_IccClock_u8 (t_USB_CCID_data_st *USB_CCID_data_pst)
{

	u8 ErrorCode_u8 = CCID_NO_ERROR;

	return ErrorCode_u8;

}
/*******************************************************************************

  PC_to_RDR_Abort_u8

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

/******************************************************************************

  PC_to_RDR_Abort_u8

******************************************************************************/

u8 PC_to_RDR_Abort_u8 (t_USB_CCID_data_st *USB_CCID_data_pst)
{

	u8 ErrorCode_u8 = CCID_NO_ERROR;

	return (ErrorCode_u8);

}
/*******************************************************************************

  RDR_to_PC_DataBlock_v

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

void RDR_to_PC_DataBlock_v (t_USB_CCID_data_st *USB_CCID_data_pst,u8 ErrorCode_u8)
{
	USB_CCID_data_pst->USB_data[CCID_OFFSET_MESSAGE_TYPE] = RDR_TO_PC_DATA_BLOCK;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_STATUS] 	  = CCID_GetSlotStatus_u8 ();
	USB_CCID_data_pst->USB_data[CCID_OFFSET_ERROR]        = 0x00;

	if(CCID_NO_ERROR != ErrorCode_u8)
	{
		USB_CCID_data_pst->USB_data[CCID_OFFSET_LENGTH]        = 0x00;
		USB_CCID_data_pst->USB_data[CCID_OFFSET_LENGTH+1]      = 0x00;
		USB_CCID_data_pst->USB_data[CCID_OFFSET_LENGTH+2]      = 0x00;
		USB_CCID_data_pst->USB_data[CCID_OFFSET_LENGTH+3]      = 0x00;

		USB_CCID_data_pst->USB_data[CCID_OFFSET_STATUS]       += 0x40;
		USB_CCID_data_pst->USB_data[CCID_OFFSET_ERROR]         = ErrorCode_u8;
	}

	USB_CCID_data_pst->USB_data[CCID_OFFSET_CHAIN_PARAMETER] = 0x00;
}
/*******************************************************************************

  RDR_to_PC_SlotStatus_v

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

void RDR_to_PC_SlotStatus_v (t_USB_CCID_data_st *USB_CCID_data_pst,u8 ErrorCode_u8)
{

	USB_CCID_data_pst->USB_data[CCID_OFFSET_MESSAGE_TYPE] = RDR_TO_PC_SLOT_STATUS;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_STATUS] 		  = CCID_GetSlotStatus_u8 ();

	USB_CCID_data_pst->USB_data[CCID_OFFSET_LENGTH]       = 0x00;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_LENGTH+1]     = 0x00;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_LENGTH+2]     = 0x00;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_LENGTH+3]     = 0x00;

	if(CCID_NO_ERROR == ErrorCode_u8)
	{
		USB_CCID_data_pst->USB_data[CCID_OFFSET_ERROR]		 = 0x00;
	}
	else
	{
		USB_CCID_data_pst->USB_data[CCID_OFFSET_STATUS]    += 0x40;
		USB_CCID_data_pst->USB_data[CCID_OFFSET_ERROR]      = ErrorCode_u8;
	}

	USB_CCID_data_pst->USB_data[CCID_OFFSET_CLOCK_STATUS] = CCID_GetClockStatus_u8 ();

}
/*******************************************************************************

  RDR_to_PC_SlotStatus_CardStopped_v

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

void RDR_to_PC_SlotStatus_CardStopped_v (t_USB_CCID_data_st *USB_CCID_data_pst)
{

	RDR_to_PC_SlotStatus_v (USB_CCID_data_pst,CCID_NO_ERROR);

	USB_CCID_data_pst->USB_data[CCID_OFFSET_SLOT_STATUS_STATUS] 		  = CCID_SLOT_STATUS_PRESENT_INACTIVE;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_SLOT_STATUS_CLOCK_STATUS] = CCID_SLOT_STATUS_CLOCK_STOPPED_LOW;
}
/*******************************************************************************

  RDR_to_PC_Parameters_v

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

void RDR_to_PC_Parameters_v (t_USB_CCID_data_st *USB_CCID_data_pst)
{
  USB_CCID_data_pst->USB_data[CCID_OFFSET_MESSAGE_TYPE] = RDR_TO_PC_PARAMETERS;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_LENGTH]       = 0x07;									// Only T1
	USB_CCID_data_pst->USB_data[CCID_OFFSET_LENGTH+1] 	  = 0x00;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_LENGTH+2] 	  = 0x00;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_LENGTH+3] 	  = 0x00;

	USB_CCID_data_pst->USB_data[CCID_OFFSET_STATUS] 	  = CCID_GetSlotStatus_u8 ();
	USB_CCID_data_pst->USB_data[CCID_OFFSET_PROTOCOL_NUM] = 0x01;


// Todo: Send default parameter
// Take the send parameter...
/*
	USB_CCID_data_pst->USB_data[CCID_OFFSET_SET_PARAMS_DATA]   = 0x00;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_SET_PARAMS_DATA+1] = 0x00;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_SET_PARAMS_DATA+2] = 0x00;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_SET_PARAMS_DATA+3] = 0x00;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_SET_PARAMS_DATA+4] = 0x00;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_SET_PARAMS_DATA+5] = 0x00;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_SET_PARAMS_DATA+6] = 0x00;
*/
/*
	USB_CCID_data_pst->USB_data[CCID_OFFSET_SET_PARAMS_DATA]   = CCID_DEFAULT_PARAMETER_FIDI;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_SET_PARAMS_DATA+1] = CCID_DEFAULT_PARAMETER_T01CONVCHECKSUM;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_SET_PARAMS_DATA+2] = CCID_DEFAULT_PARAMETER_GUARDTIME;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_SET_PARAMS_DATA+3] = CCID_DEFAULT_PARAMETER_WAITING_INTEGER;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_SET_PARAMS_DATA+4] = CCID_DEFAULT_PARAMETER_CLOCK_STOP;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_SET_PARAMS_DATA+5] = 0x00;
	USB_CCID_data_pst->USB_data[CCID_OFFSET_SET_PARAMS_DATA+6] = 0x00;
*/
}
/*******************************************************************************

  RDR_to_PC_Escape_v

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

void RDR_to_PC_Escape_v (t_USB_CCID_data_st *USB_CCID_data_pst)
{
/*
	UsbMessageBuffer[OFFSET_BMESSAGETYPE] = RDR_TO_PC_ESCAPE;
	UsbMessageBuffer[OFFSET_BSTATUS] 			= CRD_GetSlotStatus();
	UsbMessageBuffer[OFFSET_BERROR] 			= 0x00;

	if(ErrorCode_u8 != SLOT_NO_ERROR)
	{
		UsbMessageBuffer[OFFSET_BSTATUS]    += 0x40;
		UsbMessageBuffer[OFFSET_DWLENGTH]    = 0x00;
		UsbMessageBuffer[OFFSET_DWLENGTH+1]  = 0x00;
		UsbMessageBuffer[OFFSET_DWLENGTH+2]  = 0x00;
		UsbMessageBuffer[OFFSET_DWLENGTH+3]  = 0x00;
		UsbMessageBuffer[OFFSET_BERROR]      = ErrorCode_u8;
	}

	UsbMessageBuffer[OFFSET_BRFU] = 0x00;
*/
}

/*******************************************************************************

  RDR_to_PC_CmdNotSupported_v

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

void RDR_to_PC_CmdNotSupported_v (t_USB_CCID_data_st *USB_CCID_data_pst)
{
/*
	//UsbMessageBuffer[OFFSET_BMESSAGETYPE] = 0x00;
	UsbMessageBuffer[OFFSET_DWLENGTH] 				= 0x00;
	UsbMessageBuffer[OFFSET_DWLENGTH+1] 			= 0x00;
	UsbMessageBuffer[OFFSET_DWLENGTH+2] 			= 0x00;
	UsbMessageBuffer[OFFSET_DWLENGTH+3]		 		= 0x00;
	UsbMessageBuffer[OFFSET_BSTATUS] 					= 0x40 + CRD_GetSlotStatus();
	UsbMessageBuffer[OFFSET_BERROR] 					= 0x00;
	UsbMessageBuffer[OFFSET_BCHAINPARAMETER] 	= 0x00;
*/
}
/*******************************************************************************

  CDR_to_USB_NotifySlotChange_v

  ISR

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

void CDR_to_USB_NotifySlotChange_v (t_USB_CCID_data_st *USB_CCID_data_pst)
{
/*
	UsbIntMessageBuffer[OFFSET_INT_BMESSAGETYPE] = RDR_TO_PC_NOTIFYSLOTCHANGE;

	if( CRD_GetSlotStatus() == CRD_NOTPRESENT )
	{
		UsbIntMessageBuffer[OFFSET_INT_BMSLOTICCSTATE] = 0x02;
	}
	else
	{
		UsbIntMessageBuffer[OFFSET_INT_BMSLOTICCSTATE] = 0x03;
	}
*/
}
/*******************************************************************************

  CRD_to_USB_HardwareError

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

/******************************************************************************

  CRD_to_USB_HardwareError

******************************************************************************/

u8 CRD_to_USB_HardwareError (t_USB_CCID_data_st *USB_CCID_data_pst)
{
/*
	unsigned char HwErrorCode = SLOT_NO_ERROR;
//	unsigned char ErrorCode_u8;

	UsbIntMessageBuffer[OFFSET_INT_BMESSAGETYPE] 				= RDR_TO_PC_HARDWAREERROR;
	UsbIntMessageBuffer[OFFSET_INT_BSLOT] 							= 0x00;
	UsbIntMessageBuffer[OFFSET_INT_BSEQ] 								= UsbMessageBuffer[OFFSET_BSEQ];
//	ErrorCode_u8 																					= CCID_GetHwError_u8(&HwErrorCode);
	UsbIntMessageBuffer[OFFSET_INT_BHARDWAREERRORCODE] 	= HwErrorCode;
*/
	return (0);
}

/*******************************************************************************

  USB_to_CRD_DispatchUSBMessage_v

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

int USB_CCID_Simulate_Answer (t_USB_CCID_data_st *USB_CCID_data_pst);

extern volatile avr32_mci_t *mci;

void USB_to_CRD_DispatchUSBMessage_v (t_USB_CCID_data_st *USB_CCID_data_pst)
{
	u8 ErrorCode_u8 = CCID_NO_ERROR;
	u8 n;

	USB_CCID_data_pst->CCID_datalen = USB_CCID_data_pst->USB_data[CCID_OFFSET_LENGTH+1] * 256 +
									   USB_CCID_data_pst->USB_data[CCID_OFFSET_LENGTH];

	n = USB_CCID_data_pst->USB_data[CCID_OFFSET_MESSAGE_TYPE];
	  if (mci != &AVR32_MCI)
	  {
		  mci = &AVR32_MCI;

	  }
	switch(USB_CCID_data_pst->USB_data[CCID_OFFSET_MESSAGE_TYPE])
	{
		case PC_TO_RDR_ICCPOWERON:
//			portDBG_TRACE("ICCPOWERON Start %d",xTaskGetTickCount());

			ErrorCode_u8	= PC_to_RDR_IccPowerOn_u8 (USB_CCID_data_pst);
			if (CCID_NO_ERROR == ErrorCode_u8)
			{
				DelayMs (100);	// We wait 100 ms for answering
				ErrorCode_u8 = CCID_SetATRData_u8 (USB_CCID_data_pst);														// Create ATR output Message
			}

#ifdef SIMULATE_USB_CCID_DISPATCH
			USB_CCID_Simulate_Answer (USB_CCID_data_pst);
#endif


			RDR_to_PC_DataBlock_u8 (USB_CCID_data_pst);
//			portDBG_TRACE("ICCPOWERON End   %d",xTaskGetTickCount());

			break;

		case PC_TO_RDR_ICCPOWEROFF:
			ErrorCode_u8 = PC_to_RDR_IccPowerOff_u8 (USB_CCID_data_pst);
//				RDR_to_PC_SlotStatus(ErrorCode_u8);
			RDR_to_PC_SlotStatus_CardStopped_v (USB_CCID_data_pst);		 	// simulate power off
			break;

		case PC_TO_RDR_GETSLOTSTATUS:
			ErrorCode_u8 = PC_to_RDR_GetSlotStatus_u8 (USB_CCID_data_pst);
			RDR_to_PC_SlotStatus_v (USB_CCID_data_pst,ErrorCode_u8);
			break;

		case PC_TO_RDR_XFRBLOCK:
if (mci != &AVR32_MCI)
{
  mci = &AVR32_MCI;

}
			ErrorCode_u8 = PC_to_RDR_XfrBlock_u8 (USB_CCID_data_pst);
if (mci != &AVR32_MCI)
{
  mci = &AVR32_MCI;

}
			RDR_to_PC_DataBlock_v (USB_CCID_data_pst,ErrorCode_u8);
#ifdef SIMULATE_USB_CCID_DISPATCH
			USB_CCID_Simulate_Answer (USB_CCID_data_pst);
#endif
			break;

		case PC_TO_RDR_GETPARAMETERS:
			ErrorCode_u8 = PC_to_RDR_GetParameters_u8 (USB_CCID_data_pst);
			RDR_to_PC_Parameters_v (USB_CCID_data_pst);
			break;

		case PC_TO_RDR_RESETPARAMETERS:
			ErrorCode_u8 = PC_to_RDR_ResetParameters_u8 (USB_CCID_data_pst);
			RDR_to_PC_Parameters_v (USB_CCID_data_pst);
			break;

		case PC_TO_RDR_SETPARAMETERS:
			ErrorCode_u8 = PC_to_RDR_SetParameters_u8 (USB_CCID_data_pst);
//USB_CCID_Simulate_Answer (USB_CCID_data_pst);
			RDR_to_PC_Parameters_v (USB_CCID_data_pst);
			break;

		case PC_TO_RDR_ESCAPE:
			ErrorCode_u8 = PC_to_RDR_Escape_u8 (USB_CCID_data_pst);
			RDR_to_PC_Escape_v (USB_CCID_data_pst);
			break;

		case PC_TO_RDR_ICCCLOCK:
			ErrorCode_u8 = PC_to_RDR_IccClock_u8 (USB_CCID_data_pst);
			RDR_to_PC_SlotStatus_v (USB_CCID_data_pst,ErrorCode_u8);
			break;

		case PC_TO_RDR_ABORT:
			ErrorCode_u8 = PC_to_RDR_Abort_u8 (USB_CCID_data_pst);
			RDR_to_PC_SlotStatus_v (USB_CCID_data_pst,ErrorCode_u8);
			break;

		case PC_TO_RDR_T0APDU:
		case PC_TO_RDR_SECURE:
		case PC_TO_RDR_MECHANICAL:
		case PC_TO_RDR_SET_DATA_RATE_AND_CLOCK_FREQUENCY:
		default:
			RDR_to_PC_CmdNotSupported_v (USB_CCID_data_pst);
			break;
	}
	  if (mci != &AVR32_MCI)
	  {
		  mci = &AVR32_MCI;

	  }
}

/*******************************************************************************

  CCID_IntMessage

  Transmit the CCID message by the Interrupt Endpoint 1.
  this message was in UsbIntMessageBuffer.

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

unsigned char	SlotStabilizationDelay = 0; // RB INITIALSLOTSTABILIZATIONDELAY;

void CCID_IntMessage(void)
{
	USB_CCID_send_INT_Message ();
}

/*******************************************************************************

  CcidClassRequestAbort

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

void CcidClassRequestAbort(void)
{
/*
	if( (sUSB_vSetup.USBbmRequestType == 0x21) &&
			(sUSB_vSetup.USBwValue0 == 0x00) && (sUSB_vSetup.USBwIndex1 == 0x00) &&
			(sUSB_vSetup.USBwIndex0 == 0x00) && (sUSB_vSetup.USBwLength1 == 0x00) &&
			(sUSB_vSetup.USBwLength0 == 0x00) )
	{
		Set_bAbortRequestFlag;
		AbortSequenceNumber = sUSB_vSetup.USBwValue1;
	}
*/
}
/*******************************************************************************

  CcidClassRequestGetClockFrequencies

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

void CcidClassRequestGetClockFrequencies(void)
{
}

/*******************************************************************************

  CcidClassRequestGetDataRates

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

void CcidClassRequestGetDataRates(void)
{
}

/*******************************************************************************

  CRD_to_USB_SendCardDetect

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

void CRD_to_USB_SendCardDetect (void)
{
	CCID_IntMessage ();
}

/*******************************************************************************

  USB_to_CRD_CheckUsbInput

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

void USB_to_CRD_CheckUsbInput (void)
{
/*
  CCID_DispatchMessage ();

  if ((TRANSMIT_HEADER    == BulkStatus)	||
			(TRANSMIT_FINISHED  == BulkStatus))
  {
	  CCID_BulkInMessage();									 // something to send ?
	}
*/
}





