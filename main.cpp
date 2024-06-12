#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>
#include <chrono>
#include <functional>

std::vector<std::vector<int>> chunked(std::vector<int>& arr, int chunk_size) {
    std::vector<std::vector<int>> chunks;
    for (size_t i = 0; i < arr.size(); i += chunk_size) {
        chunks.push_back(std::vector<int>(arr.begin() + i, arr.begin() + std::min(i + chunk_size, arr.size())));
    }
    return chunks;
}

// O(n)
int pick_pivot(std::vector<int>& arr) {

    if (arr.size() < 5) {
        std::sort(arr.begin(), arr.end());
        return arr[arr.size() / 2];
    }

    std::vector<std::vector<int>> chunks = chunked(arr, 5);
    std::vector<std::vector<int>> full_chunks; // For simplicity, we can drop any chunks that aren't full

    for (const auto& chunk : chunks) {
        if (chunk.size() == 5) {
            full_chunks.push_back(chunk);
        }
    }
    for (auto& chunk : full_chunks) {
        std::sort(chunk.begin(), chunk.end());
    }

    std::vector<int> medians;
    for (const auto& chunk : full_chunks) {
        medians.push_back(chunk[2]);
    }
    int median_of_medians = pick_pivot(medians);
    return median_of_medians;
}

int quickselect(std::vector<int>& arr, int k) {
    if (arr.size() == 1) {
        return arr[0];
    }
    int pivot = pick_pivot(arr);

    std::vector<int> lows, highs, pivots;
    for (int el : arr) {
        if (el < pivot) {
            lows.push_back(el);
        } else if (el > pivot) {
            highs.push_back(el);
        } else {
            pivots.push_back(el);
        }
    }

    if (k < lows.size()) {
        return quickselect(lows, k);
    } else if (k < lows.size() + pivots.size()) {
        return pivots[0];
    } else {
        return quickselect(highs, k - lows.size() - pivots.size());
    }
}
// O(n)
double calculateMedian(std::vector<int>& numbers) {
    int n = numbers.size();
    if (n % 2 == 1)
        return quickselect(numbers, n / 2);
    else {
        long long left = quickselect(numbers, n / 2 - 1);
        long long right = quickselect(numbers, n / 2);
        return (left + right) / 2;
    }
}

std::pair<int, int> findMinimumMaximum(std::vector<int>& numbers) {
    int minNumber = numbers[0];
    int maxNumber = numbers[0];

    for (int number : numbers) {
        if (number < minNumber) {
            minNumber = number;
        }
        if (number > maxNumber) {
            maxNumber = number;
        }
    }
    return std::make_pair(minNumber, maxNumber);
}

double calculateMean(const std::vector<int>& numbers) {
    double sum = 0;
    for (int number : numbers) {
        sum += number;
    }
    return sum / numbers.size();
}

std::vector<int> longestIncSeq(const std::vector<int>& numbers) {
    if (numbers.empty())
        return {};

    std::vector<int> longestSeq;
    std::vector<int> currentSeq;
    currentSeq.push_back(numbers[0]);

    for (size_t i = 1; i < numbers.size(); ++i) {
        if (numbers[i] > numbers[i - 1]) {
            currentSeq.push_back(numbers[i]);
        } else {
            if (currentSeq.size() > longestSeq.size()) {
                longestSeq = currentSeq;
            }
            currentSeq.clear();
            currentSeq.push_back(numbers[i]);
        }
    }

    if (currentSeq.size() > longestSeq.size()) {
        longestSeq = currentSeq;
    }

    return longestSeq;
}

std::vector<int> longestDecSeq(const std::vector<int>& numbers) {
    if (numbers.empty())
        return {};

    std::vector<int> longestSeq;
    std::vector<int> currentSeq;
    currentSeq.push_back(numbers[0]);

    for (size_t i = 1; i < numbers.size(); i++) {
        if (numbers[i] < numbers[i - 1]) {
            currentSeq.push_back(numbers[i]);
        } else {
            if (currentSeq.size() > longestSeq.size()) {
                longestSeq = currentSeq;
            }
            currentSeq.clear();
            currentSeq.push_back(numbers[i]);
        }
    }

    if (currentSeq.size() > longestSeq.size()) {
        longestSeq = currentSeq;
    }

    return longestSeq;
}

int main() {
    const std::string filePath = "10m.txt";

    std::ifstream inputFile(filePath);

    if (!inputFile) {
        std::cerr << "Error: could not open the file!" << std::endl;
        return 1;
    }

    std::vector<int> numbers;
    int number;

    while (inputFile >> number) {
        numbers.push_back(number);
    }

    inputFile.close();

    if (numbers.empty()) {
        std::cerr << "The file is empty. Use another file." << std::endl;
        return 1;
    }

    auto start = std::chrono::high_resolution_clock::now();

    auto minMaxNumber = findMinimumMaximum(numbers);
    double median = calculateMedian(numbers);
    double mean = calculateMean(numbers);
    auto longestInc = longestIncSeq(numbers);
    auto longestDec = longestDecSeq(numbers);

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;

    std::cout << "Maximum: " << minMaxNumber.second << std::endl;
    std::cout << "Minimum: " << minMaxNumber.first << std::endl;
    std::cout << "Median: " << median << std::endl;
    std::cout << "Mean: " << mean << std::endl;
    std::cout << "Longest inc sequence: " << longestInc.size() << std::endl;
    for (auto num : longestInc) {
        std::cout << num << "\n";
    }
    std::cout << "Longest dec sequence: " << longestDec.size() << std::endl;
    for (auto num : longestDec) {
        std::cout << num << "\n";
    }

    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;

    return 0;
}
