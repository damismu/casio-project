BINARY = aichat.g3a
OBJECTS = main.o

# Configuración para Casio fx-CG50
TARGET = fxcg50
AS = sh-elf-as
CC = sh-elf-gcc
LD = sh-elf-gcc
OBJCOPY = sh-elf-objcopy

include $(GINT_SDK)/etc/Makefile.common