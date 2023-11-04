#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <thread>
#include <mutex>
#include <cstdlib>

struct Point {
    double x;
    double y;
};

double calculateDistance(Point point1, Point point2) {
    double xDist = point2.x - point1.x;
    double yDist = point2.y - point1.y;
    return std::sqrt(xDist * xDist + yDist * yDist);
}

int findNearestCluster(std::vector<Point>& centroids, Point point) {
    double minDistance = std::numeric_limits<double>::max();
    int nearestCluster = -1;
    for (size_t i = 0; i < centroids.size(); ++i) {
        double distance = calculateDistance(centroids[i], point);
        if (distance < minDistance) {
            minDistance = distance;
            nearestCluster = i;
        }
    }
    return nearestCluster;
}

void updateCentroids(std::vector<Point>& centroids, std::vector<Point>& points, std::vector<int>& clusters, std::mutex& mutex) {
    std::vector<int> clusterCounts(centroids.size(), 0);
    std::vector<double> clusterSumsX(centroids.size(), 0.0);
    std::vector<double> clusterSumsY(centroids.size(), 0.0);
    for (size_t i = 0; i < points.size(); ++i) {
        int cluster = clusters[i];
        clusterCounts[cluster]++;
        clusterSumsX[cluster] += points[i].x;
        clusterSumsY[cluster] += points[i].y;
    }
    std::lock_guard<std::mutex> lock(mutex);
    for (size_t i = 0; i < centroids.size(); ++i) {
        if (clusterCounts[i] > 0) {
            centroids[i].x = clusterSumsX[i] / clusterCounts[i];
            centroids[i].y = clusterSumsY[i] / clusterCounts[i];
        }
    }
}

std::vector<int> kMeansClustering(std::vector<Point>& points, std::vector<Point>& initialCentroids, int k, int numThreads) {
    std::vector<Point> centroids = initialCentroids;
    std::vector<int> clusters(points.size(), -1);
    std::mutex mutex;
    while (true) {
        bool centroidsUpdated = false;
        for (size_t i = 0; i < points.size(); ++i) {
            int nearestCluster = findNearestCluster(centroids, points[i]);
            if (clusters[i] != nearestCluster) {
                clusters[i] = nearestCluster;
                centroidsUpdated = true;
            }
        }
        if (!centroidsUpdated) {
            break;
        }
        std::vector<std::thread> threads;
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back([&centroids, &points, &clusters, &mutex, i]() {
                updateCentroids(centroids, points, clusters, mutex);
            });
        }
        for (auto& thread : threads) {
            thread.join();
        }
    }
    return clusters;
}

int main(int argc, char* argv[]) {
    std::vector<Point> points = {{1.0, 2.0}, {2.5, 1.5}, {3.0, 4.0}, {5.0, 6.0}, {6.0, 3.0}};
    std::vector<Point> initialCentroids = {{1.5, 1.5}, {4.5, 4.5}};
    int k = 2;

    int numThreads = 2; 
    if (argc > 1) {
        numThreads = std::atoi(argv[1]);
    }
    if(numThreads < 1){
        throw std::logic_error("Num of threads must be positive");
    }

    std::vector<int> clusters = kMeansClustering(points, initialCentroids, k, numThreads);
    for (size_t i = 0; i < points.size(); ++i) {
        std::cout << "Point (" << points[i].x << ", " << points[i].y << ") belongs to Cluster " << clusters[i] << std::endl;
    }
    return 0;
}

