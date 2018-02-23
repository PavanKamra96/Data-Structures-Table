/**
	Name: Pavan Kumar Kamra
	Date: 01/04/2018
	Course: BTP500
	Purpose: To learn about how to implement different type of tables and how to look at code and fix it. 
**/

/*************************************************************/
/*    V 1.1:                                                 */
/*      - added prototypes for numRecords() and isEmpty()    */
/*      - coded isEmpty() and numRecords() in SimpleTable    */
/*      - created empty functions for isEmpty() and          */
/*        numRecords() in LPTable                            */
/*************************************************************/

#include <string>
#include <utility>
#include <functional>
#include <string>
#include <iostream>
using namespace std;

/** abstract class that is implemented in both SimpleTable and LPTable -- these methods have to be implemented **/
template <class TYPE>
class Table {
public:
	Table() {}
	virtual bool update(const string& key, const TYPE& value) = 0;
	virtual bool remove(const string& key) = 0;
	virtual bool find(const string& key, TYPE& value) = 0;
	virtual int numRecords() const = 0;
	virtual bool isEmpty() const = 0;
	virtual ~Table() {}
};

/** Class Definition for Simple Table **/
template <class TYPE>
class SimpleTable :public Table<TYPE> {

	struct Record {
		TYPE data_;
		string key_;
		Record(const string& key, const TYPE& data) {
			key_ = key;
			data_ = data;
		}

	};
	
	Record** records_;  //the table
	int recordsMaxSize_;           //capacity of the array
	int currRecordSize_;          //current number of records held
	int search(const string& key);
	void grow();
public:
	SimpleTable(int maxExpected);
	SimpleTable(const SimpleTable& other);
	SimpleTable(SimpleTable&& other);
	virtual bool update(const string& key, const TYPE& value);
	virtual bool remove(const string& key);
	virtual bool find(const string& key, TYPE& value);
	virtual const SimpleTable& operator=(const SimpleTable& other);
	virtual const SimpleTable& operator=(SimpleTable&& other);
	virtual ~SimpleTable();
	virtual bool isEmpty() const { return currRecordSize_ == 0; }
	virtual int numRecords() const { return currRecordSize_; }
};


/** returns index of where key is found in the table of Records -- uses binary search **/
template <class TYPE>
int SimpleTable<TYPE>::search(const string& key) {
	int returnFlag = -1;
	int low = 0;
	int high = currRecordSize_ - 1;
	int mid;
	
	while (low <= high && returnFlag == -1) {
		mid = (low + high) / 2;
		if (records_[mid]->key_ > key)
			high = mid - 1;
		else if (records_[mid]->key_ < key)
			low = mid + 1;
		else
			returnFlag = mid;
	}
	return returnFlag;
}

/** increases the table array size **/
template <class TYPE>
void SimpleTable<TYPE>::grow() {
	Record** newArray = new Record*[recordsMaxSize_ * 2];
	for (int i = 0; i < numRecords(); i++) {
		newArray[i] = records_[i];
	}
	delete[] records_;
	records_ = newArray;
	recordsMaxSize_ = recordsMaxSize_ * 2;  
}

/** default constructor -- creates a array of Records of size maxExpected, and sets the max / current size. **/ 
template <class TYPE>
SimpleTable<TYPE>::SimpleTable(int maxExpected) : Table<TYPE>() {
	records_ = new Record*[maxExpected];
	recordsMaxSize_ = maxExpected;
	currRecordSize_ = 0;
	for (int i =0; i<recordsMaxSize_; i++)
		records_[i] = nullptr; 
}

/** copy constructor -- intialize one object from another of the same type;  **/
template <class TYPE>
SimpleTable<TYPE>::SimpleTable(const SimpleTable<TYPE>& other) {
	recordsMaxSize_ = other.recordsMaxSize_;
	records_ = new Record*[recordsMaxSize_];
	currRecordSize_ = other.currRecordSize_;
		for (int i = 0; i < other.numRecords(); i++){
          		if (other.records_[i]) {
                    		records_[i] = new Record(other.records_[i]->key_, other.records_[i]->data_);
         		 } else {
                     		records_[i] = nullptr;
          		}
		}
}

