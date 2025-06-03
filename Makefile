CC := gcc
CFLAGS  := -Wall -Wextra -g

SRCS 	:= main.c
TARGET	:= main

JAVAC = javac
JAVA = java
JAVA_SOURCE = Main.java
JAVA_CLASS = Main

.PHONY: all cpp java run-ccc run-java clean

all: cc java

cc: $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

java: $(JAVA_SOURCE)
	$(JAVAC) $(JAVA_SOURCE)

run-cpp: cpp
	./$(CPP_TARGET)

run-java: java
	$(JAVA) $(JAVA_CLASS)

clean:
	rm -f $(CPP_TARGET) *.class
