IDIR=lib
CC=gcc
CFLAGS=-g -Wall -I$(IDIR)
TFLAGS=-g -Wall -I$(IDIR)

ODIR=out
SDIR=src


TARGET=tp2virtual

_DEPS = table.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = table.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	mkdir -p $(ODIR) && $(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)


.PHONY: clean

clean:
	rm -f $(ODIR)/*.o
	rm -f $(TARGET)
