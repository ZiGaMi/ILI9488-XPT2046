//////////////////////////////////////////////////////////////
// 
//	project:		ILI9488 EVALUATION
//	date:			07.07.2020
//	
//	author:			Ziga Miklosic
//
//////////////////////////////////////////////////////////////


// TODO LIST:
/*
 * 		1. Apply filter, if needed
 */


//////////////////////////////////////////////////////////////
//	INCLUDES
//////////////////////////////////////////////////////////////
#include "stm32f7xx_hal.h"
#include "stdint.h"
#include "string.h"

#include "xpt2046.h"
#include "xpt2046_module/xpt2046_low_if.h"
#include "xpt2046_config.h"

// Display
#include "ili9488_module/ili9488.h"


//////////////////////////////////////////////////////////////
//	DEFINITIONS
//////////////////////////////////////////////////////////////

// Max. FSM state
#define XPT2046_LIMIT_FMS_MS					( 1000000UL ) // [ms]
#define XPT2046_LIMIT_FMS_DURATION(time)		(( time > XPT2046_LIMIT_FMS_MS ) ? ( XPT2046_LIMIT_FMS_MS ) : ( time ))

// Touch
typedef struct
{
	uint16_t 	page;
	uint16_t 	col;
	uint16_t 	force;
	bool		pressed;
} xpt2046_touch_t;

// Point
typedef struct
{
	int64_t x;
	int64_t y;
} xpt2046_point_t;

// Points
typedef enum
{
	eXPT2046_CAL_P1 = 0,
	eXPT2046_CAL_P2,
	eXPT2046_CAL_P3,

	eXPT2046_CAL_P_NUM_OF,
} xpt2046_points_t;

// Calibration data
typedef struct
{
	xpt2046_point_t 	Dp[ eXPT2046_CAL_P_NUM_OF ];	// Display points (predefined)
	xpt2046_point_t 	Tp[ eXPT2046_CAL_P_NUM_OF ];	// Touch points
	int32_t				factors[ 7 ];					// Calibration factors
	bool				start;
	bool				busy;
	bool 				done;
} xpt2046_cal_data_t;

// FSM states
typedef enum
{
	eXPT2046_FSM_NORMAL = 0,
	eXPT2046_FSM_P1_ACQ,
	eXPT2046_FSM_P2_ACQ,
	eXPT2046_FSM_P3_ACQ,
	eXPT2046_FSM_CALC_FACTORS,
} xpt2046_cal_state_t;

// Calibration FSM
typedef struct
{
	struct
	{
		uint32_t 	duration;
		bool 		first_entry;
	} time;

	struct
	{
		xpt2046_cal_state_t cur;
		xpt2046_cal_state_t next;
	} state;
} xpt2046_fsm_t;


//////////////////////////////////////////////////////////////
//	VARIABLES
//////////////////////////////////////////////////////////////

// Touch data
static xpt2046_touch_t g_touch;

// Calibration data
static xpt2046_cal_data_t g_cal_data =
{
	// Predefined display points
	.Dp =
	{
		XPT2046_POINT_1_XY,
		XPT2046_POINT_2_XY,
		XPT2046_POINT_3_XY,
	},

	.start = false,
	.busy = false,
	.done = false
};

// FSM handler
static xpt2046_fsm_t g_cal_fsm;

// Calibration point
ili9488_circ_attr_t g_cal_circ_attr =
{
	.position.radius	= XPT2046_POINT_SIZE,

	.border.enable		= false,
	.border.width		= 0,
	.border.color		= eILI9488_COLOR_BLACK,

	.fill.enable		= true,
};



//////////////////////////////////////////////////////////////
// FUNCTIONS PROTOTYPES
//////////////////////////////////////////////////////////////
static void 	xpt2046_read_data_from_controler	(uint16_t * const p_X, uint16_t * const p_Y, uint16_t * const p_force, bool * const p_is_pressed);
static void 	xpt2046_filter_data					(uint16_t * const p_X, uint16_t * const p_Y, uint16_t * const p_force);
static void 	xpt2046_calibrate_data				(uint16_t * const p_X, uint16_t * const p_Y, const int32_t * const p_factors);
static void 	xpt2046_cal_hndl					(void);
static void 	xpt2046_calculate_factors			(int32_t * p_factors, const xpt2046_point_t * const p_Dp, const xpt2046_point_t * const p_Tp);
static int32_t 	xpt2046_limit_cal_Y_data			(const int32_t unlimited_data);
static int32_t 	xpt2046_limit_cal_X_data			(const int32_t unlimited_data);


