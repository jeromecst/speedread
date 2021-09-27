# Customize below to fit your system

# paths
PREFIX = /usr/local

PKG_CONFIG = pkg-config

# includes and libs
LIBS = -lpthread \
       `$(PKG_CONFIG) --libs ncursesw`

# flags
LDFLAGS = $(LIBS)
CFLAGS = $(INCS) $(STCPPFLAGS) $(CPPFLAGS)

# compiler and linker
CC = gcc
FLEX = flex
