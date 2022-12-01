#include <iostream>
#include <math.h>
#include <stdio.h>

using namespace std;

int main(){
	int input;

	cout << "Enter height of AVL tree: ";
	cin >> input;

	int maxVert = pow(2, (input+1)) - 1;

	cout << "Maximum number of vertices: " << maxVert << endl;

	return 0;
}