static void xpt2046_fms_manager			(void);
static void xpt2046_fsm_normal			(void);
static void xpt2046_fsm_p1_acq			(void);
static void xpt2046_fsm_p2_acq			(void);
static void xpt2046_fsm_p3_acq			(void);
static void xpt2046_fsm_calc_factors	(void);

static void xpt2046_set_cal_point		(const xpt2046_points_t px);
static void xpt2046_clear_cal_point		(const xpt2046_points_t px);




//////////////////////////////////////////////////////////////
// FUNCTIONS 
//////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////
/*
*			Initialize touch controller
*
*	param:		none
*	return:		status - Status of initialization
*/
//////////////////////////////////////////////////////////////
xpt2046_status_t xpt2046_init(void)
{
	xpt2046_status_t status = eXPT2046_OK;

	// Initialize GPIOs & SPI
	if ( eXPT2046_OK != xpt2046_low_if_init())
	{
		status = eXPT2046_ERROR;
	}

	// Initialize FSM
	g_cal_fsm.state.cur = eXPT2046_FSM_NORMAL;
	g_cal_fsm.state.next = eXPT2046_FSM_NORMAL;
	g_cal_fsm.time.duration = 0;
	g_cal_fsm.time.first_entry = false;

	return status;
}


//////////////////////////////////////////////////////////////
/*
*			Get touch data
*
*	param:		p_page 		- Pointer to page (x) coordinate
*	param:		p_col 		- Pointer to column (y) coordinate
*	param:		p_force 	- Pointer to pressure (force) of touch
*	param:		p_pressed 	- Pointer to pressed flag
*	return:		status - Status of initialization
*/
//////////////////////////////////////////////////////////////
xpt2046_status_t xpt2046_get_touch(uint16_t * const p_page, uint16_t * const p_col, uint16_t * const p_force, bool * const p_pressed)
{
	xpt2046_status_t status = eXPT2046_OK;

	*p_page 	= g_touch.page;
	*p_col 		= g_touch.col;
	*p_force 	= g_touch.force;
	*p_pressed 	= g_touch.pressed;

	return status;
}


//////////////////////////////////////////////////////////////
/*
*			Touch controler handler. This shall be called
*			periodically every 10ms.
*
*	param:		none
*	return:		none
*/
//////////////////////////////////////////////////////////////
void xpt2046_hndl(void)
{
	uint16_t X;
	uint16_t Y;
	uint16_t force;
	bool is_pressed;

	// Get data
	xpt2046_read_data_from_controler( &X, &Y, &force, &is_pressed );

	// Apply filter
	xpt2046_filter_data( &X, &Y, &force );

	// Apply calibration
	if ( g_cal_data.done )
	{
		xpt2046_calibrate_data( &X, &Y, (const int32_t*)&g_cal_data.factors );
	}

	// Store
	g_touch.page = X;
	g_touch.col = Y;
	g_touch.force = force;
	g_touch.pressed = is_pressed;

	// Calibration handler
	xpt2046_cal_hndl();
}


//////////////////////////////////////////////////////////////
/*
*			Initialize touch controller
*
*	param:		p_X - Pointer to x coordinate
*	param:		p_Y - Pointer to y coordinate
*	param:		p_force - Pointer to touch pressure
*	param:		p_is_pressed - Pointer to pressed flag
*	return:		none
*/
//////////////////////////////////////////////////////////////
static void xpt2046_read_data_from_controler(uint16_t * const p_X, uint16_t * const p_Y, uint16_t * const p_force, bool * const p_is_pressed)
{
	xpt2046_status_t status = eXPT2046_OK;
	uint16_t Z1;
	uint16_t Z2;
	static uint16_t X_prev;
	static uint16_t Y_prev;
	static uint16_t force_prev;

	// Is pressed
	if ( eXPT2046_INT_ON == xpt2046_low_if_get_int() )
	{
		*p_is_pressed = true;

		// Get X & Y position
		status |= xpt2046_low_if_exchange( eXPT2046_ADDR_X_POS, eXPT2046_PD_POWER_DOWN, eXPT2046_START_ON, p_X );
		status |= xpt2046_low_if_exchange( eXPT2046_ADDR_Y_POS, eXPT2046_PD_POWER_DOWN, eXPT2046_START_ON, p_Y );

		// Get pressure data
		status |= xpt2046_low_if_exchange( eXPT2046_ADDR_Z1_POS, eXPT2046_PD_POWER_DOWN, eXPT2046_START_ON, &Z1 );
		status |= xpt2046_low_if_exchange( eXPT2046_ADDR_YN, eXPT2046_PD_POWER_DOWN, eXPT2046_START_ON, &Z2 );

		if ( eXPT2046_OK == status )
		{
			// Calculate force
			*p_force = (uint16_t) ((((float) *p_X / 4096.0f ) * (((float) Z2  / (float) Z1 ) - 1.0f )) * 4095.0f );

			X_prev = *p_X;
			Y_prev = *p_Y;
			force_prev = *p_force;
		}
	}
	else
	{
		*p_is_pressed = false;

		// Return old value
		*p_X = X_prev;
		*p_Y = Y_prev;
		*p_force = force_prev;
	}
}


