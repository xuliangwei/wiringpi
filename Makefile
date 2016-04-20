LOCAL_PATH=`pwd`
CC = gcc
CPP = g++
CFLAGS = -g -I$(LOCAL_PATH)/../include
LFLAGS = -lwiringPi
TARGET = a
SOURCES = $(wildcard *.c *.cpp)
OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))

$(TARGET) : $(OBJS)
	$(CPP) $(LFLAGS) $(OBJS) -o $(TARGET)

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
