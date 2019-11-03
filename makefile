pr1.out: driver.o blockTable.o directory.o
	gcc -o pr1.out driver.o blockTable.o directory.o

driver.o: driver.c
	gcc -c driver.c

blockTable.o: blockTable.c blockTable.h
	gcc -c blockTable.c

directory.o: directory.c directory.h
	gcc -c directory.c
