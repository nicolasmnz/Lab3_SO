mport java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;
import java.io.IOException;
import java.lang.management.ManagementFactory;
import java.lang.management.ThreadMXBean;
import java.util.ArrayList;

public class Main {
    static List<matriz> LeerArchivo() {
        String nombreArchivo = "archivo.txt";
        List<matriz> matrices = new ArrayList<>();
        try {
            List<String> lineas = Files.readAllLines(Paths.get(nombreArchivo));
            int n_m = Integer.parseInt(lineas.get(0)); 
            int i = 1; 
            int m_c = 0;
            while(m_c < n_m) {
                List<Integer> dimensions = extraerEnteros(lineas.get(i));
                if(!dimensions.isEmpty()) {
                    int y = dimensions.get(0); 
                    int x = dimensions.get(1); 
                    matriz mat = new matriz(y, x);
                    for(int ejey = 0; ejey < y; ejey++) {
                        i++;
                        for(int ejex = 0; ejex < x; ejex++) {
                            mat.set(ejey, ejex, (extraerEnteros(lineas.get(i)).get(ejex)));
                        }
                    }
                    matrices.add(mat);
                    m_c++;
                }
                i++;
            }
        } 
        catch (IOException e) {
            System.err.println("Error al leer el archivo: " + e.getMessage());
        }
        return matrices;
    }
    public static List<Integer> extraerEnteros(String texto) {
        List<Integer> numeros = new ArrayList<>();
        StringBuilder numeroActual = new StringBuilder();
        
        for (int i = 0; i < texto.length(); i++) {
            char c = texto.charAt(i);
            if (Character.isDigit(c)) {
                numeroActual.append(c);
            } else if (numeroActual.length() > 0) {
                try {
                    numeros.add(Integer.parseInt(numeroActual.toString()));
                } catch (NumberFormatException e) {
                }
                numeroActual.setLength(0);
            }
        }
        if (numeroActual.length() > 0) {
            try {
                numeros.add(Integer.parseInt(numeroActual.toString()));
            } catch (NumberFormatException e) {
            }
        }
        return numeros;
    }
    public static void main(String[] args) {
        String nombreArchivo = "archivo.txt";
        System.out.println("=== Procesando archivo: " + nombreArchivo + " ===");
    
        long inicioArchivo = System.nanoTime();
    
        List<matriz> matricesLeidas = LeerArchivo();
        try {
            int n_filas = matricesLeidas.get(0).getFilas();
    
            Runtime runtime = Runtime.getRuntime();
            runtime.gc();
            long memoriaAntes = runtime.totalMemory() - runtime.freeMemory();
    
            ThreadMXBean gestion = ManagementFactory.getThreadMXBean();
            long inicio = bean.getCurrentThreadCpuTime();
            long tiempoInicio = System.nanoTime();
    
            System.out.println("Multiplicando 2 matrices...");
            matriz resultado = matricesLeidas.get(0).multiplicarParalelo(matricesLeidas.get(1), n_filas);
    
            long tiempoFin = System.nanoTime();
            long cpuFin = bean.getCurrentThreadCpuTime();
            long memoriaDespues = runtime.totalMemory() - runtime.freeMemory();
    
            long finArchivo = System.nanoTime();
    
            double tiempoMultiplicacion = (tiempoFin - tiempoInicio) / 1_000_000_000.0;
            double tiempoArchivoTotal = (finArchivo - inicioArchivo) / 1_000_000_000.0;
            double tiempoCpuUsuario = (cpuFin - cpuInicio) / 1_000_000_000.0;
            double tiempoCpuSistema = tiempoArchivoTotal - tiempoCpuUsuario;
            long memoriaUsadaKB = (memoriaDespues - memoriaAntes) / 1024;
    
            System.out.printf("Tiempo multiplicación: %.7f segundos%n", tiempoMultiplicacion);
            System.out.printf("Tiempo total archivo: %.7f segundos%n%n", tiempoArchivoTotal);
            
            System.out.println("=== Estadísticas Finales ===");
            System.out.printf("Tiempo total ejecución: %.5f segundos%n", tiempoArchivoTotal);
            System.out.printf("Tiempo CPU usuario: %.6f segundos%n", tiempoCpuUsuario);
            System.out.printf("Tiempo CPU sistema: %.5f segundos%n", tiempoCpuSistema);
            System.out.printf("Memoria máxima utilizada: %d KB%n", memoriaUsadaKB);
    
            System.out.println("Resultado de la multiplicación:");
            resultado.imprimir();
    
        } catch (IllegalArgumentException e) {
            System.err.println("Error: " + e.getMessage());
        }
    }
    
}