#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <chrono> // Agregado para medir el tiempo
using namespace std;
using namespace chrono;

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

    random_shuffle(assignments.begin(), assignments.end());
    for (int i = 0; i < k; ++i) {
        centroids[i] = data[assignments[i]];
    }

    for (int iteration = 0; iteration < maxIterations; ++iteration) {
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

    return centroids;
}

int main() {
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

    int k = 18;
    int maxIterations = 100;

    // Medición del tiempo de ejecución
    auto start_time = high_resolution_clock::now();

    vector<Point> centroids = kMeansBruteforce(data, k, maxIterations);

    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time);

    // Imprimir los centroides
    cout << "Centroides:\n";
    for (const auto& centroid : centroids) {
        cout << "Centroide: (" << centroid.x << ", " << centroid.y << ")\n";
    }

    // Imprimir el tiempo de ejecución
    cout << "Tiempo de ejecución: " << duration.count() << " milliseconds\n";

    return 0;
}
