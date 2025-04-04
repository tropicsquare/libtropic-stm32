# NUCLE-F439ZI

Devboard's web: https://os.mbed.com/platforms/ST-Nucleo-F439ZI/


## Wiring

Use TROPIC01 shield (Arduino form factor), or wire chip according to following diagram:

```
|------------|------------------|
|  TROPIC01  |   NUCLEO F437ZI  |
|------------|------------------|
|  GND       |  GND             |
|  3V3       |  3V3             |
|  MISO      |  GPIOA_6         |
|  MOSI      |  GPIOA_7         |
|  SCK       |  GPIOA_5         |
|  CS        |  GPIOD_14        |
|------------|------------------|
```
