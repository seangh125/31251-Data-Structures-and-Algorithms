#ifndef BST_ASSIGNMENT_HPP
#define BST_ASSIGNMENT_HPP

#include <iostream>
#include <algorithm>
#include <math.h>
#include <string>

template <typename T>
class BST
{
public:
    
    class Node 
    {
    public:
        T key;
        int height = 0;
        Node* left = nullptr;
        Node* right = nullptr;
        Node* parent = nullptr;
        Node(){}
        Node(T k, Node* input_node = nullptr)
        {
            key = k;
            parent = input_node;
        }
    };

private:

    Node* root_ = nullptr;
    unsigned int size_ = 0;


public:
    
    BST(); 

    ~BST();

    void insert(T k);

    Node* successor(T k);

    void delete_min();

    void erase(T k);

    void rotate_right(Node* node);

    unsigned size();

    void print();

    Node* find(T k);

    std::vector<T> make_vec();

    std::vector<int> your_postorder_heights();

    std::vector<int> real_postorder_heights();

    T get_root_value();

    Node* min();

    void pretty_print();

    void pretty_print(const std::string &prefix, const Node *node, bool is_left, bool has_left_sibling);

private: 
    //custom function.
    int fix_height(Node* node);

    void delete_subtree(Node* node);

    Node* min(Node* node);

    void print(Node* node);

    void make_vec(Node* node, std::vector<T>& vec);

    void your_postorder_heights(Node* node, std::vector<int>& vec);

    int real_postorder_heights(Node* node, std::vector<int>& vec);


};

template <typename T>
int BST<T>::fix_height(Node* node)
{
    //recursively checks the max value between the left and the right child and adds 1 to it and sets the height to be the result.

    if (node == nullptr){

        return -1;
        
    }

    int currentHeight = std::max(fix_height(node -> left), fix_height(node -> right)) + 1;
    node -> height = currentHeight;
    return currentHeight;

}

// Default constructor
// You do not need to change this
template <typename T>
BST<T>::BST()
{

}

// Destructor
// We implement this for you
template <typename T>
BST<T>::~BST()
{

    delete_subtree(root_);

}

// helper function for destructor
template <typename T>
void BST<T>::delete_subtree(Node* node)
{

    if(node==nullptr){

        return;

    }

    delete_subtree(node->left);
    delete_subtree(node->right);
    delete node;

}

//*** For you to implement
template <typename T>
void BST<T>::insert(T k)
{
    //Solution adapted from Tutorial 9, insert a new node and adjust the height of all affected nodes up to the root.
    Node* node = root_;
    Node* prevNode = node;
    bool wentRight;
    
    if (root_ == nullptr){

        root_ = new Node(k);
        size_++;
        return;

    }
    
    while (node != nullptr){

        prevNode = node;

        if (k < node -> key){

            node = node -> left;
            wentRight = false;

        }

        else if (k > node -> key){

            node = node -> right;
            wentRight = true;

        }

        else {

            return;

        }
    }

    if (wentRight){

        prevNode -> right = new Node(k, prevNode);
        size_++;

    }
    else {

        prevNode -> left = new Node(k , prevNode);
        size_++;

    }
    fix_height(root_);

}

//*** For you to implement
template <typename T>
typename BST<T>::Node* BST<T>::successor(T k)
{
    //follow example from lecture.
    Node* node = root_;
    Node* parent = nullptr;

    while (node != nullptr && node -> key != k){
        //start at the root and traverse down the tree, checking if 
        //the key to be found is larger than the key that node is
        //pointing to, go down left sub tree and set the parent to be the previous
        //node it was on.
        if (k < node -> key){
            
            parent = node;
            node = node -> left;

        }

        //if the key is bigger than the key at node
        //then go down right subtree.
        else {

            node = node -> right;
            
        }
    }

    if (node == nullptr){

        return nullptr;

    }

    //if the node at k, has a right child then the successor node is within the right subtree.
    //specifically the minimum of the right child.
    else if (node -> right != nullptr){
        
        Node *successorNode = min(node -> right);
        return successorNode;

    }

    //otherwise if the node at k has no left or right child, return the parent
    //as the successor.

    else {

        return parent;

    }

    fix_height(node);
    
}

