import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Main {
        public static void main(String[] args) {

        List<int[][]> matrices = new ArrayList<>();
        System.out.println("¡Hola, mundo!");
        try{
        File archivo = new File("entrada.txt");
        boolean verificacion = archivo.canRead();
        
            if (verificacion){
                System.out.println("El archivo se puede leer.");
                Scanner lector = new Scanner(archivo);

            
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

            }else{
                System.out.println("No se puede leer el archivo o no existe.");
            }


            for(int i=0; i<2 ;i++){
                System.out.println("#### matriz i ####");
                dimensiones(matrices.get(i));

            }



        } catch (FileNotFoundException e) {
            System.out.println("Archivo no encontrado: " + e.getMessage());
        }

       

    }

static int[] dimensiones(int[][] matriz) {
    int filas = matriz.length;
    int columnas = matriz[0].length;
    return new int[] { filas, columnas };
}


    
}

