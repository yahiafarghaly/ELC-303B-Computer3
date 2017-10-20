/*
 * A2.cpp
 *
 *  Created on: Mar 23, 2016
 *      Author: Yahia
 */
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <stdlib.h>
#include <stdio.h>
#include<fstream>
#include<string>
#include <sstream>
#include<vector>

#include "inord_list.h"
using namespace std;

//error message parameter
enum errorMsg{UndefinedCommand,IncorrectNumberOfArguments,FileNotFound};
//responsible for throwing error messages
void throwError(errorMsg err);
//Read the file data and insert into the linkedList
bool readFile(string fileName,InOrderLinkedList & mylist);
//read command file and store commands in vector
bool readCommandFile(string fileName,vector<string>&commands);
//is command
bool isCommand(string command);
//count spaces
inline long countSpaces(string & line);
//commands
void wordCount(string line,InOrderLinkedList & mylist);
void distWords(string line,InOrderLinkedList & mylist);
void charCount(string line,InOrderLinkedList & mylist,string&filepath);
void frequentWord(string line,InOrderLinkedList & mylist);
void countWord(string line,InOrderLinkedList & mylist);
void starting(string line,InOrderLinkedList & mylist);
void containing(string line,InOrderLinkedList & mylist);
void search(string line,InOrderLinkedList & mylist);

int main(int argc, char* argv[])
{
	//checking the args
	if(argc != 3) {
		throwError(IncorrectNumberOfArguments);
		exit(0);
	}

	string filePath = argv[1];
	string commandFilePath = argv[2];

	InOrderLinkedList mylist;
	vector<string>commands;

	if(!readCommandFile(commandFilePath,commands)){throwError(FileNotFound); exit(0);}
	if(!readFile(filePath,mylist)){throwError(FileNotFound); exit(0);}

	mylist.updateWordRepeation();

	//execute each command
	long commandCapacity = commands.size();
	string Command;
	for(long i=0;i<commandCapacity;i++)
	{
		// checking every command[the checking of its parameters will be inside the function]
		istringstream lineStr(commands.at(i));
		lineStr >> Command;
		//command.size() is used to neglect empty lines in the file
		if(!isCommand(Command)&&Command.size()!= 0) throwError(UndefinedCommand);
		else
		{
			//see which command and call the function and check his pars
			//all output is done from the functions
			if(Command == "wordCount") wordCount(commands.at(i),mylist);
			else if(Command == "distWords") distWords(commands.at(i),mylist);
			else if(Command == "charCount") charCount(commands.at(i),mylist,filePath);
			else if(Command == "frequentWord") frequentWord(commands.at(i),mylist);
			else if(Command == "countWord") countWord(commands.at(i),mylist);
			else if(Command == "starting") starting(commands.at(i),mylist);
			else if(Command == "containing") containing(commands.at(i),mylist);
			else if(Command == "search") search(commands.at(i),mylist);
			Command.clear();
		}
	}

	return 0;
}

void throwError(errorMsg err)
{
	if(err == UndefinedCommand)
		cout<<"Undefined command"<<endl;
	else if(err == IncorrectNumberOfArguments)
		cout<<"Incorrect number of arguments"<<endl;
	else if(err == FileNotFound)
		cout<<"File not found"<<endl;
	//exit(0);
}

bool readFile(string fileName,InOrderLinkedList & mylist)
{

	ifstream myfile(fileName.c_str(), ios::in);

	if (myfile.fail()) return false;
	else
	{
		long lineNum = 1;
		string line;
		string word;

		while(getline(myfile,line))
		{
			istringstream lineStr(line);
			while( lineStr >> word)
			{
				mylist.insert(word,lineNum);
			}
			lineNum++;
		}
		mylist.setNumOfLines(lineNum);
	}
	myfile.close();
	return true;
}


bool readCommandFile(string fileName,vector<string>&commands)
{

	ifstream myfile(fileName.c_str(), ios::in);
	string line;

	if (myfile.fail()) return false;
	else
	{
		while(getline(myfile,line))
			commands.push_back(line);
	}
	myfile.close();
	return true;

}

bool isCommand(string command)
{
	if (command == "wordCount") return true;
	else if(command == "distWords") return true;
	else if(command == "charCount") return true;
	else if(command == "frequentWord") return true;
	else if(command == "countWord") return true;
	else if(command == "starting") return true;
	else if(command == "containing") return true;
	else if(command == "search") return true;
	else return false;
}

