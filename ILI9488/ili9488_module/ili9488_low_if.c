//////////////////////////////////////////////////////////////
// 
//	project:		ILI9488 EVALUATION
//	date:			07.07.2020
//	
//	author:			Ziga Miklosic
//
//////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////
//	INCLUDES
//////////////////////////////////////////////////////////////
#include "stm32f7xx_hal.h"

#include "ili9488_low_if.h"
#include "ili9488_config.h"
#include "spi.h"



//////////////////////////////////////////////////////////////
//	DEFINITIONS
//////////////////////////////////////////////////////////////

// CS
#define ILI9488_LOW_IF_CS_LOW()			( HAL_GPIO_WritePin( ILI9488_CS__PORT, ILI9488_CS__PIN, GPIO_PIN_RESET ))
#define ILI9488_LOW_IF_CS_HIGH()		( HAL_GPIO_WritePin( ILI9488_CS__PORT, ILI9488_CS__PIN, GPIO_PIN_SET ))

// DC
#define ILI9488_LOW_IF_DC_COMMAND()		( HAL_GPIO_WritePin( ILI9488_DC__PORT, ILI9488_DC__PIN, GPIO_PIN_RESET ))
#define ILI9488_LOW_IF_DC_DATA()		( HAL_GPIO_WritePin( ILI9488_DC__PORT, ILI9488_DC__PIN, GPIO_PIN_SET ))

// Spi functions
typedef spi_status_t (*pf_spi_tx_t) (uint8_t * p_data, const uint32_t size);
typedef spi_status_t (*pf_spi_rx_t) (uint8_t * p_data, const uint32_t size);


//////////////////////////////////////////////////////////////
//	VARIABLES
//////////////////////////////////////////////////////////////

// Pointer to SPI functions
// NOTE: 	User shall connect to these two variables SPI
//			tx/rx function. This makes low level platform
//			independent.
static pf_spi_tx_t gpf_spi_transmit = spi_transmit;
static pf_spi_rx_t gpf_spi_receive = spi_receive;

// LED PWM timer
static TIM_HandleTypeDef gh_led_timer;


//////////////////////////////////////////////////////////////
// FUNCTIONS 
//////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////
/*
*			Write to LCD register
*
*	If sending only command, size shall be 0. Add size only
*	for data bytes.
*
*
*	param: 		cmd - command name
*	param: 		tx_data - pointer to transmit data
*	param: 		size - number of data to transmit
*	return:		status - Either Ok or Error
*/
//////////////////////////////////////////////////////////////
ili9488_status_t ili9488_low_if_write_register(const ili9488_cmd_t cmd, const uint8_t * const tx_data, const uint32_t size)
{
	ili9488_status_t status = eILI9488_OK;
	uint8_t command = cmd;

	// Set CS & DC
	ILI9488_LOW_IF_CS_LOW();
	ILI9488_LOW_IF_DC_COMMAND();

	// Send command
	if ( eSPI_OK != gpf_spi_transmit( &command, 1U ))
	{
		status = eILI9488_ERROR;
	}

	// Command send OK
	else
	{
		// In case of parameters send them as well
		if ( size > 0 )
		{
			// Data transmit
			ILI9488_LOW_IF_DC_DATA();

			if ( eSPI_OK != gpf_spi_transmit((uint8_t*) tx_data, size ))
			{
				status = eILI9488_ERROR;
			}
		}
	}

	// Set CS
	ILI9488_LOW_IF_CS_HIGH();

	return status;
}


//////////////////////////////////////////////////////////////
/*
*			Write to RGB code to GRAM
*
*	This function writes constant RGB code and write size
*	number pixels.
*
*
*	param: 		rgb - pointer to rgb code
*	param: 		size - number of data to transmit
*	return:		status - Either Ok or Error
*/
//////////////////////////////////////////////////////////////
ili9488_status_t ili9488_low_if_write_rgb_to_gram (const ili9488_rgb_t * const p_rgb, const uint32_t size)
{
	ili9488_status_t status = eILI9488_OK;
	const uint8_t cmd = eILI9488_WRITE_MEM_CMD;
	uint32_t i;

	// Set CS & DC
	ILI9488_LOW_IF_CS_LOW();
	ILI9488_LOW_IF_DC_COMMAND();

	// Send command
	if ( eSPI_OK != gpf_spi_transmit((uint8_t*) &cmd, 1U ))
	{
		status = eILI9488_ERROR;
	}
	else
	{
		// In case of parameters send them as well
		if ( size > 0 )
		{
			// Data transmit
			ILI9488_LOW_IF_DC_DATA();

			for ( i = 0; i < size; i++ )
			{
				gpf_spi_transmit((uint8_t*) p_rgb, 3U );
			}
		}
	}

	// Set CS
	ILI9488_LOW_IF_CS_HIGH();

	return status;
}


