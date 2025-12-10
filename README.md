# Description
This repository is derived from [raspberrypi/utils](https://github.com/raspberrypi/utils), and further encapsulates the ws2812, pwm, and servo libraries based on this.

****
The specific modifications are as follows:
1. Added [freenove_ws2812.h](file://e:\GitHub\utils\piolib\examples\freenove_ws2812.h) and [freenove_ws2812.c](file://e:\GitHub\utils\piolib\examples\freenove_ws2812.c) in `utils/piolib/examples` for the freenove_ws2812 library
2. Added [freenove_pwm.h](file://e:\GitHub\utils\piolib\examples\freenove_pwm.h) and [freenove_pwm.c](file://e:\GitHub\utils\piolib\examples\freenove_pwm.c) in `utils/piolib/examples` for the freenove_pwm library
3. Modified [CMakeLists.txt](file://e:\GitHub\utils\CMakeLists.txt) in `utils/piolib/examples` to generate shared libraries and test files
4. Added [setup.py](file://e:\GitHub\utils\python\setup.py) in `utils/python` to set up the Python environment, compile code and generate shared libraries
5. Added [piolib_ws2812.py](file://e:\GitHub\utils\python\piolib_ws2812.py) in `utils/python` for the freenove_ws2812 library
6. Added [piolib_pwm.py](file://e:\GitHub\utils\python\piolib_pwm.py) in `utils/python` for the freenove_pwm library
7. Added [piolib_servo.py](file://e:\GitHub\utils\python\piolib_servo.py) in `utils/python` for the freenove_servo library
8. Added [ws2812_demo.py](file://e:\GitHub\utils\python\ws2812_demo.py), [pwm_demo.py](file://e:\GitHub\utils\python\pwm_demo.py) and [servo_demo.py](file://e:\GitHub\utils\python\servo_demo.py) in `utils/python` to test the freenove_ws2812, freenove_pwm, and freenove_servo libraries

# Usage
1. Clone the repository: `cd ~ && git clone https://github.com/Zhentao-Lin/utils.git`
2. Install build tools, compile the code to generate shared libraries, and install the shared libraries to the Raspberry Pi 5 directory: `cd utils/python/ && sudo python setup.py`
3. Test: 
* Navigate to the python directory: `cd ~/utils/python`
* Test WS2812 LED driver: `python ws2812_demo.py`
* Test PWM driver: `python pwm_demo.py`
* Test servo driver: `python servo_demo.py`

# utils
A collection of scripts and simple applications

* [dtmerge](dtmerge/) - A tool for applying compiled DT overlays (`*.dtbo`) to base Device
    Tree files (`*.dtb`). Also includes the `dtoverlay` and `dtparam` utilities.
* [eeptools](eeptools/) - Tools for creating and managing EEPROMs for HAT+ and HAT board.
* [kdtc](kdtc/) - A tool for compiling overlays with #includes, etc., as used in the kernel tree.
* [otpset](otpset/) - A short script to help with reading and setting the customer OTP
    bits.
* [overlaycheck](overlaycheck/) - A tool for validating the overlay files and README in a
    kernel source tree.
* [ovmerge](ovmerge/) - A tool for merging DT overlay source files (`*-overlay.dts`),
    flattening and sorting `.dts` files for easy comparison, displaying
    the include tree, etc.
* [pinctrl](pinctrl/) - A more powerful replacement for raspi-gpio, a tool for
    displaying and modifying the GPIO and pin muxing state of a system, bypassing
    the kernel.
* [piolib](piolib/) - A library for accessing the Pi 5's PIO hardware.
* [raspinfo](raspinfo/) - A short script to dump information about the Pi. Intended for
    the submission of bug reports.
* [rpifwcrypto](rpifwcrypto/) - A command line application and shared library for the
    firmware cryptography service. Intended for use with Raspberry Pi Connect and
    secure-boot provisioner.
* [vclog](vclog/) - A tool to get VideoCore 'assert' or 'msg' logs
    with optional -f to wait for new logs to arrive.


**Build Instructions**

Install the prerequisites with "sudo apt install cmake device-tree-compiler libfdt-dev libgnutls28-dev" - you need at least version 3.10 of cmake. Run the following commands to build and install everything, or see the README files in the subdirectories to just build utilities individually:

 - *cmake .*
    N.B. Use *cmake -DBUILD_SHARED_LIBS=1 .* to build the libraries in the subprojects (libdtovl, gpiolib and piolib) as shared (as opposed to static) libraries.
 - *make*
 - *sudo make install*
