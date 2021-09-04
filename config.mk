# Customize below to fit your system

# paths
PREFIX = /usr/local

PKG_CONFIG = pkg-config

# includes and libs
LIBS = -lpthread \
       `$(PKG_CONFIG) --libs ncursesw`

# flags
STLDFLAGS = $(LIBS)

# compiler and linker
CC = gcc
FLEX = lex
# uncomment if runing debian based distro
#FLEX = flex