//*** For you to implement
template <typename T>
void BST<T>::delete_min()
{
    //minChild will always be min -> right, as anything left of min would invalidate min.
    Node* minNode = min();
    Node* minParent = minNode -> parent;
    Node* minChild = minNode -> right;
    //check if tree is empty.
    if (root_ == nullptr){

        return;

    }
    
    //check if the minimum node is the root and if it has no child.
    //set the root to nullptr and delete the minNode to avoid memory leak.
    if (minNode == root_){
        
        if (minChild == nullptr){

            root_ = nullptr;

        }

    //check if the min is the root and if it has a child.
        else if (minChild != nullptr){
        
            minChild -> parent = nullptr;
            root_ = minChild;

        }

        delete minNode;

    }
    
    //check if the min is not the root and if it has a child or not.
    else if (minNode != root_){
        
        if (minChild == nullptr){

            minParent -> left = nullptr;

        }

        else if (minChild != nullptr){
            
            minParent -> left = minChild;
            minChild -> parent = minParent;
            
        }

        delete minNode;

    }

    //adjust height and size of tree.
    size_--;
    fix_height(root_);
    
}

//*** For you to implement
template <typename T>
void BST<T>::erase(T k)
{
    
    Node* nodeToErase = find(k);
    Node* leftChild = nodeToErase -> left;
    Node* rightChild = nodeToErase -> right;
    Node* parentNode = nodeToErase -> parent;

    if (root_ == nullptr){

        return;

    }

    //Just an anecdote. In hindsight I should have made a function that checks for the children alignment.
    //or alternatively just make three functions that deal with whether it has no children, one child or two children.
    //and use those functions to deal with the cases in Erase.
    
    //checks if nodeToErase is the root.
    else if (parentNode == nullptr){
        
        //check if both children are present.
        if(leftChild != nullptr && rightChild != nullptr){
            Node* rootSuccessor = min(rightChild);
            
            //check if the rootsuccessor has a right child and is not the rightchild.
            if (rootSuccessor != rightChild && rootSuccessor -> right == nullptr){

                (rootSuccessor -> parent) -> left = nullptr;
                rootSuccessor -> parent = nullptr;
                root_ = rootSuccessor;
                leftChild -> parent = rootSuccessor;
                rootSuccessor -> left = leftChild;
                rightChild -> parent = rootSuccessor;
                rootSuccessor -> right = rightChild;

            }
            
            //check if the successor is the right child.
            else if (rootSuccessor == rightChild){

                rootSuccessor -> parent = nullptr;
                root_ = rootSuccessor;
                leftChild -> parent = rootSuccessor;
                rootSuccessor -> left = leftChild;

            }
            
            //check if the successor has a rightchild.
            else if (rootSuccessor -> right != nullptr){

                (rootSuccessor -> parent) -> left = rootSuccessor -> right;
                rootSuccessor -> parent = nullptr;
                root_ = rootSuccessor;
                rootSuccessor -> right = rightChild;
                rightChild -> parent = rootSuccessor;
                rootSuccessor -> left = leftChild;
                leftChild -> parent = rootSuccessor;

            }
        }
        
        //check if root is a leaf.
        else if (leftChild == nullptr && rightChild == nullptr){

            root_ = nullptr;

        }
        
        //check if right child is empty.
        else if (leftChild != nullptr && rightChild == nullptr){

            leftChild -> parent = nullptr;
            root_ = leftChild;
           
        }
        
        //check if left child is empty.
        else if (rightChild != nullptr && leftChild == nullptr){

            rightChild -> parent = nullptr;
            root_ = rightChild;
           
        }

        delete nodeToErase;

    }

    //checks if the nodeToErase is on the right subtree and is not the root.
    else if (parentNode != nullptr && parentNode -> right == nodeToErase){
            
            if (leftChild != nullptr && rightChild != nullptr){
                Node* successorNode = min(rightChild);
                
                //checks if the successor has a right child and is not the rightChild.
                if (successorNode -> right != nullptr && successorNode != rightChild){

                    (successorNode -> parent) -> left = successorNode -> right;
                    (successorNode -> right) -> parent = successorNode -> parent;
                    successorNode -> parent = parentNode;
                    parentNode -> right = successorNode;
                    successorNode -> left = leftChild;
                    leftChild -> parent = successorNode;
                    successorNode -> right = rightChild;
                    rightChild -> parent = successorNode;

                }
                
                //checks if the successor is the rightchild.
                else if (successorNode == rightChild){

                    successorNode -> parent = parentNode;
                    parentNode -> right = successorNode;
                    successorNode -> left = leftChild;
                    leftChild -> parent = successorNode;
                    
                }
                
                //else if the successor is not the right child and has no right child.
                else if (successorNode -> right == nullptr && successorNode != rightChild) {

                    (successorNode -> parent) -> left = nullptr;
                    (successorNode -> parent) = parentNode;
                    parentNode -> right = successorNode;
                    successorNode -> left = leftChild;
                    leftChild -> parent = successorNode;
                    successorNode -> right = rightChild;
                    rightChild -> parent = successorNode;
                    
                }
            }

            //checks if both children are empty.
            else if (leftChild == nullptr && rightChild == nullptr){

                parentNode -> right = nullptr;

            }
            
            //checks if right child is not empty.
            else if (leftChild == nullptr && rightChild != nullptr){

                rightChild -> parent = parentNode;
                parentNode -> right = rightChild;

            }
            
            //checks if leftchild is not empty.
            else if (rightChild == nullptr && leftChild != nullptr){

                leftChild -> parent = parentNode;
                parentNode -> right = leftChild;

            }

            delete nodeToErase;
            
    }
    
    //checks if nodeToErase is not the root and if it is on the left subtree.
    else if (parentNode != nullptr && parentNode -> left == nodeToErase){
        
        //checks if both children are here and replaces with successor.
        if (leftChild != nullptr && rightChild != nullptr){
            Node* succ = min(rightChild);

            //check if successor is not null and then set relationship.
            if (succ -> right != nullptr && succ != rightChild){

                (succ -> parent) -> left = succ -> right;
                (succ -> right) -> parent = succ -> parent;
                succ -> parent = parentNode;
                succ -> right = rightChild;
                rightChild -> parent = succ;
                parentNode -> left = succ;
                succ -> left = leftChild;
                leftChild -> parent = succ;

            }

            //check if the successor is the right child.
            else if (succ == rightChild){

                succ -> parent = parentNode;
                parentNode -> left = succ;
                succ -> left = leftChild;
                leftChild -> parent = succ;

            }

            else {

                succ -> parent -> left = nullptr;
                succ -> right = rightChild;
                rightChild -> parent = succ;
                succ -> parent = parentNode;
                parentNode -> left = succ;
                succ -> left = leftChild;
                leftChild -> parent = succ;

            }
            
        }
        // if both children are empty and nodeToErase is a leaf.
        else if (leftChild == nullptr && rightChild == nullptr){

            parentNode -> left = nullptr;
           
        }

        //check if right child is not empty.
        else if (leftChild == nullptr && rightChild != nullptr){

            rightChild -> parent = parentNode;
            parentNode -> left = rightChild;
           
        }
        //check if left child is not empty.
        else if (rightChild == nullptr && leftChild != nullptr){

            leftChild -> parent = parentNode;
            parentNode -> left = leftChild;
        
        }

        delete nodeToErase;

    }

    size_--;
    fix_height(root_);
    
}

