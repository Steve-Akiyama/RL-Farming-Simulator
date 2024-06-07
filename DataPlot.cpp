#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

class DataPlot {
private:
    struct DataPoint {
        double x;
        double y;
    };

    std::vector<DataPoint> data;
    double percentage;

    // Function to calculate average
    double calculateAverage(const std::vector<double>& values) {
        double sum = 0.0;
        for (double val : values) {
            sum += val;
        }
        return sum / values.size();
    }

    // Function to calculate standard deviation
    double calculateStdDev(const std::vector<double>& values, double average) {
        double variance = 0.0;
        for (double val : values) {
            variance += pow(val - average, 2);
        }
        return sqrt(variance / values.size());
    }

    // Simple linear regression curve fitting algorithm
    void linearRegression(double& slope, double& intercept) {
        double sumX = 0.0, sumY = 0.0, sumXY = 0.0, sumX2 = 0.0;
        int n = data.size();

        for (const auto& point : data) {
            sumX += point.x;
            sumY += point.y;
            sumXY += point.x * point.y;
            sumX2 += point.x * point.x;
        }

        slope = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
        intercept = (sumY - slope * sumX) / n;
    }

public:
    // Constructor
    DataPlot(const std::string& filename, double percentage) : percentage(percentage) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }

        double x, y;
        while (file >> x >> y) {
            data.push_back({x, y});
        }
        file.close();
    }

    // Function to process data and print results
    void process() {
        // Calculate number of points for curve fitting
        int numPointsToFit = static_cast<int>(data.size() * percentage);

        // Perform curve fitting
        double slope, intercept;
        linearRegression(slope, intercept);

        // Print curve fitting results
        //std::cout << "Curve fitting results for first " << (percentage * 100) << "% of data points:" << std::endl;
        //for (int i = 0; i < numPointsToFit; ++i) {
        //    std::cout << "Data point " << i + 1 << ": (" << data[i].x << ", " << data[i].y << ")" << std::endl;
        //}

        // Calculate average and standard deviation for the remaining data points
        std::vector<double> remainingValues;
        for (int i = numPointsToFit; i < data.size(); ++i) {
            remainingValues.push_back(data[i].y);
        }

        double average = calculateAverage(remainingValues);
        double stdDev = calculateStdDev(remainingValues, average);

        // Print average and standard deviation for the remaining data points
        std::cout << "Average for remaining data points: " << average << std::endl;
        std::cout << "Standard deviation for remaining data points: " << stdDev << std::endl;

        // Print linear regression
        std::cout << "Linear regression equation: y = " << slope << "x + " << intercept << std::endl;
    }
};