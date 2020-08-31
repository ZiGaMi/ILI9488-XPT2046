//////////////////////////////////////////////////////////////
//
//	project:		ILI9488 EVALUATION
//	date:			10.07.2020
//
//	author:			Ziga Miklosic
//
//////////////////////////////////////////////////////////////

#ifndef _ILI9488_CONFIG_H_
#define _ILI9488_CONFIG_H_

//////////////////////////////////////////////////////////////
//	INCLUDES
//////////////////////////////////////////////////////////////
#include "stdint.h"
#include "stm32f7xx_hal.h"

// Debug communication port
#include "com_dbg.h"


//////////////////////////////////////////////////////////////
//	DEFINITIONS
//////////////////////////////////////////////////////////////

// Enable debug mode (0/1)
#define ILI9488_DEBUG_EN				( 1 )


// **********************************************************
// 	SPI INTERFACE
// **********************************************************
#define ILI9488_SPI						( SPI1 )
#define ILI9488_SPI_BAUDRATE_PSC		( SPI_BAUDRATEPRESCALER_4 )		// NOTE: Max. SPI clock is 20 MHz
#define ILI9488_SPI_TIMEOUT_MS			( 10 )	// [ms]
#define ILI9488_SPI_EN_CLK()			__HAL_RCC_SPI1_CLK_ENABLE()


// **********************************************************
// 	GPIO PINS
// **********************************************************

// SPI pins
#define ILI9488_SCK__PORT				( GPIOA )
#define ILI9488_SCK__PIN				( GPIO_PIN_5 )
#define ILI9488_SCK__PULL				( GPIO_NOPULL )
#define ILI9488_SCK_CLK_EN()			__HAL_RCC_GPIOA_CLK_ENABLE()

#define ILI9488_MISO__PORT				( GPIOA )
#define ILI9488_MISO__PIN				( GPIO_PIN_6 )
#define ILI9488_MISO__PULL				( GPIO_NOPULL )
#define ILI9488_MISO_CLK_EN()			__HAL_RCC_GPIOA_CLK_ENABLE()

#define ILI9488_MOSI__PORT				( GPIOA )
#define ILI9488_MOSI__PIN				( GPIO_PIN_7 )
#define ILI9488_MOSI__PULL				( GPIO_NOPULL )
#define ILI9488_MOSI_CLK_EN()			__HAL_RCC_GPIOA_CLK_ENABLE()

#define ILI9488_CS__PORT				( GPIOF )
#define ILI9488_CS__PIN					( GPIO_PIN_12 )
#define ILI9488_CS__PULL				( GPIO_NOPULL )
#define ILI9488_CS_CLK_EN()				__HAL_RCC_GPIOF_CLK_ENABLE()

// LCD Reset line
#define ILI9488_RESET__PORT				( GPIOD )
#define ILI9488_RESET__PIN				( GPIO_PIN_14 )
#define ILI9488_RESET__PULL				( GPIO_NOPULL )
#define ILI9488_RESET_CLK_EN()			__HAL_RCC_GPIOD_CLK_ENABLE()

// Data/Command line
#define ILI9488_DC__PORT				( GPIOD )
#define ILI9488_DC__PIN					( GPIO_PIN_15 )
#define ILI9488_DC__PULL				( GPIO_NOPULL )
#define ILI9488_DC_CLK_EN()				__HAL_RCC_GPIOD_CLK_ENABLE()

// LED
#define ILI9488_LED__PORT				( GPIOB )
#define ILI9488_LED__PIN				( GPIO_PIN_4 )
#define ILI9488_LED__PULL				( GPIO_NOPULL )
#define ILI9488_LED_CLK_EN()			__HAL_RCC_GPIOB_CLK_ENABLE()


// **********************************************************
// 	DISPLAY ORIENTATION
// **********************************************************

// Flip display landscape orientation (0/1)
#define ILI9488_DISPLAY_FLIP			( 0 )


// **********************************************************
// 	DISPLAY FONTS
// **********************************************************
//
//	Here unused fonts can be disabled in order to save
// 	flash memory space

// Enable usage of font 8 (0/1)
// Size: 760 B
#define ILI9488_FONT_8_USE_EN			( 1 )

// Enable usage of font 12 (0/1)
// Size: 1,11 kB
#define ILI9488_FONT_12_USE_EN			( 1 )

// Enable usage of font 16 (0/1)
// Size: 2,97 kB
#define ILI9488_FONT_16_USE_EN			( 1 )

// Enable usage of font 20 (0/1)
// Size: 3,71 kB
#define ILI9488_FONT_20_USE_EN			( 1 )

// Enable usage of font 24 (0/1)
// Size: 6,68 kB
#define ILI9488_FONT_24_USE_EN			( 1 )


// **********************************************************
// 	DISPLAY BRIGHTNESS CONTROL
// **********************************************************

// HW timer unit
#define ILI9488_LED_TIMER				( TIM3 )
#define ILI9488_LED_TIMER_CH			( TIM_CHANNEL_1 )
#define ILI9488_LED_TIMER_ALT_FUNC		( GPIO_AF2_TIM3 )
#define ILI9488_LED_TIMER_EN_CLK()		__HAL_RCC_TIM3_CLK_ENABLE()

// PWM frequency
// NOTE: From 100Hz - 1000Hz
// According to measurements 200Hz produce best results
// Additionally capacitors were places on VCC (100uF tantalum, 4uF & 100nF ceramic)
#define ILI9488_LED_TIMER_FREQ_HZ		( 200 ) // [Hz]

// Start-up brightness (0.0f - 1.0f)
#define ILI9488_LED_STARTUP_VAL			( 1.0f )

// **********************************************************
// 	DEBUG COM PORT
// **********************************************************
#if ( ILI9488_DEBUG_EN )
	#define ILI9488_DBG_PRINT( ... )					( com_dbg_print( eCOM_DBG_CH_DISPLAY, (const char*) __VA_ARGS__ ))
	#define ILI9488_ASSERT(x)							if ( 0 == x ) { ILI9488_DBG_PRINT("ASSERT ERROR! file: %s, line: %d", __FILE__, __LINE__ ); while(1){} }
#else
	#define ILI9488_DBG_PRINT( ... )					{ ; }
	#define ILI9488_ASSERT(x)							{ ; }
#endif


//////////////////////////////////////////////////////////////
// END OF FILE
//////////////////////////////////////////////////////////////

#endif // _ILI9488_CONFIG_H_
