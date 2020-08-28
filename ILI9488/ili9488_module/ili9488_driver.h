//////////////////////////////////////////////////////////////
// 
//	project:		ILI9488 EVALUATION
//	date:			07.07.2020
//	
//	author:			Ziga Miklosic
//
//////////////////////////////////////////////////////////////

#ifndef _ILI9488_DRIVER_H_
#define _ILI9488_DRIVER_H_

//////////////////////////////////////////////////////////////
//	INCLUDES
//////////////////////////////////////////////////////////////
#include "ili9488_low_if.h"
#include "ili9488.h"
#include "ili9488_font.h"


//////////////////////////////////////////////////////////////
//	DEFINITIONS
//////////////////////////////////////////////////////////////

// Display on/off
typedef enum
{
	eILI9488_DISPLAY_OFF = 0,
	eILI9488_DISPLAY_ON,
} ili9488_display_t;

// Display sleep
typedef enum
{
	eILI9488_SLEEP_OFF = 0,
	eILI9488_SLEEP_ON,
} ili9488_sleep_t;

// Display idle
typedef enum
{
	eILI9488_IDLE_OFF = 0,
	eILI9488_IDLE_ON,
} ili9488_idle_t;

// Display mode
typedef enum
{
	eILI9488_MODE_NORMAL = 0,
	eILI9488_MODE_PARTIAL,
} ili9488_mode_t;

// Display inversion
typedef enum
{
	eILI9488_DISPLAY_INVERSION_OFF = 0,
	eILI9488_DISPLAY_INVERSION_ON,
} ili9488_inversion_t;

// All pixels
typedef enum
{
	eILI9488_ALL_PIXELS_OFF = 0,
	eILI9488_ALL_PIXELS_ON
} ili9488_all_pixels_t;

// Memory access control
typedef enum
{
	eILI9488_MADCTL_NORMAL = 0,
	eILI9488_MADCTL_Y_MIRROR,
	eILI9488_MADCTL_X_MIRROR,
	eILI9488_MADCTL_XY_MIRROR,
	eILI9488_MADCTL_XY_EXCHANGE,
	eILI9488_MADCTL_XY_EXCHANGE_Y_MIRROR,
	eILI9488_MADCTL_XY_EXCHANGE_X_MIRROR,
	eILI9488_MADCTL_XY_EXCHANGE_XY_MIRROR,
} ili9488_madctl_t;

// Pixel format
typedef enum
{
	eILI9488_PIXEL_FORMAT_3_BIT = 0,
	eILI9488_PIXEL_FORMAT_16_BIT,
	eILI9488_PIXEL_FORMAT_18_BIT,
	eILI9488_PIXEL_FORMAT_24_BIT,
} ili9488_pixel_format_t;

// Display orientation
typedef enum
{
	eILI9488_ORIENTATION_LANDSCAPE = 0,
	eILI9488_ORIENTATION_LANDSCAPE_FLIP,
} ili9488_orientation_t;



//////////////////////////////////////////////////////////////
//	VARIABLES
//////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////
// FUNCTIONS PROTOTYPES
//////////////////////////////////////////////////////////////
ili9488_status_t ili9488_driver_init						(void);

ili9488_status_t ili9488_driver_soft_reset					(void);
ili9488_status_t ili9488_driver_set_display_on_off			(const ili9488_display_t on_off);
ili9488_status_t ili9488_driver_set_sleep_on_off			(const ili9488_sleep_t on_off);
ili9488_status_t ili9488_driver_set_idle_on_off				(const ili9488_idle_t on_off);
ili9488_status_t ili9488_driver_set_mode					(const ili9488_mode_t mode);
ili9488_status_t ili9488_driver_set_display_inversion		(const ili9488_inversion_t inv);
ili9488_status_t ili9488_driver_set_all_pixels				(const ili9488_all_pixels_t on_off);

ili9488_status_t ili9488_driver_set_brightness_control		(const bool brightness_en, const bool dimming_en, const bool ctrl_en);
ili9488_status_t ili9488_driver_read_brightness_control		(bool * const p_brightness_en, bool * const p_dimming_en, bool * const p_ctrl_en);

ili9488_status_t ili9488_driver_set_orientation				(const ili9488_orientation_t orientation);
ili9488_status_t ili9488_driver_get_orientation				(ili9488_orientation_t * const p_orientation);

ili9488_status_t ili9488_driver_set_pixel_format			(const ili9488_pixel_format_t format);
ili9488_status_t ili9488_driver_read_pixel_format			(ili9488_pixel_format_t * const p_format);

ili9488_status_t ili9488_driver_set_cursor					(const uint16_t col_s, const uint16_t col_e, const uint16_t page_s, const uint16_t page_e);

ili9488_status_t ili9488_driver_write_memory				(const uint8_t * const p_mem, const uint32_t size);
ili9488_status_t ili9488_driver_read_memory					(uint8_t * const p_mem, const uint32_t size);

// Graphics functions
ili9488_status_t ili9488_driver_set_pixel					(const uint16_t page, const uint16_t col, const ili9488_color_t color);
ili9488_status_t ili9488_driver_fill_rectangle				(const uint16_t page, const uint16_t col, const uint16_t page_size, const uint16_t col_size, const ili9488_color_t color);
ili9488_status_t ili9488_driver_fill_circle					(const uint16_t page, const uint16_t col, const uint16_t radius, const ili9488_color_t color);
ili9488_status_t ili9488_driver_set_circle					(const uint16_t page, const uint16_t col, const uint16_t radius, const ili9488_color_t color);
ili9488_status_t ili9488_driver_set_char					(const uint8_t ch, const uint16_t page, const uint16_t col, const ili9488_color_t fg_color, const ili9488_color_t  bg_color, const ili9488_font_opt_t font_opt);
ili9488_status_t ili9488_driver_set_string					(const char *str, const uint16_t page, const uint16_t col, const ili9488_color_t fg_color, const ili9488_color_t  bg_color, const ili9488_font_opt_t font_opt);


//////////////////////////////////////////////////////////////
// END OF FILE
//////////////////////////////////////////////////////////////

#endif // _ILI9488_DRIVER_H_
