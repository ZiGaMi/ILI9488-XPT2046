//////////////////////////////////////////////////////////////
// 
//	project:		ILI9488 EVALUATION
//	date:			07.07.2020
//	
//	author:			Ziga Miklosic
//
//////////////////////////////////////////////////////////////

#ifndef _ILI9488_H_
#define _ILI9488_H_

//////////////////////////////////////////////////////////////
//	INCLUDES
//////////////////////////////////////////////////////////////
#include <project_config.h>
#include "stdint.h"
#include "stdbool.h"

// Definiton of float32_t

//////////////////////////////////////////////////////////////
//	DEFINITIONS
//////////////////////////////////////////////////////////////

// General status
typedef enum
{
	eILI9488_OK 	= 0x00,
	eILI9488_ERROR 	= 0x01,
} ili9488_status_t;


// Colors
typedef enum
{
	eILI9488_COLOR_BLACK = 0,
	eILI9488_COLOR_BLUE,
	eILI9488_COLOR_GREEN,
	eILI9488_COLOR_TURQUOISE,
	eILI9488_COLOR_RED,
	eILI9488_COLOR_PURPLE,
	eILI9488_COLOR_YELLOW,
	eILI9488_COLOR_WHITE,
	eILI9488_COLOR_LIGHT_GRAY,
	eILI9488_COLOR_GRAY,

} ili9488_color_t;

// Font options
typedef enum
{
	eILI9488_FONT_8 = 0,
	eILI9488_FONT_12,
	eILI9488_FONT_16,
	eILI9488_FONT_20,
	eILI9488_FONT_24,

	eILI9488_FONT_NUM_OF,
} ili9488_font_opt_t;

// Rectangle attributes
typedef struct
{
	// Position of rectangle
	struct
	{
		uint16_t start_page;
		uint16_t start_col;
		uint16_t page_size;
		uint16_t col_size;
	} position;

	// Rounded corners
	struct
	{
		bool	enable;
		uint8_t	radius;
	} rounded;

	// Filled rectangle
	struct
	{
		bool			enable;
		ili9488_color_t color;
	} fill;

	// Rectangle with border
	struct
	{
		bool			enable;
		ili9488_color_t color;
		uint8_t			width;
	} border;

} ili9488_rect_attr_t;

// Circle attributes
typedef struct
{
	// Position & size of circle
	struct
	{
		uint16_t start_page;
		uint16_t start_col;
		uint8_t radius;
	} position;

	// Filled circle
	struct
	{
		bool			enable;
		ili9488_color_t color;
	} fill;

	// Border
	struct
	{
		bool 			enable;
		ili9488_color_t	color;
		uint8_t			width;
	} border;

} ili9488_circ_attr_t;


//////////////////////////////////////////////////////////////
//	VARIABLES
//////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////
// FUNCTIONS PROTOTYPES
//////////////////////////////////////////////////////////////

// General functions
ili9488_status_t 	ili9488_init			(void);
bool				ili9488_is_init			(void);
ili9488_status_t 	ili9488_set_backlight	(const float32_t brightness);

// Graphics functions
ili9488_status_t 	ili9488_set_background	(const ili9488_color_t color);
ili9488_status_t	ili9488_draw_rectangle	(const ili9488_rect_attr_t * const p_rectanegle_attr);
ili9488_status_t	ili9488_draw_circle		(const ili9488_circ_attr_t * const p_circ_attr);

// Text functions
ili9488_status_t 	ili9488_set_string_pen	(const ili9488_color_t fg_color, const ili9488_color_t bg_color, const ili9488_font_opt_t font_opt);
ili9488_status_t 	ili9488_set_string		(const char* str, const uint16_t page, const uint16_t col);
ili9488_status_t	ili9488_set_cursor		(const uint16_t page, const uint16_t col);
ili9488_status_t	ili9488_printf			(const char *args, ...);


//////////////////////////////////////////////////////////////
// END OF FILE
//////////////////////////////////////////////////////////////

#endif // _ILI9488_H_
