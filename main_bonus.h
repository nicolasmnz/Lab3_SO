#ifndef MAIN_BONUS_H
#define MAIN_BONUS_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>

/**
 * @brief Lista todos los archivos en un directorio (sin subdirectorios).
 *        Excluye "." y "..". Devuelve nombres simples de archivos.
 * @param rutaDir Ruta al directorio que se quiere recorrer.
 * @param numArchivos Salida: número de archivos leídos.
 * @return char** Lista dinámica de nombres. El llamador debe liberar cada nombre y la lista.
 */
char **listarArchivos_tests(const char* rutaDir, int* numArchivos);

/**
 * @brief Lee una matriz desde archivo. Formato: 
 *        primera línea "row col", luego una línea por fila con valores separados por espacios.
 * @param f Archivo ya abierto para lectura.
 * @param rows Salida: número de filas leídas.
 * @param columns Salida: número de columnas leídas.
 * @return int** Matriz dinámicamente asignada. Debe liberarse con eliminarMatriz().
 */
int **leerMatriz(FILE *f, int *rows, int *columns);

/**
 * @brief Verifica si una matriz es simétrica (i.e., A[i][j] == A[j][i]).
 * @param Matriz Matriz cuadrada a verificar.
 * @param row Número de filas.
 * @param column Número de columnas.
 * @return int 1 si es simétrica, 0 si no lo es.
 */
int esSimetrica(int** Matriz, int row, int column);

/**
 * @brief Escribe una matriz en el directorio ./salidaFork/bonus/
 *        y agrega información de simetría si aplica.
 *        Formato: primera línea con "row column", luego una línea por fila.
 * @param Matriz Matriz a escribir.
 * @param row Número de filas.
 * @param column Número de columnas.
 * @param nombreArchivo Nombre base del archivo (sin ruta).
 */
void escribirMatriz(int** Matriz, int row, int column, const char* nombreArchivo);

/**
 * @brief Multiplica dos matrices A × B usando procesos `fork()` y `pipe`.
 *        Cada proceso calcula una fila de la matriz resultado.
 * @param MatrizA Matriz de entrada A (rowA × columnA).
 * @param MatrizB Matriz de entrada B (rowB × columnB).
 * @param rowA Número de filas de A.
 * @param columnA Número de columnas de A.
 * @param rowB Número de filas de B.
 * @param columnB Número de columnas de B.
 * @return int** Matriz resultado (rowA × columnB). NULL si dimensiones incompatibles.
 */
int **multMatriz(int **MatrizA, int **MatrizB, int rowA, int columnA, int rowB, int columnB);

/**
 * @brief Libera la memoria dinámica de una matriz.
 * @param Matriz Matriz previamente creada.
 * @param rows Número de filas (cada fila se libera).
 */
void eliminarMatriz(int **Matriz, int rows);

#endif // MAIN_BONUS_H
