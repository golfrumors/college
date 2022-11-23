//implementation fo algorithm to find the degree of a vertex in a directed graph
//using an adjacency matrix

#include <iostream>

using namespace std;

class Graph{
private:
    int **adjMatrix;
    int numVertices;

public:
    Graph(int n){
        this->numVertices = n;
        adjMatrix = new int*[numVertices];
        for(int i = 0; i < n; i++){
            adjMatrix[i] = new int[numVertices];
            for(int j = 0; j < n; j++){
                adjMatrix[i][j] = 0;
            }
        }
    }

    void addEdge(int i, int j){
        adjMatrix[i][j] = 1;
    }

    void removeEdge(int i, int j){
        adjMatrix[i][j] = 0;
    }

    void printGraph(){
        for(int i = 0; i < numVertices; i++){
            cout << i << " : ";
            for(int j = 0; j < numVertices; j++){
                cout << adjMatrix[i][j] << " ";
            }
            cout << endl;
        }

        int sum = 0;

        cout << endl;
        for(int i = 0; i < numVertices; i++){
            for(int j = 0; j < numVertices; j++){
                sum += adjMatrix[i][j];
            }
            cout << "Out " << i << " : " << sum;
            cout << endl;
            sum = 0;
        }

        for(int i = 0; i < numVertices; i++){
            for(int j = 0; j < numVertices; j++){
                sum += adjMatrix[j][i];
            }
            cout << "In " << i << " : " << sum;
            cout << endl;
            sum = 0;
        }

    }

    ~Graph(){
        for(int i = 0; i < numVertices; i++){
            delete[] adjMatrix[i];
        }
        delete[] adjMatrix;
    }

};

int main(){
    Graph g(6);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(2, 3);
    g.addEdge(0, 4);
    g.addEdge(4, 0);
    g.addEdge(5, 3);

    g.printGraph();

    return 0;
}
