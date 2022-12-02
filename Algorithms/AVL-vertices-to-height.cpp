#include <iostream>
#include <math.h>
#include <stdio.h>

using namespace std;

int main(){
	int input;

	cout << "Enter the number of vertices: ";
	cin >> input;

	cout << "The minimum height of the tree is: " << (int)ceil(log2(input+1)) << endl;

	cout << "The maximum height of the tree is: " << (int)floor(1.44*log2(input+2)-0.328) << endl;

	return 0;
}
