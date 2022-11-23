/*
    Basic program to receive a string as an input, and display it in a plus pattern as a matrix
    golfrumros 2022-11-16


    Example:
    Input: "Hello World"
    Output:
    XXXXXHXXXXX
    XXXXXeXXXXX
    XXXXXlXXXXX
    XXXXXlXXXXX
    XXXXXoXXXXX
    Hello World
    XXXXXWXXXXX
    XXXXXoXXXXX
    XXXXXrXXXXX
    XXXXXlXXXXX
    XXXXXdXXXXX
*/

#include <iostream>
#include <vector>
#include <sstream> //because strings are immutable

using namespace std; //acceptable sin for small programs

//convencience to print the matrix
void printMatrix(vector<vector<char>>& matrix){
    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix[i].size(); j++){
            cout << matrix[i][j];
        }
        cout << endl;
    }
}

//main function
int main(){
    //input for string to process,
    string input;
    //initializing the length of the string, and the middle
    int len, mid;
    //as well as the stringstream incase the string is even
    stringstream buff;

    //prompt for input
    cout << "Enter a string: ";
    cin >> input;

    //prepare the stringstream buffer with the input just in case
    buff << input;

    //get the length of the string and the middle
    len = input.length();

    //if the string is of even length
    if(len % 2 == 0){
        buff.put('X'); //add an X to the end of the input string so we have an uneven number
        len++; //increment the length
    }

    input = buff.str(); //update the input string
    buff.str(string());    
    buff.clear(); //clear the buffer

    mid = len/2; //get the middle

    //initalize the matrix of size n x n and fill it with X's
    vector<vector<char>> matrix(len, vector<char>(len, 'X'));

    //fill the matrix with the input strbg in the middle
    for(int i = 0; i < len; i++){
        matrix[i][mid] = input[i];
        matrix[mid][i] = input[i];
    }

    //print the matrix
    printMatrix(matrix);

    return EXIT_SUCCESS;
}
