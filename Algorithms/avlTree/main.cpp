#include <iostream>

#include "avl.h"
#include "bst.h"

bst::BST<int, void> createTree() {
    bst::BST<int, void> tree;

    tree = tree.Add(1);
    tree = tree.Add(2);
    tree = tree.Add(3);
    tree = tree.Add(4);
    tree = tree.Add(5);
    tree = tree.Add(6);
    tree = tree.Add(7);
    tree = tree.Add(8);
    tree = tree.Add(9);
    tree = tree.Add(10);
    tree = tree.Add(11);
    tree = tree.Add(12);
    tree = tree.Add(13);
    tree = tree.Add(14);
    tree = tree.Add(15);
    tree = tree.Add(16);
    tree = tree.Add(17);
    tree = tree.Add(18);
    tree = tree.Add(19);
    tree = tree.Add(20);
    tree = tree.Add(21);
    tree = tree.Add(22);
    tree = tree.Add(23);
    tree = tree.Add(24);
    tree = tree.Add(25);
    tree = tree.Add(26);
    tree = tree.Add(27);
    tree = tree.Add(28);
    tree = tree.Add(29);
    tree = tree.Add(30);
    tree = tree.Add(31);
    tree = tree.Add(32);
    tree = tree.Add(33);
    tree = tree.Add(34);
    tree = tree.Add(35);
    tree = tree.Add(36);
    tree = tree.Add(37);
    tree = tree.Add(38);
    tree = tree.Add(39);
    tree = tree.Add(40);
    tree = tree.Add(41);
    tree = tree.Add(42);
    tree = tree.Add(43);
    tree = tree.Add(44);
    tree = tree.Add(45);
    tree = tree.Add(46);
    tree = tree.Add(47);
    tree = tree.Add(48);
    tree = tree.Add(49);
    tree = tree.Add(50);
    tree = tree.Add(51);
    tree = tree.Add(52);
    tree = tree.Add(53);
    tree = tree.Add(54);
    tree = tree.Add(55);
    tree = tree.Add(56);
    tree = tree.Add(57);
    tree = tree.Add(58);
    tree = tree.Add(59);
    tree = tree.Add(60);
    
    return tree;
}

//using avl.h, construct an avl tree, and print it out in order, pre order, and post order
int main()
{
    avl::AVL<int> avl;
    avl::AVL<int> avlSame;
    bst::BST<int, void> bstTree = createTree();

    //printing out bst tree
    std::cout << "BST Tree: " << std::endl;
    std::cout << "In Order: " << std::endl;
    bstTree.Print();
    std::cout << std::endl;
    std::cout << "Pre Order: " << std::endl;
    bstTree.PrintPre();
    std::cout << std::endl;
    std::cout << "Post Order: " << std::endl;
    bstTree.PrintPost();

    //creating avl tree from bst tree using a loop and passing in the bst tree
    for (int i = 1; i <= 60; i++) {
        avl = avl.Add(bstTree.Get(i));
    }

    std::cout << std::endl;

    //printing out avl tree
    std::cout << std::endl;
    std::cout << "AVL Tree: " << std::endl;
    std::cout << "In Order: " << std::endl;
    avl.Print();
    std::cout << std::endl;
    std::cout << "Pre Order: " << std::endl;
    avl.PrintPre();
    std::cout << std::endl;
    std::cout << "Post Order: " << std::endl;
    avl.PrintPost();

    /*
    The big O of the conversion from BST to AVL is O(n) because we have to loop through the entire BST tree to add each node to the AVL tree.
    The insertion of the nodes into the AVL tree is O(log n) because we are using a binary search tree, as well as the fact that we are balancing the tree.
    We use std::move to move the nodes from the BST tree to the AVL tree, so we are not copying the nodes, which would be O(n) if we were copying the nodes.
    Also, std::move, is faster and more efficient than std::copy, so we are using the faster and more efficient method, especially if dealing with large numbers.
    The big O of the deletion of the nodes from the AVL tree is O(log n) because we are using a binary search tree, as well as the fact that we are balancing the tree, just like in the previous example,
    since we have to search through the tree to find the node we want to delete, and then we have to balance the tree after the deletion, but we make it more efficient depending on the height of the tree,
    and the value itself, by searching from the head or the tail of the tree.

    The space complexity of the insertion and deletion is O(n) because we are using a binary search tree, and we are storing the nodes in the tree, so the space complexity is O(n), where n is the number of nodes in the tree.
    The space complexity of the conversion from BST to AVL is O(n), since we are moving the nodes from the BST to the AVL.

    Converting a AVL to a BST is O(n) because we have to loop through the entire AVL tree to add each node to the BST tree, so they are equivalent in terms of time complexity.
    */

   /*We will prove that both AVL trees are equal by inserting the elements of the BST tree in reverse order
    Then we print out the created avl tree, and use our built in comparator to prove that both trees are equal
    Which will return 1 if true, and 0 if not.
   */
   for(int i = 60; i >= 1; i--) {
       avlSame = avlSame.Add(bstTree.Get(i));
    } 

    //printing out avl tree
    std::cout << std::endl;
    std::cout << "AVL Tree: " << std::endl;
    std::cout << "In Order: " << std::endl;
    avlSame.Print();
    std::cout << std::endl;
    std::cout << "Pre Order: " << std::endl;
    avlSame.PrintPre();
    std::cout << std::endl;
    std::cout << "Post Order: " << std::endl;
    avlSame.PrintPost();

    std::cout << std::endl;
    std::cout << "Proving both trees are equal, by using our comparison operators defined in avl.h: " << std::endl;
    std::cout << (avl == avlSame) << std::endl;
    std::cout << "Since we got an output of one, both of our trees are equal in this case" << std::endl;

    return 0;
}