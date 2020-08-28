# ILI9488-XPT2046
Display driver for 3.5" SPI TFT 480x320 with resistive touch. Featuring two chips, ILI9488 (display controler) and XPT2046 (touch controler). Written and tested on STM32F746 Nucleo board, using STM32CubeIDE. 

## USAGE
### 1. Setting up configurations
Both drivers are fully configurable via **ili9488_config.h** and **xpt2046_config.h** file. In there GPIOs, display orientation, font usage, timer peripheral usage, calibration routine    setup and option for debug mode can be change.

##### NOTE: When debug mode is enabled user shall provide debug communication port by the choise!

### 2. Low level interface
- User shall provide low level SPI interface, with predefined functions form and glued it to low level interface modules of drivers. There are two functions for each driver that shall be provided: **spi_transmit** and **spi_receive**.

#### Function forms
```
  // SPI receive number of bytes
  spi_status_t spi_receive(uint8_t * p_rx, const uint32_t size);
  
  // SPI transmit number of bytes
  spi_status_t spi_transmit(uint8_t * p_tx, const uint32_t size);
  
  // Where status is:
  typedef enum
  {
    eSPI_OK = 0,
    eSPI_ERROR
  } spi_status_t;
```

#### Gluing SPI functions to low level driver
- Linking user SPI functions to drivers low level functions is done via function pointers. As seen from ili9488_low_if.c file:
```
// Pointer to SPI functions
// NOTE: 	User shall connect to these two variables SPI
//			tx/rx function. This makes low level platform
//			independent.
static pf_spi_tx_t gpf_spi_transmit = spi_transmit;
static pf_spi_rx_t gpf_spi_receive = spi_receive;
```

### 3. Includes
  Only top level modules are needed, thus two includes shall be provided by user:
```
  #include "ili9488.h"
  #include "xpt2046.h"
```

### 4. Initialization

```
  // Init display controller
  if ( eILI9488_OK != ili9488_init() )
  { 
    // Initialization failed, further actions here...
  }

  // Init touch controller
  if ( eXPT2046_OK != xpt2046_init() )
  {
    // Initialization failed, further actions here...
  }
```

### 5. Touch handler
- Touch controler need to be handle every x ms in order to preserve real-time behaviour. Thus calling **xpt2046_hndl()** every x ms is mandatory.
- Display controler does not have any handler

```
  // Touch variables
  uint16_t x_pos;
  uint16_t y_pos;
  uint16_t force;
  bool touch;

  // Touch task or simple main loop
  @every 10 ms
  {
    // Handle touch controller
    xpt2046_hndl();
  }
```
- Access touch data via **xpt2046_get_touch** function. This function only returns values from local data and doesn't interface with touch controller, thus can be used in multithreaded system without any constrains.
```
  // Touch variables
  uint16_t x_pos;
  uint16_t y_pos;
  uint16_t force;
  bool touch;
  
  // Get touch data
  xpt2046_get_touch( &x_pos, &y_pos, &force, &touch );
```


## CONSTRAINS
- Both drivers are written using ST HAL libraries and thus suitable more for STM32
- For know low level SPI interface must be defined by user
- Both drivers are written based on single thread system, thus if using drivers on multithread platform take that into consideration. Furhtermore both drivers were tested on signle and multi threaded system (testing on STM32F746ZQ & FreeRTOS v10.2.1 ).
- SPI interface with display and touch controler is blocking in nature (DMA will be implemented in future)
- Two separate SPI peripheral is used (combining SPI bus will be implemented in future)
- Due to serial interface fast drawing to display cannot be achieved, thus applications using that kind of display/driver is limited in refresh speed of screen



## TOUCH CALIBRATION ROUTINE
Display features also resistive touch and in case of need, calibration routine is mandatory. 
Calibration routine is based on three points and takes care of three errors: scale, offset and rotation. 
### Steps of calibration
1. Initiate calibration by calling following function:
```
  xpt2046_start_calibration();
```
2. Touch points on display 
3. Store calibration factors into power independent memory. Factors can be access as shown bellow:
```
  int32_t factors[7];
  xpt2046_get_cal_factors( &factors );
```


## GRAPHICS

### Rectange

### Circle

## TEXT

### String type

### Formated string type




