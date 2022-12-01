#include <iostream>
#include <math.h>
#include <stdio.h>

using namespace std;

int findMinVerts(int h){
	int a = 1;
	int b = 2;
	int c;

	for(int i = 1; i < h; i++){
		c = a + b + 1;
		a = b;
		b = c;
	}
	return b;
}

int main(){
	int input;

	cout << "Enter the height of the AVL tree: ";
	cin >> input;

	cout << "The minimum number of vertices is: " << findMinVerts(input) << endl;

	return 0;
}
