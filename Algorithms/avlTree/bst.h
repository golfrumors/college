#ifndef BST_H
#define BST_H

//implement simple binary search tree class, for the sake of converting it to an AVL tree
//this is a simple binary search tree, with no balancing

namespace bst
{
    template <typename K, typename V>
    class BST
    {
    public:
        BST() : root(nullptr) {}
        BST(const BST& other) : root(nullptr) { root = Copy(other.root); }
        BST(BST&& other) : root(other.root) { other.root = nullptr; }
        ~BST() { Clear(); }

        BST& operator=(const BST& other)
        {
            if (this != &other)
            {
                Clear();
                root = Copy(other.root);
            }
            return *this;
        }

        BST& operator=(BST&& other)
        {
            if (this != &other)
            {
                Clear();
                root = other.root;
                other.root = nullptr;
            }
            return *this;
        }

        BST Add(const K& key, const V& value) const
        {
            BST result(*this);
            result.root = Add(result.root, key, value);
            return result;
        }

        BST Remove(const K& key) const
        {
            BST result(*this);
            result.root = Remove(result.root, key);
            return result;
        }

        V Find(const K& key) const
        {
            Node* node = Find(root, key);
            if (node == nullptr)
                throw std::out_of_range("Key not found");
            return node->value;
        }

        void Print() const { Print(root); }
        void PrintPre() const { PrintPre(root); }
        void PrintPost() const { PrintPost(root); }

    private:
        struct Node
        {
            K key;
            V value;
            Node* left;
            Node* right;

            Node(const K& key, const V& value, Node* left, Node* right)
                : key(key), value(value), left(left), right(right) {}
        };

        Node* root;

        void Clear() { Clear(root); }
        void Clear(Node* node)
        {
            if (node != nullptr)
            {
                Clear(node->left);
                Clear(node->right);
                delete node;
            }
        }

        Node* Copy(Node* node) const
        {
            if (node == nullptr)
                return nullptr;
            return new Node(node->key, node->value, Copy(node->left), Copy(node->right));
        }

        Node* Add(Node* node, const K& key, const V& value) const
        {
            if (node == nullptr)
                return new Node(key, value, nullptr, nullptr);
            if (key < node->key)
                return new Node(node->key, node->value, Add(node->left, key, value), node->right);
            if (key > node->key)
                return new Node(node->key, node->value, node->left, Add(node->right, key, value));
            return new Node(key, value, node->left, node->right);
        }

        Node* Remove(Node* node, const K& key) const
        {
            if (node == nullptr)
                return nullptr;
            if (key < node->key)
                return new Node(node->key, node->value, Remove(node->left, key), node->right);
            if (key > node->key)
                return new Node(node->key, node->value, node->left, Remove(node->right, key));
            if (node->left == nullptr)
            {
                Node* right = node->right;
                delete node;
                return right;
            }
            if (node->right == nullptr)
            {
                Node* left = node->left;
                delete node;
                return left;
            }
            Node* min = FindMin(node->right);
            return new Node(min->key, min->value, node->left, Remove(node->right, min->key));
        }

        Node* Find(Node* node, const K& key) const
        {
            if (node == nullptr)
                return nullptr;
            if (key < node->key)
                return Find(node->left, key);
            if (key > node->key)
                return Find(node->right, key);
            return node;
        }

        Node* FindMin(Node* node) const
        {
            if (node->left == nullptr)
                return node;
            return FindMin(node->left);
        }

        void Print(Node* node) const
        {
            if (node != nullptr)
            {
                Print(node->left);
                std::cout << node->key << " ";
                Print(node->right);
            }
        }

        void PrintPre(Node* node) const
        {
            if (node != nullptr)
            {
                std::cout << node->key << " ";
                PrintPre(node->left);
                PrintPre(node->right);
            }
        }

