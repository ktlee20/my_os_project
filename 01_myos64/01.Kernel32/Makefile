all: Kernel32.bin
# all: VirtualOS.bin

Kernel32.bin: Source/EntryPoint.s
	nasm -o Kernel32.bin $<
	# $<(the first file of depencency list) <=> Source/Entrypoint.s

#VirtualOS.bin: VirtualOS.asm
#	nasm -o VirtualOS.bin VirtualOS.asm

clean:
	rm -f Kernel32.bin
	# rm -f VirtualOS.bin
