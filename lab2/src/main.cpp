#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <pthread.h>
#include <cstdlib>
#include <random>
#include <algorithm>

struct Point {
    double x;
    double y;
};

struct Args {
    std::vector<Point>* centroids;
    std::vector<int> clusters;
    std::vector<Point> points;
    int start;
    int end;
};

double calculateDistance(Point point1, Point point2) {
    double x = point2.x - point1.x;
    double y = point2.y - point1.y;
    return std::sqrt(x * x + y * y);
}

int findNearestCluster(std::vector<Point>& centroids, Point point) {
    double min_distance = std::numeric_limits<double>::max();
    int near_cluster = -1;

    for (size_t i = 0; i < centroids.size(); ++i) {
        double distance = calculateDistance(centroids[i], point);

        if (distance < min_distance) {
            min_distance = distance;
            near_cluster = i;
        }
    }

    return near_cluster;
}

void* updateCentroids(void* args_void) {
    auto args = static_cast<Args*>(args_void);
    std::vector<Point>& centroids = *(args->centroids);
    std::vector<Point>& points = args->points;
    std::vector<int>& clusters = args->clusters;

    std::vector<int> count(centroids.size(), 0);
    std::vector<double> sumX(centroids.size(), 0.0);
    std::vector<double> sumY(centroids.size(), 0.0);

    for (size_t i = 0; i < points.size(); ++i) {
        int cluster = clusters[i];
        count[cluster]++;
        sumX[cluster] += points[i].x;
        sumY[cluster] += points[i].y;
    }

    for (int i = args->start; i < args->end; ++i) {
        if (count[i] > 0) {
            centroids[i].x = sumX[i] / count[i];
            centroids[i].y = sumY[i] / count[i];
        }
    }

    return nullptr;
}

std::vector<int> kMeansClustering(std::vector<Point>& points, std::vector<Point>& init_centroids, int thread_num) {
    std::vector<Point> centroids = init_centroids;
    std::vector<int> clusters(points.size(), -1);

    bool update_centroid = true;

    while (update_centroid) {
        update_centroid = false;

        for (size_t i = 0; i < points.size(); ++i) {
            int near_cluster = findNearestCluster(centroids, points[i]);
            if (clusters[i] != near_cluster) {
                clusters[i] = near_cluster;
                update_centroid = true;
            }
        }

        if (!update_centroid) {
            break;
        }

        pthread_t threads[thread_num];
        std::vector<Args*> args(thread_num);

        int k = centroids.size() / thread_num;
        int m = centroids.size() % thread_num;
        int start = 0;
        int end = k;

        for (int i = 0; i < thread_num; ++i) {
            if (i < m) {
                end++;
            }

            args[i] = new Args{&centroids, clusters, points, start, end};
            pthread_create(&threads[i], NULL, updateCentroids, static_cast<void*>(args[i]));

            start = end;
            end += k;
        }
        for (int i = 0; i < thread_num; ++i) {
            pthread_join(threads[i], NULL);
            delete args[i];
        }
    }

    return clusters;
}

int main(int argc, char* argv[]) {
    int thread_num = 1;
    int k = 2;

    if (argc > 2) {
        thread_num = std::atoi(argv[1]);
        k = std::atoi(argv[2]);
    }

    if (thread_num < 1) {
        throw std::logic_error("Num of threads must be positive");
    }

    if (k < 1) {
        throw std::logic_error("Num of cluster must be positive");
    }

    std::vector<Point> points;

    std::vector<Point> init_centroids(k);
    std::default_random_engine rng(9999);
    std::uniform_real_distribution<double> dist(-1111111.1, 1111111.1);

    for (int i = 0; i < 10000000; ++i) {
        double x = dist(rng);
        double y = dist(rng);
        points.emplace_back(Point{x, y});
    }

    std::sample(points.begin(), points.end(), init_centroids.begin(), k, rng);
    std::vector<int> clusters = kMeansClustering(points, init_centroids, thread_num);
     
    // for (int cluster = 0; cluster < k; ++cluster) {
    //     std::cout << "Cluster " << cluster + 1 << ":" << std::endl;
    //     for (size_t i = 0; i < points.size(); ++i) {
    //         if (clusters[i] == cluster) {
    //             std::cout << "Point (" << points[i].x << ", " << points[i].y << ")" << std::endl;
    //         }
    //     }
    //     std::cout << std::endl;
    // }

    return 0;
}
