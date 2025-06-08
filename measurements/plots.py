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

# Parámetros de estilo comunes
ALPHA = 0.5       # transparencia
SIZE  = 15        # tamaño de marcador
DPI   = 120       # resolución
GRID_KWARGS = {
    'linestyle': '--',
    'linewidth': 0.5,
    'alpha': 0.5
}

def save_scatter(x, y1, y2, ylabel, title, fname):
    plt.figure(dpi=DPI)
    plt.scatter(x, y1,
                label='C (fork + pipe)',
                alpha=ALPHA,
                s=SIZE,
                linewidths=0)
    plt.scatter(x, y2,
                label='Java (threads)',
                alpha=ALPHA,
                s=SIZE,
                linewidths=0)
    plt.xlabel('Número de fila')
    plt.ylabel(ylabel)
    plt.title(title)
    plt.legend()
    plt.grid(True, **GRID_KWARGS)
    leg = plt.legend(
    loc='upper left',               # posición ancla dentro de la figura
    bbox_to_anchor=(1.02, 1),       # 1.02 en x saca la leyenda un poco a la derecha
    borderaxespad=0                 # sin espacio extra entre ejes y caja
)

    plt.tight_layout(rect=[0, 0, 0.85, 1])  # deja un margen derecho para la leyenda
    plt.tight_layout()
    salida = os.path.join(BASE_DIR, fname)
    plt.savefig(salida)
    print(f"Guardado: {fname}")

# Índice de prueba (o el campo que uses en X)
x = c_df['num_fila']

# 1) Tiempo de ejecución
save_scatter(
    x,
    c_df['wall_time-s'],
    java_df['wall_time-s'],
    'Tiempo de ejecución (s)',
    'Comparativa de Tiempos',
    'tiempos_comparativa_scatter.png'
)

# 2) Uso de memoria
save_scatter(
    x,
    c_df['mem_rss-kb'],
    java_df['mem_rss-kb'],
    'Memoria RSS (KB)',
    'Comparativa de Memoria',
    'memoria_comparativa_scatter.png'
)

# 3) Uso de CPU
save_scatter(
    x,
    c_df['percent_cpu'],
    java_df['percent_cpu'],
    'Uso de CPU (%)',
    'Comparativa de CPU',
    'cpu_comparativa_scatter.png'
)

print("\n¡Todas las gráficas generadas con éxito en:", BASE_DIR, "!")
