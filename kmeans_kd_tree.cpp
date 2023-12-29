#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <numeric>

struct Point {
    double x, y;

    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}

    // Sobrecarga del operador de igualdad
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

struct KDNode {
    Point point;
    int axis;
    KDNode* left;
    KDNode* right;

    KDNode(Point p, int axis) : point(p), axis(axis), left(nullptr), right(nullptr) {}
};

double distance(const Point& p1, const Point& p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

KDNode* buildKDTree(const std::vector<Point>& points, int depth) {
    if (points.empty()) {
        return nullptr;
    }

    int axis = depth % 2; // Alternar entre las dimensiones x e y
    auto cmp = (axis == 0) ? [](const Point& p1, const Point& p2) { return p1.x < p2.x; }
                           : [](const Point& p1, const Point& p2) { return p1.y < p2.y; };

    std::vector<Point> sortedPoints = points;
    std::sort(sortedPoints.begin(), sortedPoints.end(), cmp);
    int medianIndex = sortedPoints.size() / 2;
    KDNode* node = new KDNode(sortedPoints[medianIndex], axis);

    // Construir subárboles recursivamente
    std::vector<Point> leftPoints(sortedPoints.begin(), sortedPoints.begin() + medianIndex);
    std::vector<Point> rightPoints(sortedPoints.begin() + medianIndex + 1, sortedPoints.end());
    node->left = buildKDTree(leftPoints, depth + 1);
    node->right = buildKDTree(rightPoints, depth + 1);

    return node;
}

std::pair<KDNode*, double> findNearestNeighbor(KDNode* root, const Point& target, int depth) {
    if (root == nullptr) {
        return std::make_pair(nullptr, std::numeric_limits<double>::max());
    }

    int axis = depth % 2;
    KDNode* nextBranch = (axis == 0 && target.x < root->point.x) || (axis == 1 && target.y < root->point.y)
                             ? root->left
                             : root->right;

    auto [bestNode, bestDistance] = findNearestNeighbor(nextBranch, target, depth + 1);

    double currentDistance = distance(target, root->point);
    if (currentDistance < bestDistance) {
        bestDistance = currentDistance;
        bestNode = root;
    }

    double axisDistance = (axis == 0) ? std::abs(target.x - root->point.x) : std::abs(target.y - root->point.y);

    if (axisDistance < bestDistance) {
        KDNode* otherBranch = (nextBranch == root->left) ? root->right : root->left;
        auto [otherBest, otherDistance] = findNearestNeighbor(otherBranch, target, depth + 1);

        if (otherBest != nullptr && otherDistance < bestDistance) {
            bestDistance = otherDistance;
            bestNode = otherBest;
        }
    }

    return std::make_pair(bestNode, bestDistance);
}

std::vector<Point> kMeansKDTree(const std::vector<Point>& data, int k, int maxIterations) {
    // Construir KD-Tree
    KDNode* root = buildKDTree(data, 0);

    // Inicialización de centroides aleatorios sin duplicados
    std::vector<Point> centroids;
    std::vector<int> centroidIndices(data.size());
    std::iota(centroidIndices.begin(), centroidIndices.end(), 0);
    std::random_shuffle(centroidIndices.begin(), centroidIndices.end());

    for (int i = 0; i < k; ++i) {
        centroids.push_back(data[centroidIndices[i]]);
    }

    for (int iteration = 0; iteration < maxIterations; ++iteration) {
        // Asignación de puntos a centroides más cercanos usando el KD-Tree
        for (int i = 0; i < data.size(); ++i) {
            auto [nearestNode, _] = findNearestNeighbor(root, data[i], 0);
            int cluster = std::distance(centroids.begin(), std::find(centroids.begin(), centroids.end(), nearestNode->point));
            centroidIndices[i] = cluster;
        }

        // Actualización de centroides
        std::vector<int> counts(k, 0);
        std::vector<Point> newCentroids(k, Point());
        for (int i = 0; i < data.size(); ++i) {
            int cluster = centroidIndices[i];
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

    // Liberar memoria del KD-Tree
    delete root;

    return centroids;
}

int main() {
    // Lectura del archivo CSV
    std::ifstream file("data2k.csv");
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return 1;
    }

    std::vector<Point> data;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        double x, y;
        char comma;
        if (iss >> x >> comma >> y) {
            data.emplace_back(x, y);
        } else {
            //std::cerr << "Error al leer la línea: " << line << std::endl;
        }
    }
    file.close();

    // Parámetros de kMeansKDTree
    int k =3;
    int maxIterations = 5;

    // Aplicar kMeansKDTree
    std::vector<Point> centroids = kMeansKDTree(data, k, maxIterations);

    // Imprimir los centroides
    std::cout << "Centroides:\n";
    for (const auto& centroid : centroids) {
        std::cout << "Centroide: (" << centroid.x << ", " << centroid.y << ")\n";
    }

    return 0;
}