//*** For you to implement
template <typename T>
void BST<T>::rotate_right(Node* node)
{

    //follow lecture example and notes. Only runs if node left is not nullptr.
    Node* moveUpNode = node -> left;
    node -> left = moveUpNode -> right;
    
    //if the right of the moveUpnode is not empty, then set it's parent to be node.
    if (moveUpNode -> right != nullptr){

        moveUpNode -> right -> parent = node;

    }
    
    //set the moveUpNode parent to be the nodes parent.
    moveUpNode -> parent = node -> parent;
    
    //check if nodes parent is empty and set it as the root.
    if (node -> parent == nullptr){

        root_ = moveUpNode;

    }
    
    //otherwise make moveUpNode the right child of node's parent.
    else if(node == node -> parent -> right){

        node -> parent -> right =moveUpNode;

    }
    
    //otherwise make the moveUpNode the left child of the node's parent.
    else {

        node -> parent -> left = moveUpNode;

    }


    moveUpNode -> right = node;
    node -> parent = moveUpNode;
    fix_height(root_);
    
}

// The rest of the functions below are already implemented

// returns a pointer to the minimum node
template <typename T>
typename BST<T>::Node* BST<T>::min()
{

    if(root_ == nullptr){

        return root_;
        
    }

    else{

        return min(root_);

    }

}

