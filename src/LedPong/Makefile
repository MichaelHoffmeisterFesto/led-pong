SOURCES := $(wildcard *.cpp)
SOURCES := $(filter-out Main.cpp, $(SOURCES))

OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))
CXX := g++ -Wall -Wvla -g

# For the main program and SDL
LDFLAGS+=`sdl2-config --cflags --libs` -L/usr/local/lib -lSDL2_mixer

# Where our RGB library resides. You mostly only need to change the
# RGB_LIB_DISTRIBUTION, this is where the library is checked out.
RGB_LIB_DISTRIBUTION=../../..
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread

all: program

program: $(OBJECTS)
       g++ $(OBJECTS) $(LDFLAGS) -o program

%.o : %.cpp
       $(CXX) -I$(RGB_INCDIR) $(CXX_FLAGS) $< -c -o $@

clean:
       rm -rf *.o program