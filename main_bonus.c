#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/resource.h>  
#include <sys/wait.h>

int **leerMatriz(FILE *f, int *rows, int *columns) {
    char linea[100];

    // leo la primera linea para definir rows y columns
    fgets(linea, sizeof(linea), f);
    sscanf(linea, "%d %d", rows, columns);

    // Reservo memoria
    int **Matriz = (int **)malloc((*rows) * sizeof(int *));
    for (int i = 0; i < (*rows); i++) {
        Matriz[i] = (int *)malloc((*columns) * sizeof(int));
    }

    // Obtener cada elemento de la fila por delimitadores
    for (int i = 0; i < (*rows); i++) {
        fgets(linea, 100, f);
        char *token = strtok(linea, " \t\r\n");
        for (int j = 0; j < (*columns); j++) {
            Matriz[i][j] = atoi(token);
            token = strtok(NULL, " \t\r\n");
        }
    }

    return Matriz;
}

void escribirMatriz(int** Matriz, int row, int column, const char* nombreArchivo) {
    // dar nombre formateado al archivo de salida
    size_t len_nombreArchivo = strlen(nombreArchivo);
    size_t len_ruta = strlen("./salidaFork/bonus/salidaFork_");
    char *path = (char*)malloc(len_ruta + len_nombreArchivo);    //revisar
    sprintf(path, "./salidaFork/bonus/salidaFork_%s", nombreArchivo);

    // crear archivo
    FILE *salida = fopen(path, "w");

    fprintf(salida, "%d %d\n", row, column);
    
    for (int i = 0; i< row; i++) {
        for (int j = 0; j < column; j++) {
            fprintf(salida, "%d ", Matriz[i][j]);
        }
        fprintf(salida, "\n");
    }

    fclose(salida);

    return;
}

void eliminarMatriz(int **Matriz, int rows) {
    for (int i = 0; i < rows; i++) {
        free(Matriz[i]);
    }
    free(Matriz);

    return;
}

// 0 -> false
// 1 -> true
int esSimetrica(int** matrizRes) {

}

int main() {
    // busco todos los archivos de bonus
    int numArchivos; 
    char **archivos = listarArchivos_tests("./pruebas/bonus", &numArchivos);

    // itero sobre la lista de nombres de los archivos
    for (int i = 0; i < numArchivos; i++) {
        // preparo ruta
        char *carpeta = "./pruebas/";
        size_t len_carpeta = strlen(carpeta);
        const char *nombreArchivo = archivos[i];
        size_t len_nombreArchivo = strlen(nombreArchivo);

        char *path = (char*)malloc(len_carpeta + len_nombreArchivo);
        sprintf(path, "%s%s", carpeta, nombreArchivo);

        // abro el archivo
        FILE *fptr;
        fptr = fopen(path, "r");

        // tomo la cantidad de matrices
        int cantMatrices;
        char linea[100];
        fgets(linea, sizeof(linea), fptr);
        sscanf(linea, "%d", cantMatrices);

        // inicializo para poder borrar despues las matrices
        int m, n;
        int **matrizA;
        int p, q;
        int **matrizB;
        int **matrizRes;

        // leo la primera matriz -> matrizA 
        matrizA = leerMatriz(fptr, &m, &n);

        for (int j = 0; j < cantMatrices - 1 ; j++) {
            // dado la linea vacia
            char linea[100];
            fgets(linea, sizeof(linea), fptr);

            // leer segunda matriz -> matrizB
            matrizB = leerMatriz(fptr, &p, &q);
            
            // llamada a multMatriz con fork() y pipe()
            matrizRes = multMatriz(matrizA, matrizB, m, n, p, q); // matrizRes (m x q)

            // si eran incompatibles las dimensiones se termina de multiplicar
            if (matrizRes == NULL) break;

            // guardar la matrizRes como matrizA y matrizres = NULL
            int ** matrizAux = matrizA;
            eliminarMatriz(matrizAux, m);
            n = q; // reajuste con la dimension 
            matrizA = matrizRes;
            matrizRes = NULL;

        }
        //al terminar de leer todo, cerrar el archivo
        fclose(fptr);

        // escribir matriz en salidasFork/bonus
        if (matrizRes != NULL) escribirMatriz(matrizRes, m, q, nombreArchivo);

        // liberar matrices
        eliminarMatriz(matrizA, m);
        eliminarMatriz(matrizB, p);
        if (matrizRes != NULL) eliminarMatriz(matrizRes, m);
    }

    for (int i = 0; i < numArchivos; i++) {
        free(archivos[i]);
    }
    free(archivos);

    return 0;
}

// busco los archivos y los listo
// los voy tomando cada dos por primera vez
// luego A = C y C = NULL
// repito por la cantidad de matrices
// si las dimensiones no son compatibles C = NUll 
//      -> printf("Error\n") y se termina y no se guarda en txt
// se guarda el resultado en txt y prosigue con los otros archivos