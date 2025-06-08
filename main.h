#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <limits.h> 
#include <sys/types.h>
#include <sys/stat.h> 
#include <sys/resource.h>  
#include <sys/wait.h>

/**
 * @brief Calcula la diferencia en segundos entre dos instantes de tiempo.
 *        Utiliza un reloj monotónico para medir tiempos de ejecución.
 * @param struct timespec inicio  Instante de tiempo de inicio.
 * @param struct timespec fin     Instante de tiempo de fin.
 * @return double                 Segundos transcurridos (puede incluir fracción).
 **/
double tiempoEnSegundos(struct timespec inicio, struct timespec fin);

/**
 * @brief Lista todos los archivos regulares en un directorio dado.
 *        Excluye carpetas, "." y "..", y devuelve solo nombres de archivos.
 * @param const_char *rutaDir     Ruta al directorio que se recorrerá.
 * @param int *numArchivos        Salida: almacena el número de archivos encontrados.
 * @return char **                Array dinámico de cadenas con cada nombre de archivo.
 *                                El llamador debe liberar cada cadena y el array.
 **/
char **listarArchivos_tests(const char* rutaDir, int* numArchivos);

/**
 * @brief Lee una matriz de un archivo de texto con formato:
 *        primera línea: "rows columns"
 *        siguientes líneas: valores separados por espacios.
 * @param FILE *f                Puntero a archivo ya abierto en modo lectura.
 * @param int *rows              Salida: número de filas leído.
 * @param int *columns           Salida: número de columnas leído.
 * @return int **                Matriz dinámica (rows × columns) con los datos leídos.
 *                               El llamador debe liberar la memoria con eliminarMatriz().
 **/
int **leerMatriz(FILE *f, int *rows, int *columns);

/**
 * @brief Libera la memoria asignada a una matriz dinámica.
 *        Recibe el puntero a puntero y el número de filas.
 * @param int **Matriz           Matriz previamente creada con leerMatriz().
 * @param int rows               Número de filas de la matriz (cada fila se libera).
 * @return void                  No devuelve valor.
 **/
void eliminarMatriz(int **Matriz, int rows);

/**
 * @brief Multiplica dos matrices A y B en paralelo usando fork() y pipes.
 *        Cada proceso hijo calcula una fila entera de la matriz C = A × B.
 *        El padre recolecta los resultados parciales por pipe y arma la matriz final.
 * @param int **MatrizA          Primera matriz de tamaño (rowA × columnA).
 * @param int **MatrizB          Segunda matriz de tamaño (rowB × columnB).
 * @param int rowA               Número de filas de la matriz A.
 * @param int columnA            Número de columnas de la matriz A.
 * @param int rowB               Número de filas de la matriz B.
 * @param int columnB            Número de columnas de la matriz B.
 * @return int **                Matriz resultante (rowA × columnB), o NULL si dimensiones son incompatibles.
 *                               El llamador debe liberar la memoria con eliminarMatriz().
 **/
int **multMatriz(int **MatrizA, int **MatrizB, int rowA, int columnA, int rowB, int columnB);

/**
 * @brief Escribe una matriz en un archivo de texto con formato:
 *        primera línea: "rows columns"
 *        siguientes líneas: valores de la matriz separados por espacios.
 *        El nombre del archivo es "./salidaFork/salidaFork_<nombreArchivo>".
 * @param int **matriz           Matriz a escribir.
 * @param int row                Número de filas de la matriz.
 * @param int column             Número de columnas de la matriz.
 * @param const_char *nombreArchivo
 *                                Sufijo para nombrar el archivo de salida.
 * @return void                  No devuelve valor.
 **/
void escribirMatriz(int** matriz, int row, int column, const char* nombreArchivo);

#endif //MAIN_H