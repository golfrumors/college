/*
Ressources:
https://en.wikipedia.org/wiki/AVL_tree
https://gap-packages.github.io/orb/doc/chap8.html

Relevant info:
"Entries" in AVL tree objects are key-value pairs and the sorting is done by the key.
If all values as true then no memory is needed to store the values (see the corresponding behaviour for hash tables).
The only requirement on the type of the keys is that two arbitrary keys must be comparable, 
in the sense that one can decide which of them is smaller. 

*/

#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include <algorithm>
#include <memory>
#include <utility>

//create header for utility funcs
//#include "utility.hpp" ?

namespace avl {

	//create class template with pair Key, Value
	template <class Key, class Val = void>
	class AVL {
	public:
		AVL() {}

		//using std::move instead of copy/address incase they are large values
		AVL Add(Key k, Val v) const {
			return AVL(AddKey(root_, std::move(key), std::move(value)));
		}

		/*
		Define func template Remove.
		Pass in const reference to object type similar to key,
		Create new AVL obj by calling RemoveKey on root_, key as arg
		AVL obj is reconstructed by calling constructor again with result of RemoveKey
		*/
		template <typename SimilarToK>
		AVL Remove(const SimilarToK &key) const {
			return AVL(RemoveKey(root_, key));
		}

		/*
		Define func template Lookup.
		Pass in const reference to object type similar to key,
		return pointer to val of node if found.
		*/
		template <typename SimilarToK>
		const Val* Lookup(const SimilarToK &key) const {
			NodePtr n = Get(root_, key);
			return n ? &n->kv.second : nullptr;
		}

		const std::pair<Key, Val>* LookupSmaller(const Key& k) const {
			NodePtr n = GetBelow(root_, *key);
			return n ? &n->kv : nullptr;
		}

		bool Empty() const { return root_ == nullptr; }

		/*
		Template ForEach func,
		Pass in f which is a callable obj (func of lambda),
		Call using std::forward, so we pass f as universal ref,
		(lvalue or rvalue, depending on how we pass it)
		So we can call f with correct val and ref category, even if complex
		*/
		template <class F>
		void ForEach(F&& f) const {
			ForEachImplementation(root_.get(), std::forward<F>(f));
		}

		bool SameId(const AVL& avl) const { return root_ == avl.root_; }

		/*
		This one is kinda long, but
		Define a friend function which takes two AVL objs as args, l and r
		First, check if they are the same obj (if root_ ptr are the same), if true, return 0
		Then, create two Iterator objs, a and b, initialize with root_ ptr,
		of left and right, go to inf loop which will break when we reach end of tree,
		or when pair of nodes that are not equal is found.

		In the loop, get current nodes pointed to by a and b,
		using the current() of Iterator class, if the same,
		check if one or both are null.
		If either is null, return -1 or 1 to indicate,
		that the corresponding AVL obj is less or greater than other

		If neither are null, call QsortCompare on the kv pair of nodes and return result

		If result is the same, check if node is null, if true, loop is exited,
		we return 0, signaling that the two AVL obj are equal, else we call
		MoveNext() of Iterator class and go to next node in tree, and loop again.

		This is our comparison func for sorting AVLs.	
		*/

		//QsortCompare = quick sort compare
		friend int QsortCompare(const AVL& left, const AVL& right){
			if(left.root_.get() == right.root_.get()) return 0;
			Iterator a(left.root_);
			Iterator b(right.root_);

			for(;;) {
				Node* p = a.current();
				Node* q = b.current();

				if (p != q) {
					if (p == nullptr) return -1;
					if (q == nullptr) return 1;
					const int kv = QsortCompare(p->kv, q->kv);
					if (kv != 0) return kv;
				} else if (p == nullptr){
					return 0;
				}
				a.MoveNext();
				b.MoveNext();
			}
		}

		//overload operators such as == , < and >

	private:
		struct Node;

		/*
		Define type alias NodePtr, which is a shared_ptr to Node object,
		so we can have a smart pointer that holds a
		reference-counter pointer to an obj.
		When the last pointer pointing to obj is destroyed or reset,
		thee obj is automatically deleted
		Very good for automatic mem managment, since shared_ptr
		is a thread-safe class, and can be used in concurrent envs,
		without the need for explicit sync.
		*/
		typedef std::shared_ptr<Node> NodePtr;

		/*
		Define Node struct derived from enable_shared_from_this template
		Used to obtain shared_ptr to obj from within the obj itself
		Used to manage lifetime of obj when it needs to be accessed
		by multiple shared_ptr objs.
		*/
		struct Node : public std::enable_shared_from_this<Node>
		{
			Node(Key k, Val v, NodePtr l, NodePtr r, long h)
				: kv(std::move(k), std::move(v)), //a std::pair containing key and val of node
				left(std::move(l)), //ptr to left child of node
				right(std::move(r)), //ptr to right child of node
				height(h) {} //height of node in the tree
		};
		NodePtr root_;

		/*
		Define IteratorStack class, using simple stack data structure,
		to store pointers to Node objects.
		All four public methods are generic stack methods.

		Push & pop to add and remove nodes from stack as itr traverses
		the tree, and back and empty to access and query the top elem of stack,
		so we keep track of position without recursion
		*/
		class ItrStack {
		public:
			void Push(Node* n) {
				nodes_[depth_] = n;
				++depth_;
			}

			Node* Pop() {
				--depth_;
				return nodes_[depth_];
			}

			Node* Back() const { return nodes_[depth_ - 1]; }

			bool Empty() const { return depth_ == 0; }

		private:
			size_t depth_{0};
			//32 is maximum acceote depth, which is ~4billion nodes
			Node* nodes_[32];
		};


	}
}

#endif