void wordCount(string line,InOrderLinkedList & mylist){

	if(line != "wordCount"){
		throwError(IncorrectNumberOfArguments);
		return;
	}

	long sum = 0;
	sum = mylist.getNumOfWords();
	cout<<sum<<" words"<<endl;
}
void distWords(string line,InOrderLinkedList & mylist){
	if(line != "distWords")
	{
		throwError(IncorrectNumberOfArguments);
		return;
	}

	long sum = 0;
	string element;
	if(mylist.isEmpty()) sum = 0;
	else
	{
		sum = mylist.getDist();
	}

	cout<<sum<<" distinct words"<<endl;
}
void charCount(string line,InOrderLinkedList & mylist,string & filePath){

	if(line != "charCount")
	{
		throwError(IncorrectNumberOfArguments);
		return;
	}
	long Nchars = mylist.getNumOfChars();
	long Nspaces = 0;
	long Nlines = 0;
	int ch;

	FILE *infile = fopen(filePath.c_str(), "r");
	ifstream myfile(filePath.c_str(), ios::in);

	string lineText;

	while (EOF != (ch=getc(infile)))
		if ('\n' == ch) Nlines++;

	while(getline(myfile,lineText)) Nspaces = Nspaces + countSpaces(lineText);

	cout<< Nspaces + Nlines + Nchars << " characters"<<endl;

}
void frequentWord(string line,InOrderLinkedList & mylist){

	if(line != "frequentWord"){
		throwError(IncorrectNumberOfArguments);
		return;
	}
	InOrderLinkedList* freqWords;
	freqWords = mylist.frequentWords();

	if (freqWords != NULL) // no freq words exist
	{
		string elem;
		cout<<"Most frequent word is: ";
		freqWords->first(elem);
		cout<<elem<<" ";
		while(freqWords->next(elem))
			cout<<elem<<" ";
		delete freqWords;
		cout<<endl;
	}
}
void countWord(string line,InOrderLinkedList & mylist){
	string word;
	istringstream lineStr(line);
	vector<string>input;
	while( lineStr >> word)
		input.push_back(word);
	if(input.size() != 2){
		throwError(IncorrectNumberOfArguments);
		return;
	}

	long sum = mylist.numOfRepeation(input.at(1));

	cout<<input.at(1)<<" is repeated "<<sum<<" times"<<endl;


}
void starting(string line,InOrderLinkedList & mylist){
	string word;
	istringstream lineStr(line);
	vector<string>input;
	while( lineStr >> word)
		input.push_back(word);
	if(input.size() != 2){
		throwError(IncorrectNumberOfArguments);
		return;

	}
	vector<WORD_DATA_t>startList = mylist.startWith(input.at(1));

	vector<WORD_DATA_t>::size_type sz = startList.size();
	if(sz != 0)
	{
		for(unsigned int i = 0; i<sz ; i++)
		{
			cout<<startList.at(i)._word<<": "<<startList.at(i)._repeated<<"\t";
		}
	cout<<endl;
	}

}
void containing(string line,InOrderLinkedList & mylist){
	string word;
	istringstream lineStr(line);
	vector<string>input;
	while( lineStr >> word)
		input.push_back(word);
	if(input.size() != 2){
		throwError(IncorrectNumberOfArguments);
		return;
	}
	vector<WORD_DATA_t>startList = mylist.containWith(input.at(1));

	vector<WORD_DATA_t>::size_type sz = startList.size();
	if(sz != 0)
	{
		for(unsigned int i = 0; i<sz ; i++)
		{
			cout<<startList.at(i)._word<<": "<<startList.at(i)._repeated<<"\t";
		}
	cout<<endl;
	}

}
void search(string line,InOrderLinkedList & mylist){

	string word;
	istringstream lineStr(line);
	vector<string>input;
	while( lineStr >> word)
		input.push_back(word);
	if(input.size() != 2){
		throwError(IncorrectNumberOfArguments);
		return;
	}

	vector<WORD_DATA_t>searchedList = mylist.search(input.at(1));
	vector<WORD_DATA_t>::size_type sz = searchedList.size();

	if(sz != 0)
	{
		for(unsigned int i = 0; i<sz ; i++)
		{
			cout<<searchedList.at(i)._word<<":\tlines";

			for(unsigned int j = 0; searchedList.at(i)._lines[j] != -1 ;j++)
				cout<<" "<<searchedList.at(i)._lines[j];

			cout<<endl;
		}

		for(unsigned int i = 0; i< sz ;i++)
			delete [] searchedList.at(i)._lines;
	}

}

inline long countSpaces(string & line)
{
	string::iterator str_it = line.begin();
	long sum = 0;
	while(str_it != line.end())
	{
		if(*str_it == ' ') sum++;
		str_it++;
	}
	return sum;
}
