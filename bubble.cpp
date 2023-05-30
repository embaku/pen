#include <iostream>
#include <ctime>
#include <cstdlib>
#include <omp.h>
using namespace std;
// Sequential Bubble Sort
void sequentialBubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Parallel Bubble Sort
void parallelBubbleSort(int arr[], int n) {
    #pragma omp parallel
    {
        for (int i = 0; i < n - 1; ++i) {
            #pragma omp for
            for (int j = 0; j < n - i - 1; ++j) {
                if (arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                }
            }
        }
    }
}

// Function to generate random array
void generateRandomArray(int arr[], int n) {
    srand(time(0));
    for (int i = 0; i < n; ++i) {
        arr[i] = rand() % 100; // Generate numbers between 0 and 99
    }
}

// Function to print the array
void printArray(int arr[], int n) {
    for (int i = 0; i < n; ++i) {
        cout << arr[i] << " ";
    }
    cout << std::endl;
}

int main() {
    const int size = 500; // Size of the array
    int arr[size];

    // Generate random array
    generateRandomArray(arr, size);

    // Sequential Bubble Sort
    clock_t startTime = clock();
    sequentialBubbleSort(arr, size);
    clock_t endTime = clock();
    double sequentialTime = static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC;

    cout << "Sequential Bubble Sort:" << std::endl;
    cout << "Sorted array: ";
    printArray(arr, size);
    cout << "Execution time: " << sequentialTime << " seconds" << endl;

    // Generate random array again
    generateRandomArray(arr, size);

    // Parallel Bubble Sort
    startTime = clock();
    parallelBubbleSort(arr, size);
    endTime = clock();
    double parallelTime = static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC;

    cout << "\nParallel Bubble Sort:" << std::endl;
    cout << "Sorted array: ";
    printArray(arr, size);
    cout << "Execution time: " << parallelTime << " seconds" << endl;

    return 0;
}
