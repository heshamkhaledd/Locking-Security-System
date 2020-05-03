MCU=atmega16
F_CPU=8000000
CC=avr-gcc
OBJCOPY=avr-objcopy
ELF=elf_out
MAP=map_out
SIZE=avr-size
CFLAGS=-Wl,-Map,out/${MAP}.map -mmcu=${MCU} -DF_CPU=${F_CPU} -Iinc -O1
TARGET=hex_out
SRCS=*.c

all:
	@echo 'Building Target: $@'
	@echo 'Invoking: AVR C Linker'
	mkdir -p out
	time ${CC} ${CFLAGS} -o "out/${ELF}.elf" ${SRCS}
	${OBJCOPY} -j .text -j .data -O ihex out/${ELF}.elf out/${TARGET}.hex
	@echo 'Finished building target: $@'
    
flash:
	avrdude -p ${MCU} -c usbasp -U flash:w:${TTARGET}.hex:i -F -P usb

clean:
	rm -f *.bin *.hex