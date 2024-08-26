CC=arm-none-eabi-
CFLAGS=-mcpu=arm926ej-s -g
INCS= -I .
LIBS=

SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)
As=$(wildcard *.s)
AsOBJ=$(As:.s=.o)

Project-Name=lab1

all: $(Project-Name).bin
		@echo "========== Build is Done =========="

%.o: %.s
		$(CC)as.exe  $(CFLAGS)  $< -o $@

%.o: %.c
		$(CC)gcc.exe $(CFLAGS) $(INCS) -c $< -o $@

$(Project-Name).elf: $(OBJ) $(AsOBJ)
		$(CC)ld.exe -T linker_script.ld $(LIBS) $(OBJ) $(AsOBJ) -o $@ -Map=Map_file.map

$(Project-Name).bin: $(Project-Name).elf
		$(CC)objcopy.exe -O binary $< $@

clean:
		del /F /Q *.o *~ $(Project-Name).elf $(Project-Name).bin Map_file.map
		@echo "========== Everything clean =========="
		