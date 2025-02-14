# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = `pkg-config --cflags opencv4 tesseract` -I/usr/local/include -I/usr/include -pg

# Linker flags
LDFLAGS = `pkg-config --libs opencv4 tesseract` -lwiringPi -lpthread -llept -ltesseract -pg

# Source files
SRCS = main.cpp reconhecer.cpp aplicar_ocr.cpp processar_contornos.cpp processar_imagem.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Output executable
TARGET = main.out

# Default target
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(TARGET) $(OBJS) gmon.out

# Clear the log
clear_log:
	> log.txt