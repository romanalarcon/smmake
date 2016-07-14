CC=g++
CFLAGS=-c -Wno-write-strings -Wreturn-type -Wall
LDFLAGS=
SOURCES=LightControllerBaseState.cpp LightControllerLampOff.cpp LightControllerConfigurator.cpp LightControllerLampOn.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE="lightController"
LIBPATH=
INCPATH=
LIBS=

all: $(SOURCES) $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)
	rm -f $(OBJECTS)
	rm -f *.cpp~
	rm -f *.h~

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBPATH) $(LIBS) 

.cpp.o:
	$(CC) $(INCPATH) $(CFLAGS) $< -o $@
