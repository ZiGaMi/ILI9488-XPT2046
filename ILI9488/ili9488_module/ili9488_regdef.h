//////////////////////////////////////////////////////////////
// 
//	project:		ILI9488 EVALUATION
//	date:			07.07.2020
//	
//	author:			Ziga Miklosic
//
//////////////////////////////////////////////////////////////

#ifndef _ILI9488_REGDEF_H_
#define _ILI9488_REGDEF_H_

//////////////////////////////////////////////////////////////
//	INCLUDES
//////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////
//	DEFINITIONS
//////////////////////////////////////////////////////////////

// **********************************************************
// 		COMMAND LIST
// **********************************************************
typedef enum
{
	// NOP
	eILI9488_NOP_CMD							= 0x00U,

	// Soft Reset
	eILI9488_SOFTRST_CMD						= 0x01U,

	// Read display identification information
	eILI9488_READ_RDII_CMD						= 0x04U,

	// Read number of errors on SPI interface
	eILI9488_READ_SPIERR_CMD					= 0x05U,

	// Read display status
	eILI9488_READ_STATUS_CMD					= 0x09U,

	// Read display power status
	eILI9488_READ_PWR_STATUS_CMD				= 0x0AU,

	// Read/Set display MADCTL =  Memory Data Access Control ,
	eILI9488_READ_MADCTL_CMD					= 0x0BU,
	eILI9488_SET_MADCTL_CMD						= 0x36U,

	// Read/Set pixel format
	eILI9488_READ_PF_CMD						= 0x0CU,
	eILI9488_SET_PF_CMD							= 0x3AU,

	// Read display image mode
	eILI9488_READ_IMMODE_CMD					= 0x0DU,

	// Read display signal mode
	eILI9488_READ_DSIG_MODE_CMD					= 0x0EU,

	// Sleep in
	eILI9488_SLEEP_IN_CMD						= 0x10U,

	// Sleep out
	eILI9488_SLEEP_OUT_CMD						= 0x11U,

	// Partial mode on
	eILI9488_PARTIAL_MODE_CMD					= 0x12U,

	// Normal mode on
	eILI9488_NORMAL_MODE_CMD					= 0x13U,

	// Display inversion on/off
	eILI9488_DISINV_OFF_CMD						= 0x20U,
	eILI9488_DISINV_ON_CMD						= 0x21U,

	// All pixels on/off
	eILI9488_ALLPIX_OFF_CMD						= 0x22U,
	eILI9488_ALLPIX_ON_CMD						= 0x23U,

	// Display on/off
	eILI9488_DISPLAY_OFF_CMD					= 0x28U,
	eILI9488_DISPLAY_ON_CMD						= 0x29U,

	// Set column address
	eILI9488_SET_COL_ADDR_CMD					= 0x2AU,

	// Set page address
	eILI9488_SET_PAGE_ADDR_CMD					= 0x2BU,

	// Memory write/read
	eILI9488_WRITE_MEM_CMD						= 0x2CU,
	eILI9488_READ_MEM_CMD						= 0x2EU,

	// Memory continuous write/read
	eILI9488_WRITE_MEM_CONT_CMD					= 0x3CU,
	eILI9488_READ_MEM_CONT_CMD					= 0x3EU,

	// Idle mode on/off
	eILI9488_IDLE_MODE_OFF_CMD					= 0x38U,
	eILI9488_IDLE_MODE_ON_CMD					= 0x39U,

	// Write/Read display brightness
	eILI9488_READ_BRIGHT_CMD					= 0x52U,
	eILI9488_WRITE_BRIGHT_CMD					= 0x51U,

	// Read ID1/ID2/ID3
	eILI9488_READ_ID1_CMD						= 0xDAU,
	eILI9488_READ_ID2_CMD						= 0xDBU,
	eILI9488_READ_ID3_CMD						= 0xDCU,

	// Positive/negative gamma control
	eILI9488_SET_POS_GAMMA_CTRL_CMD				= 0xE0U,
	eILI9488_SET_NEG_GAMMA_CTRL_CMD				= 0xE1U,

	// Power control 1, 2, 3, 4, 5
	eILI9488_SET_POWER_CTRL_1_CMD				= 0xC0U,
	eILI9488_SET_POWER_CTRL_2_CMD				= 0xC1U,
	eILI9488_SET_POWER_CTRL_3_CMD				= 0xC2U,
	eILI9488_SET_POWER_CTRL_4_CMD				= 0xC3U,
	eILI9488_SET_POWER_CTRL_5_CMD				= 0xC4U,

	// Interface mode control
	eILI9488_SET_IF_MODE_CMD					= 0xB0U,

	// Frame rate control
	eILI9488_SET_FR_NOR_MODE_CMD				= 0xB1U,
	eILI9488_SET_FR_8_COLOR_CMD					= 0xB2U,
	eILI9488_SET_FR_PAR_MODE_CMD				= 0xB3U,

	// Display inversion control
	eILI9488_SET_INV_CTRL_CMD					= 0xB4U,

	// Display function control
	eILI9488_SET_FUNC_CTRL_CMD					= 0xB6U,

	// Display image function
	eILI9488_SET_IMG_FUNC_CMD					= 0xE9U,

} ili9488_cmd_t;


// Display size
#define ILI9488_DISPLAY_SIZE_COLUMN				( 320 )
#define ILI9488_DISPLAY_SIZE_PAGE				( 480 )



//////////////////////////////////////////////////////////////
//	VARIABLES
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// FUNCTIONS
//////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////
// END OF FILE
//////////////////////////////////////////////////////////////

#endif // _ILI9488_REGDEF_H_
