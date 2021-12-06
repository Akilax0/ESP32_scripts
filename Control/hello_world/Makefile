#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := hello-world

PORT = /dev/ttyUSB0


# include $(IDF_PATH)/make/project.mk

config:
	get_idf

flash: 
	idf.py -p $(PORT) -b 921600 flash

monitor:
	idf.py -p $(PORT) monitor
