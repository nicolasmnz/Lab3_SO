import pandas as pd
import matplotlib.pyplot as plt
import os

# Ruta de trabajo (mismo directorio del script)
BASE_DIR = os.path.dirname(os.path.abspath(__file__))

# Archivos CSV
c_path    = os.path.join(BASE_DIR, 'c_measurements.csv')
java_path = os.path.join(BASE_DIR, 'java_measurements.csv')

# Cargar datos
c_df    = pd.read_csv(c_path)
java_df = pd.read_csv(java_path)

# Índice de prueba (0, 1, 2, …)
pruebas = list(range(len(c_df)))

# 1) Tiempos de ejecución
plt.figure()
plt.plot(pruebas, c_df['wall_time-s'], label='C (fork + pipe)')
plt.plot(pruebas, java_df['wall_time-s'], label='Java (threads)')
plt.xlabel('Índice de prueba')
plt.ylabel('Tiempo de ejecución (s)')
plt.title('Comparativa de Tiempos')
plt.legend()
plt.tight_layout()
plt.savefig(os.path.join(BASE_DIR, 'tiempos_comparativa.png'))

# 2) Uso de memoria
plt.figure()
plt.plot(pruebas, c_df['mem_rss-kb'], label='C (fork + pipe)')
plt.plot(pruebas, java_df['mem_rss-kb'], label='Java (threads)')
plt.xlabel('Índice de prueba')
plt.ylabel('Memoria RSS (KB)')
plt.title('Comparativa de Memoria')
plt.legend()
plt.tight_layout()
plt.savefig(os.path.join(BASE_DIR, 'memoria_comparativa.png'))

# 3) Uso de CPU
plt.figure()
plt.plot(pruebas, c_df['percent_cpu'], label='C (fork + pipe)')
plt.plot(pruebas, java_df['percent_cpu'], label='Java (threads)')
plt.xlabel('Índice de prueba')
plt.ylabel('Uso de CPU (%)')
plt.title('Comparativa de CPU')
plt.legend()
plt.tight_layout()
plt.savefig(os.path.join(BASE_DIR, 'cpu_comparativa.png'))

print("Gráficas generadas en:", BASE_DIR)
print("  - tiempos_comparativa.png")
print("  - memoria_comparativa.png")
print("  - cpu_comparativa.png")
