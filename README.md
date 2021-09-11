
Setting up ESP32 IDF

git clone --recursive https://github.com/espressif/esp-idf.git
./install.sh

. ./export.sh 
Can be set on bashrc for an alias(get_idf)

idf.py set-target esp32
idf.py menuconfig

idf.py -p /dev/ttyUSB0 -b 921600 flash

idf.py -p /dev/ttyUSB0 monitor
<Ctrl> + ] <- To exit

Updating can be done by removing esp-idf folder and running above steps again



========================================================================================

Creating a new project
	idf.py create-project "name"

Inside the project 
	idf.py set-target esp32

Setup and compile features for board
	idf.py build

	This is for initializing os (compiling)
	But every time we modify only compiles the application ontop
