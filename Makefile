# Makefile for OS Memory + Scheduling Simulator

CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall

SRC = main.cpp \
      Process.cpp \
      Scheduler.cpp \
      CPU.cpp \
      TLB.cpp \
      TwoLevelPageTable.cpp \
      Clock.cpp

OBJ = $(SRC:.cpp=.o)

TARGET = simulator

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

