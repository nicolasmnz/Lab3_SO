# tarea-Grupo11

Nombre: Joaquin Rojo Olea       - 202173550-k   
Nombre: Nicolás Muñoz Ramírez   - 202104641-0  

## Resumen
La tarea es hacer un informe para responder una serie de preguntas sobre la diferencias entre el uso `Fork()` y el `Thread()` para multiplicar matrices. Usandos diferntes codigos para poder comparar la efeciencia de estos diferentes metodos.
## Contenido
* `LAB3_Munoz_Rojo.c` - Código fuente principal en C
* `LAB3_Munoz_Rojo_b.c` - Código fuente principal del bonus en C
* `LAB3_Munoz_Rojo.h` - Código fuente principal en h
* `LAB3_Munoz_Rojo_b.h` - Código fuente principal en h
  
* `LAB3_Munoz_Rojo.java` - Código fuente principal en java
* `LAB3_Munoz_Rojo_b.java` - Código fuente principal para el Bonus en java
* `Makefile` - Script de compilación (`make`, `make run`, `make clean`)
* `Readme.md` - Documentación



## Características
* Usando librerias de lecturas de archivos vamos a extraer multiples matrices 
* Se hace una comprobacion de las dimensiones de las matrices.
* Usando `Fork()` o `Thread` dependiendo del caso para resolver una o multiples filas 
* Usando librerias para por medir el tiempo de ejecucion de la matriz

## Supuestos
* NO habran matrices de 0x0
* Todos lo archivos tendran al menos 2 matrices
* Todos los archivos siguen el formato indicado en la tarea.
* Para la ejecucion de los programas originales (no bonus) los casos de pruebas deben iniciar directamente con la primera matriz
* La carpetas de salidaThread y salidaFork deben contener el una subcarpeta llamada bonus
* Los nombres de los archivos de pruebas deben ser "numero".txt y los del bonus deben estar en la carpeta .pruebas.bonus con el mismo nombre 




# Configuración de la tarea

## Entorno de desarrollo
* Entorno virtualizado WSL 2.4.12.0 en Windows 11 24H2
* Ubuntu 22.04 LTS with Windows Subsystem for Linux
* Compilador gcc 13.3.0 con soporte para `C11`
* Compilador javac 21.0.7
* Herramienta GNU Make 4.3

## Ejecutar
```
# Ejecución (y compilación si se requiere) con los siguientes comandos
	all               Compila C y Java
	cprog             Sólo compila el código C
	javaprog          Sólo compila el código Java"
	run-c             Ejecuta el programa principal en C
	run-bonus-c       Ejecuta el programa bonus en C
	run-java          Ejecuta el programa principal en Java
	run-bonus-java    Ejecuta el programa bonus en Java
	clean             Elimina objetos, binarios y clases compiladas
	clean-outputs     Elimina archivos de salida generados
	help              Muestra esta ayuda
```
## Estructura
```
├── c
  ├── LAB3_Munoz_Rojo.c      # Lógica principal
  ├──LAB3_Munoz_Rojo.h
  ├──LAB3_Munoz_Rojo_b.c
  ├──LAB3_Munoz_Rojo_b.h
├── java
  ├── LAB3_Munoz_Rojo.java   # Código fuente principal en java
  ├── LAB3_Munoz_Rojo_b.java   # Código fuente principal en java del bonus
├──Pruebas
   ├──bonus
├──salidaFork #contiene una carpeta llama en bonus y es donde se guarda los archivos con las respuestas
├──salidaThreard #contiene una carpeta llama en bonus y es donde se guarda los archivos con las respuestas
├──measurements
├── Makefile               # Reglas de compilación y ejecución
├── LAB_3_Munoz_Rojo_informe.pdf # Informe solicitado en la tarea 
└── README.md              # Este documento
