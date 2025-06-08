import pandas as pd
import matplotlib.pyplot as plt

# Ajusta estas rutas a tus archivos CSV
c_file = 'c_measurements.csv'
java_file = 'java_measurements.csv'

# Leer los datos
c_df = pd.read_csv(c_file)
java_df = pd.read_csv(java_file)

# Crear un índice de pruebas
pruebas = list(range(len(c_df)))

# 1. Gráfica de tiempos de ejecución (wall_time-s)
plt.figure()
plt.plot(pruebas, c_df['wall_time-s'], label='C (forks + pipes)')
plt.plot(pruebas, java_df['wall_time-s'], label='Java (threads)')
plt.xlabel('Índice de prueba')
plt.ylabel('Tiempo de ejecución (s)')
plt.title('Comparación de Tiempos de Ejecución')
plt.legend()
plt.tight_layout()
plt.show()

# 2. Gráfica de uso de memoria (mem_rss-kb)
plt.figure()
plt.plot(pruebas, c_df['mem_rss-kb'], label='C (forks + pipes)')
plt.plot(pruebas, java_df['mem_rss-kb'], label='Java (threads)')
plt.xlabel('Índice de prueba')
plt.ylabel('Memoria RSS (KB)')
plt.title('Comparación de Uso de Memoria')
plt.legend()
plt.tight_layout()
plt.show()

# 3. Gráfica de uso de CPU (percent_cpu)
plt.figure()
plt.plot(pruebas, c_df['percent_cpu'], label='C (forks + pipes)')
plt.plot(pruebas, java_df['percent_cpu'], label='Java (threads)')
plt.xlabel('Índice de prueba')
plt.ylabel('Uso de CPU (%)')
plt.title('Comparación de Uso de CPU')
plt.legend()
plt.tight_layout()
plt.show()
