#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <chrono>
using namespace std;
using namespace std::chrono;

struct Point {
    double x, y;

    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}
};

double distance(const Point& p1, const Point& p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

vector<Point> kMeansBruteforce(const vector<Point>& data, int k, int maxIterations) {
    vector<Point> centroids(k, Point());  
    vector<int> assignments(data.size(), 0);

    // Inicialización de centroides aleatorios
    random_shuffle(assignments.begin(), assignments.end());
    for (int i = 0; i < k; ++i) {
        centroids[i] = data[assignments[i]];
    }

    auto start = high_resolution_clock::now();  // Inicio de la medición de tiempo

    for (int iteration = 0; iteration < maxIterations; ++iteration) {
        // Asignación de puntos a centroides más cercanos
        for (int i = 0; i < data.size(); ++i) {
            double minDist = numeric_limits<double>::max();
            for (int j = 0; j < k; ++j) {
                double dist = distance(data[i], centroids[j]);
                if (dist < minDist) {
                    minDist = dist;
                    assignments[i] = j;
                }
            }
        }

        // Actualización de centroides
        vector<int> counts(k, 0);
        vector<Point> newCentroids(k, Point());
        for (int i = 0; i < data.size(); ++i) {
            int cluster = assignments[i];
            newCentroids[cluster].x += data[i].x;
            newCentroids[cluster].y += data[i].y;
            counts[cluster]++;
        }

        for (int i = 0; i < k; ++i) {
            if (counts[i] > 0) {
                centroids[i].x = newCentroids[i].x / counts[i];
                centroids[i].y = newCentroids[i].y / counts[i];
            }
        }
    }

    auto stop = high_resolution_clock::now();  // Fin de la medición de tiempo
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Tiempo tomado por kMeansBruteforce: " << duration.count() << " microsegundos." << endl;

    return centroids;
}

int main() {
    // Lectura del archivo CSV
    ifstream file("data2k.csv");
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo." << endl;
        return 1;
    }

    vector<Point> data;
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        double x, y;
        char comma;
        if (iss >> x >> comma >> y) {
            data.emplace_back(x, y);
        } else {
            //cerr << "Error al leer la línea: " << line << endl;
        }
    }
    file.close();

    // Parámetros de kMeansBruteforce
    int k = 18;
    int maxIterations = 100;

    // Aplicar kMeansBruteforce
    vector<Point> centroids = kMeansBruteforce(data, k, maxIterations);

    // Imprimir los centroides
    cout << "Centroides:\n";
    for (const auto& centroid : centroids) {
        cout << "Centroide: (" << centroid.x << ", " << centroid.y << ")\n";
    }

    return 0;
}