//////////////////////////////////////////////////////////////
/*
*			Get touch data
*
*	param:		p_X - Pointer to page (x) coordinate
*	param:		p_Y - Pointer to column (y) coordinate
*	param:		p_force - Pointer to pressure (force) of touch
*	return:		status - Status of initialization
*/
//////////////////////////////////////////////////////////////
static void xpt2046_filter_data(uint16_t * const p_X, uint16_t * const p_Y, uint16_t * const p_force)
{
	// TODO: apply filter
	// Lowpass RC filter

}


//////////////////////////////////////////////////////////////
/*
*			Start calibration routine.
*
*	param:		none
*	return:		status - Status of invoking calibration
*/
//////////////////////////////////////////////////////////////
xpt2046_status_t xpt2046_start_calibration(void)
{
	xpt2046_status_t status = eXPT2046_OK;

	if ( false == g_cal_data.busy )
	{
		g_cal_data.start = true;
		g_cal_data.done = false;
	}
	else
	{
		status = eXPT2046_CAL_IN_PROGRESS;
	}

	return status;
}


//////////////////////////////////////////////////////////////
/*
*			Calibration handler
*
*		NOTE: 	This handler must be called within ili9488
*				task, as it is used display functionalities!
*
*	param:		none
*	return:		none
*/
//////////////////////////////////////////////////////////////
static void xpt2046_cal_hndl(void)
{
	xpt2046_fms_manager();

	switch( g_cal_fsm.state.cur )
	{
		case eXPT2046_FSM_NORMAL:
			xpt2046_fsm_normal();
			break;

		case eXPT2046_FSM_P1_ACQ:
			xpt2046_fsm_p1_acq();
			break;

		case eXPT2046_FSM_P2_ACQ:
			xpt2046_fsm_p2_acq();
			break;

		case eXPT2046_FSM_P3_ACQ:
			xpt2046_fsm_p3_acq();
			break;

		case eXPT2046_FSM_CALC_FACTORS:
			xpt2046_fsm_calc_factors();
			break;

		default:
			xpt2046_fsm_normal();

			XPT2046_DBG_PRINT( "Invalid FSM state..." );
			XPT2046_ASSERT( 0 );
			break;
	}
}


//////////////////////////////////////////////////////////////
/*
*			Calibration FSM manager
*
*	param:		none
*	return:		none
*/
//////////////////////////////////////////////////////////////
static void xpt2046_fms_manager(void)
{
	static uint32_t tick = 0;

	//if ( state_prev != g_cal_fsm.state.cur )
	if ( g_cal_fsm.state.cur != g_cal_fsm.state.next )
	{
		g_cal_fsm.state.cur = g_cal_fsm.state.next;
		g_cal_fsm.time.duration = 0;
		g_cal_fsm.time.first_entry = true;
	}
	else
	{
		g_cal_fsm.time.duration += (uint32_t) ( HAL_GetTick() - tick );
		g_cal_fsm.time.duration = XPT2046_LIMIT_FMS_DURATION( g_cal_fsm.time.duration );
		g_cal_fsm.time.first_entry = false;
	}

	tick = HAL_GetTick();
}


//////////////////////////////////////////////////////////////
/*
*			Normal state
*
*	param:		none
*	return:		none
*/
//////////////////////////////////////////////////////////////
static void xpt2046_fsm_normal(void)
{
	if ( true == g_cal_data.start )
	{
		g_cal_data.start = false;
		g_cal_data.busy = true;

		g_cal_fsm.state.next = eXPT2046_FSM_P1_ACQ;
	}
}


