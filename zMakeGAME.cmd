del SKAZKA.sc

sdasz80 -l -o ipl.s
sdasz80 -l -o psg.s
sdasz80 -l -o vdp.s
sdasz80 -l -o ppi.s
sdasz80 -l -o kbd.s

sdcc -mz80 -c --code-loc 0x0100 --data-loc 0xC100 --xram-loc 0xC100 main.c

sdcc -mz80 --no-std-crt0 --code-loc 0x0100 --data-loc 0xC100 --xram-loc 0xC000 ipl.rel psg.rel vdp.rel ppi.rel kbd.rel main.rel 

packihx ipl.ihx > ipl.hex
tools\hex2bin -p FF -s 0 -l 4000 -e rom ipl.hex

copy ipl.rom SKAZKA.sc

pause

