/*
 * InOrderLinkedList.h
 *
 *  Created on: Mar 23, 2016
 *      Author: Yahia
 */

#ifndef INORD_LIST_H_
#define INORD_LIST_H_

using namespace std;
#include<string>
#include<iostream>
#include <algorithm>
#include<assert.h>
#include<vector>
#define _INVALID_STR "INVALIDSTR#@!"

struct WORD_DATA_t
{
	string _word;
	unsigned long _repeated;
	long * _lines; //-1 means it doesn't exist or end of array
};

class InOrderLinkedList {

private:
	struct Node{
		string value;
		Node* next;
		unsigned int wordRepeatition;
		unsigned long WhichLine;
	};

	Node* _head,*_current;
	unsigned long _n_lines;

public:
	/*Initialize _head = _tail = _current = NULL*/
	InOrderLinkedList();
	/*delete List at the end*/
	~InOrderLinkedList();
	/*insert element at its correct order*/
	bool insert(const string&e,unsigned long line);
	/*Get the first element in the list and set _currentPos to first node*/
	bool first(string&e);
	/*Get the element of _current++ */
	bool next(string&e);
	/*void set number of lines to _n_lines*/
	void setNumOfLines(unsigned long n);
	/*Update linkedList with Word repetition*/
	bool updateWordRepeation();
	/*Print LinkedList*/
	bool printList();
	/*check if the list is empty or not*/
	bool isEmpty();
	/*get distinguish words in the list*/
	long getDist();
	/*get how many words stored in the list*/
	long getNumOfWords();
	/*get number of chars*/
	long getNumOfChars();
	/*convert list elements to small literals*/
	void convertToSmall();
	/*how many this word is repeated*/
	long numOfRepeation(string word);
	/*return a pointer to a list of ordered frequentWords in "This" list*/
	InOrderLinkedList* frequentWords();
	/*if the word is from common language words return true*/
	inline bool isCommon(string& word);
	/*delete repeated word from the list*/
	void deleteRepeated();
	/*return number of words containing "this" start and #ofOcurrances*/
	vector<WORD_DATA_t> startWith(string& start);
	/*return number of words containing "this" literals and #ofOcurrances*/
	vector<WORD_DATA_t> containWith(string& literals);
	/*return searched words with number of lines ,they exist*/
	vector<WORD_DATA_t> search(string & literals);
};

string getPartOf(string& str,unsigned long start,unsigned long end);

#endif /* INORD_LIST_H_ */