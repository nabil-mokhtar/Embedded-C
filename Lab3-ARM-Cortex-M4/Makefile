CC=arm-none-eabi-
CFLAGS=-mcpu=cortex-m4   -gdwarf-2 -g
INCS= -I .
LIBS=

SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)
As=$(wildcard *.s)
AsOBJ=$(As:.s=.o)

TARGET=lab3

all: $(TARGET).bin
		@echo "========== Build is Done =========="

%.o: %.s
		$(CC)as.exe  $(CFLAGS)  $< -o $@

%.o: %.c
		$(CC)gcc.exe $(CFLAGS) $(INCS) -c $< -o $@

$(TARGET).elf: $(OBJ) $(AsOBJ)
		$(CC)ld.exe -T linker_script.ld $(LIBS) $(OBJ) $(AsOBJ) -o $@ -Map=Map_file.map
		copy $(TARGET).elf $(TARGET).axf

$(TARGET).bin: $(TARGET).elf
		$(CC)objcopy.exe -O binary $< $@

clean:
		del /F /Q *.o *~ $(TARGET).elf $(TARGET).bin Map_file.map
		@echo "========== Everything clean =========="
		