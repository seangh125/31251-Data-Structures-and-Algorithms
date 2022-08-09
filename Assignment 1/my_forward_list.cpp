#include <iostream>
#include <ctime>
#include <vector>
#include <forward_list>
#include <algorithm>
#include "my_forward_list.hpp"
#include "tests.hpp"

int main()
{
    Forward_list<int> my_list;
    Tests tester;
    // Here are all the tests run during the marking.
    // You can uncomment them as you implement the methods

    // test_size_initialization should work from the scaffold
    tester.test_size_initialization();

    // test_push_pop requires: push_front, front, pop_front, and the 
    // size_ variable incremented correctly
    tester.test_push_pop();

    // test_null_copy requires the copy constructor method only
    tester.test_null_copy();

    // test_copy_constructor requires push_front, front, pop_front and copy constructor
    tester.test_copy_constructor();

    // test_initializer_list requires initializer list constructor, front, and pop_front
    tester.test_initializer_list();

    // test_initializer_list_str requires the same as test_initializer_list
    tester.test_initializer_list_str();

    // test_merge requires push_front, front, pop_front and merge
    tester.test_merge();

    // same as test_merge
    tester.test_merge_edge_cases();

    // test_split requires push_front, front, pop_front, copy constructor and split
    tester.test_split();

    // test_split_and_merge requires push_front, front, pop_front, copy constructor, merge and split
    tester.test_split_and_merge();

    // test_sort requires push_front, front, pop_front, copy constructor, merge and split
    tester.test_sort();
    return 0;
}
