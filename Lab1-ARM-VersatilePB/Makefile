CC=arm-none-eabi-
CFLAGS=-mcpu=arm926ej-s 
INCS= -I .
LIBS=

SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)
As=$(wildcard *.s)
AsOBJ=$(As:.s=.o)

TARGET=lab1

all: $(TARGET).bin
		@echo "========== Build is Done =========="

%.o: %.s
		$(CC)as.exe  $(CFLAGS)  $< -o $@

%.o: %.c
		$(CC)gcc.exe $(CFLAGS) $(INCS) -c $< -o $@

$(TARGET).elf: $(OBJ) $(AsOBJ)
		$(CC)ld.exe -T linker_script.ld $(LIBS) $(OBJ) $(AsOBJ) -o $@ -Map=Map_file.map

$(TARGET).bin: $(TARGET).elf
		$(CC)objcopy.exe -O binary $< $@

clean:
		del /F /Q *.o *~ $(TARGET).elf $(TARGET).bin Map_file.map
		@echo "========== Everything clean =========="
		