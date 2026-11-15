#include "ShakerSort_Anisimov.h"

#include <utility>

std::vector<int> shakerSort(std::vector<int> arr) {
    int left = 0;
    int right = static_cast<int>(arr.size()) - 1;

    while (left < right) {
        for (int i = left; i < right; ++i) {
            if (arr[i] > arr[i + 1]) {
                std::swap(arr[i], arr[i + 1]);
            }
        }
        --right;

        for (int i = right; i > left; --i) {
            if (arr[i - 1] > arr[i]) {
                std::swap(arr[i - 1], arr[i]);
            }
        }
        ++left;
    }
    return arr;
}

std::vector<double> shakerSort(std::vector<double> arr) {
    int left = 0;
    int right = static_cast<int>(arr.size()) - 1;

    while (left < right) {
        for (int i = left; i < right; ++i) {
            if (arr[i] > arr[i + 1]) {
                std::swap(arr[i], arr[i + 1]);
            }
        }
        --right;

        for (int i = right; i > left; --i) {
            if (arr[i - 1] > arr[i]) {
                std::swap(arr[i - 1], arr[i]);
            }
        }
        ++left;
    }
    return arr;
}
