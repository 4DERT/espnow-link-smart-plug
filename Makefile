#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := espnow-link-smart-plug

EXTRA_COMPONENT_DIRS := $(CURDIR)/components

include $(IDF_PATH)/make/project.mk

