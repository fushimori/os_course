
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <string>
#include <cmath>
#include <cstring>
#include <atomic>

std::vector<std::pair<int, int>> points;
int num_clusters = 3;
std::mutex mtx;
bool errorFlag{false};

double distance(const std::pair<int, int>& point1, const std::pair<int, int>& point2) {
    int xDiff = point2.first - point1.first;
    int yDiff = point2.second - point1.second;
    return std::sqrt(xDiff * xDiff + yDiff * yDiff);
}

void kMeansClustering(std::vector<std::pair<int, int>>& centers, std::vector<std::vector<std::pair<int, int>>>& clusters) {
    if (points.empty() || static_cast<int>(points.size()) < num_clusters || num_clusters <= 0) {
        errorFlag = true;
        return;
    }

    clusters.clear();
    clusters.resize(num_clusters);

    for (int i = 0; i < num_clusters; ++i) {
        centers[i] = points[i];
    }

    bool done = false;
    while (!done) {
        mtx.lock();
        for (auto& cluster : clusters) {
            cluster.clear();
        }

        for (const auto& point : points) {
            double minDist = distance(point, centers[0]);
            int clusterIndex = 0;

            for (int i = 1; i < num_clusters; ++i) {
                double dist = distance(point, centers[i]);
                if (dist < minDist) {
                    minDist = dist;
                    clusterIndex = i;
                }
            }

            clusters[clusterIndex].push_back(point);
        }

        std::vector<std::pair<int, int>> prevCenters = centers;

        for (int i = 0; i < num_clusters; ++i) {
            int totalPoints = clusters[i].size();
            if (totalPoints > 0) {
                int sumX = 0, sumY = 0;
                for (const auto& point : clusters[i]) {
                    sumX += point.first;
                    sumY += point.second;
                }
                centers[i] = std::make_pair(sumX / totalPoints, sumY / totalPoints);
            }
        }

        done = true;
        for (int i = 0; i < num_clusters; ++i) {
            if (centers[i] != prevCenters[i]) {
                done = false;
                break;
            }
        }
        mtx.unlock();
    }
}

int main(int argc, char* argv[]) {
    int num_threads = 4;

    if (argc >= 5) {
        for (int i = 1; i < argc; i += 2) {
            if (std::strcmp(argv[i], "-p") == 0) {
                num_threads = std::stoi(argv[i + 1]);
            }
            else if (std::strcmp(argv[i], "-c") == 0) {
                num_clusters = std::stoi(argv[i + 1]);
            }
        }
    }
    else {
        std::cout << "Invalid command line arguments, using default values." << std::endl;
    }

    int x, y;
    while (std::cin >> x >> y) {
        std::pair<int, int> pr{ x, y };
        points.push_back(pr);
    }

    std::vector<std::pair<int, int>> centers(num_clusters);
    std::vector<std::vector<std::pair<int, int>>> clusters(num_clusters);

    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; i++) {
        threads.push_back(std::thread(kMeansClustering, std::ref(centers), std::ref(clusters)));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    if (errorFlag) {
        std::cout << "Error occurred in kMeansClustering. Exiting..." << std::endl;
        return 1;
    }

    for (int i = 0; i < num_clusters; ++i) {

        std::cout << "Cluster " << i + 1 << " center: (" << centers[i].first << ", " << centers[i].second << ")\n";
        std::cout << "Points in cluster " << i + 1 << ":\n";
        for (const auto& point : clusters[i]) {
            std::cout << "(" << point.first << ", " << point.second << ")\n";
        }
        std::cout << std::endl;
    }

    return 0;
}