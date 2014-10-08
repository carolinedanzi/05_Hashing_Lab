05_Hashing_Lab
==============

Implement a hash table in C++

Requirements
------------

1. `keyExists`, `find`, `remove`, and `size` should all be O(1)
2. `add` should be reasonably fast. Use linear probing to find an open slot in the hash table. This will be O(1), on average, except when `grow` is called.
3. `grow` should be O(n)
4. Do not leak memory


Reading
=======
"Open Data Structures," Chapter 5, except for 5.2.3. http://opendatastructures.org/ods-cpp/5_Hash_Tables.html

Questions
=========

#### 1. Which of the above requirements work, and which do not? For each requirement, write a brief response.

1. I do not think this requirement was met. All of these methods call on calcIndex, which has to cycle through slots to find an appropriate one. I do not feel this would take constant time.
2. I think this was met. When add does not need to call grow, the most expensive part of the operation would be finding a spot to put the item (linear probing). On average, this should take O(1) time. It will take more time if there is a cluster of items where the additional element needs to go. 
3. Grow definitely takes linear time. It has to cycle through all the elements in the old array in order to copy them over to the new array, which takes O(n) time.
4. I do not think I leaked any memory. I deleted the backing array in both the destructor and grow, and set backingArray equal to NULL in the destructor to avoid a dangling pointer.

#### 2. I decided to use two function (`keyExists` and `find`) to enable lookup of keys. Another option would have been to have `find` return a `T*`, which would be `NULL` if an item with matching key is not found. Which design do you think would be better? Explain your reasoning. You may notice that the designers of C++ made the same decision I did when they designed http://www.cplusplus.com/reference/unordered_map/unordered_map/

I feel that having find return a T* might be more useful. Currently, find returns T, which represents the data (x) of the object. However, I do not feel that x is the most useful thing to return, because index calculations and referencing objects depends on the key, not the data. Returning a pointer could be more useful. Also, having find return NULL if the key does not exist would remove the need for the keyExists method.

#### 3. What is one question that confused you about this exercise, or one piece of advice you would share with students next semester?

I feel that I understand the logic behind a HashTable, but implementing one was difficult. I do not fully understand the difference between and the usefulness of keyExists, find, and calcIndex. I do not think I ever used find in my code.  Also, I realize my grow() method is not working, but I am not sure why. I will continue to look at it.