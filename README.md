# Echo
Program for echoing received serial data like the  [Arduino example](https://www.arduino.cc/en/Tutorial/SoftwareSerialExample). I used this program as an intro to learning about cross compiling and how to better structure project directories.
Usage: ```./echo [-p port] [-b baudrate]```

## Getting Started
I precompiled this program, so it should work as soon as it's on a Raspberry Pi. The executable is located in the build folder. Instructions on how to recompile are below.
### Compilation Prerequisites
This repository comes with all the files for cross compiling (except CMake) in the project root, include/, and opt/ folders.

*  Raspberry Pi Libraries
	* [wiringPi](http://wiringpi.com/) - Interface to Raspberry Pi pins
	* [wiringSerial](http://wiringpi.com/reference/serial-library/) - Raspberry Pi serial interface, included with wiringPi download.
	```
	git clone git://git.drogon.net/wiringpi/
	```
* Cross Compiling Tools
	* [CMake](https://cmake.org/)
	```
	sudo apt install cmake
	```
	* [Raspbery Pi Toolchain](https://github.com/raspberrypi/tools)
	```
	git clone https://github.com/raspberrypi/tools
	```
#### Directory Structure Overview
```
echo/
|  README.md
|  LICENSE.md
|  CMakeLists.txt
|  Various CMake Files
|----build/
     |  echo
     |
|----include/
     |----wiringPi/
          |  Various wiringPi scripts and folders
          |----wiringPi/
               |  wiringPi.c
               |  wiringPi.h
               |  wiringSerial.c
               |  wiringSerial.h
               |  CMakeLists.txt
               |  Various CMake Files
               |  libwiringPi.so
               |  libwiringSerial.so
               |  Extraneous Raspberry Pi libraries
               |
|------opt/
       |  clean_cmake.sh
       |  raspberrypi-tool/
       |  toolchain-rpi.cmake
       |
|------src/
       |  echo.c
```
### Compiling on a Raspberry Pi
 I cross compiled this project on an Ubuntu 17.04 machine, so to recompile it on a Raspberry Pi, follow instructions on  the [wiringPi website](http://wiringpi.com/) after installing the libraries. It may also be necessary to remove the CMake files in the project subdirectories to avoid conflicts.
### Cross Compiling on Ubuntu/Linux
An excellent reference on this subject is [Alex C.U.'s Raspberry Pi Cross Compile Guide](https://medium.com/@au42/the-useful-raspberrypi-cross-compile-guide-ea56054de187), which I  used heavily during this project.

To just recompile the program, use the provided makefile in the project directory:
```
cd path/to/echo/
make clean
make
```

To recompile after editing the makefile or any CMake settings, first remove the files CMake generates (if you're experienced with CMake you probably already know this, but I didn't when I started). To that end, I've placed a cleanup script, clean_cmake.sh, in the opt/ folder. It will remove the typical files CMake creates in whatever directory it is called.

```
cd path/to/cmake/files
path/to/opt/clean_cmake.sh
```

CMake can then be invoked like so:

```
cmake . -DCMAKE_TOOLCHAIN_FILE=path/to/opt/toolchain-rpi.cmake
make
```

## Built With
* [Raspberry Pi](https://www.raspberrypi.org/) - A hobbyist-oriented single-board computer
* [wiringPi](http://wiringpi.com/) - Provides an interface to Raspberry Pi UART pins
* [CMake](https://cmake.org/) - Used to cross compile for Raspberry Pi
*  [Raspbery Pi Toolchain](https://github.com/raspberrypi/tools) - Toolchain with compilers for Raspberry Pi

## Authors
* **Marion Anderson** - *Raspberry Pi code and project organization* - [lmander42](https://github.com/lmander42)

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE.md) file for details

## Acknowledgments
* [The Raspberry Pi Foundation](https://www.raspberrypi.org/) for creating the Raspberry Pi and their documentation.
* [Alex C.U.](https://medium.com/@au42) for his intro to cross compiling for Raspberry Pis.
* [PurpleBooth](https://gist.github.com/PurpleBooth) for the README template on which I based this document.
