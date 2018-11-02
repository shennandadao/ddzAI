SOURCES = $(wildcard *.cpp ./json/*.cpp)
GCC := g++
TARGET := libGameAI.so
CFLAGE := -std=c++11 -Wall -g  #-I./lua/ -L./lua/ -llua -ldl

SHARE	:= -fPIC --shared -o
all: $(SOURCES)
	g++ $(SOURCES) -std=c++11 -Wall -g -ldl  -fPIC --shared -o libGameAI.so

.PHONY:clean
clean: $(TARGET)
	$(RM) $?












