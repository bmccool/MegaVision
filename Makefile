CC = g++
CFLAGS = -c -Wall `pkg-config --cflags opencv`
LDFLAGS = `pkg-config --libs opencv`
SOURCES = main.cpp HW_camera.cpp IP_ledge_detect.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = BeagleVision

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@