#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <pthread.h>
#include <cstdlib>
#include <random>
#include <algorithm>
pthread_mutex_t centroid_mutex;
pthread_mutex_t cluster_mutex;

struct Point {
    double x;
    double y;
};

struct Args {
    std::vector<Point>* centroids;
    std::vector<int> clusters;
    std::vector<Point> points;
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

void* updateCentroids(void* args_void) {
    auto args = static_cast<Args*>(args_void);
    std::vector<Point>& centroids = *(args->centroids);
    std::vector<Point>& points = args->points;
    std::vector<int>& clusters = args->clusters;

    std::vector<int> clusterCounts(centroids.size(), 0);
    std::vector<double> clusterSumsX(centroids.size(), 0.0);
    std::vector<double> clusterSumsY(centroids.size(), 0.0);

    for (size_t i = 0; i < points.size(); ++i) {
        int cluster = clusters[i];
        clusterCounts[cluster]++;
        clusterSumsX[cluster] += points[i].x;
        clusterSumsY[cluster] += points[i].y;
    }

    pthread_mutex_lock(&centroid_mutex);

    for (size_t i = 0; i < centroids.size(); ++i) {
        if (clusterCounts[i] > 0) {
            centroids[i].x = clusterSumsX[i] / clusterCounts[i];
            centroids[i].y = clusterSumsY[i] / clusterCounts[i];
        }
    }

    pthread_mutex_unlock(&centroid_mutex);
    return nullptr;
}

std::vector<int> kMeansClustering(std::vector<Point>& points, std::vector<Point>& initialCentroids, int k, int numThreads) {
    std::vector<Point> centroids = initialCentroids;
    std::vector<int> clusters(points.size(), -1);
    pthread_mutex_init(&centroid_mutex, NULL);
    pthread_mutex_init(&cluster_mutex, NULL);

    bool centroidsUpdated = true;

    while (centroidsUpdated) {
        centroidsUpdated = false; 

        for (size_t i = 0; i < points.size(); ++i) {
            int nearestCluster = findNearestCluster(centroids, points[i]);

            pthread_mutex_lock(&cluster_mutex);
            if (clusters[i] != nearestCluster) {
                clusters[i] = nearestCluster;
                centroidsUpdated = true; 
            }
            pthread_mutex_unlock(&cluster_mutex);
        }

        if (!centroidsUpdated) {
            break;

        }

        pthread_t threads[numThreads];
        std::vector<Args*> args(numThreads);

        for (int i = 0; i < numThreads; ++i) {
            args[i] = new Args{&centroids, clusters, points};
            pthread_create(&threads[i], NULL, updateCentroids, static_cast<void*>(args[i]));
        }

        for (int i = 0; i < numThreads; ++i) {
            pthread_join(threads[i], NULL);
        }
    }

    pthread_mutex_destroy(&centroid_mutex);
    pthread_mutex_destroy(&cluster_mutex);
    return clusters;
}

int main(int argc, char* argv[]) {
    int numThreads = 2;
    int k = 2;
    if (argc > 2) {
        numThreads = std::atoi(argv[1]);
        k = std::atoi(argv[2]);
    }

    if (numThreads < 1) {
        throw std::logic_error("Num of threads must be positive");
    }
    if (k < 1) {
        throw std::logic_error("Num of cluster must be positive");
    }

    std::vector<Point> points;

    std::vector<Point> initialCentroids(k);
    std::random_device rd;
    std::mt19937 rng(rd());

    std::uniform_real_distribution<double> dist(-11111.1, 11111.1);

    for (int i = 0; i < 10000; ++i) {
        double x = dist(rng);
        double y = dist(rng);
        points.emplace_back(Point{x, y});
    }
    std::sample(points.begin(), points.end(), initialCentroids.begin(), k, rng);


    std::vector<int> clusters = kMeansClustering(points, initialCentroids, k, numThreads);

    for (int cluster = 0; cluster < k; ++cluster) {
        std::cout << "Cluster " << cluster + 1 << ":" << std::endl;
        std::cout << "Centroid: (" << initialCentroids[cluster].x << ", " << initialCentroids[cluster].y << ")" << std::endl;
        for (size_t i = 0; i < points.size(); ++i) {
            if (clusters[i] == cluster) {
                std::cout << "Point (" << points[i].x << ", " << points[i].y << ")" << std::endl;
            }
        }
        std::cout << std::endl;
    }

    return 0;
}
