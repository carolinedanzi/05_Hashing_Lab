//DO NOT CHANGE THIS FILE
//Author: Bo Brinkman
//Date: 2013/07/24
#include "USet.h"

/*
 * Note: Just above your template declaration when you use this class, you
 * must define method called "hash" that takes a Key as input, and returns
 * an unsigned long (which is the hash value)
 *
 * For example, you might do:
 * unsigned long hash(char c){ return 10*((unsigned long)c)%backingArraySize; }
 * HashTable<char,int> mySillyTable;
 *
 * If you don't define an appropriate hash function, the class won't compile.
 */

template <class Key, class T>
class HashTable : public USet < Key, T > {
private:
	class HashRecord {
	public:
		Key k;
		T x;

		//If the slot in the hash table is totally empty, set this to true.
		bool isNull;

		//If the slot used to have something in it, but doesn't now, set
		// isDel to true, and isNull to false. isNull is only for slots
		// that have never been used
		bool isDel;

		HashRecord() { isNull = true; isDel = false; };
	};

public:
	//See USet.h for documentation of these methods
	virtual unsigned long size();
	virtual void add(Key k, T x);
	virtual void remove(Key k);
	virtual T find(Key k);
	virtual bool keyExists(Key k);

	//Initialize all private member variables.
	HashTable();
	//Delete any dynamically allocated memory.
	virtual ~HashTable();

private:
	//A pointer to the array that holds the hash table data
	HashRecord* backingArray;

	//Whenever numItems + numRemoved >= backingArraySize/2, call
	// grow(). grow() should make a new backing array that is twice the
	// size of the old one, similar to what we did in the ArrayQueue
	// lab.
	//Note: You cannot just loop through the old array and copy it to the
	// new one! Since the backing array size has changed, each item will likely
	// map to a different slot in the array. You may just want to use add()
	// after initializing the new array.
	void grow();

	//This helper method should take a key, and return the index for that
	// item within the hash table. If the item already exists, return the
	// index of the existing item. If the item doesn't exist, return the index
	// where it OUGHT to be. This function can then be used as a helper method in
	// your other methods.
	unsigned long calcIndex(Key k);

	unsigned long numItems; //Number of items in the hash table

	//Note: Ordinarily, these OUGHT to be private. In this case I have
	// made them public for easy of testing.
public:
	unsigned long numRemoved; //Number of slots that have been removed but not re-used. Those that have isDel == true
	unsigned long backingArraySize;
};


/*
* During class I worked with Justin Reed and Brandon Wilson.
* We discussed the logic behind keyExists, find, and calcIndex.
*/
//You will need this so you can make a string to throw in
// remove
#include <string>

template <class Key, class T>
HashTable<Key, T>::HashTable(){
	// The size of the backing array should be prime
	backingArraySize = hashPrimes[0];
	backingArray = new HashRecord[backingArraySize];
	// There are no items or isDel elements yet
	numItems = 0;
	numRemoved = 0;
}

template <class Key, class T>
HashTable<Key, T>::~HashTable() {
	//TODO
}

// Goal:  Return an index where the item should go
// (or where it already is)
template <class Key, class T>
unsigned long HashTable<Key, T>::calcIndex(Key k){
	unsigned long index = hash(k) % backingArraySize;

	while (backingArray[index].isNull == false){
		// I think this should search through and
		// either find where the item is and return
		// that index, or return the next available index
		if (backingArray[index].k == k)
			return index;
		index = (index + 1) % backingArraySize;
	}
	return index;




	//// Caculate the proper index - the one where the item should be
	//unsigned long properIndex = hash(k) % backingArraySize;

	//// As long as there is an item at the index (starting with the 
	//// proper index), keep looking until finding an empty spot
	//unsigned long index = properIndex;
	//while (backingArray[index].isNull == false){
	//	// If there is an item that has not been deleted that 
	//	// matches the key, return that index instead of the proper index
	//	if (backingArray[index].k == k && backingArray[index].isDel == false)
	//		return index;
	//	index = (index + 1) % backingArraySize;
	//}
	//// If there was an empty spot at the proper index or
	//// the item was never found, return the proper index for that item
	//return properIndex;
}

template <class Key, class T>
void HashTable<Key, T>::add(Key k, T x){
	// If the key already exists in the array,
	// do not add the item again
	if (keyExists(k))
		return;
	// If adding another item pushes the number of items
	// over half the size of the bakcing array, grow the array
	if (numItems + 1 > backingArraySize / 2)
		grow();

	unsigned long index = calcIndex(k);
	std::cout << "The index of the key in add: " << index << std::endl;
	HashRecord addLocation = backingArray[index];
	if (addLocation.isDel == true){
		numRemoved--;
	}
	addLocation.k = k;
	addLocation.x = x;
	addLocation.isNull = false;
	addLocation.isDel = false;
	numItems++;
	std::cout << "key added: " << addLocation.k << std::endl;
	std::cout << "Current state of isNull: " << addLocation.isNull << std::endl;
}

template <class Key, class T>
void HashTable<Key, T>::remove(Key k){
	if (keyExists(k) == false)
		throw std::string("in remove, key does not exist");
	if (numItems == 0)
		throw std::string("Tried to remove when there are no items.");
	else{
		unsigned long index = calcIndex(k);
		backingArray[index].isDel = true;
	}
}

template <class Key, class T>
T HashTable<Key, T>::find(Key k){
	unsigned long index = hash(k) % backingArraySize;

	std::cout << "The index of the key in find: " << index << std::endl;

	std::cout << "at the index location, k = " << backingArray[index].k << std::endl;
	while (backingArray[index].isNull == false){
		if (backingArray[index].k == k && backingArray[index].isDel == false)
			return backingArray[index].x;
		index = (index + 1) % backingArraySize;
	}

	throw std::string("In find, the key does not exist.");

	// If the key does not exist, throw an exception
	//if (keyExists(k) == false)
	//	throw std::string("In find, the key does not exist");

	// Otherwise, return the data for the item with that key
	//unsigned long index = calcIndex(k);
	//return backingArray[index].x;
}

template <class Key, class T>
bool HashTable<Key, T>::keyExists(Key k){
	// Calc index will return the location of where
	// the item should be (or should go)
	unsigned long index = calcIndex(k);
	// If the item is there, the key exists so return true
	if (backingArray[index].k == k)
		return true;
	// Otherwise, the key does not exist
	return false;
}

template <class Key, class T>
unsigned long HashTable<Key, T>::size(){
	return numItems;
}

template <class Key, class T>
void HashTable<Key, T>::grow(){
	//TODO
}

