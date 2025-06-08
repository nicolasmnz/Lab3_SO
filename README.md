# tarea-Grupo11

Nombre: Joaquin Rojo Olea       - 202173550-k   
Nombre: Nicolás Muñoz Ramírez   - 202104641-0  

## Resumen
La tarea es hacer un informe para responder una serie de preguntas sobre la diferencias entre el uso `Fork()` y el `Thread()` para multiplicar matrices. Usandos diferntes codigos para poder comparar la efeciencia de estos diferentes metodos.
## Contenido
* `LAB3_Munoz_Rojo.c` - Código fuente principal en C
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
* Todos los archivos siguen el formato indicado, más se le agrega una linea vacía en el término de estos.



# Configuración de la tarea

## Entorno de desarrollo
* Entorno virtualizado WSL 2.4.12.0 en Windows 11 24H2
* Ubuntu 22.04 LTS with Windows Subsystem for Linux
* Compilador gcc 13.3.0 con soporte para `C11`
* Compilador javac 21.0.7
* Herramienta GNU Make 4.3

## Ejecutar
```
# Ejecución (y compilación si se requiere)
make run
```
## Estructura
```
├── LAB3_Munoz_Rojo.c      # Lógica principal
├── LAB3_Munoz_Rojo.java   # Código fuente principal en java
├── Makefile               # Reglas de compilación y ejecución
└── README.md              # Este documento
```
