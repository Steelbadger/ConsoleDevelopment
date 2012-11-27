TARGET = main

CC = g++
INCPATH=-I./PS2Framework/include \
		-I./Classes/include
CFLAGS = -g -Wall $(INCPATH)

.SUFFIXES: .cpp .o

OBJS = 	PS2Framework/src/primitives.o \
		PS2Framework/src/ps2maths.o \
		PS2Framework/src/ps2matrix4x4.o \
		PS2Framework/src/ps2vector4.o \
		PS2Framework/src/pad.o \
		PS2Framework/src/dma.o \
		PS2Framework/src/sps2wrap.o \
		PS2Framework/src/texture.o \
		PS2Framework/src/font.o \
		Classes/src/Board.o \
		Classes/src/Position.o \
		Classes/src/MorrisManager.o \
		Classes/src/Piece.o \
		Classes/src/Player.o \
		Classes/src/Cursor.o \
		Classes/src/3dSprite.o \
		Classes/src/3dCircleSprite.o \
		MyPS2Application.o \
		main.o

LIBS = -lm -lsps2util

LIBPATH=-L./

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBPATH) $(LIBS)

.cpp.o:
	$(CC) -c $<  $(CFLAGS) -o $@
	
clean:
	rm -f $(TARGET)
	rm -f $(OBJS)
	rm -f depend.mk
	
submission: $(TARGET)
	rm -f $(OBJS)
	rm -f depend.mk
	
run: $(TARGET)
	./$(TARGET)


# create the dependancy file	
depend:
	$(CC) $(CFLAGS) -MM *.cpp > depend.mk

	
#include the dependencies into the build
-include depend.mk