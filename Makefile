NAME=main
OBJS=main.o
CPU=atmega32u4
CC=avr-gcc
F_CPU=16000000ul
CFLAGS=-ansi -Wall -pedantic -O2 -g -mmcu=$(CPU) -DF_CPU=$(F_CPU)
LDFLAGS=-mmcu=$(CPU)

.PHONY:	all prog clean

all:	$(NAME).elf

$(NAME).elf:	$(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

prog:	$(NAME).hex
	avrdude -p $(CPU) -c avr109 -P /dev/serial/by-id/usb-Arduino_srl_Arduino_Leonardo-if00 -U flash:w:$(NAME).hex:i

$(NAME).hex:	$(NAME).elf
	avr-objcopy -j .text -j .data -O ihex $(NAME).elf $(NAME).hex

$(NAME).lst:	$(NAME).elf
	avr-objdump -S $(NAME).elf >$@

$(OBJS):	types.h saddle.h

saddle.h:	saddle.awk
	awk -f $< - >$@

clean:
	rm -f $(NAME).elf $(NAME).hex $(NAME).lst $(OBJS)
