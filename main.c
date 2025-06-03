#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void printMatriz(int **matriz, int rows, int columns)
{
    printf("Matriz (%d x %d):\n", rows, columns);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
}

int **leerMatriz(FILE *f, int *rows, int *columns)
{
    char linea[100];

    // leeo la primera linea para definir rows y columns
    fgets(linea, sizeof(linea), f);
    sscanf(linea, "%d %d", rows, columns);

    // Reservo memoria
    int **Matriz = (int **)malloc((*rows) * sizeof(int *));
    for (int i = 0; i < (*rows); i++)
    {
        Matriz[i] = (int *)malloc((*columns) * sizeof(int));
    }

    // Obtener cada elemento de la fila por delimitadores
    for (int i = 0; i < (*rows); i++)
    {
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

int **multMatriz(int **MatrizA, int **MatrizB, int rowA, int columnA, int rowB, int columnB)
{
    // Reservar memoria para matriz resultante
    int **matrizC = (int **)malloc(rowA * sizeof(int *));
    for (int i = 0; i < rowA; i++)
    {
        matrizC = (int *)malloc(columnB * sizeof(int));
    }
    // Pipes y PIDs
    int *from_child[2] = (int *)malloc(rowA * sizeof(int[2])); // PIPE para hijo escriba, padre lee
    pid_t *pids = (pid_t)malloc(rowA * sizeof(pid_t));
    // ============================== fork =======================================
    for (int i = 0; i < rowA; i++)
    {
        pipe(from_child[i]);
        pids[i] = fork();
        int wr = from_child[i][1];

        // =========================== hijo =====================================
        if (pids[i] == 0)
        {
            close(from_child[i][0]); // cierro extremo de lectura para el hijo
            for (int j = 0; j < columnB; j++)
            {
                int elemC = 0;
                for (int k = 0; k < columnA; k++)
                {
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
        else
        {
            close(from_child[i][1]); // cierro extremo de escritura para el padre
        }
    }
    for (int i = 0; i < rowA; i++)
    {
        int elemC, rowC, columnC;
        for (int j = 0; j < rowA; j++)
        {
            read(from_child[i][0], &rowC, sizeof(int));
            read(from_child[i][0], &columnC, sizeof(int));
            read(from_child[i][0], &elemC, sizeof(int));

            matrizC[rowC][columnC] = elemC;
        }
        close(from_child[i][0]);
    }
}

int main()
{
    FILE *fptr;
    fptr = fopen("./entrada.txt", "r");

    int m, n, p, q;
    int **matrizA = leerMatriz(fptr, &m, &n);
    char linea[100];
    fgets(linea, sizeof(linea), fptr); // dado la linea vacia
    int **matrizB = leerMatriz(fptr, &p, &q);

    fclose(fptr);

    printMatriz(matrizA, m, n);
    printMatriz(matrizB, p, q);
    return 0;
}

// leer el archivo
// crear arreglo de [m][n]
// guardar elementos dentro del arreglo
// realizar fork para cada fila de la matrizA
// realizar pipes cada fork con el padre
