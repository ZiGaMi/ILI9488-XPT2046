# ILI9488-XPT2046
Display driver for 3.5" SPI TFT 480x320 with resistive touch. Featuring two chips, ILI9488 (display controler) and XPT2046 (touch controler)

## USAGE
### 1. Setting up configurations
Both drivers are fully configurable via ili9488_config.h and xpt2046_config.h file. In there GPIOs, display orientation, font usage, timer peripheral usage, calibration routine    setup and option for debug mode can be change.

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

## CONSTRAINS


## TOUCH CALIBRATION ROUTINE


## GRAPHICS

### Rectange

### Circle

## TEXT

### String type

### Formated string type




