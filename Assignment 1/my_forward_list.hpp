#ifndef MY_FORWARD_LIST_HPP
#define MY_FORWARD_LIST_HPP

#include <algorithm>
#include <cassert>

template <typename T>
class Forward_list
{
public:
    class Node
    {
    public:

        T data{};

        Node* next = nullptr;

        Node(){}

        Node(T input_data, Node* next_node= nullptr)
        {
            data = input_data;
            next = next_node;
        }

        ~Node(){}
    };

private:

    unsigned size_ = 0;
    Node* head_ = nullptr;

public:
    Forward_list();

    ~Forward_list();

    Forward_list(const Forward_list<T>& other);

    Forward_list(std::initializer_list<T> input);

    void push_front(const T& data);

    void push_back(const T& data);

    void pop_front();

    T front() const;

    void display() const;

    bool empty() const;

    unsigned size() const;

    void merge(Forward_list& other);

    Forward_list split();

    void sort();

private:

    void merge_sort(Forward_list&);

    //checks if node is empty and traverses if it isnt.
    static void advance(Node* &current){
        assert(current != nullptr);
        current = current -> next;
    }

};

template <typename T>
Forward_list<T>::Forward_list()
{

}

template <typename T>
Forward_list<T>::~Forward_list()
{
    while(head_ != nullptr)
    {
        Node* tmp = head_;
        head_ = head_->next;
        delete tmp;
        --size_;
    }
}

template <typename T>
Forward_list<T>::Forward_list(const Forward_list& other)
{ 
    //if the other list isnt empty then add values to the end 
    //and continue traversing list.
    Node* otherHead = other.head_;
    while (otherHead != nullptr){
        push_back(otherHead -> data);
        advance(otherHead);
    }
}

template <typename T>
Forward_list<T>::Forward_list(std::initializer_list<T> input)
{
    //iterated through the initializer list with a ranged based for loop
    //and used my push_back method.
    for (auto i : input){
        push_back(i);
    }

}

//Custom Function.
template <typename T>
void Forward_list<T>::push_back(const T& data)
{
    //implemented a push_back function to avoid future methods returning a list in reversed order.
    Node* newNode = new Node(data);
    
    if (head_ == nullptr){
        head_ = newNode;
    } 

    else{
        Node* temp = head_;
        while (temp -> next != nullptr){
            advance(temp);
        }
        temp -> next = newNode;
    }
    
    size_++;

}

template <typename T>
void Forward_list<T>::push_front(const T& data)
{
    //creates a new node at the start of the list.
    Node* newNode = new Node(data);
    newNode -> next = head_;
    head_ = newNode;
    size_++;

}

template <typename T>
void Forward_list<T>::pop_front()
{
    //checks if the list is empty and traverses it, deleting each node from the front
    //of the list until there is no nodes left.
    if (head_ != nullptr){
        Node* currentHead = head_;
        head_ = head_ -> next;
        delete currentHead;
        size_--;
    }    

}

template <typename T>
T Forward_list<T>::front() const
{
    //if this list is not empty returns the data at the start of the list.
    //or an arbitary value if it is.
    while (!this -> empty()){
        return head_ -> data;
    }

    return T();

}

template <typename T>
void Forward_list<T>::display() const
{
   //head_ is the start of the list.
   //if head_ is not empty, print out the data of the head_ and 
   //iterate through the list with advance(head).
   Node* head = this -> head_;
   
   while (head != nullptr){
       std::cout << head -> data << std::endl;
       advance(head);
   }

}

template <typename T>
bool Forward_list<T>::empty() const
{

    return (head_ == nullptr);

}

template <typename T>
unsigned Forward_list<T>::size() const
{

    return size_;

}

template <typename T>
Forward_list<T> Forward_list<T>::split()
{

    Forward_list<T> other;
    int totalSize = size_;
    int firstHalfSize = totalSize / 2; 

    //checks if the totalSize of the list is odd and makes firstHalf of the list 
    //the ceiling.
    if (totalSize % 2 != 0) {
        firstHalfSize = firstHalfSize + 1;
    }
    
    Node* currentNode = head_;

    for (int i = 0; i < firstHalfSize -1; i++){
        advance(currentNode);
    }

    //sets the head node of the other list to the next node of the original list and 
    //starts the new list from that point.
    //set the size of this list to be the ceiling and the other list to be the floor.
    other.head_ = currentNode -> next;
    currentNode -> next = nullptr;
    this -> size_ = firstHalfSize;
    other.size_ = totalSize - firstHalfSize;
    return other;

}

template <typename T>
void Forward_list<T>::merge(Forward_list& other)
{

    Node* currentNode = head_;
    Node* otherNode = other.head_;
    
    //merged list should be size of both lists combined.
    this -> size_ = size_ + other.size_;

    // if "this" list is empty, take the other list as "this" list, or if other is empty.
    //nothing happens.
    if (this -> empty()) {
        head_ = other.head_;
        other.head_ = nullptr;
        return;
    } 
    
    else if (other.empty()) {
        return;
    }

    //checks values of this and other list and orders it based on which list has the smaller value 
    //at the current node.
    if ((currentNode -> data) > (otherNode -> data)) {
        Node* tempNode = currentNode;
        currentNode = otherNode;
        otherNode = tempNode;
        head_ = other.head_;
    }

    //while there are still elements within the list,
    //we iterate through it until we reach the end.
    //then we connect this list to the other list and merge is completed.
    while (true) {
        Node* thisNextNode = currentNode -> next;
        
        if (thisNextNode == nullptr) {
            currentNode -> next = otherNode;
            break;
        }

        T thisNextData = thisNextNode -> data;
        T otherData = otherNode -> data;

        // if the next value of this is greater than the next other value, switch the end of the lists
        if (thisNextData > otherData) {
            Node* tempNode = thisNextNode;
            currentNode -> next = otherNode;
            otherNode = tempNode;
        }

        advance(currentNode);
    }

    //make head of other list equal to nullptr
    //to signify other list is empty.
    other.head_ = nullptr;

}

template <typename T>
void Forward_list<T>::merge_sort(Forward_list& my_list)
{
    
    if(my_list.size() == 0 || my_list.size() == 1)
    {
        return;
    }
    Forward_list<T> second = my_list.split();
    merge_sort(my_list);
    merge_sort(second);
    my_list.merge(second);

}

template <typename T>
void Forward_list<T>::sort()
{
    
    merge_sort(*this);
    
}

#endif