/** move constructor -- moving the information from others and then setting it to null/0 **/
template <class TYPE>
SimpleTable<TYPE>::SimpleTable(SimpleTable<TYPE>&& other) {
	currRecordSize_ = other.currRecordSize_;
	recordsMaxSize_ = other.recordsMaxSize_;
	records_ = other.records_;
	
	other.records_ = nullptr;
	other.currRecordSize_ = 0;
	other.recordMaxSize_ = 0;
}

/** update function -- it searches for key, if it doesn't find it then it dynamically adds it into
the array. If the current array size == the max array size; then we call the grow function to 
increment the amount of space in the table. Before it inserts the key, it finds the sorted position
it belongs in the array. If the key exists -- it updates the information **/
template <class TYPE>
bool SimpleTable<TYPE>::update(const string& key, const TYPE& value) {
	int indexSearch = search(key);
	if (indexSearch == -1) {
		if (currRecordSize_ == recordsMaxSize_) {
			grow();
		}
		
		int i = currRecordSize_; 

		while ((i>0) && (key < records_[i - 1]->key_))
        	{
            		records_[i] = records_[i - 1];
            		--i;
        	}
        	records_[i] = new Record(key, value);
        	++currRecordSize_;	
	}
	else {
		records_[indexSearch]->data_ = value;
	}
	return true;	
}

/** remove function -- search for the key, if it find its; then we delete the key/data properly. 
If it can't find the key -- it means it does not exist; thus you can't delete it. **/
template <class TYPE>
bool SimpleTable<TYPE>::remove(const string& key) {
	int removeKeySearch = search(key);
	bool removeSuccessfulFlag = false; 
	if (removeKeySearch != -1) {
		delete records_[removeKeySearch];
		for (int i = removeKeySearch; i < numRecords() - 1; i++) {
			records_[i] = records_[i + 1];
		}
		currRecordSize_--;
		removeSuccessfulFlag = true; 
	}
	return removeSuccessfulFlag; 
}

/** the find function -- this searches for the key and finds the value of that key and returns 
true to tell the program that it was found. If not -- returns false. **/
template <class TYPE>
bool SimpleTable<TYPE>::find(const string& key, TYPE& value) {
	int findIndexSearch = search(key);
	bool findResultFlag = false; 
	if (findIndexSearch != -1 && records_[findIndexSearch]){
		value = records_[findIndexSearch]->data_;
		return true;  
	}
	return false; 
}

/** copy assignment operator -- this copies the data from other to the current records_ **/
template <class TYPE>
const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(const SimpleTable<TYPE>& other) {
	if (this != &other) {
		if (!isEmpty()) {
				delete [] records_; 
		}
		recordsMaxSize_ = other.recordsMaxSize_;
		records_ = new Record*[recordsMaxSize_];
		currRecordSize_ = other.currRecordSize_;
		for (int i = 0; i < other.currRecordSize_; i++) {		
		 	if (other.records_[i]) {
                                records_[i] = new Record(other.records_[i]->key_, other.records_[i]->data_);
                         } else {
                                records_[i] = nullptr;
                         }
		}
	}
	return *this;
}

/** move assignment operator -- moves the data from the right side of the = operator to the left
and then it empties the right Simple table data **/
template <class TYPE>
const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(SimpleTable<TYPE>&& other) {
	if (this != &other)
	{
		delete [] records_;

		currRecordSize_ = other.currRecordSize_; 
		recordsMaxSize_ = other.recordsMaxSize_; 
		records_ = other.records_; 

		other.currRecordSize_ = 0; 
		other.recordsMaxSize_ = 0;
		other.records_ = nullptr;
	}
	return *this;
}

/** deconstructor -- destroys the objects at the end of the program **/
template <class TYPE>
SimpleTable<TYPE>::~SimpleTable() {
	if (records_) {
		delete[] records_;
	}
}

