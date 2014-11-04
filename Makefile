CC = g++
CFLAGS = -c -g -Wall `pkg-config --cflags opencv`
LDFLAGS = `pkg-config --libs opencv` -lX11
SOURCES = main.cpp HW_camera.cpp IP_ledge_detect.cpp CS_time.cpp CT_window_manager.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = BeagleVision

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) $(EXECUTABLE) *.o *~