// returns pointer to minimum node in the subtree rooted by node
// Assumes node is not nullptr
template <typename T>
typename BST<T>::Node* BST<T>::min(Node* node)
{

    while(node->left != nullptr){

        node = node->left;

    } 

    return node;

}

// returns a pointer to node with key k
template <typename T>
typename BST<T>::Node* BST<T>::find(T k)
{

    Node* node = root_;  
    while(node != nullptr && node->key != k){

        node = k < node ->key ?  node->left : node->right;

    }

    return node;  

}

template <typename T>
unsigned BST<T>::size()
{

    return size_;

}

// prints out the keys in the tree using in-order traversal
template <typename T>
void BST<T>::print()
{

    print(root_);

}

// you can modify what is printed out to suit your needs
template <typename T>
void BST<T>::print(Node* node)
{

    if(node == nullptr){

        return;

    }

    print(node->left);
    std::cout << node->key << " left height " << node->height << '\n';
    print(node->right);
    std::cout << node->key << " right height " << node->height << '\n';

}

// This is used in our testing, please do not modify
template <typename T>
typename std::vector<T> BST<T>::make_vec()
{

    std::vector<T> vec;
    vec.reserve(size_);
    make_vec(root_, vec);
    return vec;

}

// This is used for our testing, please do not modify
template <typename T>
void BST<T>::make_vec(Node* node, std::vector<T>& vec)
{

    if(node == nullptr){

        return;

    }

    make_vec(node->left, vec);
    vec.push_back(node->key);
    make_vec(node->right, vec);
    
}

// This is used for our testing, please do not modify
template <typename T>
void BST<T>::your_postorder_heights(Node* node, std::vector<int>& vec)
{

    if(node == nullptr){

        return;

    }

    your_postorder_heights(node->left, vec);
    your_postorder_heights(node->right, vec);
    vec.push_back(node->height);
    
}

// This is used for our testing, please do not modify
template <typename T>
int BST<T>::real_postorder_heights(Node* node, std::vector<int>& vec)
{

    if(node == nullptr){

        return -1;

    }

    int left_height = real_postorder_heights(node->left, vec);
    int right_height = real_postorder_heights(node->right, vec);
    int node_height = 1 + std::max(left_height, right_height);
    vec.push_back(node_height);
    return node_height;

}

// This is used for our testing, please do not modify
template <typename T>
typename std::vector<int> BST<T>::your_postorder_heights()
{

    std::vector<int> vec;
    vec.reserve(size_);
    your_postorder_heights(root_, vec);
    return vec;

}

// This is used for our testing, please do not modify
template <typename T>
typename std::vector<int> BST<T>::real_postorder_heights()
{
    
    std::vector<int> vec;
    vec.reserve(size_);
    real_postorder_heights(root_, vec);
    return vec;

}



// This is used for our testing, please do not modify
template <typename T>
T BST<T>::get_root_value() {

    if (root_ == nullptr){

        T dummy {};
        return dummy;

    }

    else{

        return root_->key;

    }

}

template <typename T>
void BST<T>::pretty_print(const std::string &prefix, const Node *node, bool is_left, bool has_left_sibling)
{
    if (node != nullptr){

        if (node != root_){

            std::cout << prefix;

            if (is_left){

                std::cout << "L└──";

            }

            else{

                std::cout << (has_left_sibling ? "R├──" : "R└──");

            }         
        }

        else{

            std::cout << "  ";

        }

        std::cout << node->key << std::endl;

        pretty_print(prefix + (is_left ? "   " : " |  "), node->right, false, node->left != nullptr);
        pretty_print(prefix + (is_left ? "   " : " |  "), node->left, true, false);

    }
}

template <typename T>
void BST<T>::pretty_print()
{

    pretty_print("", root_, true, false);

}

#endif

