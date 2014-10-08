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
* Otherwise, I worked alone.
*
* Caroline Danzi
* October 7th, 2014
*/
//You will need this so you can make a string to throw in
// remove
#include <string>

template <class Key, class T>
HashTable<Key, T>::HashTable(){
	// The size of the backing array should be prime
	backingArraySize = hashPrimes[0];
	backingArray = new HashRecord[backingArraySize];
	// There are no items or removed elements yet
	numItems = 0;
	numRemoved = 0;
}

template <class Key, class T>
HashTable<Key, T>::~HashTable() {
	delete[] backingArray;
	backingArray = NULL;
}

// Goal:  Return an index where the item should go
// (or where it already is)
template <class Key, class T>
unsigned long HashTable<Key, T>::calcIndex(Key k){
	// Calculate the index value of a key 
	// based on a given hash function
	unsigned long index = hash(k) % backingArraySize;

	// If there is an element at that index,
	// return the index if it has key k
	while (backingArray[index].isNull == false){
		if (backingArray[index].k == k)
			return index;
		index = (index + 1) % backingArraySize;
	}
	// If the HashRecord is null at that index,
	// return the index 
	return index;
}

template <class Key, class T>
void HashTable<Key, T>::add(Key k, T x){
	// If the key already exists in the array,
	// do not add the item again
	if (keyExists(k))
		return;
	// If adding another item pushes the number of items
	// over half the size of the backing array, grow the array
	if (numItems + 1 > backingArraySize / 2)
		grow();
	// Otherwise, calculate the index of where the item
	// should go and add it there by changing the 
	// member variables for that HashRecord
	else{
		unsigned long index = calcIndex(k);
		// Only if we are replacing an item marked for
		// deletion do we subtract the number of removed items
		if (backingArray[index].isDel == true){
			numRemoved--;
		}
		// Change the variables for that HashRecord
		backingArray[index].k = k;
		backingArray[index].x = x;
		backingArray[index].isNull = false;
		backingArray[index].isDel = false;
		// Increase the number of items
		numItems++;
	}

}

template <class Key, class T>
void HashTable<Key, T>::remove(Key k){
	// If there is no key, throw an exception.
	if (keyExists(k) == false)
		throw std::string("in remove, key does not exist");
	else{
		// Otherwise, calculate the index of where the key is
		// Set the HashRecord isDel variable to true
		unsigned long index = calcIndex(k);
		backingArray[index].isDel = true;
		// Decrease the number of items in the table, but
		// increase the number of removed items
		numItems--;
		numRemoved++;
	}
}

template <class Key, class T>
T HashTable<Key, T>::find(Key k){
	// If the key does not exist, throw an exception
	if (!keyExists(k))
		throw std::string("In find, the key does not exist");

	// Otherwise, return the data for the item with that key
	return backingArray[calcIndex(k)].x;
}

template <class Key, class T>
bool HashTable<Key, T>::keyExists(Key k){
	// Calc index will return the location of where
	// the item should be (or should go)
	unsigned long index = calcIndex(k);
	// If the item is there and has not been marked for deletion,
	// the key exists so return true
	if (backingArray[index].k == k && backingArray[index].isDel == false)
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
	// Find the new prime size for the backing array
	unsigned long oldArraySize = backingArraySize;
	int i = 1;
	while (hashPrimes[i] < oldArraySize)
		i++;
	backingArraySize = hashPrimes[i];

	HashRecord* oldArray = backingArray;
	// Create a new, bigger array to store the items
	backingArray = new HashRecord[backingArraySize];

	// Copy the items over into the new array
	for (unsigned int i = 0; i < oldArraySize; i++){
		if (oldArray[i].isNull == false && oldArray[i].isDel == false)
			add(oldArray[i].k, oldArray[i].x);
	}

	// Delete the old backingArray and use the new one
	delete[] oldArray;
}

