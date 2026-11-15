#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "GnomeSort_Zykov.h"
#include "Radix.h"
#include "CombSort_Sannikov.h"
#include "heap_sort_Romanovskiy.hpp"
#include "ShakerSort_Anisimov.h"


int main() {
    using Array = std::vector<double>;

    struct SortMethod {
        const char* name;
        void (*run)(Array&);
    };

    const SortMethod methods[] = {
        {"std::sort", [](Array& arr) {
            std::sort(arr.begin(), arr.end());
        }},
        {"Radix", [](Array& arr) {
            arr = radixSort(std::move(arr));
        }},
        { "Comb", [](Array& arr) {
            arr = CombSort(std::move(arr));
        }},
        {"Heap", [](Array& arr) {
            heap_sort(arr.data(), static_cast<int>(arr.size()));
        }},
        {"Gnome", [](Array& arr) {
            arr = gnomeSort(std::move(arr));
        }},
        {"Shaker", [](Array& arr) {
            arr = shakerSort(std::move(arr));
        }},
        // Здесь добавляем вызовы сортировок.
    };

    std::ifstream list("files.txt");
    std::vector<std::string> files;
    std::string filename;
    int fileWidth = 24;
    while (list >> filename) {
        files.push_back(filename);
        fileWidth = std::max(fileWidth,
                             static_cast<int>(filename.size()) + 2);
    }
    if (files.empty()) {
        std::cerr << "Не удалось прочитать список files.txt.\n";
        return 1;
    }

    constexpr int countWidth = 9;
    constexpr int timeWidth = 13;
    const auto methodCount = sizeof(methods) / sizeof(methods[0]);
    const std::string line(
        fileWidth + countWidth + methodCount * timeWidth, '-'
    );

    std::cout << "Время сортировок в миллисекундах.\n"
              << std::left << std::setw(fileWidth) << "File"
              << std::right << std::setw(countWidth) << "Count";
    for (const auto& method : methods) {
        std::cout << std::setw(timeWidth) << method.name;
    }
    std::cout << '\n' << line << '\n'
              << std::fixed << std::setprecision(6);

    std::size_t loaded = 0;
    std::size_t runs = 0;
    std::size_t errors = 0;

    for (const auto& name : files) {
        std::ifstream file(name);
        Array original;
        double value;
        bool valid = file.is_open();

        while (valid && (file >> std::ws) && !file.eof()) {
            if (!(file >> value) || !std::isfinite(value)) {
                valid = false;
            } else {
                original.push_back(value);
            }
        }

        std::cout << std::left << std::setw(fileWidth) << name
                  << std::right;
        if (!valid || original.empty()) {
            std::cout << std::setw(countWidth) << "-";
            for (std::size_t i = 0; i < methodCount; ++i) {
                std::cout << std::setw(timeWidth) << "FILE ERROR";
            }
            std::cout << '\n';
            continue;
        }
        ++loaded;
        std::cout << std::setw(countWidth) << original.size();

        Array expected = original;
        std::sort(expected.begin(), expected.end());

        for (const auto& method : methods) {
            Array arr = original;
            const auto start = std::chrono::steady_clock::now();
            method.run(arr);
            const auto finish = std::chrono::steady_clock::now();
            ++runs;

            if (arr != expected) {
                std::cout << std::setw(timeWidth) << "ERROR";
                ++errors;
            } else {
                const double ms =
                    std::chrono::duration<double, std::milli>(
                        finish - start
                    ).count();
                std::cout << std::setw(timeWidth) << ms;
            }
        }
        std::cout << '\n';
    }

    std::cout << line << '\n'
              << "Прочитано файлов: " << loaded
              << '/' << files.size() << '\n'
              << "Выполнено сортировок: " << runs << '\n'
              << "Ошибок результатов: " << errors << '\n';

    return (loaded == files.size() && errors == 0) ? 0 : 1;
}

