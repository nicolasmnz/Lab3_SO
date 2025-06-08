#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/resource.h>  
#include <sys/wait.h>

#include "./main_bonus.h"

void printMatriz(int **matriz, int rows, int columns) {
    printf("Matriz (%d x %d):\n", rows, columns);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++)
        {
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
    return;
}

char **listarArchivos_tests(const char* rutaDir, int* numArchivos) {
    DIR *dir = opendir(rutaDir);

    int capacidad = 100;
    int count = 0; 

    char **lista = (char**)malloc(sizeof(char *) * capacidad);

    struct dirent *archivo;
    while ((archivo = readdir(dir)) != NULL) {
        // Saltar "." y ".."
        if (strcmp(archivo->d_name, ".") == 0 || strcmp(archivo->d_name, "..") == 0) continue;
        
        // Obtenemos nombre del archivo
        size_t lenName = strlen(archivo->d_name); // se obtiene el tamanio de nombre archivo
        char *pathCompleto = malloc(lenName + 1);

        sprintf(pathCompleto, "%s", archivo->d_name); // dar formato al nombre del archivo

        if (count >= capacidad) {
            // se agranda si no hay suficiente espacio con realloc
            capacidad *= 2;
            char **nuevaLista = realloc(lista, sizeof(char *) * capacidad);
            lista = nuevaLista;
        }
        lista[count++] = pathCompleto; //entra en lista[count] y luego incrementa count
    }

    closedir(dir);
    *numArchivos = count;

    return lista;
}

int **leerMatriz(FILE *f, int *rows, int *columns) {
    // Leer dimensiones 
    fscanf(f, "%d %d", rows, columns);

    // Reservo memoria
    int **Matriz = (int **)malloc((*rows) * sizeof(int *));
    for (int i = 0; i < (*rows); i++) {
        Matriz[i] = (int *)malloc((*columns) * sizeof(int));
    }

    // Obtener cada elemento de la fila por delimitadores
    for (int i = 0; i < (*rows); i++) {
        for (int j = 0; j < (*columns); j++) {
            fscanf(f, "%d", &Matriz[i][j]);
        }
    }

    return Matriz;
}

// 0 -> false
// 1 -> true
int esSimetrica(int** Matriz, int row, int column) {
    // condicion fuerte: la matriz debe ser cuadrada (nxn) para ser simetrica
    if (row != column) return 0;
    
    for (int i = 0; i < row; i++) {
        for (int j = i + 1; j < column; j++) {
            if (Matriz[i][j] != Matriz[j][i]) return 0;
        }
    }

    return 1;
}

void escribirMatriz(int** Matriz, int row, int column, const char* nombreArchivo) {
    // dar nombre formateado al archivo de salida
    size_t len_nombreArchivo = strlen(nombreArchivo);
    size_t len_ruta = strlen("./salidaFork/bonus/salidaFork_");
    char *path = (char*)malloc(len_ruta + len_nombreArchivo + 1);    //revisar
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

    if (esSimetrica(Matriz, row, column)) {
        fprintf(salida, "La matriz final es simetrica\n");
    }

    fclose(salida);

    return;
}

int **multMatriz(int **MatrizA, int **MatrizB, int rowA, int columnA, int rowB, int columnB) {
    if (columnA != rowB) {
        printf("Error: Dimesiones no compatibles\n");
        return NULL;
    }
    // Reservar memoria para matriz resultante
    int **matrizC = (int **)malloc(rowA * sizeof(int *));
    for (int i = 0; i < rowA; i++) {
        matrizC[i] = (int *)malloc(columnB * sizeof(int));
    }
    // Pipes y PIDs
    int (*from_child)[2] = malloc(rowA * sizeof(int[2])); // PIPE para hijo escriba, padre lee
    pid_t *pids = (pid_t *)malloc(rowA * sizeof(pid_t));
    // ============================== fork =======================================
    for (int i = 0; i < rowA; i++) {
        pipe(from_child[i]);
        pids[i] = fork();
        int wr = from_child[i][1];

        // =========================== hijo =====================================
        if (pids[i] == 0) {
            close(from_child[i][0]); // cierro extremo de lectura para el hijo
            for (int j = 0; j < columnB; j++) {
                int elemC = 0;
                for (int k = 0; k < columnA; k++) {
                    elemC += MatrizA[i][k] * MatrizB[k][j];
                }
                write(wr, &i, sizeof(int)); // mando la fila en que se debe escribir elemC
                write(wr, &j, sizeof(int)); // mando la columna en que se debe escribir elemC
                write(wr, &elemC, sizeof(int));
            }
            close(from_child[i][1]);
            exit(0); // cuidado con los procesos zombi!
        }
        // =========================== padre =====================================
        else {
            close(from_child[i][1]); // cierro extremo de escritura para el padre
        }
    }
    for (int i = 0; i < rowA; i++) {
        int elemC, rowC, columnC;
        for (int j = 0; j < columnB; j++) {
            read(from_child[i][0], &rowC, sizeof(int));
            read(from_child[i][0], &columnC, sizeof(int));
            read(from_child[i][0], &elemC, sizeof(int));

            matrizC[rowC][columnC] = elemC;
        }
        close(from_child[i][0]);
    }
    // matar procesos zombi
    for (int i = 0; i < rowA; i++) {
        waitpid(pids[i], NULL, 0);
    }
    free(from_child);
    free(pids);

    return matrizC;
}

void eliminarMatriz(int **Matriz, int rows) {
    for (int i = 0; i < rows; i++) {
        free(Matriz[i]);
    }
    free(Matriz);

    return;
}


int main() {
    // busco todos los archivos de bonus
    int numArchivos; 
    char **archivos = listarArchivos_tests("./pruebas/bonus", &numArchivos);

    // itero sobre la lista de nombres de los archivos
    for (int i = 0; i < numArchivos; i++) {
        // preparo ruta
        const char *carpeta = "./pruebas/bonus";
        size_t len_carpeta = strlen(carpeta);
        const char *nombreArchivo = archivos[i];
        size_t len_nombreArchivo = strlen(nombreArchivo);

        char *path = (char*)malloc(len_carpeta + 1 + len_nombreArchivo + 1);
        sprintf(path, "%s/%s", carpeta, nombreArchivo);

        // abro el archivo
        FILE *fptr;
        fptr = fopen(path, "r");

        // tomo la cantidad de matrices
        int cantMatrices;
        fscanf(fptr, "%d", &cantMatrices);

        // inicializo para poder borrar despues las matrices
        int m, n;
        int **matrizA;
        int p, q;
        int **matrizB;
        int **matrizRes;

        // leo la primera matriz -> matrizA 
        matrizA = leerMatriz(fptr, &m, &n);
        printMatriz(matrizA, m, n);

        for (int j = 0; j < cantMatrices - 1 ; j++) {
            // guardar la matrizRes como matrizA y matrizres = NULL
            if (j > 0) {
                int ** matrizAux = matrizA;
                eliminarMatriz(matrizAux, m);
                eliminarMatriz(matrizB, p);
                n = q; // reajuste con la dimension 
                matrizA = matrizRes;
                matrizRes = NULL;
            }

            // leer segunda matriz -> matrizB
            matrizB = leerMatriz(fptr, &p, &q);
            printMatriz(matrizB, p, q);
            // llamada a multMatriz con fork() y pipe()
            matrizRes = multMatriz(matrizA, matrizB, m, n, p, q); // matrizRes (m x q)

            // si eran incompatibles las dimensiones se termina de multiplicar
            if (matrizRes == NULL) break;

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