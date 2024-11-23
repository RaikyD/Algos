#include <iostream>
#include <cmath>
#include <random>
#include <vector>
#include <fstream>
#include <iomanip>

std::vector<double> Intersection(std::vector<double> X, std::vector<double> Y, std::vector<double> Z, int points) {
    double min_x = std::min({X[0] - X[2], Y[0] - Y[2], Z[0] - Z[2]});
    double max_x = std::max({X[0] + X[2], Y[0] + Y[2], Z[0] + Z[2]});
    double min_y = std::min({X[1] - X[2], Y[1] - Y[2], Z[1] - Z[2]});
    double max_y = std::max({X[1] + X[2], Y[1] + Y[2], Z[1] + Z[2]});
    double area_wide = (max_x - min_x) * (max_y - min_y);

    double tight_min_x = std::max({X[0] - X[2], Y[0] - Y[2], Z[0] - Z[2]});
    double tight_max_x = std::min({X[0] + X[2], Y[0] + Y[2], Z[0] + Z[2]});
    double tight_min_y = std::max({X[1] - X[2], Y[1] - Y[2], Z[1] - Z[2]});
    double tight_max_y = std::min({X[1] + X[2], Y[1] + Y[2], Z[1] + Z[2]});
    double area_tight = (tight_max_x - tight_min_x) * (tight_max_y - tight_min_y);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distr_x(min_x, max_x);
    std::uniform_real_distribution<> distr_y(min_y, max_y);
    std::uniform_real_distribution<> distr_x_t(tight_min_x, tight_max_x);
    std::uniform_real_distribution<> distr_y_t(tight_min_y, tight_max_y);

    int count_wide = 0;
    int count_tight = 0;

    for (int i = 0; i < points; ++i) {
        double rand_x = distr_x(gen);
        double rand_y = distr_y(gen);
        double rand_x_t = distr_x_t(gen);
        double rand_y_t = distr_y_t(gen);

        bool is_inside1 = (rand_x - X[0]) * (rand_x - X[0]) + (rand_y - X[1]) * (rand_y - X[1]) <= X[2] * X[2];
        bool is_inside2 = (rand_x - Y[0]) * (rand_x - Y[0]) + (rand_y - Y[1]) * (rand_y - Y[1]) <= Y[2] * Y[2];
        bool is_inside3 = (rand_x - Z[0]) * (rand_x - Z[0]) + (rand_y - Z[1]) * (rand_y - Z[1]) <= Z[2] * Z[2];

        bool is_inside1_t = (rand_x_t - X[0]) * (rand_x_t - X[0]) + (rand_y_t - X[1]) * (rand_y_t - X[1]) <= X[2] * X[2];
        bool is_inside2_t = (rand_x_t - Y[0]) * (rand_x_t - Y[0]) + (rand_y_t - Y[1]) * (rand_y_t - Y[1]) <= Y[2] * Y[2];
        bool is_inside3_t = (rand_x_t - Z[0]) * (rand_x_t - Z[0]) + (rand_y_t - Z[1]) * (rand_y_t - Z[1]) <= Z[2] * Z[2];

        if (is_inside1 && is_inside2 && is_inside3) {
            count_wide++;
        }

        if (is_inside1_t && is_inside2_t && is_inside3_t) {
            count_tight++;
        }
    }

    double intersection_area_wide = (static_cast<double>(count_wide) / points) * area_wide;
    double intersection_area_tight = (static_cast<double>(count_tight) / points) * area_tight;
    return {intersection_area_wide, intersection_area_tight};
}

int main() {
    std::vector<double> X {1.0, 1.0, 1.0};
    std::vector<double> Y {1.5, 2.0, (std::sqrt(5) / 2.0)};
    std::vector<double> Z {2.0, 1.5, (std::sqrt(5) / 2.0)};

    int minPoints = 100;
    int maxPoints = 100000;
    int step = 500;

    std::ofstream outFile("results.csv");
    if (!outFile) {
        std::cerr << "Не удалось открыть файл для записи.\n";
        return 1;
    }

    outFile << "Points,Area_wide,Area_tight\n";

    for (int points = minPoints; points <= maxPoints; points += step) {
        std::vector<double> area = Intersection(X, Y, Z, points);

        outFile << points << "," << area[0] << "," << area[1] << "\n";

        std::cout << "Points: " << points
                  << " | Wide: " << std::setprecision(8) << area[0]
                  << " | Tight: " << area[1] << "\n";
    }

    outFile.close();
    std::cout << "Результаты сохранены в файл results.csv" << std::endl;

    return 0;
}