/** class definition, private and public functions **/
template <class TYPE>
class LPTable :public Table<TYPE> {
	struct Record {
		TYPE data_;
		string key_;
		Record(const string& key, const TYPE& data) {
			key_ = key;
			data_ = data;
		}
	};

	int capacity;
	int maxRecordsSize_;
	int currRecordSize_;
	Record** records_;
	int search(const string& key);
	int hashFunction(const string& key) const;
	void grow();
public:
	LPTable(int maxExpected, double percentOpen);
	LPTable(const LPTable& other);
	LPTable(LPTable&& other);
	virtual bool update(const string& key, const TYPE& value);
	virtual bool remove(const string& key);
	virtual bool find(const string& key, TYPE& value);
	virtual const LPTable& operator=(const LPTable& other);
	virtual const LPTable& operator=(LPTable&& other);
	virtual ~LPTable();
	virtual bool isEmpty() const { return currRecordSize_ == 0; }
	virtual int numRecords() const { return currRecordSize_; } 
};

/** Default Constructor -- made the array of Records object equal to null, and set default values **/
template <class TYPE>
LPTable<TYPE>::LPTable(int maxExpected, double percentOpen) : Table<TYPE>() {
	capacity = maxExpected + (maxExpected * percentOpen);
	records_ = new Record*[capacity];
	for (int i = 0; i < capacity; i++)
		records_[i] = nullptr;
	maxRecordsSize_ = maxExpected;
	currRecordSize_ = 0;
}

/** copy constructor -- **/
template <class TYPE>
LPTable<TYPE>::LPTable(const LPTable<TYPE>& other) {
	capacity = other.capacity;
	records_ = new Record*[capacity];

	for (int i = 0; i < other.capacity; i++) {
      	  if (other.records_[i]) {
        	    records_[i] = new Record(other.records_[i]->key_, other.records_[i]->data_);
          } else {
           	     records_[i] = nullptr;
          }
    	}
	maxRecordsSize_ = other.maxRecordsSize_;
	currRecordSize_ = other.currRecordSize_;
}

/** move constructor -- **/
template <class TYPE>
LPTable<TYPE>::LPTable(LPTable<TYPE>&& other) {
	capacity = other.capacity;
	records_ = new Record*[capacity];
	for (int i = 0; i < capacity; i++)
		records_[i] = new Record(other.records_[i]->key_, other.records_[i]->data_);
	maxRecordsSize_ = other.maxRecordsSize_;
	currRecordSize_ = other.currRecordSize_;

	for (int i = 0; i < capacity; i++)
	{
		delete records_[i];
		records_[i] = nullptr;
	}

	delete[] other.records;
	other.records_ = nullptr;
	other.currRecordSize_ = 0;
	other.maxRecordsSize_ = 0;
	other.capacity = 0;
}


/**The search function looks for the given key and either returns -1 which means it doesnt
exist, or the index of where the key is located. **/
template <class TYPE>
int LPTable<TYPE>::search(const string& key) {
	std::hash<std::string>hashFunction;
	int hashCodeIndex = hashFunction(key)%capacity;
	int returnValueFlag = 0;

	if (records_[hashCodeIndex] == nullptr){
		returnValueFlag = -1;		 
	}
	else if (records_[hashCodeIndex]->key_ == key) {
		returnValueFlag = hashCodeIndex; 
	}
	else {
		while(returnValueFlag == 0){
			hashCodeIndex++;

			if (hashCodeIndex == capacity){
				hashCodeIndex = 0;
			}
			if (records_[hashCodeIndex] == nullptr){
				returnValueFlag = -1; 
			}
			else if (records_[hashCodeIndex]->key_ == key){
				returnValueFlag = hashCodeIndex;
			}
		}
	}
	return returnValueFlag;
} 

