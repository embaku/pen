#include <iostream>
#include <omp.h>
#include <climits>

using namespace std;

int min_reduction(int arr[], int size){
	int min_val = INT_MAX;
	#pragma omp parallel for reduction(min:min_val)
	for(int i=0; i<size; i++){
		if(arr[i]<min_val){
			min_val=arr[i];
		}
	}
	return min_val;
}

int max_reduction(int arr[], int size){
	int max_val = INT_MIN;
	#pragma omp parallel for reduction(max:max_val)
	for(int i=0; i<size; i++){
		if(arr[i]>max_val){
			max_val=arr[i];
		}
	}
	return max_val;
}

int sum_reduction(int arr[], int size){
	int sum_val = 0;
	#pragma omp parallel for reduction(+:sum_val)
	for(int i=0; i<size; i++){
		sum_val+=arr[i];
	}
	return sum_val;
}

double avg_reduction(int arr[], int size){
	int sum_val = 0;
	#pragma omp parallel for reduction(+:sum_val)
	for(int i=0; i<size; i++){
		sum_val+=arr[i];
	}
	
	double avg=static_cast<double>(sum_val)/size;
	return avg;
}

int main(){
	int size;
	cout << "Enter size of array: ";
	cin >> size;
	
	int arr[size];
	cout << "Enter array elements: " << endl;
	for(int i=0; i<size; i++){
		cin >> arr[i];
	}
	
	int min = min_reduction(arr, size);
	int max = max_reduction(arr, size);
	int sum = sum_reduction(arr, size);
	double avg = avg_reduction(arr, size);

	cout << "Minimum value: " << min << endl;
	cout << "Maximum value: " << max << endl;
	cout << "Sum of elements: " << sum << endl;
	cout << "Average of elements: " << avg << endl; 

	return 0;
}
