LOCAL_PATH=`pwd`
CC = gcc
CPP = g++
LD = gcc
CFLAGS = -I../include
LFLAGS = -L../lib -lwiringPi -lpi3
TARGET = a
SOURCES = $(wildcard *.c *.cpp)
OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))

$(TARGET) : $(OBJS)
	$(LD) $(OBJS) $(LFLAGS) -o $(TARGET)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o : %.cpp
	$(CPP) $(CFLAGS) -c $< -o $@

.PHONY : clean distclean cscope run
clean:
	rm -f *.o $(TARGET)

distclean:
	rm -rf cscope.*
	find ./ -iname "*.o" | xargs rm -rf
	find ./ -iname "$(TARGET)" | xargs rm -rf

cscope:
	cscope -Rbq -f cscope.out

run:
	@-./$(TARGET)
