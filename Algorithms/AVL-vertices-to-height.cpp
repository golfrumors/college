#include <iostream>
#include <math.h>
#include <stdio.h>

using namespace std;

int main(){
	int input;

	cout << "Enter the number of vertices: ";
	cin >> input;

	cout << "The minimum height of the tree is: " << (int)ceil(log2(input+1)) << endl;

	cout << "The maxmum height of the tree is: " << (int)floor(log2(input+1)) << endl;

	return 0;
}
