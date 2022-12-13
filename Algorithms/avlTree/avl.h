#ifndef AVL_H
#define AVL_H

#include <memory>
#include <algorithm>
#include <utility>
#include <stack>

namespace avl {

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

			//qsort compare function
			friend int QsortCompare(const AVL& left, const AVL& right) {
				if (left.root_.get() == right.root_.get()) return 0;
				Itr a(left.root_);
				Itr b(right.root_);

				for(;;) {
					Node* p = a.current();
					Node* q = b.current();

					if(p != q) {
						if (p == nullptr) return -1;
						if (q == nullptr) return 1;

						const int kv = QsortCompare(p->kv, q->kv);

						if (kv != 0) return kv;
					} else if (p == nullptr) {
						return 0;
					}
					a.MoveNext();
					b.MoveNext();
				}
			}

			//comparison of two tree
			bool operator==(const AVL& other) const {
				return QsortCompare(*this, other) == 0;
			}

			//print tree in order
			void Print() const {
				PrintInOrder(root_);
			}

			//print tree in pre order
			void PrintPre() const {
				PrintPreOrder(root_);
			}

			//print tree in post order
			void PrintPost() const {
				PrintPostOrder(root_);
			}

		private:
			struct Node;

			typedef std::shared_ptr<Node> NodePtr;
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
					explicit Itr(const NodePtr &root) {
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

			//print tree in order
			static void PrintInOrder(const NodePtr &n) {
				if(n == nullptr) return;

				PrintInOrder(n->left);
				std::cout << n->kv.first << " ";
				PrintInOrder(n->right);
			}

			//print tree in pre order
			static void PrintPreOrder(const NodePtr &n) {
				if(n == nullptr) return;

				std::cout << n->kv.first << " ";
				PrintPreOrder(n->left);
				PrintPreOrder(n->right);
			}

			//print tree in post order
			static void PrintPostOrder(const NodePtr &n) {
				if(n == nullptr) return;

				PrintPostOrder(n->left);
				PrintPostOrder(n->right);
				std::cout << n->kv.first << " ";
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
					right->left->kv.first, 
					right->left->kv.second,
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
					return Rebalance(node->kv.first, node->kv.second,
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

			static NodePtr InOrderT(NodePtr node) {
				while (node->right != nullptr) {
					node = node->right;
				}
				return node;
			}

			template <typename LikeK>
			static NodePtr RemoveKey(const NodePtr& node, const LikeK& key) {
				if (!node) return nullptr;

				if (node->kv.first < key) {
					return Rebalance(node->kv.first, node->kv.second, node->left,
						RemoveKey(node->right, key));
				}

				if (key < node->kv.first) {
					return Rebalance(node->kv.first, node->kv.second,
						RemoveKey(node->left, key), node->right);
				}

				if (!node->left) return node->right;
				if (!node->right) return node->left;

				if (node->left->height < node->right->height) {
					NodePtr h = InOrderH(node->right);
					return Rebalance(h->kv.first, h->kv.second,
						node->left, RemoveKey(node->right, h->kv.first));
				} else {
					NodePtr t = InOrderT(node->left);
					return Rebalance(t->kv.first, t->kv.second,
						RemoveKey(node->left, t->kv.first), node->right);
				}
			}
			abort();
};

template <class K>
class AVL<K, void> {
	public:
		AVL() {}

		AVL Add(K key) const { return AVL(AddKey(root_, std::move(key))); }
		AVL Remove(const K& key) const { return AVL(RemoveKey(root_, key)); }
		bool Lookup(const K& key) const { return Get(root_, key) != nullptr; }
		bool Empty() const { return root_ == nullptr; }

		template <class F>
		void ForEach(F&& f) const {
			ForEachImplementation(root_.get(), std::forward<F>(f));
		}

		bool SameRoot(const AVL &avl) const {
					return root_ == avl.root_;
		}

		//print tree in order
		void Print() const {
			PrintInOrder(root_);
		}

		//print tree in pre order
		void PrintPre() const {
			PrintPreOrder(root_);
		}

		//print tree in post order
		void PrintPost() const {
			PrintPostOrder(root_);
		}

		//friend function to loop & compare two trees
		friend bool operator==(const AVL& tree1, const AVL& tree2) {
			return Compare(tree1, tree2);
		}


	private:
		struct Node;

		typedef std::shared_ptr<Node> NodePtr;
		struct Node : public std::enable_shared_from_this<Node> {
			Node(K key, NodePtr l, NodePtr r, long h)
				: key(std::move(key)),
				left(std::move(l)),
				right(std::move(r)),
				height(h) {}

			const K key;
			const NodePtr left;
			const NodePtr right;
			const long height;
		};
		NodePtr root_;

		//compare the two trees
		//loop through the trees and compare the keys
		static bool Compare(const AVL& tree1, const AVL& tree2) {
			if (tree1.Empty() && tree2.Empty()) return true;
			if (tree1.Empty() || tree2.Empty()) return false;

			std::stack<NodePtr> stack1;
			std::stack<NodePtr> stack2;

			NodePtr node1 = tree1.root_;
			NodePtr node2 = tree2.root_;

			while (node1 != nullptr || !stack1.empty()) {
				while (node1 != nullptr) {
					stack1.push(node1);
					node1 = node1->left;
				}

				node1 = stack1.top();
				stack1.pop();

				while (node2 != nullptr) {
					stack2.push(node2);
					node2 = node2->left;
				}

				node2 = stack2.top();
				stack2.pop();

				if (node1->key != node2->key) return false;

				node1 = node1->right;
				node2 = node2->right;
			}

			return true;
		}

		//print
		static void PrintInOrder(const NodePtr& node) {
			if (node == nullptr) return;
			PrintInOrder(node->left);
			std::cout << node->key << " ";
			PrintInOrder(node->right);
		}

		static void PrintPreOrder(const NodePtr& node) {
			if (node == nullptr) return;
			std::cout << node->key << " ";
			PrintPreOrder(node->left);
			PrintPreOrder(node->right);
		}

		static void PrintPostOrder(const NodePtr& node) {
			if (node == nullptr) return;
			PrintPostOrder(node->left);
			PrintPostOrder(node->right);
			std::cout << node->key << " ";
		}

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
					explicit Itr(const NodePtr &root) {
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
		static void ForEachImplementation(const Node *node, F&& f) {
			if (!node) return;
			ForEachImplementation(node->left.get(), std::forward<F>(f));
			f(const_cast<const K&>(node->key));
			ForEachImplementation(node->right.get(), std::forward<F>(f));
		}

		static long Height(const NodePtr& node) {
			return node ? node->height : 0;
		}

		static NodePtr MakeNode(K key, const NodePtr& left, const NodePtr& right) {
			return std::make_shared<Node>(std::move(key), left, right,
				1 + std::max(Height(left), Height(right)));
		}

		static NodePtr Get(const NodePtr& node, const K& key) {
			if (!node) return nullptr;
			if (node->key < key) return Get(node->right, key);
			if (key < node->key) return Get(node->left, key);
			return node;
		}

		static NodePtr RotateLeft(K key, const NodePtr& left, const NodePtr& right) {
			return MakeNode(right->key, MakeNode(std::move(key), left, right->left),
				right->right);
		}

		static NodePtr RotateRight(K key, const NodePtr& left, const NodePtr& right) {
			return MakeNode(left->key, left->left,
				MakeNode(std::move(key), left->right, right));
		}

		static NodePtr RotateLR(K key, const NodePtr& left, const NodePtr& right) {
			return RotateLeft(key, left, RotateRight(right->key, right->left, right->right));
		}

		static NodePtr RotateRL(K key, const NodePtr& left, const NodePtr& right) {
			return RotateRight(key, RotateLeft(left->key, left->left, left->right), right);
		}

		static NodePtr Rebalance(K key, const NodePtr& left, const NodePtr& right) {
			switch (Height(left) - Height(right)) {
				case 2:
					if (Height(left->left) - Height(left->right) == -1) {
						return RotateLR(std::move(key), left, right);
					} else {
						return RotateRight(std::move(key), left, right);
					}
				case -2:
					if (Height(right->left) - Height(right->right) == 1) {
						return RotateRL(std::move(key), left, right);
					} else {
						return RotateLeft(std::move(key), left, right);
					}
				default:
					return MakeNode(key, left, right);
			}
		}

		static NodePtr AddKey(const NodePtr& node, K key) {
			if (!node) return MakeNode(std::move(key), nullptr, nullptr);
			if (key < node->key) {
				return Rebalance(node->key, AddKey(node->left, std::move(key)), node->right);
			}
			if (node->key < key) {
				return Rebalance(node->key, node->left, AddKey(node->right, std::move(key)));
			}
			return MakeNode(std::move(key), node->left, node->right);
		}

		static NodePtr InOrderH(NodePtr node) {
			while (node->left) node = node->left;
			return node;
		}

		static NodePtr InOrderT(NodePtr node) {
			while (node->right) node = node->right;
			return node;
		}

		static NodePtr RemoveKey(const NodePtr& node, const K& key) {
			if (!node) return nullptr;

			if (key < node->key) {
				return Rebalance(node->key, RemoveKey(node->left, key), node->right);
			}
			if (node->key < key) {
				return Rebalance(node->key, node->left, RemoveKey(node->right, key));
			}

			if (!node->left) return node->right;
			if (!node->right) return node->left;
			
			if(node->left->height < node->right->height) {
				NodePtr h = InOrderH(node->right);
				return Rebalance(h->key, node->left, RemoveKey(node->right, h->key));
			} else {
				NodePtr t = InOrderT(node->left);
				return Rebalance(t->key, RemoveKey(node->left, t->key), node->right);
			}
		}
		abort();
	

};

}

#endif 