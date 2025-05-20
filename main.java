import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class main {
        public static void main(String[] args) {
        System.out.println("¡Hola, mundo!");

        File archivo = new File("entrada.txt");
        boolean verificacion = archivo.canRead();
        
        if (verificacion){
            System.out.println("El archivo se puede leer.");
        }else{
            System.out.println("No se puede leer el archivo o no existe.");
        }
    }
}
dasds