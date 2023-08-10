TARGET = meuAlocador
PRINCIPAL = avalia

all: $(TARGET)

$(TARGET): $(TARGET).s $(PRINCIPAL).c
	as $(TARGET).s -o $(TARGET).o -g
	gcc -g -c $(PRINCIPAL).c -o $(PRINCIPAL).o
	ld $(TARGET).o $(PRINCIPAL).o -o $(TARGET)  -dynamic-linker /lib/x86_64-linux-gnu/ld-linux-x86-64.so.2 \/usr/lib/x86_64-linux-gnu/crt1.o  /usr/lib/x86_64-linux-gnu/crti.o \/usr/lib/x86_64-linux-gnu/crtn.o -lc

purge: clean
	rm -f $(TARGET)

clean: 
	rm -f *.o

run:
	./$(TARGET)
