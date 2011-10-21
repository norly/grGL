CINCLUDE=
#CWARNS=-Wall -Wpointer-arith -Wmissing-prototypes -Wmissing-declarations -Wnested-externs
CWARNS=-Wall -Wpointer-arith -Wnested-externs
CFLAGS=$(CINCLUDE) $(CWARNS) -O2
LIBS=-lglut -lGL -lGLU

SRCDIR=src
SRCFILES=*.c
BINFILE=build/grGL

$(BINFILE): src/*.c src/*.h Makefile
	mkdir -p build
	gcc $(CFLAGS) $(LIBS) -o $(BINFILE) src/*.c

clean:
	rm -rf build

run: $(BINFILE)
	build/grGL
