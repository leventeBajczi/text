text:	*.c *.h text/*.c text/*/*.h text/*/*.c text/*/*/*.h text/*/*/*.s
		gcc -c text/*/*/*.s
		gcc *.c *.h text/*.c text/*/*.h text/*/*.c text/*/*/*.h -o build/text
		build/text
		rm *.o

gdb:	*.c *.h text/*.c text/*/*.h text/*/*.c text/*/*/*.h text/*/*/*.s
		gcc -c text/*/*/*.s -g
		gcc *.c *.h text/*.c text/*/*.h text/*/*.c text/*/*/*.h -o build/text -g
		gdb build/text
		rm *.o