//we know that the original tree is unbalanced because the heights of the subtrees are different by more than 1,
//as well as having values greater than the root on the left and less than the root on the right
//which is not allowed in a AVL tree, since it is effictively a binary search tree, that balances itself
//make an unbalanced tree with char values

#include <iostream>

using namespace std;

struct node
{
    char data;
    node *left;
    node *right;
};

node *newNode(char data)
{
    node *temp = new node;
    temp->data = data;
    temp->left = NULL;
    temp->right = NULL;
    return temp;
}

//print the tree in order
void printInorder(node *root)
{
    if (root == NULL)
	return;
    printInorder(root->left);
    cout << root->data << " ";
    printInorder(root->right);
}

//print the tree in preorder
void printPreorder(node *root)
{
    if (root == NULL)
	return;
    cout << root->data << " ";
    printPreorder(root->left);
    printPreorder(root->right);
}

//print the tree in postorder
void printPostorder(node *root)
{
    if (root == NULL)
	return;
    printPostorder(root->left);
    printPostorder(root->right);
    cout << root->data << " ";
}

//function to conver the unordered tree to an AVL tree
node *convertToAVL(node *root)
{
    //if the tree is empty
    if (root == NULL)
	return root;
    //if the tree has only one node
    if (root->left == NULL && root->right == NULL)
	return root;
    //if the tree has two nodes
    if (root->left != NULL && root->right != NULL)
    {
	//if the left node is greater than the right node
	if (root->left->data > root->right->data)
	{
	    //swap the data
	    char temp = root->left->data;
	    root->left->data = root->right->data;
	    root->right->data = temp;
	}
    }
    //if the tree has only one node on the left
    if (root->left != NULL && root->right == NULL)
    {
	//if the left node is greater than the root node
	if (root->left->data > root->data)
	{
	    //swap the data
	    char temp = root->left->data;
	    root->left->data = root->data;
	    root->data = temp;
	}
    }

    //if the tree has only one node on the right
    if (root->left == NULL && root->right != NULL)
    {
	//if the right node is less than the root node
	if (root->right->data < root->data)
	{
	    //swap the data
	    char temp = root->right->data;
	    root->right->data = root->data;
	    root->data = temp;
	}
    }
    //recursively call the function on the left and right subtrees
    convertToAVL(root->left);
    convertToAVL(root->right);
    return root;
}

int main(){
	//note this code doesn't balance the tree automatically, just for the sake of clarity and demonstration
	//it simply converts the tree to an AVL tree
	//however, if one wishes to make a self-balancing AVL tree, all that is needed to do is to add a function
	//that checks the height of the left and right subtrees and balances the tree accordingly
	//which is trivial to do, and if you need me to do it, shoot me an email :) or look at my final project

	//create the unbalanced tree
	node *root = newNode('A');
	root->left = newNode('B');
	root->left->left = newNode('C');

	root->right = newNode('I');
	root->right->left = newNode('D');
	root->right->right = newNode('G');

	root->right->left->left = newNode('H');
	root->right->left->left->right = newNode('F');

	root->right->left->right = newNode('J');
	root->right->left->right->right = newNode('E');

	//print out unbalanced tree
	cout << "The inorder traversal of the unbalanced tree is: ";
	printInorder(root);
	cout << "\nThe preorder traversal of the unbalanced tree is: ";
	printPreorder(root);
	cout << "\nThe postorder traversal of the unbalanced tree is: ";
	printPostorder(root);

	//convert the unbalanced tree to an AVL tree and print it out
	cout << "\n\nThe inorder traveersal of the AVL tree is: ";
	root = convertToAVL(root);
	printInorder(root);
	cout << "\nThe preorder traversal of the AVL tree is: ";
	printPreorder(root);
	cout << "\nThe postorder traversal of the AVL tree is: ";
	printPostorder(root);

	return 0;
}
