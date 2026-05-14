CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -O2
LDFLAGS = -lm

TARGET = dd_pow

SOURCES = main.cpp dd.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

main.o: main.cpp dd.h utils.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o
	
dd.o: dd.cpp dd.h exact_math.h
	$(CXX) $(CXXFLAGS) -c dd.cpp -o dd.o

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run