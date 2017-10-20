/*
 * InOrderLinkedList.cpp
 *
 *  Created on: Mar 23, 2016
 *      Author: Yahia
 */

#include "inord_list.h"

InOrderLinkedList::InOrderLinkedList() {
	_head = NULL;
	_current = NULL;
	_n_lines = 0;
}

InOrderLinkedList::~InOrderLinkedList() {

	if(_head != NULL)
	{
			Node* tmpPointer = new Node;
			while (_head->next != NULL)
			{
				tmpPointer = _head;
				_head = _head->next;
				delete tmpPointer;
			}
			delete _head; //delete last element
	}

}

bool InOrderLinkedList::insert(const string&e,unsigned long line)
{
	Node* addedNode = new Node;
	if(addedNode == NULL) return false;
	addedNode->value = e;
	addedNode->WhichLine = line;
	//convert to small first
	transform(addedNode->value.begin(),addedNode->value.end(),addedNode->value.begin(),::tolower);
	//list is empty
	if(_head == NULL || addedNode->value <= _head->value)
	{
		addedNode->next = _head ;
		_head = addedNode;
	}
	else
	{
		Node* pred = new Node;
		if(pred == NULL )return false;
		pred = _head;
		while(pred->next != NULL && pred->next->value < addedNode->value)
			pred = pred->next;

		if(pred->next != NULL)
		{
			if(addedNode->value <= pred->next->value)
			{
					addedNode->next = pred->next;
					pred->next = addedNode;
			}

		}
		else
		{
				pred->next = addedNode;
				addedNode->next =NULL;
		}

	}
	return true;
}

bool InOrderLinkedList::first(string &e)
{
	if (_head == NULL) return false;
	else
	{
		_current = _head;
		e = _current->value;
	}
	return true;
}

bool InOrderLinkedList::next(string &e)
{
	if (_current == NULL || _current->next == NULL ) return false;
	else
	{
		_current = _current->next;
		e = _current->value;
		return true;
	}

}

bool InOrderLinkedList::printList()
{
	if(_head == NULL) return false;
	else
	{
		Node * ptr = _head;
		while(ptr != NULL)
		{
			cout << "Value: " << ptr->value << " Line: " << ptr->WhichLine << " Repeated: " << ptr->wordRepeatition << endl;
			ptr = ptr->next;
		}
		return true;
	}
}

bool InOrderLinkedList::isEmpty()
{
	if(_head == NULL) return true;
	else return false;
}

bool InOrderLinkedList::updateWordRepeation()
{
	if(_head == NULL) return false;
	else
	{
		Node * piolat = new Node;
		if(piolat == NULL) return false;
		Node * seeker = new Node;
		if(seeker == NULL) return false;

		piolat = _head;
		seeker = piolat;

		long repeation=0;

		while(piolat != NULL)
		{
			repeation = 0;
				while(seeker != NULL && seeker->value == piolat->value)
				{
					++repeation;
					seeker = seeker->next;
				}
				while(piolat != seeker)
				{
					piolat->wordRepeatition = repeation;
					piolat = piolat->next;
				}
		}


		return true;
	}

}

long InOrderLinkedList::getDist()
{
	if(_head == NULL) return 0;
	else
	{
		long sum = 0;
		Node *tmp;
		InOrderLinkedList list;
		tmp = _head;

		while(tmp != NULL)
		{
			list.insert(tmp->value,0);
			tmp = tmp->next;
		}
		list.deleteRepeated();
		tmp = list._head;

		while(tmp != NULL)
		{
			sum++;
			tmp = tmp->next;
		}
		return sum;
	}
}

long InOrderLinkedList::getNumOfWords()
{
	if(_head == NULL) return 0;
	else
	{
		long sum = 0;
		Node *tmp = _head;
		while(tmp != NULL)
		{
			++sum;
			tmp = tmp->next;
		}
		return sum;
	}
}

long InOrderLinkedList::getNumOfChars()
{

	if(_head == NULL) return 0;
		else
		{
			long sum = 0;
			Node *tmp = _head;
			while(tmp != NULL)
			{
				sum = sum + (tmp->value).size();
				tmp = tmp->next;
			}
			return sum;
		}


}

void InOrderLinkedList::convertToSmall()
{
	if(_head == NULL) return;
	else
	{
		Node * ptr = _head;
		while(ptr != NULL)
		{
			transform(ptr->value.begin(),ptr->value.end(),ptr->value.begin(),::tolower);
			ptr = ptr->next;
		}
	}
}

long InOrderLinkedList::numOfRepeation(string word)
{
	if(_head == NULL) return 0;
	else
	{
		Node *tmp = _head;
		while(tmp != NULL)
		{
			if(tmp->value == word) return tmp->wordRepeatition;
			tmp = tmp->next;
		}
	}
return 0;
}

InOrderLinkedList* InOrderLinkedList::frequentWords()
{
	InOrderLinkedList * freqList = new InOrderLinkedList;
	assert(freqList != NULL);

	Node* ptr;
	ptr = _head;
	unsigned long maxRepeatition = 1;
	//find max repetition of the words
	while(ptr != NULL)
	{
		if(ptr->wordRepeatition > maxRepeatition && !isCommon(ptr->value))
			maxRepeatition = ptr->wordRepeatition;
		ptr = ptr->next;
	}
	// get the words with the MaxRepeatition
	ptr = _head;
	while(ptr != NULL)
	{
		if(ptr->wordRepeatition == maxRepeatition && !isCommon(ptr->value))
			freqList->insert(ptr->value,0); // it will store in alphabetical if more than word exist
											//with repeatition
		ptr = ptr->next;
	}

	freqList->deleteRepeated();
	if(!freqList->isEmpty()) return freqList;
	else return NULL;
}

