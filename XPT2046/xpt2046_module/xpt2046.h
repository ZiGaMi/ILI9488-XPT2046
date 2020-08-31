//////////////////////////////////////////////////////////////
// 
//	project:		ILI9488 EVALUATION
//	date:			07.07.2020
//	
//	author:			Ziga Miklosic
//
//////////////////////////////////////////////////////////////

#ifndef _XPT2046_H_
#define _XPT2046_H_

//////////////////////////////////////////////////////////////
//	INCLUDES
//////////////////////////////////////////////////////////////
#include "stdint.h"
#include "stdbool.h"


//////////////////////////////////////////////////////////////
//	DEFINITIONS
//////////////////////////////////////////////////////////////

// General status
typedef enum
{
	eXPT2046_OK = 0,
	eXPT2046_ERROR,
	eXPT2046_CAL_IN_PROGRESS,
} xpt2046_status_t;

//////////////////////////////////////////////////////////////
//	VARIABLES
//////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////
// FUNCTIONS PROTOTYPES
//////////////////////////////////////////////////////////////
xpt2046_status_t 	xpt2046_init					(void);
void 				xpt2046_hndl					(void);
xpt2046_status_t 	xpt2046_get_touch				(uint16_t * const p_page, uint16_t * const p_col, uint16_t * const p_force, bool * const p_pressed);
xpt2046_status_t 	xpt2046_start_calibration		(void);
bool				xpt2046_is_calibrated			(void);
void				xpt2046_set_cal_factors			(const int32_t * const p_factors);
void				xpt2046_get_cal_factors			(const int32_t * p_factors);

//////////////////////////////////////////////////////////////
// END OF FILE
//////////////////////////////////////////////////////////////

#endif // _XPT2046_H_
