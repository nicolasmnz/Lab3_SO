CC 		:= gcc
CFLAGS  := -Wall -Wextra -g

SRCS 	:= main.c
TARGET	:= main

JAVAC = javac
JAVA = java
JAVA_SOURCE = Main.java
JAVA_CLASS = Main

.PHONY: all cc java run-c run-java clean

all: cc java

cc: $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

java: $(JAVA_SOURCE)
	$(JAVAC) $(JAVA_SOURCE)

c-run: cc
	./$(TARGET)

java-run: java
	$(JAVA) $(JAVA_CLASS)

clean:
	rm -f $(CPP_TARGET) *.class
