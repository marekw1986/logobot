# Adjust the run address to match the .org in the source code
all: logobot.hex

logobot.hex: a.out
	bin2hex.py a.out logobot.hex

a.out: interrupt.o vectors.o hd44780.o delay.o io.o cf.o main.o 
	ld65 -C ./lib/ethergeiger.cfg -m main.map interrupt.o vectors.o hd44780.o delay.o io.o cf.o main.o ./lib/ethergeiger.lib	

main.s: main.c
	cc65 -t none -O --cpu 65c02 main.c
	
main.o: main.s
	ca65 --cpu 65c02 main.s
	
delay.o: delay.s
	ca65 --cpu 65c02 delay.s
	
#mc6840.s: mc6840.c
#	cc65 -t none -O --cpu 65c02 mc6840.c	
	
#mc6840.o: mc6840.s
#	ca65 --cpu 65c02 mc6840.s

#mos6551.s: mos6551.c
#	cc65 -t none -O --cpu 65c02 mos6551.c	
	
#mos6551.o: mos6551.s
#	ca65 --cpu 65c02 mos6551.s
	
hd44780.s: hd44780.c
	cc65 -t none -O --cpu 65c02 hd44780.c	
	
hd44780.o: hd44780.s
	ca65 --cpu 65c02 hd44780.s	
	
io.s: io.c
	cc65 -t none -O --cpu 65c02 io.c	

io.o: io.s
	ca65 --cpu 65c02 io.s	
	
cf.s: cf.c
	cc65 -t none -O --cpu 65c02 cf.c	

cf.o: cf.s
	ca65 --cpu 65c02 cf.s			
	
interrupt.o: interrupt.s
	ca65 --cpu 65c02 interrupt.s
	
vectors.o: vectors.s
	ca65 --cpu 65c02 vectors.s	
	
clean:
	$(RM) *.o *.lst *.map *.out *.hex a.out ff.s diskio.s cf.s main.s hd44780.s mc6840.s mos6551.s io.s
