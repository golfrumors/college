//a naive imperative implementation of binary search

//standard library to handle input output
#include <iostream>

using namespace std;

//function to find the index of given element
//runs in 0(log n) time, and has an auxiliary space complexity of 0(1)
int BinarySearch(int a[], int left, int right, int x){
    while(left<=right){
        int middle = left + (right - left)/2;

        if(a[middle] == x)
            return middle;
        else if(a[middle] < x)
            left = middle + 1;
        else
            right = middle - 1;
    }

    return -1;
}


//driver code
int main(){
    int sortedArr[] = {1,2,3,4,5,6,7,8,9,10};
    int n = sizeof(sortedArr)/sizeof(sortedArr[0]);
    int target = 3;

    int result = BinarySearch(sortedArr, 0, n, target);

    if(result == -1){
        cout << "Target not found\n";
    } else {
        cout << "Target found at index " << result;
    }

    return 0;
}