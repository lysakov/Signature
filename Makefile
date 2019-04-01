CC = g++
CFLAGS = -Iinc -Wall -Werror -pedantic-errors -Wextra -std=c++17 -ftrapv -O2
OBJECTS_HASH = bin/functions.o bin/mappings.o bin/hash.o
OBJECTS = bin/main.o bin/modular_arithmetic.o bin/elliptic_curve.o bin/rand.o bin/signature.o
EXEC1 = main.out
PRIV_KEY = genpkey.out
PUB_KEY = convpkey.out
SIGN = sign.out
VERIFY = verify.out
OBJECTS_PRIV_KEY = bin/modular_arithmetic.o bin/elliptic_curve.o bin/genpkey.o bin/rand.o
OBJECTS_PUB_KEY = bin/modular_arithmetic.o bin/elliptic_curve.o bin/convpkey.o bin/rand.o
OBJECTS_SIGN = bin/modular_arithmetic.o bin/elliptic_curve.o bin/rand.o bin/signature.o bin/sign.o
OBJECTS_VERIFY = bin/modular_arithmetic.o bin/elliptic_curve.o bin/rand.o bin/signature.o bin/verify.o
BLOCK = inc/block.hpp inc/block.cpp

all : $(EXEC1) $(PRIV_KEY) $(PUB_KEY) $(SIGN) $(VERIFY)

$(EXEC1) : $(OBJECTS) $(OBJECTS_HASH)
	$(CC) $(OBJECTS) $(OBJECTS_HASH) -o $(EXEC1)

$(PRIV_KEY) : $(OBJECTS_PRIV_KEY)
	$(CC) $(OBJECTS_PRIV_KEY) -o $(PRIV_KEY)

$(PUB_KEY) : $(OBJECTS_PUB_KEY)
	$(CC) $(OBJECTS_PUB_KEY) -o $(PUB_KEY)

$(SIGN) : $(OBJECTS_SIGN) $(OBJECTS_HASH)
	$(CC) $(OBJECTS_SIGN) $(OBJECTS_HASH) -o $(SIGN)

$(VERIFY) : $(OBJECTS_VERIFY) $(OBJECTS_HASH)
	$(CC) $(OBJECTS_VERIFY) $(OBJECTS_HASH) -o $(VERIFY)

bin/main.o : src/main.cpp inc/functions.hpp inc/hash.hpp inc/modular_arithmetic.hpp inc/elliptic_curve.hpp inc/signature.hpp $(BLOCK)
	$(CC) $< $(CFLAGS) -c -o $@

bin/functions.o : src/functions.cpp inc/functions.hpp $(BLOCK)
	$(CC) $< $(CFLAGS) -c -o $@

bin/mappings.o : src/hash/mappings.cpp inc/hash.hpp $(BLOCK)
	$(CC) $< $(CFLAGS) -c -o $@

bin/hash.o : src/hash/hash.cpp inc/hash.hpp $(BLOCK)
	$(CC) $< $(CFLAGS) -c -o $@

bin/modular_arithmetic.o : src/modular_arithmetic.cpp inc/modular_arithmetic.hpp $(BLOCK)
	$(CC) $< $(CFLAGS) -c -o $@

bin/elliptic_curve.o : src/elliptic_curve.cpp inc/elliptic_curve.hpp inc/modular_arithmetic.hpp $(BLOCK)
	$(CC) $< $(CFLAGS) -c -o $@

bin/signature.o : src/signature.cpp inc/signature.hpp inc/elliptic_curve.hpp $(BLOCK)
	$(CC) $< $(CFLAGS) -c -o $@

bin/rand.o : src/rand.c
	gcc $< -Iinc -c -o $@

bin/genpkey.o : src/genpkey.cpp inc/elliptic_curve.hpp $(BLOCK)
	$(CC) $< $(CFLAGS) -c -o $@

bin/convpkey.o : src/convpkey.cpp inc/elliptic_curve.hpp $(BLOCK)
	$(CC) $< $(CFLAGS) -c -o $@

bin/sign.o : src/sign.cpp inc/signature.hpp $(BLOCK)
	$(CC) $< $(CFLAGS) -c -o $@

bin/verify.o : src/verify.cpp inc/signature.hpp $(BLOCK)
	$(CC) $< $(CFLAGS) -c -o $@

clean :
	rm -f *.out bin/*.o *.txt keys/*.pub signatures/*.crt
	rm -rf *.dSYM
.PHONY : clean
