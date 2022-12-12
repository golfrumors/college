//create a tree, and print the tree in preorder, inorder, and postorder
//the value of the node is a char
//the tree is unordered

#include <iostream>

using namespace std;

//node struct, with a char value and two pointers to a node on either side
struct node
{
    char data;
    node *left;
    node *right;
};

//function to create a new node
node *newNode(char data)
{
    node *temp = new node;
    temp->data = data;
    temp->left = NULL;
    temp->right = NULL;
    return temp;
}

//function to print the tree in preorder
void preorder(node *root)
{
    if (root != NULL)
    {
	cout << root->data << " ";
	preorder(root->left);
	preorder(root->right);
    }
}

//function to print the tree in inorder
void inorder(node *root)
{
    if (root != NULL)
    {
	inorder(root->left);
	cout << root->data << " ";
	inorder(root->right);
    }
}

//function to print the tree in postorder
void postorder(node *root)
{
    if (root != NULL)
    {
	postorder(root->left);
	postorder(root->right);
	cout << root->data << " ";
    }
}

//create the tree in the main function, insert some nodes, and print
int main()
{
    node *root = newNode('A');
    root->left = newNode('B');
    root->left->left = newNode('C');
    
    root->right = newNode('I');
    root->right->right = newNode('G');

    root->left->right = newNode('D');

    root->left->right->left = newNode('H');
    root->left->right->left->right = newNode('F');

    root->left->right->right = newNode('J');
    root->left->right->right->right = newNode('E');
    
    cout << "Preorder: ";
    preorder(root);
    cout << endl;
    cout << "Inorder: ";
    inorder(root);
    cout << endl;
    cout << "Postorder: ";
    postorder(root);
    cout << endl;
    return 0;
}
