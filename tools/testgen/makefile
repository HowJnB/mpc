OPT = -pedantic -Wall -Werror -g --std=c99
CC = gcc
DIR = .

write_test_frsub : write_test_frsub.o
	$(CC) -o $(DIR)/write_test_frsub write_test_frsub.o $(OPT)

write_test_subfr : write_test_subfr.o
	$(CC) -o $(DIR)/write_test_subfr write_test_subfr.o $(OPT)

write_test_frdiv : write_test_frdiv.o
	$(CC) -o $(DIR)/write_test_frdiv write_test_frdiv.o $(OPT)

write_test_div : write_test_div.o
	$(CC) -o $(DIR)/write_test_div write_test_div.o $(OPT)

write_test_divfr : write_test_divfr.o
	$(CC) -o $(DIR)/write_test_divfr write_test_divfr.o $(OPT)

write_test_norm : write_test_norm.o
	$(CC) -o $(DIR)/write_test_norm write_test_norm.o $(OPT)

write_test_mulfr : write_test_mulfr.o
	$(CC) -o $(DIR)/write_test_mulfr write_test_mulfr.o $(OPT)

%.o : %.c write_test.h
	$(CC) $< -c -o $@ $(OPT)

clean :
	rm -f *.o *~
