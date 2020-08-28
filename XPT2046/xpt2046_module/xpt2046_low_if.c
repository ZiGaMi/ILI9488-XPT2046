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
#include "xpt2046_low_if.h"
#include "xpt2046_config.h"

#include "stm32f7xx_hal.h"
#include "spi.h"

#include "string.h"


//////////////////////////////////////////////////////////////
//	DEFINITIONS
//////////////////////////////////////////////////////////////

// Control frame
typedef union
{
	struct
	{
		uint8_t pd 			: 2;	// Power down
		uint8_t ser_dfr 	: 1;	// Reference mode
		uint8_t mode		: 1;	// ADC resolution
		uint8_t addr		: 3;	// Address
		uint8_t source		: 1;	// Source (always 1)
	} bits;
	uint8_t U;
} xpt2046_control_t;

// Conversion result
typedef union
{
	struct
	{
		uint16_t res0	 	: 3;
#if ( 0 == XPT2046_ADC_RESOLUTION )
		uint16_t adc_result	: 12;	// ADC result
		uint16_t res1		: 1;
#else
		uint16_t adc_result	: 8;	// ADC result
		uint16_t res1		: 7;
#endif
	} bits;
	uint16_t U;
} xpt2046_result_t;


// SPI interface
typedef spi_status_t (*pf_spi_exchange_t) (uint8_t * p_tx_data, uint8_t * p_rx_data, const uint32_t size);

// CS
#define XPT2046_LOW_IF_CS_LOW()			( HAL_GPIO_WritePin( XPT2046_CS__PORT, XPT2046_CS__PIN, GPIO_PIN_RESET ))
#define XPT2046_LOW_IF_CS_HIGH()		( HAL_GPIO_WritePin( XPT2046_CS__PORT, XPT2046_CS__PIN, GPIO_PIN_SET ))


//////////////////////////////////////////////////////////////
//	VARIABLES
//////////////////////////////////////////////////////////////

// Spi exchange function
static pf_spi_exchange_t gpf_spi_exchange = spi_exchage_touch;


//////////////////////////////////////////////////////////////
// FUNCTIONS 
//////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////
/*
*			Low level interface exchange
*
*	param:		addr - Address of operation
*	param:		pd_mode - Power down mode
*	param:		start - Start bit
*	param:		p_adc_result - Pointer to measurement result
*	return:		status - Either Ok or Error
*/
//////////////////////////////////////////////////////////////
xpt2046_status_t xpt2046_low_if_exchange(const xpt2046_addr_t addr, const xpt2046_pd_t pd_mode, const xpt2046_start_t start, uint16_t * const p_adc_result)
{
	xpt2046_status_t status = eXPT2046_OK;
	xpt2046_control_t control;
	xpt2046_result_t result;
	uint8_t rx_data[3];
	uint8_t tx_data[3];
	uint16_t rx_data_w;

	// Check if function set
	if ( NULL != gpf_spi_exchange )
	{
		// Assemble frame
		control.U = 0;
		control.bits.source 	= start;
		control.bits.addr 		= addr;
		control.bits.mode 		= XPT2046_ADC_RESOLUTION;
		control.bits.ser_dfr 	= XPT2046_REF_MODE;
		control.bits.pd			= pd_mode;

		// Copy assemble frame
		tx_data[0] = control.U;

		// CS low
		XPT2046_LOW_IF_CS_LOW();

		// Spi interface
		if ( eSPI_OK == gpf_spi_exchange((uint8_t*) &tx_data, (uint8_t*) &rx_data, 3U ) )
		{
			// NOTE: Big endian
			rx_data_w = ( rx_data[1] << 8 ) | ( rx_data[2] );

			// Parse received frame
			memcpy( &result.U, &rx_data_w, 2U );

			// Set result
			*p_adc_result = result.bits.adc_result;
		}
		else
		{
			status = eXPT2046_ERROR;
			*p_adc_result = 0;
		}

		// CS high
		XPT2046_LOW_IF_CS_HIGH();
	}
	else
	{
		status = eXPT2046_ERROR;
		*p_adc_result = 0;

		XPT2046_DBG_PRINT( "SPI interface function not set..." );
		XPT2046_ASSERT( 0 );
	}

	return status;
}


//////////////////////////////////////////////////////////////
/*
*			Get status of touch
*
*	param:		none
*	return:		touch_int - Status of interrupt
*/
//////////////////////////////////////////////////////////////
xpt2046_int_t xpt2046_low_if_get_int(void)
{
	xpt2046_int_t touch_int;

	if ( GPIO_PIN_SET == HAL_GPIO_ReadPin( XPT2046_INT__PORT, XPT2046_INT__PIN ))
	{
		touch_int = eXPT2046_INT_OFF;
	}
	else
	{
		touch_int = eXPT2046_INT_ON;
	}

	return touch_int;
}


//////////////////////////////////////////////////////////////
// END OF FILE
//////////////////////////////////////////////////////////////

