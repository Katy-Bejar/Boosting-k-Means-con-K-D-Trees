import csv
import random
import matplotlib.pyplot as plt
import numpy as np

class KDNode:
    def __init__(self, point, left=None, right=None, axis=None):
        self.point = point
        self.left = left
        self.right = right
        self.axis = axis

def build_kdtree(points, depth=0):
    if not points:
        return None
    k = len(points[0])
    axis = depth % k
    points.sort(key=lambda x: x[axis])
    median = len(points) // 2
    return KDNode(
        point=points[median],
        left=build_kdtree(points[:median], depth + 1),
        right=build_kdtree(points[median + 1:], depth + 1),
        axis=axis
    )

def euclidean_distance(point1, point2):
    return np.linalg.norm(np.array(point1) - np.array(point2))

def closest_point(root, target, depth=0, best=None, tol=1e-5):
    if root is None:
        return best

    k = len(target)
    axis = depth % k
    next_best = None
    next_branch = None

    if best is None or euclidean_distance(target, best) > euclidean_distance(target, root.point):
        next_best = root.point
    else:
        next_best = best

    if target[axis] < root.point[axis]:
        next_branch = root.left
    else:
        next_branch = root.right

    if np.allclose(target, next_best, atol=tol):
        return next_best
    else:
        return closest_point(next_branch, target, depth + 1, next_best, tol)

def kmeans_kdtree(points, k, iterations):
    kdtree = build_kdtree(points)
    centroids = random.sample(points, k)

    for _ in range(iterations):
        clusters = {i: [] for i in range(k)}
        for point in points:
            closest_centroid = closest_point(kdtree, point, tol=1e-5)

            # Encontrar el índice del centroide más cercano
            cluster_index = None
            for i, centroid in enumerate(centroids):
                if np.allclose(centroid, closest_centroid, atol=1e-5):
                    cluster_index = i
                    break

            # Asignar el punto al cluster correspondiente
            if cluster_index is not None:
                clusters[cluster_index].append(point)

        # Actualizar los centroides
        for i in range(k):
            if clusters[i]:
                centroids[i] = np.mean(clusters[i], axis=0)

        # Imprimir puntos finales de todos los centroides
        print(f"Iteration {_ + 1}: Centroides finales")
        for i, centroid in enumerate(centroids):
            print(f"Centroide {i + 1}: {centroid}")
        print()

        # Imprimir todos los puntos
        print(f"Iteration {_ + 1}: All Points")
        print(np.array(points))
        print()

        # Graficar todos los puntos y centroides
        plot_clusters_all_points(points, centroids, clusters, _ + 1)

# Función para graficar todos los puntos y centroides
def plot_clusters_all_points(points, centroids, clusters, iteration):
    colors = ['r', 'g', 'b', 'c', 'm', 'y', 'k']
    plt.figure()

    # Graficar todos los puntos
    all_points = np.array(points)
    plt.scatter(all_points[:, 0], all_points[:, 1], c='gray', alpha=0.5, label='All Points')

    # Graficar centroides
    centroids = np.array(centroids)
    plt.scatter(centroids[:, 0], centroids[:, 1], c='black', marker='X', label='Centroids')

    # Graficar clusters
    for i, cluster_points in clusters.items():
        cluster_points = np.array(cluster_points)
        color_index = i % len(colors)  # Ajustar el índice para evitar desbordamiento
        plt.scatter(cluster_points[:, 0], cluster_points[:, 1], c=colors[color_index], label=f'Cluster {i + 1}')

    plt.title(f'K-means Clustering - Iteration {iteration}')
    plt.legend()
    plt.show()

# Leer datos desde el archivo CSV
data = []
with open('data2k.csv', 'r') as file:
    reader = csv.reader(file)
    for row in reader:
        data.append([float(row[0]), float(row[1])])

# Parámetros del algoritmo
k_value = 18
num_iterations = 10

# Ejecutar K-means con KD-tree
kmeans_kdtree(data, k_value, num_iterations)
