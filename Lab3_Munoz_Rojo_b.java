import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.lang.Thread;
import java.lang.management.ManagementFactory;
import java.lang.management.ThreadMXBean;


class MultiplicarFila extends Thread {
    private int[] fila;
    private int[][] matrizM;
    private int[] resultado;
    private int filaIndice;

    public MultiplicarFila(int[] fila, int[][] matrizM, int filaIndice) {
        this.fila = fila;
        this.matrizM = matrizM;
        this.filaIndice = filaIndice;
        this.resultado= new int[matrizM[0].length];
    }

    @Override public void run() {
        int nfila=fila.length;
        int columna=matrizM[0].length;

        for (int i =0; i <columna;i++ ) {
            int suma =0;
            for(int j =0; j< nfila; j++ ){
                suma += fila[j] * matrizM[j][i];
            }
            resultado[i]= suma;
        }
    }

    public int[] getResultado() {
        return resultado;
    }

    public int getFilaIndice() {
        return filaIndice;
    }
}

public class Lab3_Munoz_Rojo_b {
        public static void main(String[] args) throws InterruptedException {
        System.out.println(12);
        try{
             //File carpeta = new File("pruebas/bonus/medium"); // Archivo necesario para la ejecucion
            File carpeta = new File("pruebas/easy");
            if (!carpeta.exists() || !carpeta.isDirectory()) {
            System.out.println("La carpeta no existe o no es válida.");
            return;
            }
            File[] archivos = carpeta.listFiles((dir, name) -> name.endsWith(".txt"));
            for(int n_archivo=0 ; n_archivo<archivos.length;n_archivo++){
                File archivo= archivos[n_archivo];
        
                boolean verificacion = archivo.canRead();
                if (verificacion){ // verifica si existe el archivo
                    Scanner lector = new Scanner(archivo); 
                    int totalMatrices = Integer.parseInt(lector.nextLine().trim()); // se lee el numero totales de matrices 
                    List<int[][]> matrices = new ArrayList<>(totalMatrices);
                    while(lector.hasNextLine()){
                        // Saltar líneas vacías
                        String linea = "";
                        while (lector.hasNextLine() && (linea = lector.nextLine().trim()).isEmpty()) {
                            // skip
                        }

                        if (linea == null || linea.isEmpty()) break;

                        // Leer dimensiones
                        String[] partes = linea.split(" ");
                        int filas = Integer.parseInt(partes[0]);
                        int columnas = Integer.parseInt(partes[1]);
                        // Leer matriz
                        int[][] matriz = new int[filas][columnas];
                        for (int i = 0; i < filas; i++) {
                            if (!lector.hasNextLine()) break;
                            String[] datos = lector.nextLine().trim().split(" ");
                            for (int j = 0; j < columnas; j++) {
                                matriz[i][j] = Integer.parseInt(datos[j]);
                            }
                        }
                        matrices.add(matriz); //añade a la matriz al arreglo   
                    }

                    lector.close(); // se cierra el scanner por buenas practicas 
                    
                    boolean validacion_total=Validacion(matrices, totalMatrices);
                    if (validacion_total){
                    

                  
                        int [][] matrizC;
                        for(int indice=1; indice<totalMatrices;indice++){
                            // Se fija la matriz 0 para hacer todos los calculos 
                            int filaA=Fila(matrices.get(0));
                            int columnaA =Columna(matrices.get(0));

                            int filaB=Fila(matrices.get(indice));
                            int columnaB=Columna(matrices.get(indice));

                            System.out.println("columnaA: " + columnaA + ", filaB: " + filaB +
                            ", columnaA == filaB: " + (columnaA == filaB) +
                            ", archivo #: " + n_archivo);
                            if (columnaA == filaB){ // condicion para multiplicar 2 matrices 
                                matrizC = new int [filaA][columnaB] ; 
                                
                                MultiplicarFila[] hebras = new MultiplicarFila[filaA];

                                long inicio_matriz= System.nanoTime();
                                Runtime runtime = Runtime.getRuntime();
                                runtime.gc(); // Limpiar lo más posible para no arruinar la relacion
                                long memoria_inicial = runtime.totalMemory() - runtime.freeMemory(); // memoria destinada - la disponible
                                ThreadMXBean gestion = ManagementFactory.getThreadMXBean(); 
                                long cpu_inicial = gestion.getCurrentThreadCpuTime(); //

                                for (int i = 0; i < filaA; i++) {
                                    hebras[i] = new MultiplicarFila(matrices.get(0)[i], matrices.get(indice), i);
                                    hebras[i].start();
                                }

                                for (int i = 0; i < filaA; i++) {
                                    hebras[i].join();
                                    matrizC[hebras[i].getFilaIndice()] = hebras[i].getResultado();
                                    //System.out.println("Duró: " + (tiempo_hebra - tiempos[i]) / 1_000_000 + " ms");

                                }

                                // Se termina la multiplicacion de las matrices y al mismo tiempo se hace un remplazo de los valores
                                // Se termina la multiplicacion de las matrices y al mismo tiempo se hace un remplazo de los valores
                                long fin_matriz = System.nanoTime();
                                long memoria_final = runtime.totalMemory() - runtime.freeMemory();
                                long cpu_final = gestion.getCurrentThreadCpuTime();



                                double tiempo_total= (fin_matriz - inicio_matriz)/ 1_000_000_000.0 ;
                                long memoria_usada = (memoria_final - memoria_inicial) / 1024;
                                double cpu_usado = (cpu_final - cpu_inicial)/1_000_000_000.0;
                                double tiempo_cpu_sistema = tiempo_total - cpu_usado;

                                try (FileWriter writer = new FileWriter("Thread.txt", true)) {
                                    writer.write("Resultado del archivo #"+ n_archivo);
                                    writer.write("\n");
                                    writer.write("Resultado de la multiplicación (matriz #" + indice + "):\n");

                                    for (int[] fila : matrizC) {
                                        for (int val : fila) {
                                            writer.write(val + " ");
                                        }
                                        writer.write("\n");
                                    }
                                    writer.write("Es una matriz transpuesta: " + trasnpuesta(matrizC)  + " s\n");
                                    writer.write("Tiempo de ejecución: " + tiempo_total + " s\n");
                                    writer.write("Memoria usada en ejecución: " + memoria_usada +" KB \n");
                                    writer.write("CPU sistema : "+ tiempo_cpu_sistema+" segundos \n" );
                                    writer.write("Cpu usada en la ejecucion: " + cpu_usado +"\n");
                                    writer.write("\n");


                                } catch (IOException e) {
                                    System.out.println("Error al escribir en el archivo: " + e.getMessage());
                                }


                                // se rempliza la matriz A por la resultante C para hacer el siguiente calculo 
                                matrices.set(0, matrizC);

                                //System.out.println(trasnpuesta(MatrizC));

                            }else{
                                System.out.println("no se puede multiplicar12");
                            }


                    }

                }else{
                    try (FileWriter writer = new FileWriter("Thread.txt", true)) {
                        writer.write("Resultado del archivo #"+ n_archivo);
                        writer.write("\nNose puede multiplicar el archivo");
                        writer.write("\n");


                    } catch (IOException e) {
                        System.out.println("Error al escribir en el archivo: " + e.getMessage());
                    }

                }
        
                }else{
                    System.out.println("No se puede leer el archivo o no existe.");
                }
             }
            } catch (FileNotFoundException e) {
                System.out.println("Archivo no encontrado: " + e.getMessage());
            }


    }


// Funciones Auxiliares


/**
 * Verifica si una matriz cuadrada es simétrica respecto a su diagonal principal.
 *
 * @param matriz La matriz bidimensional de enteros.
 * @return el numero de filas de la matriz .
 * @throws IllegalArgumentException si la matriz es {@code null}.
 **/
static int Fila(int[][] matriz) {
    return matriz.length;
}


/**
 * Verifica si una matriz cuadrada es simétrica respecto a su diagonal principal.
 *
 * @param matriz La matriz bidimensional de enteros.
 * @return El valor de numero de columnas de la matriz.
 * @throws IllegalArgumentException si la matriz es {@code null}.
 **/
static int Columna(int[][] matriz) {
    if (matriz.length > 0) {
        return matriz[0].length;
    } else {
        return 0; // O puedes lanzar una excepción si prefieres
    }
}

/**
 * Verifica si una matriz cuadrada es simétrica respecto a su diagonal principal.
 *
 * @param matriz La matriz bidimensional de enteros.
 * @return {@code true} si es igual a su transpuesta; {@code false} en caso contrario.
 * @throws IllegalArgumentException si la matriz es {@code null}.
 **/
static boolean trasnpuesta(int[][] matriz) {
    int filas = Fila(matriz);
    int columnas = Columna(matriz);

    if (filas != columnas) {
        return false; // Una matriz no cuadrada no puede ser simétrica
    }

    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            if (matriz[i][j] != matriz[j][i]) {
                return false;
            }
        }
    }

    return true;
}


/**
 * Verifica si es posible hacer la multiplicacion de las matrices.
 *
 * @param List  totalMatrices ; 
 * @return {@code true} si es igual a su transpuesta; {@code false} en caso contrario.
 * @throws IllegalArgumentException si la matriz es {@code null}.
 **/
static boolean Validacion( List<int[][]> totalMatrices, int numero_matrices) {

    int columnas_inicial = Columna(totalMatrices.get(0));

    for (int i = 1; i < numero_matrices; i++) {
        int columna_a_mutiplicar=Columna(totalMatrices.get(i)) ;
        int fila_a_multiplicar =Fila(totalMatrices.get(i));
        if (columnas_inicial == fila_a_multiplicar){
            columnas_inicial=columna_a_mutiplicar;
        }else{
            return false;
        }
        
    }

    return true;
}

    
}

