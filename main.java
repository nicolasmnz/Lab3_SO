import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Scanner;


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

    public void run() {
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



public class main {


        public static void main(String[] args) throws InterruptedException {

        System.out.println("¡Hola, mundo!");
        try{
            File archivo = new File("entrada.txt");
            boolean verificacion = archivo.canRead();
            if (verificacion){
                System.out.println("El archivo se puede leer.");
                Scanner lector = new Scanner(archivo);
                int totalMatrices = Integer.parseInt(lector.nextLine().trim());
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
                    matrices.add(matriz);   
                }

                for(int indice=1; indice<totalMatrices;indice++){
                    // Se fija la matriz 0 para hacer todos los calculos 
                    int filaA=Fila(matrices.get(0));
                    int columnaA =Columna(matrices.get(0));

                    int filaB=Fila(matrices.get(indice));
                    int columnaB=Columna(matrices.get(indice));




                        System.out.println("A:");
                        for (int[] fila : matrices.get(0)) {
                            for (int val : fila) {
                                System.out.print(val + " ");
                            }
                            System.out.println();
                        }

                        System.out.println("B:");
                        for (int[] fila : matrices.get(indice)) {
                            for (int val : fila) {
                                System.out.print(val + " ");
                            }
                            System.out.println();
                        }



                    System.out.println("filaA=" + filaA + ", columnaA=" + columnaA + ", filaB=" + filaB + ", columnaB=" + columnaB);



                    if (columnaA == filaB){ // condicion para multiplicar 2 matrices 
                        System.out.println("se puede multiplicar"); 
                        int [][] matrizC = new int [filaA][columnaB] ;

                        MultiplicarFila[] hebras = new MultiplicarFila[filaA];

                        for (int i = 0; i < filaA; i++) {
                            hebras[i] = new MultiplicarFila(matrices.get(0)[i], matrices.get(indice), i);
                            hebras[i].start();
                        }

                        for (int i = 0; i < filaA; i++) {
                            hebras[i].join();
                            matrizC[hebras[i].getFilaIndice()] = hebras[i].getResultado();

                        }
                        // Se termina la multiplicacion de las matrices y al mismo tiempo se hace un remplazo de los valores

                        System.out.println("Resultado de la multiplicación:");
                        for (int[] fila : matrizC) {
                            for (int val : fila) {
                                System.out.print(val + " ");
                            }
                            System.out.println();
                        }

                        // se rempliza la matriz A por la resultante C para hacer el siguiente calculo 
                        matrices.set(0, matrizC);

                        //System.out.println(trasnpuesta(MatrizC));

                    }else{
                        System.out.println("no se puede multiplicar");
                    }
                }





            }else{
                System.out.println("No se puede leer el archivo o no existe.");
            }
       
            } catch (FileNotFoundException e) {
                System.out.println("Archivo no encontrado: " + e.getMessage());
            }


    }

static int Fila(int[][] matriz) {
    return matriz.length;
}

static int Columna(int[][] matriz) {
    if (matriz.length > 0) {
        return matriz[0].length;
    } else {
        return 0; // O puedes lanzar una excepción si prefieres
    }
}

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

    
}