/** The update function searches for the key -- if it find the key
then it updates the data. If it doesnt -- it creates it. **/
template <class TYPE>
bool LPTable<TYPE>::update(const string& key, const TYPE& value) {

	std::hash<std::string>hashFunction;
	int hashCodeIndex = hashFunction(key)%capacity;
	int searchHashCode = search(key);
	bool returnValueFlag = false;  
	
	if (searchHashCode == -1){
	    if (currRecordSize_ != maxRecordsSize_) {	
		if (records_[hashCodeIndex] == nullptr) {
			records_[hashCodeIndex] = new Record(key, value);
			currRecordSize_++;
			returnValueFlag = true; 
		}
		else
			while (!returnValueFlag){
				hashCodeIndex++;
				if(hashCodeIndex == capacity){
					searchHashCode = 0;
				}
				
				if(records_[hashCodeIndex] == nullptr){
					currRecordSize_= currRecordSize_ + 1;
					records_[hashCodeIndex] = new Record(key, value);
					returnValueFlag= true;
					break;
				}
			}
		} 	
	}
	else {
		if (records_[hashCodeIndex] != nullptr){
			records_[searchHashCode]->data_ = value;
			returnValueFlag= true;	
		}
	}
	return returnValueFlag;
}

/** The hash function finds the hash code for the the string key to insert into Records_ **/
template <class TYPE>
int LPTable<TYPE>::hashFunction(const string& key) const
{
    hash<string> hashFunc;
    size_t hashVal = hashFunc(key);
    return (hashVal % capacity);
}

/** Remove function removes the desired element and then rearranges/sorts the table to make sure the
elements are at the correct index **/ 
template <class TYPE>
bool LPTable<TYPE>::remove(const string& key) {
	int searchedHashCode = search(key);
	bool returnValueFlag= false;
	
	if(searchedHashCode != -1 && records_[searchedHashCode]) {	
			delete records_[searchedHashCode];
			records_[searchedHashCode] = nullptr;
				--currRecordSize_; 
				int deletedSpot = searchedHashCode; 
				int currentSpot = (searchedHashCode + 1) % capacity;		
				
				while(records_[currentSpot])
				{
					int currentHashValue = hashFunction(records_[currentSpot]->key_);
					if (currentHashValue <= deletedSpot) 
					{
                				records_[deletedSpot] = records_[currentSpot];
                				records_[currentSpot] = nullptr;
                				deletedSpot = currentSpot;
					}
						currentSpot = (currentSpot +1) % capacity;
				}
				returnValueFlag = true;
	}
return returnValueFlag;	
} 

template <class TYPE>
bool LPTable<TYPE>::find(const string& key, TYPE& value) {
	int code = search(key);
	if ( code != -1 && records_[code]){
		value = records_[code]->data_;
		return true;
	}
	return false;
}

template <class TYPE>
const LPTable<TYPE>& LPTable<TYPE>::operator=(const LPTable<TYPE>& other) {

	if (this != &other)
	{
		if (capacity < other.capacity){
			delete [] records_; 
			records_ = new Record*[other.capacity]; 
			capacity = other.capacity;
		}
		
		currRecordSize_ = other.currRecordSize_;
		maxRecordsSize_ = other.maxRecordsSize_; 
		
		for(int i=0; i<other.capacity; ++i){
			if(other.records_[i])
				if(records_[i])
					*records_[i] = *other.records_[i];
 
				else
					records_[i] = new Record(*other.records_[i]);
				
				else
					records_[i] = nullptr;
		}
	}
	return *this; 
}

// move assignment operator 
template <class TYPE>
const LPTable<TYPE>& LPTable<TYPE>::operator=(LPTable<TYPE>&& other) {
	if (this != &other)
	{
		delete [] records_;
		currRecordSize_ = other.currRecordSize_;
		maxRecordsSize_ = other.maxRecordsSize_;
		capacity = other.capacity;
		records_ = other.records_;
		
		other.currRecordSize_ = 0; 
		other.maxRecordsSize_ = 0;
		other.capacity = 0;
		other.records_ = nullptr;
	}

	return *this;

}

/** deconstructor -- destroys the objects created at the end of the program **/
template <class TYPE>
LPTable<TYPE>::~LPTable() {
		if(records_)
		delete[] records_;
}


