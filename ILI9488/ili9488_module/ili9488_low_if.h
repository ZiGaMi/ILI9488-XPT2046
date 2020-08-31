//////////////////////////////////////////////////////////////
// 
//	project:		ILI9488 EVALUATION
//	date:			07.07.2020
//	
//	author:			Ziga Miklosic
//
//////////////////////////////////////////////////////////////

#ifndef _ILI9488_LOW_IF_H_
#define _ILI9488_LOW_IF_H_

//////////////////////////////////////////////////////////////
//	INCLUDES
//////////////////////////////////////////////////////////////
#include "stdbool.h"
#include "stm32f7xx_hal.h"

#include "ili9488.h"
#include "ili9488_regdef.h"


//////////////////////////////////////////////////////////////
//	DEFINITIONS
//////////////////////////////////////////////////////////////

// Reset line
typedef enum
{
	eILI9488_RESET_OFF = 0,
	eILI9488_RESET_ON,
} ili9488_reset_t;


// LED brightness
typedef enum
{
	eILI9488_LED_OFF = 0,
	eILI9488_LED_8_PER,
	eILI9488_LED_16_PER,
	eILI9488_LED_25_PER,
	eILI9488_LED_36_PER,
	eILI9488_LED_49_PER,
	eILI9488_LED_64_PER,
	eILI9488_LED_81_PER,
	eILI9488_LED_100_PER,
} ili9488_led_t;

// RGB color code
typedef struct
{
	uint8_t R;
	uint8_t G;
	uint8_t B;
} ili9488_rgb_t;


//////////////////////////////////////////////////////////////
//	VARIABLES
//////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////
// FUNCTIONS PROTOTYPES
//////////////////////////////////////////////////////////////
ili9488_status_t 	ili9488_low_if_init					(void);
ili9488_status_t 	ili9488_low_if_write_register		(const ili9488_cmd_t cmd, const uint8_t * const tx_data, const uint32_t size);
ili9488_status_t 	ili9488_low_if_read_register		(const ili9488_cmd_t cmd, uint8_t * const rx_data, const uint32_t size);
ili9488_status_t 	ili9488_low_if_write_rgb_to_gram 	(const ili9488_rgb_t * const p_rgb, const uint32_t size);
void 				ili9488_low_if_set_reset			(const ili9488_reset_t rst);
ili9488_status_t	ili9488_low_if_set_led				(const float32_t brigthness);


//////////////////////////////////////////////////////////////
// END OF FILE
//////////////////////////////////////////////////////////////

#endif // _ILI9488_LOW_IF_H_
