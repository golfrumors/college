//explaining and implementing a binary search tree

/*explanation of binary search tree
A binary search tree is a tree in which each node has an associated value, and each node has at most two children
The left subtree of each node contains only nodes with values less than the node's own value
The right subtree of a node contains only nodes with values greater than the node's own value
The left and right subtree each must also be a binary search tree
There must be no duplicate nodes for the tree to be a valid BST */

//general include statements
#include <iostream>

using namespace std;

//class definition
class BST
{
    //private data members
    private:
	int data;
	BST *left, *right;

    //public data members
    public:
	//default constructor
	BST();

	//parameterized constructor
	BST(int);

	//destructor
	~BST();

	//insert function
	BST* Insert(BST*, int);

	//inorder traversal function
	void Inorder(BST*);
};

//default constructor
//sets data to 0 and left and right to NULL, so this can be our root node
BST::BST() : data(0), left(NULL), right(NULL)
{
}

//parameterized constructor
//sets data to the value passed in and left and right to NULL
BST::BST(int value)
{
	data = value;
	left = right = NULL;
}

//destructor
//deletes the left and right nodes
//this is a recursive function
//this is called when the program ends, or the BST is out of scope and will not be used again
BST::~BST()
{
	delete left;
	delete right;
}

//insert function
//this function inserts a new node into the BST, which is called when we want to create a new node
//this function returns a pointer to the root node for which we are creating a child
//this function takes in a pointer to the root node and the value to be inserted
BST* BST::Insert(BST* root, int value)
{
	//if the root is NULL, then we are at the root node
	if (!root)
	{
		//return a new BST with the value passed in
		return new BST(value);
	}

	//if the value passed in is less than the root's data
	if (value < root->data)
	{
		//insert the value into the left subtree
		root->left = Insert(root->left, value);
	}
	//if the value passed in is greater than the root's data
	else
	{
		//insert the value into the right subtree
		root->right = Insert(root->right, value);
	}

	//return the root node
	return root;
}

//inorder traversal function
//this function prints the BST in order, by taking a pointer to the root node
//this function is recursive
void BST::Inorder(BST* root)
{
	//if the root is NULL, then we are at the end of the tree
	if (!root)
	{
		//return
		return;
	}

	//print the left subtree
	Inorder(root->left);

	//print the root's data
	cout << root->data << endl;

	//print the right subtree
	Inorder(root->right);
}

//driver code
int main()
{
	//create a new BST
	BST tree, *root = NULL;

	//insert the values into the BST
	root = tree.Insert(root, 50);
	tree.Insert(root, 30);
	tree.Insert(root, 20);
	tree.Insert(root, 40);
	tree.Insert(root, 70);
	tree.Insert(root, 60);
	tree.Insert(root, 80);

	//print the BST
	tree.Inorder(root);

	return 0;
}