//////////////////////////////////////////////////////////////
/*
*			Read from LCD register
*
*
*
*	param: 		cmd - command name
*	param: 		rx_data - pointer to read data
*	param: 		size - number of data to read
*	return:		status - Either Ok or Error
*/
//////////////////////////////////////////////////////////////
ili9488_status_t ili9488_low_if_read_register(const ili9488_cmd_t cmd, uint8_t * const rx_data, const uint32_t size)
{
	ili9488_status_t status = eILI9488_OK;
	uint8_t command = cmd;

	// Set CS & DC
	ILI9488_LOW_IF_CS_LOW();
	ILI9488_LOW_IF_DC_COMMAND();

	// Send command
	if ( eSPI_OK != gpf_spi_transmit( &command, 1U ))
	{
		status = eILI9488_ERROR;
	}

	// Command send OK
	else
	{
		// In case of parameters send them as well
		if ( size > 0 )
		{
			// Data transmit
			ILI9488_LOW_IF_DC_DATA();

			if ( eSPI_OK != gpf_spi_receive( rx_data, size ))
			{
				status = eILI9488_ERROR;
			}
		}
	}

	// Set CS
	ILI9488_LOW_IF_CS_HIGH();

	return status;
}


//////////////////////////////////////////////////////////////
/*
*			Reset line
*
*	param: 		rst - state of reset line
*	return:		none
*/
//////////////////////////////////////////////////////////////
void ili9488_low_if_set_reset(const ili9488_reset_t rst)
{
	if ( eILI9488_RESET_OFF == rst )
	{
		HAL_GPIO_WritePin( ILI9488_RESET__PORT, ILI9488_RESET__PIN, GPIO_PIN_SET );
	}
	else
	{
		HAL_GPIO_WritePin( ILI9488_RESET__PORT, ILI9488_RESET__PIN, GPIO_PIN_RESET );
	}
}


//////////////////////////////////////////////////////////////
/*
*			Display backlight LED
*
*	param: 		brightness 	- Brightness in value of 0.0 - 1.0
*	return:		status 		- Status of operation
*/
//////////////////////////////////////////////////////////////
ili9488_status_t ili9488_low_if_set_led(const float32_t brightness)
{
	ili9488_status_t status = eILI9488_OK;
	uint16_t CMPR;

	if (( brightness >= 0.0f ) && ( brightness <= 1.0f ))
	{
		// Calculate duty
		CMPR = (uint16_t) ( brightness * __HAL_TIM_GET_AUTORELOAD( &gh_led_timer ));

	    // Set duty
	    __HAL_TIM_SET_COMPARE( &gh_led_timer, ILI9488_LED_TIMER_CH, CMPR );
	}
	else
	{
		status = eILI9488_ERROR;

		ILI9488_DBG_PRINT( "Invalid brightness value..." );
		ILI9488_ASSERT( 0 );
	}

	return status;
}


//////////////////////////////////////////////////////////////
/*
*			Initialize timer for backlight PWM control
*
*	param: 		none
*	return:		status 		- Status of operation
*/
//////////////////////////////////////////////////////////////
ili9488_status_t ili9488_low_if_init_led_timer(void)
{
	ili9488_status_t status = eILI9488_OK;

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Enable clocks
    ILI9488_LED_CLK_EN();
    ILI9488_LED_TIMER_EN_CLK();

    // Configure Pins
    GPIO_InitStruct.Pin 			= ILI9488_LED__PIN;
    GPIO_InitStruct.Mode 			= GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull 			= GPIO_NOPULL;
    GPIO_InitStruct.Speed 			= GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate 		= ILI9488_LED_TIMER_ALT_FUNC;
    HAL_GPIO_Init( ILI9488_LED__PORT, &GPIO_InitStruct);

    // Configure Timer
    gh_led_timer.Instance 					= ILI9488_LED_TIMER;
    gh_led_timer.Init.Prescaler 			= 32;
    gh_led_timer.Init.CounterMode 			= TIM_COUNTERMODE_UP;
	gh_led_timer.Init.Period 				= ( HAL_RCC_GetPCLK2Freq() / ( ILI9488_LED_TIMER_FREQ_HZ * ( gh_led_timer.Init.Prescaler + 1U )));
    gh_led_timer.Init.ClockDivision 		= TIM_CLOCKDIVISION_DIV1;
    gh_led_timer.Init.RepetitionCounter 	= 0;
    gh_led_timer.Init.AutoReloadPreload 	= TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Base_Init( &gh_led_timer );

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;

    // Set up clock source
    if ( HAL_OK != HAL_TIM_ConfigClockSource( &gh_led_timer, &sClockSourceConfig ))
    {
    	status |= eILI9488_ERROR;
    }

    // Init PWM unit
	if ( HAL_OK != HAL_TIM_PWM_Init( &gh_led_timer ))
	{
		status |= eILI9488_ERROR;
	}

	sConfigOC.OCMode 				= TIM_OCMODE_PWM1;
	sConfigOC.Pulse 				= 0;
	sConfigOC.OCPolarity 			= TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity 			= TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode 			= TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState 			= TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState 			= TIM_OCNIDLESTATE_RESET;

	// Config channel
	if ( HAL_OK != HAL_TIM_PWM_ConfigChannel( &gh_led_timer, &sConfigOC, ILI9488_LED_TIMER_CH ))
	{
		status |= eILI9488_ERROR;
	}

	// Start Timer
	if ( HAL_OK != HAL_TIM_PWM_Start( &gh_led_timer, ILI9488_LED_TIMER_CH ))
	{
		status |= eILI9488_ERROR;
	}

	return status;
}



//////////////////////////////////////////////////////////////
// END OF FILE
//////////////////////////////////////////////////////////////