//////////////////////////////////////////////////////////////
/*
*			Acquiring P1 state
*
*	param:		none
*	return:		none
*/
//////////////////////////////////////////////////////////////
static void xpt2046_fsm_p1_acq(void)
{
	static bool point_touched = false;

	if ( true == g_cal_fsm.time.first_entry )
	{
		// Clear display
		ili9488_set_background( eILI9488_COLOR_BLACK );

		// Set up P1
		xpt2046_set_cal_point( eXPT2046_CAL_P1 );

		point_touched = false;
	}
	else
	{
		// Wait for first touch
		if ( false == point_touched )
		{
			if ( true == g_touch.pressed )
			{
				point_touched = true;
			}
		}

		// Point touch
		else
		{
			// Acquire data
			g_cal_data.Tp[0].x = g_touch.page;
			g_cal_data.Tp[0].y = g_touch.col;

			// Wait for release
			if ( false == g_touch.pressed )
			{
				g_cal_fsm.state.next = eXPT2046_FSM_P2_ACQ;

				// Clear P1
				xpt2046_clear_cal_point( eXPT2046_CAL_P1 );
			}
		}
	}
}


//////////////////////////////////////////////////////////////
/*
*			Acquiring P2 state
*
*	param:		none
*	return:		none
*/
//////////////////////////////////////////////////////////////
static void xpt2046_fsm_p2_acq(void)
{
	static bool point_touched = false;

	if ( true == g_cal_fsm.time.first_entry )
	{
		// Set up P2
		xpt2046_set_cal_point( eXPT2046_CAL_P2 );

		point_touched = false;
	}
	else
	{
		// Wait for first touch
		if ( false == point_touched )
		{
			if ( true == g_touch.pressed )
			{
				point_touched = true;
			}
		}

		// Point touch
		else
		{
			// Acquire data
			g_cal_data.Tp[1].x = g_touch.page;
			g_cal_data.Tp[1].y = g_touch.col;

			// Wait for release
			if ( false == g_touch.pressed )
			{
				g_cal_fsm.state.next = eXPT2046_FSM_P3_ACQ;

				// Clear point
				xpt2046_clear_cal_point( eXPT2046_CAL_P2 );
			}
		}
	}
}


//////////////////////////////////////////////////////////////
/*
*			Acquiring P3 state
*
*	param:		none
*	return:		none
*/
//////////////////////////////////////////////////////////////
static void xpt2046_fsm_p3_acq(void)
{
	static bool point_touched = false;

	if ( true == g_cal_fsm.time.first_entry )
	{
		// Set up P3
		xpt2046_set_cal_point( eXPT2046_CAL_P3 );

		point_touched = false;
	}
	else
	{
		// Wait for first touch
		if ( false == point_touched )
		{
			if ( true == g_touch.pressed )
			{
				point_touched = true;
			}
		}

		// Point touch
		else
		{
			// Acquire data
			g_cal_data.Tp[2].x = g_touch.page;
			g_cal_data.Tp[2].y = g_touch.col;

			// Wait for release
			if ( false == g_touch.pressed )
			{
				g_cal_fsm.state.next = eXPT2046_FSM_CALC_FACTORS;

				// Clear point
				xpt2046_clear_cal_point( eXPT2046_CAL_P3 );
			}
		}
	}
}


//////////////////////////////////////////////////////////////
/*
*			Calculate calibration factors state
*
*	param:		none
*	return:		none
*/
//////////////////////////////////////////////////////////////
static void xpt2046_fsm_calc_factors(void)
{
	int32_t cal_factors[7];

	// Calculate calibration data
	xpt2046_calculate_factors( (int32_t*) &cal_factors, (const xpt2046_point_t*) &g_cal_data.Dp, (const xpt2046_point_t*) &g_cal_data.Tp );

	// Store
	memcpy( g_cal_data.factors, cal_factors, sizeof( cal_factors ));

	// Go to normal
	g_cal_fsm.state.next = eXPT2046_FSM_NORMAL;

	// Manage flags
	g_cal_data.busy = false;
	g_cal_data.done = true;
}


