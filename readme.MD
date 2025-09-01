# libtropic-stm32

This repository contains examples of our official SDK [`libtropic`](https://github.com/tropicsquare/libtropic)'s usage on STM32-based platforms (NUCLEO development kits).

Contributors, please follow [guidelines](https://github.com/tropicsquare/libtropic-stm32/blob/master/CONTRIBUTING.md).


 > [!IMPORTANT]
 > Example projects in separate folders represent TROPIC01 connected to different NUCLEO development boards. They all wrap [`libtropic`](https://github.com/tropicsquare/libtropic) and are ready to be cross-compiled and programmed into Nucleo-64 or Nucleo-144 development board.

### Supported Hardware

We have integration examples for following nucleo boards:

 > [!IMPORTANT]
 > Before you start with various examples, we strongly recommend to do two things first:
 > * Read CHIP ID and TROPIC01's firmware versions and **save printed output for future reference**
 > * Update TROPIC01's both internal firmware to latest version.

Choose instruction based on hardware you want to use:
* [Nucleo F439ZI](./NUCLEO_F439ZI/README.md)
* [Nucleo L432KC](./NUCLEO_L432KC/README.md) (Obsolete and not supported)

### License

See the [LICENSE.md](LICENSE.md) file in the root of this repository or consult license information at [Tropic Square website](https://tropicsquare.com/license).

# FAQ

If you encounter any issue, please have a look [here](./FAQ.md) before filling an issue here or before reaching out to our [support](https://support.desk.tropicsquare.com/).
