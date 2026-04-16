CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -pthread
LDFLAGS = -pthread

# Source files
SOURCES = bmp_server.cpp main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = bmp_server

# Default target
all: $(TARGET)

# Build executable
$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^

# Compile source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)

# Run the server
run: $(TARGET)
	./$(TARGET)

# Run with custom port
run-port: $(TARGET)
	./$(TARGET) 11019

# Phony targets
.PHONY: all clean run run-port

# Dependencies
bmp_server.o: bmp_server.cpp bmp_server.h
main.o: main.cpp bmp_server.h
