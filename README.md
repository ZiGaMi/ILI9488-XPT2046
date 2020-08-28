# ILI9488-XPT2046
Display driver for 3.5" SPI TFT 480x320 with resistive touch. Featuring two chips, ILI9488 (display controler) and XPT2046 (touch controler)

## USAGE
### 1. Setting up configurations
Both drivers are fully configurable via **ili9488_config.h** and **xpt2046_config.h** file. In there GPIOs, display orientation, font usage, timer peripheral usage, calibration routine    setup and option for debug mode can be change.

##### NOTE: When debug mode is enabled user shall provide debug communication port by the choise!

### 2. Includes
  Only top level modules are needed, thus two includes shall be provided by user:
```
  #include "ili9488.h"
  #include "xpt2046.h"
```

### 3. Initialization

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

### 4. Checking for touch

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

    // Get data
    xpt2046_get_touch( &x_pos, &y_pos, &force, &touch );
  }
```


## CONSTRAINS
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
3. Store calibration factors into power independent memory. Factors are provided by following function:
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




