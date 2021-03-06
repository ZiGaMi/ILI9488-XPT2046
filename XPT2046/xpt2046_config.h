//////////////////////////////////////////////////////////////
//
//	project:		ILI9488 EVALUATION
//	date:			10.07.2020
//
//	author:			Ziga Miklosic
//
//////////////////////////////////////////////////////////////

#ifndef _XPT2046_CONFIG_H_
#define _XPT2046_CONFIG_H_

//////////////////////////////////////////////////////////////
//	INCLUDES
//////////////////////////////////////////////////////////////
#include "stdint.h"
#include "stm32f7xx_hal.h"

// Debug communication port
#include "com_dbg.h"

// Graphics
#include "ili9488.h"


//////////////////////////////////////////////////////////////
//	DEFINITIONS
//////////////////////////////////////////////////////////////

// Enable debug mode
#define XPT2046_DEBUG_EN				( 1 )



// **********************************************************
// 	SPI INTERFACE
// **********************************************************
#define XPT2046_SPI						( SPI3 )
#define XPT2046_SPI_BAUDRATE_PSC		( SPI_BAUDRATEPRESCALER_32 )	// NOTE: Max. SPI clock is 2 MHz
#define XPT2046_SPI_TIMEOUT_MS			( 10 )	// [ms]
#define XPT2046_SPI_EN_CLK()			__HAL_RCC_SPI3_CLK_ENABLE()



// **********************************************************
// 	GPIO PINS
// **********************************************************

// SPI pins
#define XPT2046_SCK__PORT				( GPIOC )
#define XPT2046_SCK__PIN				( GPIO_PIN_10 )
#define XPT2046_SCK__PULL				( GPIO_NOPULL )
#define XPT2046_SCK_CLK_EN()			__HAL_RCC_GPIOC_CLK_ENABLE()

#define XPT2046_MISO__PORT				( GPIOC )
#define XPT2046_MISO__PIN				( GPIO_PIN_11 )
#define XPT2046_MISO__PULL				( GPIO_NOPULL )
#define XPT2046_MISO_CLK_EN()			__HAL_RCC_GPIOC_CLK_ENABLE()

#define XPT2046_MOSI__PORT				( GPIOC )
#define XPT2046_MOSI__PIN				( GPIO_PIN_12 )
#define XPT2046_MOSI__PULL				( GPIO_NOPULL )
#define XPT2046_MOSI_CLK_EN()			__HAL_RCC_GPIOC_CLK_ENABLE()

#define XPT2046_CS__PORT				( GPIOC )
#define XPT2046_CS__PIN					( GPIO_PIN_9 )
#define XPT2046_CS__PULL				( GPIO_NOPULL )
#define XPT2046_CS_CLK_EN()				__HAL_RCC_GPIOC_CLK_ENABLE()

// Interrupt line
#define XPT2046_INT__PORT				( GPIOC )
#define XPT2046_INT__PIN				( GPIO_PIN_8 )
#define XPT2046_INT__PULL				( GPIO_NOPULL )
#define XPT2046_INT_CLK_EN()			__HAL_RCC_GPIOC_CLK_ENABLE()




// **********************************************************
// 	ADC RESOLUTION
// **********************************************************

#define XPT2046_ADC_12_BIT				( 0 )
#define XPT2046_ADC_8_BIT				( 1 )
#define XPT2046_ADC_RESOLUTION 			( XPT2046_ADC_12_BIT )


// **********************************************************
// 	REFERENCE MODE
// **********************************************************

#define XPT2046_REF_MODE_DIFFERENTIAL	( 0 )
#define XPT2046_REF_MODE_SINGLE_ENDED	( 1 )
#define XPT2046_REF_MODE 				( XPT2046_REF_MODE_DIFFERENTIAL )


// **********************************************************
// 	3 POINT CALIBRATION
// **********************************************************

// Coordinates
#define XPT2046_POINT_1_XY				{ 48,  32  }
#define XPT2046_POINT_2_XY				{ 240, 288 }
#define XPT2046_POINT_3_XY				{ 432, 160 }

// Point graphics
// NOTE: For know only rectangle is supported
#define XPT2046_POINT_COLOR_BG			( eILI9488_COLOR_BLACK )
#define XPT2046_POINT_COLOR_FG			( eILI9488_COLOR_YELLOW )
#define XPT2046_POINT_SIZE				( 4 )

// Display limitations
#define XPT2046_DISPLAY_MAX_X			( 480 )
#define XPT2046_DISPLAY_MAX_Y			( 320 )


// **********************************************************
// 	TOUCH FILTER (moving average)
// **********************************************************

// Enable touch filter(0/1)
#define XPT2046_FILTER_EN				( 1 )

// Filter window in samples
#define XPT2046_FILTER_WIN_SAMP			( 8 )


// **********************************************************
// 	DEBUG COM PORT
// **********************************************************
#if ( XPT2046_DEBUG_EN )
	#define XPT2046_DBG_PRINT( ... )					( com_dbg_print( eCOM_DBG_CH_TOUCH, (const char*) __VA_ARGS__ ))
	#define XPT2046_ASSERT(x)							if ( 0 == x ) { XPT2046_DBG_PRINT("ASSERT ERROR! file: %s, line: %d", __FILE__, __LINE__ ); while(1){} }
#else
	#define XPT2046_DBG_PRINT( ... )					{ ; }
	#define XPT2046_ASSERT(x)							{ ; }
#endif


//////////////////////////////////////////////////////////////
// END OF FILE
//////////////////////////////////////////////////////////////

#endif // _XPT2046_CONFIG_H_
