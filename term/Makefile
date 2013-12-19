CC=g++
STLINK=../../stlink
CFLAGS=-g -c -Wall `pkg-config --cflags --static glfw3` -I$(STLINK)/src/ `pkg-config --cflags libusb` -Wno-write-strings
LDFLAGS=`pkg-config --static --libs glfw3` `pkg-config --static --libs libusb`
SOURCES=graph.cpp shader_utils.cpp
OBJECTS=$(SOURCES:.cpp=.o) $(STLINK)/src/libstlink_a-stlink-common.o $(STLINK)/src/libstlink_a-stlink-usb.o $(STLINK)/src/libstlink_a-uglylogging.o $(STLINK)/src/libstlink_a-stlink-sg.o
EXECUTABLE=graph

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(EXECUTABLE)
	rm *.o
