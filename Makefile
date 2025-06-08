CC 		  := gcc
CFLAGS    := -Wall -Wextra -g

JAVAC 	  := javac
JAVA 	  := java
#--------------------------------------------
C_SRCS    := main.c main_bonus.c
C_OBJS    := $(C_SRCS:.c=.o)
C_HDRS    := main.h main_bonus.h

C_TARGETS := main main_bonus
#-------------------------------------------
.PHONY: all cc java run-c run-bonus-c run-java run-bonus-java clean clean-outputs

all: cc java
#------------------------------------------
cc: $(C_TARGETS)

main: main.o
	$(CC) $(CFLAGS) -o $@ $^ 

main_bonus: main_bonus.o
	$(CC) $(CFLAGS) -o $@ $^

# Regla para compilar cualquier .c a .o
%.o: %.c $(C_HDRS)
	$(CC) $(CFLAGS) -c $< -o $@
#------------------------------------------
JAVA_SRCS   := Main.java
JAVA_CLASSES:= $(JAVA_SRCS:.java=.class)
#------------------------------------------
java: $(JAVA_SRCS)
	$(JAVAC) $(JAVA_SRCS)
#-----------------------------------------
run-c: main
	./main

run-bonus-c: main_bonus
	./main_bonus

run-java: $(JAVA_CLASSES)
	$(JAVA) Main
#-----------------------------------------
clean:
	rm -f *.o $(C_TARGETS) *.class

clean-outputs:
	rm -f ./salidaFork/*.txt ./salidaFork/bonus/*.txt || true