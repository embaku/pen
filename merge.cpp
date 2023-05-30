#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <omp.h>
using namespace std;

// Merge two sorted subarrays into one
void merge(std::vector<int>& arr, int left, int middle, int right) {
    int i, j, k;
    int n1 = middle - left + 1;
    int n2 = right - middle;

    std::vector<int> L(n1), R(n2);

    // Copy data to temporary arrays
    for (i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (j = 0; j < n2; ++j)
        R[j] = arr[middle + 1 + j];

    // Merge the temporary arrays back into arr
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = left; // Initial index of merged subarray

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            ++i;
        }
        else {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    // Copy the remaining elements of L, if there are any
    while (i < n1) {
        arr[k] = L[i];
        ++i;
        ++k;
    }

    // Copy the remaining elements of R, if there are any
    while (j < n2) {
        arr[k] = R[j];
        ++j;
        ++k;
    }
}

// Sequential Merge Sort
void sequentialMergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;

        sequentialMergeSort(arr, left, middle);
        sequentialMergeSort(arr, middle + 1, right);

        merge(arr, left, middle, right);
    }
}

// Parallel Merge Sort
void parallelMergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                parallelMergeSort(arr, left, middle);
            }
            #pragma omp section
            {
                parallelMergeSort(arr, middle + 1, right);
            }
        }

        merge(arr, left, middle, right);
    }
}

// Function to generate a random array
void generateRandomArray(std::vector<int>& arr, int n) {
    srand(time(0));
    for (int i = 0; i < n; ++i) {
        arr.push_back(rand() % 100); // Generate numbers between 0 and 99
    }
}

// Function to print the array
void printArray(const std::vector<int>& arr) {
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

int main() {
    const int size = 500; // Size of the array
    std::vector<int> arr;

    // Generate random array
    generateRandomArray(arr, size);

    // Sequential Merge Sort
    clock_t startTime = clock();
    sequentialMergeSort(arr, 0, size - 1);
    clock_t endTime = clock();
    double sequentialTime = static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC;

    std::cout << "Sequential Merge Sort:" << std::endl;
    std::cout << "Sorted array: ";
    printArray(arr);
    std::cout << "Execution time: " << sequentialTime << " seconds" << std::endl;

    // Generate random array again
    arr.clear();
    generateRandomArray(arr, size);

    // Parallel Merge Sort
    startTime = clock();
    parallelMergeSort(arr, 0, size - 1);
    endTime = clock();
    double parallelTime = static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC;

    std::cout << "\nParallel Merge Sort:" << std::endl;
    std::cout << "Sorted array: ";
    printArray(arr);
    std::cout << "Execution time: " << parallelTime << " seconds" << std::endl;

    return 0;
}
