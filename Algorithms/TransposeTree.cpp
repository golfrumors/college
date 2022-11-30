#include <iostream>
#include <vector>

using namespace std;

void addEdge(vector<int> adj[], int u, int v)
{
    adj[u].push_back(v);
}

void printGraph(vector<int> adj[], int v){
	for (int i = 0; i < v; i++){
		cout << i << " -> ";
		for (int j = 0; j < adj[i].size(); j++){
			cout << adj[i][j] << " ";
		}
		cout << endl;
	}
}

void transposeGraph(vector<int> adj[], vector<int> transpose[], int v){
	for (int i = 0; i < v; i++){
		for (int j = 0; j < adj[i].size(); j++){
			addEdge(transpose, adj[i][j], i);
		}
	}
}

int main(){
	int v = 7;
	vector<int> adj[v];
	addEdge(adj, 0, 1);
	addEdge(adj, 0, 2);
	addEdge(adj, 1, 3);
	addEdge(adj, 1, 4);
	addEdge(adj, 4, 1);
	addEdge(adj, 2, 5);
	addEdge(adj, 6, 2);

	//transpose graph with adjacency list
	vector<int> transpose[v];
	transposeGraph(adj, transpose, v);

	//print original graph
	cout << "Original graph: \n";
	printGraph(adj, v);

	//print transpose graph
	cout << "\nTransposed graph: \n";
	printGraph(transpose, v);

	return 0;
}