//////////////////////////////////////////////////////////////
/*
*			Set calibration point on diplay
*
*	param:		px - Calibration point number
*	return:		none
*/
//////////////////////////////////////////////////////////////
static void xpt2046_set_cal_point(const xpt2046_points_t px)
{
	if ( px < eXPT2046_CAL_P_NUM_OF )
	{
		//ili9488_fill_rectangle( g_cal_data.Dp[ px ].x, g_cal_data.Dp[ px ].y, XPT2046_POINT_SIZE, XPT2046_POINT_SIZE, XPT2046_POINT_COLOR_FG );

		g_cal_circ_attr.position.start_page = g_cal_data.Dp[ px ].x;
		g_cal_circ_attr.position.start_col 	= g_cal_data.Dp[ px ].y;
		g_cal_circ_attr.fill.color			= XPT2046_POINT_COLOR_FG;
		ili9488_draw_circle( &g_cal_circ_attr );
	}
}


//////////////////////////////////////////////////////////////
/*
*			Clear calibration point on display
*
*	param:		px - Calibration point number
*	return:		none
*/
//////////////////////////////////////////////////////////////
static void xpt2046_clear_cal_point(const xpt2046_points_t px)
{
	if ( px < eXPT2046_CAL_P_NUM_OF )
	{
		//ili9488_fill_rectangle( g_cal_data.Dp[ px ].x, g_cal_data.Dp[ px ].y, XPT2046_POINT_SIZE, XPT2046_POINT_SIZE, XPT2046_POINT_COLOR_BG );

		g_cal_circ_attr.position.start_page = g_cal_data.Dp[ px ].x;
		g_cal_circ_attr.position.start_col 	= g_cal_data.Dp[ px ].y;
		g_cal_circ_attr.fill.color			= XPT2046_POINT_COLOR_BG;
		ili9488_draw_circle( &g_cal_circ_attr );
	}
}


//////////////////////////////////////////////////////////////
/*
*			Calculation of calibration factors
*
*	param:		p_factor - Pointer to cal factors
*	param:		p_Dp - Pointer display points
*	param:		p_Tp - Pointer touch points
*	return:		none
*/
//////////////////////////////////////////////////////////////
static void xpt2046_calculate_factors(int32_t * p_factors, const xpt2046_point_t * const p_Dp, const xpt2046_point_t * const p_Tp)
{
	// Calculate C vectors
	p_factors[0] = 	(	(int32_t) (( p_Tp[0].x - p_Tp[2].x ) * ( p_Tp[1].y - p_Tp[2].y ))
					- 	(int32_t) (( p_Tp[1].x - p_Tp[2].x ) * ( p_Tp[0].y - p_Tp[2].y )));

	p_factors[1] = 	(	(int32_t) (( p_Dp[0].x - p_Dp[2].x ) * ( p_Tp[1].y - p_Tp[2].y ))
					- 	(int32_t) (( p_Dp[1].x - p_Dp[2].x ) * ( p_Tp[0].y - p_Tp[2].y )));

	p_factors[2] = 	(	(int32_t) (( p_Tp[0].x - p_Tp[2].x ) * ( p_Dp[1].x - p_Dp[2].x ))
					- 	(int32_t) (( p_Dp[0].x - p_Dp[2].x ) * ( p_Tp[1].x - p_Tp[2].x )));

	p_factors[3] = 	(	(int32_t) ( p_Tp[0].y * (((int32_t) p_Tp[2].x * p_Dp[1].x ) - ((int32_t) p_Tp[1].x * p_Dp[2].x )))
					+	(int32_t) ( p_Tp[1].y * (((int32_t) p_Tp[0].x * p_Dp[2].x ) - ((int32_t) p_Tp[2].x * p_Dp[0].x )))
					+	(int32_t) ( p_Tp[2].y * (((int32_t) p_Tp[1].x * p_Dp[0].x ) - ((int32_t) p_Tp[0].x * p_Dp[1].x ))));

	p_factors[4] = 	(	(int32_t) (( p_Dp[0].y - p_Dp[2].y ) * ( p_Tp[1].y - p_Tp[2].y ))
					- 	(int32_t) (( p_Dp[1].y - p_Dp[2].y ) * ( p_Tp[0].y - p_Tp[2].y )));

	p_factors[5] = 	(	(int32_t) (( p_Tp[0].x - p_Tp[2].x ) * ( p_Dp[1].y - p_Dp[2].y ))
					- 	(int32_t) (( p_Dp[0].y - p_Dp[2].y ) * ( p_Tp[1].x - p_Tp[2].x )));


	p_factors[6] = 	(	(int32_t) ( p_Tp[0].y * (((int32_t) p_Tp[2].x * p_Dp[1].y ) - ((int32_t) p_Tp[1].x * p_Dp[2].y )))
					+	(int32_t) ( p_Tp[1].y * (((int32_t) p_Tp[0].x * p_Dp[2].y ) - ((int32_t) p_Tp[2].x * p_Dp[0].y )))
					+	(int32_t) ( p_Tp[2].y * (((int32_t) p_Tp[1].x * p_Dp[0].y ) - ((int32_t) p_Tp[0].x * p_Dp[1].y ))));
}


