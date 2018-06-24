ifeq ($(OS),Windows_NT)
	CLEANUP          = del /F /Q
	MKDIR            = mkdir
	TARGET_EXTENSION = exe
else
	CLEANUP          = rm -f
	MKDIR            = mkdir -p
	TARGET_EXTENSION = out
endif

PATH_UNITY   = vendor/unity
PATH_SOURCE  = src/
PATH_TEST    = test/
PATH_BUILD   = build/
PATH_DEPENDS = build/depends/
PATH_OBJECTS = build/objects/
PATH_RESULTS = build/results/

BUILD_PATHS  = $(PATH_BUILD) $(PATH_DEPENDS) $(PATH_OBJECTS) $(PATH_RESULTS)

SRC_TEST     = $(wildcard $(PATH_TEST)*.c)

CC           = gcc
LINK         = $(CC)
DEPEND       = $(CC) -MM -MG -MF
CFLAGS       = -g -Wall -std=gnu11 -I. -I$(PATH_UNITY) -I$(PATH_SOURCE) -DTEST

