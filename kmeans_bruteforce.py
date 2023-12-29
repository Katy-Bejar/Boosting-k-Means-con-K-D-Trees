import matplotlib.pyplot as plt
import pandas as pd
import random
import math

class Point:
    def __init__(self, x=0, y=0):
        self.x = x
        self.y = y

def distance(p1, p2):
    return math.sqrt((p1.x - p2.x)**2 + (p1.y - p2.y)**2)

def kMeansBruteforce(data, k, max_iterations):
    centroids = [Point(random.uniform(min(data, key=lambda p: p.x).x, max(data, key=lambda p: p.x).x),
                      random.uniform(min(data, key=lambda p: p.y).y, max(data, key=lambda p: p.y).y))
                 for _ in range(k)]
    
    assignments = [0] * len(data)

    for iteration in range(max_iterations):
        # Assign points to the nearest centroids
        for i in range(len(data)):
            min_dist = float('inf')
            for j in range(k):
                dist = distance(data[i], centroids[j])
                if dist < min_dist:
                    min_dist = dist
                    assignments[i] = j

        # Update centroids
        counts = [0] * k
        new_centroids = [Point() for _ in range(k)]
        for i in range(len(data)):
            cluster = assignments[i]
            new_centroids[cluster].x += data[i].x
            new_centroids[cluster].y += data[i].y
            counts[cluster] += 1

        for i in range(k):
            if counts[i] > 0:
                centroids[i].x = new_centroids[i].x / counts[i]
                centroids[i].y = new_centroids[i].y / counts[i]

        # Plot centroids and data points
        plt.scatter([p.x for p in data], [p.y for p in data], c=assignments, cmap='viridis')
        plt.scatter([c.x for c in centroids], [c.y for c in centroids], marker='X', s=200, c='red')
        plt.title(f"Iteration {iteration + 1}")
        plt.xlabel('X')
        plt.ylabel('Y')
        plt.show()

    return centroids

# Read data from CSV
data_df = pd.read_csv('data2k.csv')

# Assuming the first and second columns are X and Y, respectively
data_points = [Point(row.iloc[0], row.iloc[1]) for index, row in data_df.iterrows()]

# Parameters
k_value = 18
max_iterations_value = 10

# Apply kMeansBruteforce
final_centroids = kMeansBruteforce(data_points, k_value, max_iterations_value)

# Print final centroids
print("\nCentroides finales:")
for centroid in final_centroids:
    print(f"Centroide: ({centroid.x}, {centroid.y})")
