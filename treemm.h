#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED

#include <vector>
#include <iostream>

template <typename KeyType, typename ValueType>
class TreeMultimap
{
private:
    struct Node
    {
        Node(const KeyType& key, const ValueType& value)
            : key(key), left_child(nullptr), right_child(nullptr)
        {
            values.push_back(value);
        }

        KeyType key;
        std::vector<ValueType> values;
        Node* left_child;
        Node* right_child;
    };

    Node* m_root;

    void deleteTree(Node* ptr)
    {
        if (ptr == nullptr)
            return;

        deleteTree(ptr->left_child);
        deleteTree(ptr->right_child);

        delete ptr;
    }

    //////////////////////////////////////////////

  public:
    class Iterator
    {
      public:
        Iterator()
            : index(0), size(0), node(nullptr)
        {
            // Replace this line with correct code.
        }

        Iterator(int size, Node* node)
            : index(0), size(size), node(node)
        {}

        ValueType& get_value() const
        {
            //throw 1;  // Replace this line with correct code.
            if (is_valid())
            {
                return node->values[index];
            }

            // user shouldn't use get_value when iterator isn't valid 
            throw - 1; 
        }

        bool is_valid() const
        {
            return (index < size && size > 0);  // Replace this line with correct code.
        }

        void advance()
        {
            // Replace this line with correct code.
            index++;
        }

      private:
          int index; 
          int size; 
          Node* node; 
    };

    TreeMultimap()
        : m_root(nullptr)
    {
        // Replace this line with correct code.
    }

    ~TreeMultimap()
    {
        // Replace this line with correct code.
        deleteTree(m_root);
    }

    void insert(const KeyType& key, const ValueType& value)
    {
        // Replace this line with correct code.
        if (m_root == nullptr)
        {
            m_root = new Node(key, value);
            return;
        }
        Node* current = m_root;
        for (;;)
        {

            if (key == current->key)
            {
                current->values.push_back(value);
                return;
            }
            if (key < current->key)
            {
                if (current->left_child != nullptr)
                    current = current->left_child; 
                else
                {
                    current->left_child = new Node(key, value); 
                    return;
                }
            }
            if (key > current->key)
            {
                if (current->right_child != nullptr)
                    current = current->right_child;
                else
                {
                    current->right_child = new Node(key, value);
                    return;
                }
            }
        }
    }

    Iterator find(const KeyType& key) const
    {
        Node* current = m_root;
        while (current != nullptr)
        {
            if (key == current->key)
            {
                return Iterator(current->values.size(), current);
            }
            else if (key < current->key)
                current = current->left_child;
            else
                current = current->right_child; 
        }
        // return invalid iterator 
        return Iterator();  
    }
};

#endif // TREEMULTIMAP_INCLUDED
