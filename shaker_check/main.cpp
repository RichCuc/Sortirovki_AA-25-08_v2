#include "../ShakerSort_Anisimov.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>

int main() {
    const char* files[] = {
        "shaker_best.txt", "shaker_average.txt", "shaker_worst.txt"
    };
    int loaded = 0;
    int runs = 0;
    int errors = 0;

    std::cout << "Шейкерная сортировка: три сценария из презентации.\n"
              << std::left << std::setw(24) << "File"
              << std::right << std::setw(9) << "Count"
              << std::setw(15) << "Shaker, ms" << '\n'
              << std::string(48, '-') << '\n'
              << std::fixed << std::setprecision(6);

    for (const char* name : files) {
        std::ifstream file(name);
        std::vector<double> arr;
        double value;
        while (file >> value) arr.push_back(value);
        if (!file.eof() || arr.empty()
            || !std::all_of(arr.begin(), arr.end(), [](double x) {
                return std::isfinite(x);
            })) return 1;
        ++loaded;
        std::vector<double> expected = arr;
        std::sort(expected.begin(), expected.end());

        const auto start = std::chrono::steady_clock::now();
        arr = shakerSort(std::move(arr));
        const auto finish = std::chrono::steady_clock::now();
        ++runs;
        const double ms = std::chrono::duration<double, std::milli>(finish - start).count();
        std::cout << std::left << std::setw(24) << name
                  << std::right << std::setw(9) << arr.size();
        if (arr == expected) std::cout << std::setw(15) << ms;
        else { std::cout << std::setw(15) << "ERROR"; ++errors; }
        std::cout << '\n';
    }
    std::cout << std::string(48, '-') << '\n'
              << "Прочитано файлов: " << loaded << "/3\n"
              << "Выполнено сортировок: " << runs << '\n'
              << "Ошибок результатов: " << errors << '\n';
    return errors == 0 ? 0 : 1;
}
