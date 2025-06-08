import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;



class MultiplicarFila extends Thread {

    private final int[] fila;
    private final int[][] matrizM;
    private final int[] resultado;
    private final int filaIndice;

    //Constructor de la clase
    public MultiplicarFila(int[] fila, int[][] matrizM, int filaIndice) {
        this.fila = fila;
        this.matrizM = matrizM;
        this.filaIndice = filaIndice;
        this.resultado= new int[matrizM[0].length];
    }

    // ejecuta las multiplicacion de las matrices 
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

public class LAB3_Munoz_Rojo_b {
        public static void main(String[] args) throws InterruptedException {
        try{
            File carpeta = new File("./pruebas/bonus/");
            if (!carpeta.exists() || !carpeta.isDirectory()) {
            System.out.println("La carpeta no existe o no es válida.");
            return;
            }
            File[] archivos = carpeta.listFiles((dir, name) -> name.endsWith(".txt"));
            for(int n_archivo=0 ; n_archivo<archivos.length;n_archivo++){
                
                String archivo_nombre = String.format("%d.txt", n_archivo);  
                File archivo = new File(carpeta, archivo_nombre); 
        
                boolean verificacion = archivo.canRead();
                if (verificacion){                     int totalMatrices;
                List<int[][]> matrices;
                    try ( // verifica si existe el archivo
                            Scanner lector = new Scanner(archivo)) {
                        totalMatrices = Integer.parseInt(lector.nextLine().trim()); // se lee el numero totales de matrices
                        matrices = new ArrayList<>(totalMatrices);
                        while (lector.hasNextLine()) {
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
                        // se cierra el scanner por buenas practicas
                    } // se lee el numero totales de matrices
                    
                    boolean validacion_total=Validacion(matrices, totalMatrices);
                    if (validacion_total){
                    

                        int [][] matrizC;
                        for(int indice=1; indice<totalMatrices;indice++){
                            // Se fija la matriz 0 para hacer todos los calculos 
                            int filaA=Fila(matrices.get(0));
                            int columnaA =Columna(matrices.get(0));

                            int filaB=Fila(matrices.get(indice));
                            int columnaB=Columna(matrices.get(indice));

                            if (columnaA == filaB){ // condicion para multiplicar 2 matrices 
                                matrizC = new int [filaA][columnaB] ; 
                                //Creo las hebras
                                MultiplicarFila[] hebras = new MultiplicarFila[filaA];


                                for (int i = 0; i < filaA; i++) {
                                    hebras[i] = new MultiplicarFila(matrices.get(0)[i], matrices.get(indice), i);
                                    hebras[i].start();
                                }

                                for (int i = 0; i < filaA; i++) {
                                    hebras[i].join();
                                    matrizC[hebras[i].getFilaIndice()] = hebras[i].getResultado();
                                    //System.out.println("Duró: " + (tiempo_hebra - tiempos[i]) / 1_000_000 + " ms");

                                }

                                // se rempliza la matriz A por la resultante C para hacer el siguiente calculo 
                                matrices.set(0, matrizC);

                                //System.out.println(trasnpuesta(MatrizC));

                            }else{
                                System.out.println("no se puede multiplicar");
                            }

                            matrizC=matrices.get(0);
                            String nombre_archivo= "./salidaThread/bonus/salidaThread_"+n_archivo+".txt";
                            try (FileWriter writer = new FileWriter(nombre_archivo, false)) {
                                writer.write( Fila(matrizC) +" "+ Columna(matrizC) + "\n");
                                for (int[] fila : matrizC) {
                                    for (int val : fila) {
                                        writer.write(val + " ");
                                    }
                                    writer.write("\n");
                                }
                                if (trasnpuesta(matrizC)){
                                    writer.write("Es Simetrica \n");
                                }
                                

                            } catch (IOException e) {
                                System.out.println("Error al escribir en el archivo: " + e.getMessage());
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