        void PrintPost(Node* node) const
        {
            if (node != nullptr)
            {
                PrintPost(node->left);
                PrintPost(node->right);
                std::cout << node->key << " ";
            }
        }

    };

    //bst template with only key
    template <typename K>
    class BST<K, void>
    {
    public:
        BST() : root(nullptr) {}
        BST(const BST& other) : root(nullptr) { root = Copy(other.root); }
        BST(BST&& other) : root(other.root) { other.root = nullptr; }
        ~BST() { Clear(); }

        BST& operator=(const BST& other)
        {
            if (this != &other)
            {
                Clear();
                root = Copy(other.root);
            }
            return *this;
        }

        BST& operator=(BST&& other)
        {
            if (this != &other)
            {
                Clear();
                root = other.root;
                other.root = nullptr;
            }
            return *this;
        }

        BST Add(const K& key) const
        {
            BST result(*this);
            result.root = Add(result.root, key);
            return result;
        }

        BST Remove(const K& key) const
        {
            BST result(*this);
            result.root = Remove(result.root, key);
            return result;
        }

        bool Contains(const K& key) const
        {
            Node* node = Find(root, key);
            return node != nullptr;
        }

        K Get(const K& key) const
        {
            Node* node = Find(root, key);
            if (node == nullptr)
                throw std::out_of_range("Key not found");
            return node->key;
        }

        void Print() const { Print(root); }
        void PrintPre() const { PrintPre(root); }
        void PrintPost() const { PrintPost(root); }

    private:
        struct Node
        {
            K key;
            Node* left;
            Node* right;

            Node(const K& key, Node* left, Node* right)
                : key(key), left(left), right(right) {}
        };

        Node* root;

        void Clear() { Clear(root); }
        void Clear(Node* node)
        {
            if (node != nullptr)
            {
                Clear(node->left);
                Clear(node->right);
                delete node;
            }
        }

        Node* Copy(Node* node) const
        {
            if (node == nullptr)
                return nullptr;
            return new Node(node->key, Copy(node->left), Copy(node->right));
        }

        Node* Add(Node* node, const K& key) const
        {
            if (node == nullptr)
                return new Node(key, nullptr, nullptr);
            if (key < node->key)
                return new Node(node->key, Add(node->left, key), node->right);
            if (key > node->key)
                return new Node(node->key, node->left, Add(node->right, key));
            return new Node(key, node->left, node->right);
        }

        Node* Remove(Node* node, const K& key) const
        {
            if (node == nullptr)
                return nullptr;
            if (key < node->key)
                return new Node(node->key, Remove(node->left, key), node->right);
            if (key > node->key)
                return new Node(node->key, node->left, Remove(node->right, key));
            if (node->left == nullptr)
            {
                Node* right = node->right;
                delete node;
                return right;
            }
            if (node->right == nullptr)
            {
                Node* left = node->left;
                delete node;
                return left;
            }
            Node* min = FindMin(node->right);
            return new Node(min->key, node->left, Remove(node->right, min->key));
        }

        Node* Find(Node* node, const K& key) const
        {
            if (node == nullptr)
                return nullptr;
            if (key < node->key)
                return Find(node->left, key);
            if (key > node->key)
                return Find(node->right, key);
            return node;
        }

        Node* FindMin(Node* node) const
        {
            if (node->left == nullptr)
                return node;
            return FindMin(node->left);
        }

        void Print(Node* node) const
        {
            if (node != nullptr)
            {
                Print(node->left);
                std::cout << node->key << " ";
                Print(node->right);
            }
        }

        void PrintPre(Node* node) const
        {
            if (node != nullptr)
            {
                std::cout << node->key << " ";
                PrintPre(node->left);
                PrintPre(node->right);
            }
        }

        void PrintPost(Node* node) const
        {
            if (node != nullptr)
            {
                PrintPost(node->left);
                PrintPost(node->right);
                std::cout << node->key << " ";
            }
        }

    };

} // namespace bst

#endif