#ifndef AVL
#define AVL

#include <memory>
#include <algorithm>
#include <utility>

#include "utility.hpp"

namespace avl{

	template <class K, class V = void>
	class AVL {
		public: 
			AVL() {}

			AVL Add(K key, V value) const {
				return AVL(AddKey(root_, std::move(key), std::move(value)));
			}

			/*
				Take in a key as a constant reference and return
				a pointer to associated value if it exists in the tree
				otherwise, return nullptr
			*/
			template <typename LikeK>
			const V* Find(const LikeK& key) const {
				NodePtr n = Get(root_, key);
				return n ? &n->kv.second : nullptr;
			}

			/*
				Take in a key as a constant reference and return
				a new AVL tree with the key removed from it
			*/
			template <typename LikeK>
			AVL Remove(const LikeK& key) const {
				return AVL(RemoveKey(root_, key));
			}

			/*
				Take in a key as a constant reference and return
				a pointer to the key-value pair in the tree with
				the largest key that is >= to given key
				If such a key is found, return pointer to key-value pair,
				otherwise, return nullptr
			*/
			const std::pair<K,V> *FindSmaller(const K &key) const {
				NodePtr n = GetSmaller(root_, *key);
				return n ? &n->kv : nullptr;
			}

			//Assert if tree is empty, check against nullptr and return bool
			bool Empty() const {
				return root_ == nullptr;
			}

			/*
				Helper function to traverse the tree in order, and apply
				function to each key-value pair.
				Traverse the tree in depth-first, visiting left, then current and then right
			*/
			template <class F>
			void ForEach(F &&f) const {
				ForEachImplementation(root_.get(), std::forward<F>(f));
			}

			//check if current & trivial tree have same root
			bool SameRoot(const AVL &avl) const {
				return root_ == avl.root_;
			}

			/*

			@TODO : IMPLEMENT QSORT COMPARE
			& OPERATOR OVERLOADING
			
			*/
		private:
			struct Node;

			typedef std::shared_ptr<Node> NodePt;
			struct Node : public std::enable_shared_from_this<Node> {
				Node(K k, V v, NodePtr l, NodePtr r, long h)
					: kv(std::move(k), std::move(v)),
					left(std::move(l)),
					right(std::move(r)),
					height(h) {}
				const std::pair<K,V> kv;
				const NodePtr left;
				const NodePtr right;
				const long height;
			};
			NodePtr root_;

			class ItrStack {
				public:
					void Push(Node* n){
						nodes_[depth_] = n;
						++depth_;
					}

					Node* Pop(){
						--depth_;
						return nodes_[depth_];
					}

					Node* Back() const {
						return nodes_[depth_ - 1]; 
					}

					bool Empty() const {
						return depth_ == 0;
					}

				private:
					size_t depth_{0};
					Node *nodes_[32];
					/*
						max depth of 32 which is ~4 billion nodes :)
					*/

			};

			class Itr {
				public:
					explicit Iterator(const NodePtr &root){
						auto *n = root.get();
						while (n != nullptr) {
							stack_.Push(n);
							n = n->left.get();
						}
					}

					Node *current() const {
						return stack_.Empty() ? nullptr : stack_.Back();
					}

					void MoveNext() {
						auto *n = stack_.Pop();
						if(n->right != nullptr) {
							n = n->right.get();
							while(n != nullptr){
								stack_.Push(n);
								n = n->left.get();
							}
						}
					}

				private:
					ItrStack stack_;
			};

			explicit AVL(NodePtr root) : root_(std::move(root)) {}

			template <class F>
			static void ForEachImplementation(const Node *n, F &&f) {
				if(n == nullptr) return;

				ForEachImplementation(n->left.get(), std::forward<F>(f));
				f(const_cast<const K&>(n->kv.first), const_cast<const V&>(n->kv.second));
				ForEachImplementation(n->right.get(), std::forward<F>(f));
			}

			static long Height(const NodePtr &n) {
				return n ? n->height : 0;
			}

			static NodePtr MakeNode(K key, V value, const NodePtr &left, const NodePtr &right) {
				return std::make_shared<Node>(std::move(key), std::move(value), left, right,
					1 + std::max(Height(left), Height(right)));
			}

			template <typename LikeK>
			static NodePtr Get(const NodePtr &node, const LikeK &key) {
				if (node == nullptr)
				{
					return nullptr;
				}

				if (node -> kv.first > key) {
					return Get(node->left, key);
				} else if (node->kv.first < key) {
					return Get(node->right, key);
				} else {
					return node;
				}
			}

			static NodePtr GetSmaller(const NodePtr &node, const K &key) {
				if (!node) return nullptr;
				if(node->kv.first > key) {
					return GetSmaller(node->left, key);
				} else if (node->kv.first < key) {
					NodePtr n = GetSmaller(node->right, key);
					if (n == nullptr) n = node;
					return n;
				} else {
					return node;
				}
			}

			static NodePtr RotateL(K key, V value, const NodePtr &left, const NodePtr &right) {
				return MakeNode(
					right->kv.first, right->kv.second,
					MakeNode(std::move(key), std::move(value), left, right->left),
					right->right);
			}

			static NodePtr RotateR(K key, V value, const NodePtr &left, const NodePtr &right) {
				return MakeNode(
					left->kv.first, left->kv.second, left->left,
					MakeNode(std::move(key), std::move(value), left->right, right));
			}

			static NodePtr RotateLR(K key, V value, const NodePtr &left, const NodePtr &right) {
				//rotate R (..., rotate L(left), right)
				return MakeNode(
					left->right->kv.first, left->right->kv.second,
					MakeNode(left->kv.first, left->kv.second, left->left,
						left->right->left),
					MakeNode(std::move(key), std::move(value), left->right->right, right));
			}

			static NodePtr RotateRL(K key, V value, const NodePtr &left, const NodePtr &right) {
				return MakeNode(
					right->left->kv.first, right->left->kv.second,
					MakeNode(std::move(key), std::move(value), left, right->left->left),
					MakeNode(right->kv.first, right->kv.second, right->left->right,
					right->right));
			}

			static NodePtr Rebalance(K key, V value, const NodePtr &left, const NodePtr &right) {
				switch(Height(left) - Height(right)) {
					case 2:
						if (Height(left->left) - Height(left->right) == -1) {
							return RotateLR(std::move(key), std::move(value), left, right);
						} else {
							return RotateR(std::move(key), std::move(value), left, right);
						}

					case -2:
						if (Height(right->left) - Height(right->right) == 1) {
							return RotateRL(std::move(key), std::move(value), left, right);
						} else {
							return RotateL(std::move(key), std::move(value), left, right);
						}

					default:
						return MakeNode(key, value, left, right);
				}
			}

			static NodePtr AddKey(const NodePtr &node, K key, V value) {
				if (!node) {
					return MakeNode(std::move(key), std::move(value), nullptr, nullptr);
				}

				if (node->kv.first < key) {
					return Rebalance(node->kv.first, node->kv.second, node->left,
						AddKey(node->right, std::move(key), std::move(value)));
				}

				if(key < node->kv.first) {
					return Rebalance(node->kv.first,, node->kv.second,
						AddKey(node->left, std::move(key), std::move(value)),
						node->right);
				}

				return MakeNode(std::move(key), std::move(value), node->left, node->right);
			}

			//in order head
			static NodePtr InOrderH(NodePtr node) {
				while (node->left != nullptr) {
					node = node->left;
				}
				return node;
			}


	}

}

#endif 