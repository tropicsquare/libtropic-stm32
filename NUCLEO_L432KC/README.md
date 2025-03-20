# NUCLEO-L432KC

Devboard's web: https://os.mbed.com/platforms/ST-Nucleo-L432KC/


## Wiring

Use TROPIC01 shield, or wire chip according to following diagram:

```
|------------|-----------------|-----------------|
|  TROPIC01  |  STM32L432KCU6  |  NUCLEO L432KC  |
|------------|-----------------|-----------------|
|  GND       |  GND            |  GND            |
|  3V3       |  3V3            |  3V3            |
|  MISO      |  GPIOA_PIN_6    |  A6             |
|  MOSI      |  GPIOA_PIN_7    |  A5             |
|  SCK       |  GPIOA_PIN_5    |  A4             |
|  CS        |  GPIOA_PIN_4    |  A3             |
|------------|-----------------|-----------------|
```

![NUCLEO L432KC wiring](img/nucleo_L432KC_connection_to_tropic.jpg)
