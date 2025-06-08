CC 		  := gcc
CFLAGS    := -Wall -Wextra -g

JAVAC 	  := javac
JAVA 	  := java
#--------------------------------------------
C_SRCS     := $(wildcard c/*.c)
C_HDRS     := $(wildcard c/*.h)
C_OBJS     := $(C_SRCS:.c=.o)
C_TARGETS  := $(C_SRCS:.c=) 
#-------------------------------------------
JAVA_SRCS    := $(wildcard java/*.java)    
JAVA_CLASSES := $(JAVA_SRCS:.java=.class)   
#-------------------------------------------
.PHONY: all cprog javaprog run-c run-bonus-c run-java run-bonus-java clean clean-outputs
#-------------------------------------------
all: cprog javaprog
#------------------------------------------
cprog: $(C_TARGETS)

$(C_TARGETS): %: %.o
	$(CC) $(CFLAGS) -o $@ $<

# Regla para compilar cualquier .c a .o
%.o: %.c $(C_HDRS)
	$(CC) $(CFLAGS) -c $< -o $@
#------------------------------------------
javaprog: $(JAVA_CLASSES)

# Compila cada .java y deja la .class en java/
java/%.class: java/%.java
	$(JAVAC) -d java $<
#-----------------------------------------
run-c: c/LAB3_Munoz_Rojo
	@echo "Ejecutando LAB3_Munoz_Rojo.c:"; ./c/LAB3_Munoz_Rojo

run-bonus-c: c/LAB3_Munoz_Rojo_b
	@echo "Ejecutando LAB3_Munoz_Rojo_b.c:"; ./c/LAB3_Munoz_Rojo_b

run-java: javaprog
	@echo "Ejecutando LAB3_Munoz_Rojo.java:"; $(JAVA) -cp java LAB3_Munoz_Rojo

run-bonus-java: javaprog
	@echo "Ejecutando LAB3_Munoz_Rojo_b.java:"; $(JAVA) -cp java LAB3_Munoz_Rojo_b


#-----------------------------------------
clean:
	rm -f ./c/*.o $(C_TARGETS) ./java/*.class

clean-outputs:
	rm -f ./salidaFork/*.txt ./salidaFork/bonus/*.txt || true
#-------------------------------------------
help:
	@echo "Uso: make [target]"
	@echo ""
	@echo "Targets disponibles:"
	@echo "  all               Compila C y Java"
	@echo "  cprog             Sólo compila el código C"
	@echo "  javaprog          Sólo compila el código Java"
	@echo "  run-c             Ejecuta el programa principal en C"
	@echo "  run-bonus-c       Ejecuta el programa bonus en C"
	@echo "  run-java          Ejecuta el programa principal en Java"
	@echo "  run-bonus-java    Ejecuta el programa bonus en Java"
	@echo "  clean             Elimina objetos, binarios y clases compiladas"
	@echo "  clean-outputs     Elimina archivos de salida generados"
	@echo "  help              Muestra esta ayuda"
