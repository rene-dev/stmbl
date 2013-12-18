CC=g++
CFLAGS=-g -c -Wall `pkg-config --cflags --static glfw3`
LDFLAGS=`pkg-config --static --libs glfw3`
SOURCES=graph.cpp shader_utils.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=graph

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(EXECUTABLE)
	rm *.o
