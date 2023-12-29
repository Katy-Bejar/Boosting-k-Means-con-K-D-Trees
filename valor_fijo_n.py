import subprocess
import time
import matplotlib.pyplot as plt
import pandas as pd

# Función para ejecutar el código C++ y medir el tiempo en milisegundos
def run_cpp_code(file_path, k, n):
    command = ["g++", file_path, "-o", "kmeans"]
    subprocess.run(command)

    start_time = time.time()
    subprocess.run(["./kmeans", str(k), str(n)])
    end_time = time.time()

    return (end_time - start_time) * 1000  # Convertir a milisegundos

# Archivo CSV con los datos
csv_file = "data2k.csv"

# Valores de k y n
k_values = [5, 15, 25, 50, 75, 100, 125, 150, 200]
n_values = [1000, 1450, 1900, 2400]

# Almacenar los tiempos de ejecución para cada versión y combinación de k y n
bruteforce_times = []
kd_tree_times = []

# Ruta de los archivos de código C++
bruteforce_file = "kmeans_bruteforce.cpp"
kd_tree_file = "kmeans_kd_tree.cpp"

# Ejecutar las versiones de k-means y medir el tiempo
for n in n_values:
    bruteforce_row = []
    kd_tree_row = []
    for k in k_values:
        bruteforce_time = run_cpp_code(bruteforce_file, k, n)
        kd_tree_time = run_cpp_code(kd_tree_file, k, n)

        bruteforce_row.append(bruteforce_time)
        kd_tree_row.append(kd_tree_time)

    bruteforce_times.append(bruteforce_row)
    kd_tree_times.append(kd_tree_row)

# Crear un DataFrame con los tiempos de ejecución
df_bruteforce = pd.DataFrame(bruteforce_times, columns=k_values, index=n_values)
df_kd_tree = pd.DataFrame(kd_tree_times, columns=k_values, index=n_values)

# Graficar los resultados
for n in n_values:
    plt.figure(figsize=(8, 6))
    plt.plot(df_bruteforce.loc[n], label="Bruteforce", marker='o')
    plt.plot(df_kd_tree.loc[n], label="KD-Tree", marker='o')
    plt.title(f"Cluster Points, n={n}")
    plt.xlabel("Number of Clusters (k)")
    plt.ylabel("Running Time (ms)")
    plt.legend()
    plt.grid(True)
    plt.show()
