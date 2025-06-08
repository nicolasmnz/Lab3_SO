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

#include "./LAB3_Munoz_Rojo.h"

/*void printMatriz(int **matriz, int rows, int columns) {
    printf("Matriz (%d x %d):\n", rows, columns);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++)
        {
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
    return;
}*/

double tiempoEnSegundos(struct timespec inicio, struct timespec fin) {
    return (fin.tv_sec - inicio.tv_sec) + (fin.tv_nsec - inicio.tv_nsec) / 1e9;
}

char **listarArchivos_tests(const char* rutaDir, int* numArchivos) {
    DIR *dir = opendir(rutaDir);

    int capacidad = 100;
    int count = 0; 

    char **lista = (char**)malloc(sizeof(char *) * capacidad);

    struct dirent *archivo;
    struct stat st;
    while ((archivo = readdir(dir)) != NULL) {
        // Saltar "." y ".."
        if (strcmp(archivo->d_name, ".") == 0 || strcmp(archivo->d_name, "..") == 0) continue;
        
        // Construir ruta completa para verificar el tipo
        char fullpath[PATH_MAX];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", rutaDir, archivo->d_name);
        
        if (stat(fullpath, &st) == -1) continue;   // Saltar si no se puede obtener info
        if (!S_ISREG(st.st_mode)) continue;        // Solo archivos regulares (no carpetas)

        // Obtenemos nombre del archivo
        char *nombreArchivo = strdup(archivo->d_name);

        if (count >= capacidad) {
            // se agranda si no hay suficiente espacio con realloc
            capacidad *= 2;
            char **nuevaLista = realloc(lista, sizeof(char *) * capacidad);
            lista = nuevaLista;
        }
        lista[count++] = nombreArchivo; //entra en lista[count] y luego incrementa count
    }

    closedir(dir);
    *numArchivos = count;

    return lista;
}


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
        for (int j = 0; j < (*columns); j++)
        {
            Matriz[i][j] = atoi(token);
            token = strtok(NULL, " \t\r\n");
        }
    }

    return Matriz;
}

void eliminarMatriz(int **Matriz, int rows) {
    for (int i = 0; i < rows; i++) {
        free(Matriz[i]);
    }
    free(Matriz);

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

void escribirMatriz(int** matriz, int row, int column, const char* nombreArchivo) {
    // dar nombre formateado al archivo de salida
    char path[PATH_MAX];
    snprintf(path, sizeof(path), "./salidaFork/salidaFork_%s", nombreArchivo);

    // crear archivo
    FILE *salida = fopen(path, "w");

    fprintf(salida, "%d %d\n", row, column);
    
    for (int i = 0; i< row; i++) {
        for (int j = 0; j < column; j++) {
            fprintf(salida, "%d ", matriz[i][j]);
        }
        fprintf(salida, "\n");
    }

    fclose(salida);

    return;
}

int main() {
    // busco todos los archivos de prueba
    int numArchivos; 
    char **archivos = listarArchivos_tests("./pruebas", &numArchivos);

    // abrir CSV para registrar resuldos
    FILE *csv = fopen("./measurements/c_measurements.csv", "w");
    fprintf(csv, "archivo,wall_time-s,user_time-s,sys_time-s,cpu_total-s,percent_cpu,mem_rss-kb\n");
    
    // itero sobre la lista de nombres de los archivos
    for (int i = 0; i < numArchivos; i++) {
        fflush(csv); // se libera el buffer para no tener problemas con el fork, Header repetido
        // preparo ruta
        const char *carpeta = "./pruebas/";
        const char *nombreArchivo = archivos[i];

        char path[PATH_MAX];
        snprintf(path, sizeof(path), "%s%s", carpeta, nombreArchivo);

        // abro el archivo
        FILE *fptr;
        fptr = fopen(path, "r");

        // leer matriz A
        int m, n;
        int **matrizA = leerMatriz(fptr, &m, &n);

        // dado la linea vacia
        char linea[100];
        fgets(linea, sizeof(linea), fptr);

        // leer matriz B
        int  p, q;
        int **matrizB = leerMatriz(fptr, &p, &q);
    
        fclose(fptr);
        // mediciones anterior a la multiplicacion de matrices
        struct timespec inicio_wall, fin_wall;
        struct rusage uso_inicio, uso_fin;
    
        clock_gettime(CLOCK_MONOTONIC, &inicio_wall);
        getrusage(RUSAGE_SELF, &uso_inicio);
    
        // llamada a multMatriz con fork() y pipe()
        int **matrizC = multMatriz(matrizA, matrizB, m, n, p, q);
        // si error en multilicacion, saltar
        if (matrizC == NULL) {
            eliminarMatriz(matrizA, m);
            eliminarMatriz(matrizB, p);
            continue;
        }

        // mediciones posteriores a la multiplicacion de matrices
        clock_gettime(CLOCK_MONOTONIC, &fin_wall);
        getrusage(RUSAGE_SELF, &uso_fin);
    
        // calculo de metricas
        double wall = tiempoEnSegundos(inicio_wall, fin_wall);
    
        double cpu_user = (uso_fin.ru_utime.tv_sec - uso_inicio.ru_utime.tv_sec)
                        + (uso_fin.ru_utime.tv_usec - uso_inicio.ru_utime.tv_usec) / 1e6;
        double cpu_sys  = (uso_fin.ru_stime.tv_sec - uso_inicio.ru_stime.tv_sec)
                        + (uso_fin.ru_stime.tv_usec - uso_inicio.ru_stime.tv_usec) / 1e6;
        double cpu_total = cpu_sys + cpu_user;
        double porc_cpu = (wall > 0) ? (100.0 * cpu_total/ wall) : 0.0;
        long memoria_kb = uso_fin.ru_maxrss;
        
        // registrar mediciones
        fprintf(csv, "%s,%.6f,%.6f,%.6f,%.6f,%.1f,%ld\n",
                path,
                wall,       // Tiempo real (wall clock)
                cpu_user,   // Tiempo CPU user
                cpu_sys,    // Tiempo CPU sys
                cpu_total,  // Tiempo CPU total
                porc_cpu,   // Porcentaje de uso aproximado de CPU
                memoria_kb  // Uso máximo de memoria (resident set size)
        );
        
        // guardar matriz en salidasFork
        escribirMatriz(matrizC, m, q, nombreArchivo);
        
        // liberar matrices
        eliminarMatriz(matrizA, m);
        eliminarMatriz(matrizB, p);
        if (matrizC != NULL) eliminarMatriz(matrizC, m);
    }
    // cerrar CSV y liberar lista de archivos
    fclose(csv);

    for (int i = 0; i < numArchivos; i++) {
        free(archivos[i]);
    }
    free(archivos);

    return 0;
}

// leer el archivo
// crear arreglo de [m][n]
// guardar elementos dentro del arreglo
// realizar fork para cada fila de la matrizA
// realizar pipes cada fork con el padre
