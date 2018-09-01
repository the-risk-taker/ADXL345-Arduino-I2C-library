# ADXL345 - Arduino I2C library

C++ library for ADXL345 accelerometer for I2C communication with Arduino Wire library. Main features:

- read raw data (with offset and gain correction),
- low pass filtering for raw data,
- read scaled data,
- read normalized data,
- read events (free fall, single tap, double tap),
- accelerometer calibration process,
- device registers setting,
- communication sniff.

The library is intended to work **without any user interference**, just open [project](https://github.com/MatthewPatyk/ADXL345-Arduino-I2C-library/blob/master/ADXL345_Project.atsln) and upload working [example](https://github.com/MatthewPatyk/ADXL345-Arduino-I2C-library/blob/master/ADXL345_Project/ADXL345_Project.ino) and use it. However, if there is such a need, the user has **the possibility of advanced configuration** and use of the library.

Library uses [I2C Interface](https://github.com/MatthewPatyk/I2C-Interface-for-Arduino-Wire-Library), [I2C Sensor Interface](https://github.com/MatthewPatyk/I2C-Sensor-Interface) to simplify and unify working with I2C devices. The library also uses [Status Class](https://github.com/MatthewPatyk/Status-Class) which is optional.

## Getting Started

The working example for this library is writen in [Atmel Studio 7](http://www.microchip.com/mplab/avr-support/atmel-studio-7) with [Visual Micro](https://www.visualmicro.com/) addon. But **it is possible to run it with Arduino IDE** by [adding files to project](https://www.arduino.cc/en/Guide/Environment#toc8) (probably there will be need to change the `#include` paths).

### Prerequisites

- Software:
  - [Atmel Studio 7](http://www.microchip.com/mplab/avr-support/atmel-studio-7) (tested) or Visual Studio,
  - [Visual Micro](https://www.visualmicro.com/) addon for above AS7 or VS.
- Hardware:
  - Arduino Due (tested) or Arduino Uno (any ATmega 328P) board,
  - ADXL345 board or any IMU sensor with ADXL345.

### Wiring

| ADXL345 board | Due board | Uno board |
| :-----------: | :-------: | :-------: |
|      SCL      | SCL (21)  | SCL (A4)  |
|      SDA      | SDA (20)  | SDA (A5)  |
|  VCC 3.3V\*   |   3.3V    |   3.3V    |
|   VCC 5V\*    |  - \*\*   |    5V     |
|      GND      |    GND    |    GND    |

\*If your ADXL345 board uses 5V or 3.3V and your MCU work on the similar voltage level (Due - 3.3V, Uno - 5V) this library provide you the possibility of restarting ADXL345 IC, see [Calibration process](https://github.com/MatthewPatyk/ADXL345-Arduino-I2C-library/blob/master/README.md#callibration-process) below.

\*\*The Due MCU cannot be exposed to the voltage above 3.3V level!

## Example

To see a real-life example open AS7 project file `ADXL345_Project.atsln` and upload it (`ADXL345_Project.ino`) to a micro-controller. You should see something like that on Serial Monitor (baud rate = 115200):

```plaintext
2	Serial initialized with baud rate = 115200
104	Found ADXL345 searching device with ID = 0x53
105	AccX = 0.50	AccY = 0.00	AccZ = 10.57	m/s^2	pitch = -2.70	roll = 0.00
115	AccX = 0.42	AccY = -0.04	AccZ = 9.96	m/s^2	pitch = -2.42	roll = -0.22
...
```

### Getting events

To see events {free fall, single tap, double tap} change defines in `ADXL345_Project.ino` file to:

```cpp
#define PRINT_DATA 0x00 // 0x01 = Enable; Other = Disable
#define PRINT_EVENTS 0x01 // 0x01 = Enable; Other = Disable
```

#### Data rate and range setting

To change above in `ADXL345_Project.ino` file uncomment and choose appropriate in below:

```cpp
// Set your Data Rate and Range or use defaults from ADXL345::init()
//		Accelerometer.setDataRate(->your_setting<-);
//		Accelerometer.setRange(->your_setting<-);
```

## Advanced usage

As it was mentioned, the user has access to advanced options.

### Calibration process

Due to manufacturing process, accelerometers have some problems with the data quality. In other words, to get the best accuracy, you need **to calibrate it**.
In order to enable calibration in `ADXL345.cpp` file change `ADXL345_ENABLE_CALIBRATION_PROCESS` define to `0x01`:

```cpp
#define ADXL345_ENABLE_CALIBRATION_PROCESS 0x01
```

You should see something like that on Serial Monitor (baudrate = 115200):

```plaintext
2	Serial initialized with baudrate = 115200
104	Put acc pointing X axis up!
104	And then press key for example '3' when ready and press Enter.
104	... Or Press '1' key and Enter to finish! and get compensation parameters
```

Then follow the instructions from Serial Monitor.
**Important**: be sure that you are able to type specific key in Serial Monitor.

### Device restarting

Sometimes there is a problem with initialization (getting device ID, basically any information from the chip). This will be revealed by this:

```plaintext
354 I repeat the initialization of ADXL345 for the: 0 time
701 I repeat the initialization of ADXL345 for the: 1 time
...
```

There are to ways to fix it:

1. unplug and plug the device (USB cable) from power,
2. use the library.

The ADXL345 class constructor accepts as one of the arguments the `deviceVCCPin`. **Instead of connecting the device VCC pin to 3.3V or 5V, it can be connected to one of the output pins**. Then in case of initialization error, the library will be setting power on/off trying to restart the device. To change that pin in `Config.h` file, set `ADXL345_VCC_PIN` define to appropriate:

```cpp
#define ADXL345_VCC_PIN 52
```

**Important**: in case of using modules like GY-80 (with multiple sensors) be sure that ADXL345 object is initializing as the first one. Otherwise, your program may behave unpredictably - many IC chips has volatile memory!

#### Single tap, double tap, free fall events detection

The IC provide many configurable registers (library sets default working values) for events detection, see functions:

```cpp
ADXL345::setFreeFall();
ADXL345::setSingleTap();
ADXL345::setDoubleTap();
```

and for more see ADXL345 datasheet.