//////////////////////////////////////////////////////////////
/*
*			Acquiring P1 state
*
*	param:		p_X - Pointer to x coordinate
*	param:		p_Y - Pointer to y coordinate
*	param:		p_factors - Pointer to cal factors
*	return:		none
*/
//////////////////////////////////////////////////////////////
static void xpt2046_calibrate_data(uint16_t * const p_X, uint16_t * const p_Y, const int32_t * const p_factors)
{
	xpt2046_point_t Dp;
	xpt2046_point_t Tp;

	// Touch coordinate
	Tp.x = (int64_t) *p_X;
	Tp.y = (int64_t) *p_Y;

	// Apply factors
	Dp.x = ((( p_factors[1] * Tp.x ) + ( p_factors[2] * Tp.y ) + p_factors[3] ) / p_factors[0] );
	Dp.y = ((( p_factors[4] * Tp.x ) + ( p_factors[5] * Tp.y ) + p_factors[6] ) / p_factors[0] );

	// Limit
	Dp.x = xpt2046_limit_cal_X_data( Dp.x );
	Dp.y = xpt2046_limit_cal_Y_data( Dp.y );

	// Return calibrated values
	*p_X = (uint16_t) Dp.x;
	*p_Y = (uint16_t) Dp.y;
}


//////////////////////////////////////////////////////////////
/*
*			Limit X calibration coordinate
*
*	param:		unlimited_data - Calibrated unlimited data
*	return:		lim_data - Limited data due to limitation of display
*/
//////////////////////////////////////////////////////////////
static int32_t xpt2046_limit_cal_X_data(const int32_t unlimited_data)
{
	int32_t lim_data;

	if ( unlimited_data < 0 )
	{
		lim_data = 0;
	}
	else if ( unlimited_data > XPT2046_DISPLAY_MAX_X )
	{
		lim_data = XPT2046_DISPLAY_MAX_X;
	}
	else
	{
		lim_data = unlimited_data;
	}

	return lim_data;
}


//////////////////////////////////////////////////////////////
/*
*			Limit Y calibration coordinate
*
*	param:		unlimited_data - Calibrated unlimited data
*	return:		lim_data - Limited data due to limitation of display
*/
//////////////////////////////////////////////////////////////
static int32_t xpt2046_limit_cal_Y_data(const int32_t unlimited_data)
{
	int32_t lim_data;

	if ( unlimited_data < 0 )
	{
		lim_data = 0;
	}
	else if ( unlimited_data > XPT2046_DISPLAY_MAX_Y )
	{
		lim_data = XPT2046_DISPLAY_MAX_Y;
	}
	else
	{
		lim_data = unlimited_data;
	}

	return lim_data;
}


//////////////////////////////////////////////////////////////
/*
*			Return calibration done flag
*
*	param:		none
*	return:		g_cal_data.done - Calibration done flag
*/
//////////////////////////////////////////////////////////////
bool xpt2046_is_calibrated(void)
{
	return g_cal_data.done;
}


//////////////////////////////////////////////////////////////
/*
*			Set calibration factors
*
*	param:		p_factors - Pointer to factors
*	return:		none
*/
//////////////////////////////////////////////////////////////
void xpt2046_set_cal_factors(const int32_t * const p_factors)
{
	// Calibration allready done some time in past
	g_cal_data.done = true;

	// Copy factors
	memcpy( &g_cal_data.factors, p_factors, sizeof( g_cal_data.factors ));
}


//////////////////////////////////////////////////////////////
/*
*			Get calibration factors
*
*	param:		p_factors - Pointer to factors
*	return:		none
*/
//////////////////////////////////////////////////////////////
void xpt2046_get_cal_factors(const int32_t * p_factors)
{
	p_factors = (int32_t*) &g_cal_data.factors;
}


//////////////////////////////////////////////////////////////
// END OF FILE
//////////////////////////////////////////////////////////////

