text:	*.c *.h text/*.c  text/*/*.c text/*/*/*.s
		gcc -c text/*/*/*.s
		gcc -I. *.c text/*.c  text/*/*.c *.o -o build/text -lgcrypt
		build/text
		rm *.o

gdb:	*.c *.h text/*.c  text/*/*.c text/*/*/*.s
		gcc -c text/*/*/*.s -g
		gcc -g -I. *.c text/*.c  text/*/*.c *.o -o build/text -lgcrypt
		gdb build/text
		rm *.o