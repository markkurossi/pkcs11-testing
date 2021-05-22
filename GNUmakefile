
HDRS := $(wildcard src/*.h)
SRCS := $(wildcard src/*.c)

all: pkcs11-testing

pkcs11-testing: $(HDRS) $(SRCS) config.h
	cc -I. -Isrc -Isrc/cryptoki_compat -o $@ $(SRCS)

clean:
	rm pkcs11-testing