inline bool InOrderLinkedList:: isCommon(string& word)
{
	if(word == "a" || word == "an" || word == "the" || word == "in" || word == "on" || word == "of"
			||word == "and" || word == "is" || word == "are") return true;
	else return false;
}

void InOrderLinkedList::deleteRepeated()
{
	if(_head == NULL) return;

	Node* ptr = _head;
	Node* deleted;
	//no need to handle _head since it's got to be referenced to others(if matched) and
	// should not be deleted !
	//this code is a special case !
	while(ptr->next != NULL)
	{
		if(ptr->value == ptr->next->value)
		{
			deleted = ptr->next;
			ptr->next = ptr->next->next;
			delete deleted;
		}
		else
			ptr = ptr->next;
	}
}

void InOrderLinkedList:: setNumOfLines(unsigned long n)
{
	_n_lines = n;
}

vector<WORD_DATA_t> InOrderLinkedList::startWith(string& start)
{

	//allocate
	vector<WORD_DATA_t> vt;
	if(_head == NULL) return vt;
	InOrderLinkedList* startList = new InOrderLinkedList;

	Node* ptr = _head;
	string startingOfStr;
	string::size_type start_sz = start.size();

	while(ptr != NULL)
	{
		startingOfStr = getPartOf(ptr->value,0,start_sz - 1);
		if(startingOfStr == start) startList->insert(ptr->value,0);
		ptr = ptr->next;
	}

	startList->updateWordRepeation();
	startList->deleteRepeated();

	ptr = startList->_head;
	int i = 0;
	while(ptr != NULL)
	{
		i++;
		ptr = ptr->next;
	}
	vt.resize(i);
	ptr = startList->_head;
	i = 0;
	while(ptr != NULL)
	{
		vt.at(i)._word = ptr->value;
		vt.at(i)._repeated = ptr->wordRepeatition;
		i++;
		ptr = ptr->next;
	}

	delete startList;

	return vt;
}

vector<WORD_DATA_t> InOrderLinkedList::containWith(string& literals)
{
	//allocate
	vector<WORD_DATA_t> vt;
	if(_head == NULL) return vt;
	Node* ptr = _head;
	InOrderLinkedList* list = new InOrderLinkedList;
	unsigned long countWord = 0;
	while(ptr != NULL)
	{
		if(ptr->value.find(literals) != string::npos)
			list->insert(ptr->value,0);
		ptr = ptr->next;
	}

	ptr = list->_head;

	if(ptr != NULL)
	{
		list->updateWordRepeation();
		list->deleteRepeated();
		while(ptr != NULL)
		{
			countWord++;
			ptr = ptr->next;
		}
		vt.resize(countWord);
		ptr = list->_head;
		int i = 0;
		while(ptr != NULL)
		{
			vt.at(i)._word = ptr->value;
			vt.at(i)._repeated = ptr->wordRepeatition;
			i++;
			ptr = ptr->next;
		}
		delete list;
		return vt;

	}
	else
	{
		delete list;
		return vt;
	}


}

vector<WORD_DATA_t> InOrderLinkedList::search(string & literals)
{
	//allocate
	vector<WORD_DATA_t> wordVector;
	if(_head == NULL) return wordVector;
	Node* piolat = _head;
	InOrderLinkedList* list = new InOrderLinkedList;
	unsigned long countWord = 0;
	while(piolat != NULL)
	{
		if(piolat->value.find(literals) != string::npos)
			list->insert(piolat->value,piolat->WhichLine);
		piolat = piolat->next;
	}

	piolat = list->_head;

	if(piolat != NULL)
	{
		//will  count the repetition
		while(piolat != NULL)
		{
			countWord++;
			piolat = piolat->next;
		}
		wordVector.resize(countWord);

		for(unsigned int i = 0; i< countWord ;i++)
			wordVector.at(i)._lines = new  long[_n_lines];

		for(unsigned int i = 0; i< countWord ;i++)
			for(unsigned int j = 0; j< _n_lines ;j++)
					wordVector.at(i)._lines[j] = -1;

		piolat = list->_head;
		Node * seeker = piolat;
		int i = 0,j = 0;
		while(piolat != NULL)
		{
			wordVector.at(i)._word = piolat->value;
			j=0;
			while(seeker != NULL && seeker->value == piolat->value)
			{
				wordVector.at(i)._lines[j] = seeker->WhichLine;
				seeker = seeker->next;
				j++;
			}

			piolat = seeker;
			i++;
		}

		wordVector.resize(i);
		delete list;
		return wordVector;
	}
	else
	{
		delete list;
		return wordVector;
	}

}

string getPartOf(string& str,unsigned long start,unsigned long end)
{
	//no overloading in error handling here as i know what i do from it ! in this task only !
	if( str.size() < end - start + 1 ) return _INVALID_STR;

	string tmp(str);

	for(unsigned long i = start,j = 0;i<end;i++,j++)
			tmp[j] = str[i];

	tmp.resize(end-start+1);

	return tmp;
}
