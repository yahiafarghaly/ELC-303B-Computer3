/*
 * a3.cpp
 *
 *  Created on: Apr 14, 2016
 *      Author: username
 */

#include "stack.h"
#include<iostream>
#include<string>
#include<stdlib.h>
#include <stdio.h>
#include<vector>
#include<utility>
#include<sstream>
#include<math.h>
#include<algorithm>
using namespace std;

enum OperatorPiorities{ADDITION=1,SUBTRACTION=1,MODULUS=2, DIVISION=2,MULTIPLICATION=2,EXPONENTIATION=3 ,FACTORIAL=4,PARENTHESES=5};
struct SimpleMathExpression {
	double leftOperand, rightOperand;
	string _operator;
	long first_left_operand_pos;//left
	long last_right_operand_pos;//right
};

void outputResult(double f);
void throwInvalid();
//boolen
bool isValidToCalculate(string & expr);//need alot of work
bool isOperation(char s);
bool is_Digit(char s);
//conversions
double convertDigitsToDouble(vector<char>& digits);
string convertDigitsToString(vector<char>& digits);
double str2double(string &str);
string char2str(char c);
string double2str(double d);
string long2str(long d);
string long2str(unsigned long d); //for factorial
//core functions
double parseMathmeticalExpression(string &mathExpression);
string startCalulation(string &mathExpression);
vector<pair<unsigned long, unsigned long> >getPiorityList(string &mathExpr);//pos piority
enum OperatorPiorities whatIsPiority(char mathSymbol);
SimpleMathExpression extractOperandsAndOperator(string _mathexpr, pair<unsigned long, unsigned long>& _maxPiority);
pair<unsigned long, unsigned long> getMaxPiority(vector<pair<unsigned long, unsigned long> >&pL);
string doSimpleMath(SimpleMathExpression &SME);
string replaceWithResult(unsigned long f_pos, unsigned long l_pos,string& actual_str ,string& replacement_str);
bool isOnlyDigits(string& mathResult);
unsigned long factorial(long n);
bool isBalanced(const string& mathExpr);
bool isGoodMath( string& mathExpr);
void deleteLastSpaces(string& str);
bool isCorrectInNegative(const string & mathExpr);
bool isBalancedoperators(string & mathExpr);

#define Y_DEBUG 0

int main(int argc, char**  argv)
{
#if(Y_DEBUG != 1)

	string mathExpression;
	vector<string> symbols;

	if(argc == 1)
		outputResult(0);
	else
	{
		for(int i = 1; i < argc;i++)
			symbols.push_back((argv[i]));

		int symbols_sz = symbols.size();
		
		if(symbols_sz  == 1) mathExpression = symbols.at(0);
		else
		{
			for(int i = 0; i< symbols_sz ;i++)
			{
				mathExpression.append(symbols.at(i));
				if(i != symbols_sz - 1) //Don't append at the last of string space
					mathExpression.append(" ");
			}
		}


		if(!isValidToCalculate(mathExpression)) throwInvalid();
		else
		{
			outputResult(parseMathmeticalExpression(mathExpression));
		}
	}

#else
	

	cout << isCorrectInNegative(string("-5")) << endl;
	system("pause");

#endif

	return 0;
}

bool isValidToCalculate(string & expression)
{
	//expression comes with spaces

	//check non proper syntax inside and escape space
	if (!(expression.find_first_not_of(".0123456789()x+-/%!^ ") == string::npos)) return false;

	//check for parentheses
	if(!isBalanced(expression)) return false;

	//only one element
	if(expression.size() == 1 && !is_Digit(expression[0])) return false;

	//2) >>remove Spaces
	if (!isGoodMath(expression)) return false;


	return true;

}

bool isGoodMath(string & mathExpr)
{
	deleteLastSpaces(mathExpr);
	if(!isCorrectInNegative(mathExpr)) return false;

	// check for the numbers to be not spaced
	string::iterator it = mathExpr.begin();
	while(it != mathExpr.end())
	{
		if(((*it) == ' ') && it != mathExpr.begin())
			if(is_Digit(*(it - 1)) && is_Digit(*(it + 1)) )
					return false;
		it++;
	}

	//REMOVE ANY SPACES
	mathExpr.erase(remove(mathExpr.begin(),mathExpr.end(),' '),mathExpr.end());
	
	if (!isBalancedoperators(mathExpr)) return false;
	

	return true;
}

bool isCorrectInNegative(const string & mathExpr)
{
	vector<long> symbol_index;
	//check is formed correctly with negative sign
	long expression_sz = mathExpr.size();

	for (long i = 0; i < expression_sz; i++)
		if (mathExpr[i] == '-') symbol_index.push_back(i);

	long symbol_size = symbol_index.size();

	//5-
	if (symbol_size != 0 && symbol_index.at(symbol_size - 1) == expression_sz - 1) return false;

	long minus_pos, fd, pd, dummy1, dummy2;
	for (long i = 0; i < symbol_size; i++)
	{
		minus_pos = symbol_index.at(i);

		if (minus_pos == 0 && symbol_size >= 1)
		{
			if (!is_Digit(mathExpr.at(1))) return false;
		}
		else
		{
			fd = pd = minus_pos;
			fd++;//one more step
			pd--;
			//move forward until find non empty space element
			while (fd < expression_sz - 1 && mathExpr[fd] == ' ') fd++;
			//move backward until find non empty space element
			while (pd > 0 && mathExpr[pd] == ' ') pd--;
			//take a decision

			dummy1 = fd - 1;//previous fd
			dummy2 = pd + 1;//after pd

			// 5 - 1
			if (is_Digit(mathExpr[fd]) && is_Digit(mathExpr[pd])
				&& mathExpr[dummy1] == ' ' && mathExpr[dummy2] == ' ') {/*good*/}
			//1 - -1
			// * - -1 wrong
			// 2 - * 2 wrong
			else if(mathExpr[fd] == '-' && mathExpr[dummy1] == ' ' && is_Digit(mathExpr[pd])) {/*good*/}
			// * -1
			else if (is_Digit(mathExpr[fd]) && isOperation(mathExpr[pd]) && mathExpr[dummy1] != ' ' && mathExpr[dummy2] == ' ') {/*good*/ }
			//( -2
			else if (mathExpr[pd] == '(' && mathExpr[dummy2] == ' ') {/*good*/ }
			//) - 2
			else if (mathExpr[pd] == ')' && mathExpr[dummy1] == ' ' &&  mathExpr[dummy2] == ' ') {/*good*/ }
			//* - ( or 2 - (
			else if (mathExpr[fd] == '(' && mathExpr[dummy1] == ' ' && mathExpr[dummy2] == ' ') {/*good*/ }
			//* -( or 2 + -(
			else if (mathExpr[fd] == '(' && isOperation(mathExpr[pd]) && mathExpr[dummy2] == ' ') {/*good*/ }
			else return false;
		}
	}
	return true;
}

bool isBalancedoperators(string & mathExpr)
{
	vector<unsigned long>operatorsPosition;

	//get Positions
	string::iterator pos_iter = mathExpr.begin();
	unsigned long index = 0;
	while (pos_iter != mathExpr.end())
	{
		//Don't consider negative sign at the first of the expression is an operator
		if (pos_iter == mathExpr.begin() && *pos_iter == '-'){/*neglect*/}
		else if (((*pos_iter) == '-') || ((*pos_iter) == '+') || ((*pos_iter) == 'x')
				|| ((*pos_iter) == '/') || ((*pos_iter) == '%') || ((*pos_iter) == '!')|| ((*pos_iter) == '^')
				||((*pos_iter) == '(') || ((*pos_iter) == ')'))
		{
			operatorsPosition.push_back(index);
		}

		index++;
		pos_iter++;
	}


	long size = operatorsPosition.size();
	char _operator;
	unsigned long _pos;

	for (long i = 0; i < size; i++)
	{
		_pos = operatorsPosition.at(i);
		_operator = mathExpr[_pos];

		if (_operator == '+'
				&& !(_pos != 0
				&& ((is_Digit(mathExpr[_pos - 1])) || (mathExpr[_pos - 1] == '!')|| (mathExpr[_pos - 1] == ')'))
				&& (is_Digit(mathExpr[_pos + 1]) || mathExpr[_pos + 1] == '-' || (mathExpr[_pos + 1] == '(')))
			) 
			return false;

		else if ((_operator == 'x' || _operator == '/' || _operator == '%')
				&& !(_pos != 0
				&& ((is_Digit(mathExpr[_pos - 1])) || (mathExpr[_pos - 1] == '!') || (mathExpr[_pos - 1] == ')') )
				&& (is_Digit(mathExpr[_pos + 1]) || mathExpr[_pos + 1] == '-' || (mathExpr[_pos + 1] == '(')))
			)
			return false;

		else if ((_operator == '!')
					&& !(_pos != 0
					&& ((is_Digit(mathExpr[_pos - 1])) || (mathExpr[_pos - 1] == '!') || (mathExpr[_pos - 1] == ')') )
					)
			)
			return false;

		else if ((_operator == '^')
				&& !(_pos != 0
				&& ((is_Digit(mathExpr[_pos - 1])) || (mathExpr[_pos - 1] == ')') )
				&& (is_Digit(mathExpr[_pos + 1]) || mathExpr[_pos + 1] == '-' || (mathExpr[_pos + 1] == '(')))
			)
			return false;
		else if ((_operator == '(') && ((_pos == mathExpr.size() - 1) || mathExpr[_pos+1] == ')'))
			return false;
		else if ((_operator == ')') && ( (_pos == 0) || mathExpr[_pos+1] == '('))
			return false;

	}

	return true;
}

double parseMathmeticalExpression(string & mathExpression)
{
	bool EvalutionDone = false;
	long firstPos, secondPos;
	unsigned long index = 0; //for iterating on the string
	unsigned long strLen;
	string tmpResult;

	while (!EvalutionDone)
	{
		if (isOnlyDigits(mathExpression))
			EvalutionDone = true;
		else
		{

			if (mathExpression.find_first_of("(") != string::npos)
			{
				//start searching for '('
				index = 0;
				firstPos = secondPos = -1; //not found
				strLen = mathExpression.size();
				while (index < strLen)
				{
					//update firstPos if find a new one
					if (mathExpression[index] == '(')
						firstPos = index;
					else if (mathExpression[index] == ')')
					{
						secondPos = index;
						break;
					}
					index++;
				}
				if (firstPos != -1 && secondPos != -1)
				{
					firstPos++;//one step after '('
					tmpResult = mathExpression.substr(firstPos, secondPos - firstPos);
					tmpResult = startCalulation(tmpResult);
					mathExpression = replaceWithResult(firstPos - 2, secondPos + 1, mathExpression, tmpResult);
					// -2 as i incremented one before and for deleting '(' when replacemnet 
					//the same for secondPos + 1
				}
			}
			else
			{
				mathExpression = startCalulation(mathExpression);
			}
		}

	}

	return str2double(mathExpression);
}

string startCalulation(string & mathExpression)
{

	vector<pair<unsigned long, unsigned long> >piorityList;
	pair<unsigned long, unsigned long> pairMaxPiority;
	SimpleMathExpression SME;
	string tmpResult;
	bool EvalutionDone = false;

	while (!EvalutionDone)
	{
		if (isOnlyDigits(mathExpression))
			EvalutionDone = true;
		else
		{
			//fill piority List with its Position
			piorityList = getPiorityList(mathExpression);
			//get max piority
			pairMaxPiority = getMaxPiority(piorityList);
														
			SME = extractOperandsAndOperator(mathExpression, pairMaxPiority);
			tmpResult = doSimpleMath(SME);
			mathExpression = replaceWithResult(SME.first_left_operand_pos, SME.last_right_operand_pos, mathExpression, tmpResult);
			piorityList.clear();
		}
	}

	return mathExpression;
}

inline void outputResult(double f)
{
	cout.setf(ios::fixed,ios::floatfield);
	cout.precision(2);
	cout << f;
	cout<<endl;
}

void throwInvalid()
{
	cout<<"Invalid Expression"<<endl;
	exit(0);
}

bool is_Digit(char s)
{
	string str;
	str.push_back(s);
    return (str.find_first_of("0123456789") != std::string::npos);
}

inline bool isOperation(char s)
{
	string str;
	str.push_back(s);
    return (str.find_first_of("x+-/%!^") != std::string::npos);
}

double convertDigitsToDouble(vector<char>& digits)
{
	long vect_sz = digits.size();
	string number;
	for(long i = 0 ; i<vect_sz; i++)
		number += digits[i];

	return str2double(number);
}

string convertDigitsToString(vector<char>& digits)
{
	long vect_sz = digits.size();
	string number;
	for(long i = 0 ; i<vect_sz; i++)
		number += digits[i];
	return number;
}

double str2double(string &str)
{
	stringstream ss(str);
	double n;
	ss >> n;
	return n;
}

string char2str(char c)
{
	stringstream ss;
	string str;
	ss << c;
	ss >> str;
	return str;
}

string double2str(double d)
{
	std::ostringstream strs;
	strs.precision(2);
	strs <<std::fixed << d; //to avoid scientific notation
	std::string str = strs.str();
	return str;
}

string long2str(long d)
{
	std::ostringstream strs;
	strs << d;
	std::string str = strs.str();
	return str;
}

string long2str(unsigned  long d)
{
	std::ostringstream strs;
	strs << d;
	std::string str = strs.str();
	return str;
}

vector<pair<unsigned long, unsigned long> > getPiorityList(string & mathExpr)
{
	vector<pair<unsigned long, unsigned long> >pList;
	string::iterator pos_iter = mathExpr.begin();
	unsigned long i = 0;
	while (pos_iter != mathExpr.end())
	{
		//Don't consider negative sign at the first of the expression is an operator
		if (pos_iter == mathExpr.begin() && *pos_iter == '-'){/*neglect*/}
		else if (((*pos_iter) == '-') || ((*pos_iter) == '+') || ((*pos_iter) == 'x')
				|| ((*pos_iter) == '/') || ((*pos_iter) == '%') || ((*pos_iter) == '!')|| ((*pos_iter) == '^'))
		{
			pList.push_back(pair<unsigned long, unsigned long>(i, whatIsPiority(*pos_iter)));
		}
		
		i++;
		pos_iter++;
	}
	
	return vector<pair<unsigned long, unsigned long> >(pList);
}

OperatorPiorities whatIsPiority(char mathSymbol)
{
	enum OperatorPiorities op;
	if (mathSymbol == '+') op = ADDITION;
	else if (mathSymbol == '-') op = SUBTRACTION;
	else if (mathSymbol == 'x') op = MULTIPLICATION;
	else if (mathSymbol == '/') op = DIVISION;
	else if (mathSymbol == '%') op = MODULUS;
	else if (mathSymbol == '!') op = FACTORIAL;
	else if (mathSymbol == '^') op = EXPONENTIATION;
	else if (mathSymbol == '(' || mathSymbol == ')') op = PARENTHESES;

	return OperatorPiorities(op);
}

SimpleMathExpression extractOperandsAndOperator(string _mathexpr, pair<unsigned long, unsigned long>& _maxPiority)
{
	SimpleMathExpression SME;
	vector<char>digits;
	long oneFloatPoint = 0;
	if (_maxPiority.second == MULTIPLICATION && _mathexpr[_maxPiority.first] == 'x') SME._operator = "x";
	else if (_maxPiority.second == ADDITION && _mathexpr[_maxPiority.first] == '+') SME._operator = "+";
	else if (_maxPiority.second == SUBTRACTION && _mathexpr[_maxPiority.first] == '-') SME._operator = "-";
	else if (_maxPiority.second == FACTORIAL && _mathexpr[_maxPiority.first] == '!') SME._operator = "!";
	else if (_maxPiority.second == DIVISION && _mathexpr[_maxPiority.first] == '/') SME._operator = "/";
	else if (_maxPiority.second == MODULUS && _mathexpr[_maxPiority.first] == '%') SME._operator = "%";
	else if (_maxPiority.second == EXPONENTIATION && _mathexpr[_maxPiority.first] == '^') SME._operator = "^";
	else SME._operator = "No Operation";

	if (SME._operator == "!")
	{
		//go left only
		//factorial works on positive numbers only
		long i = _maxPiority.first; // operator position
		i--;//go backward
		while (i >= 0 && !isOperation(_mathexpr[i]))
		{
			if(_mathexpr[i] == '.') oneFloatPoint++;
			digits.push_back(_mathexpr[i]);
			i--;
		}

		if(oneFloatPoint > 1) throwInvalid();

		if (i>=0 && _mathexpr[i] == '-')
			digits.push_back('-');

		std::reverse(digits.begin(), digits.end());

		SME.leftOperand = convertDigitsToDouble(digits);
		SME.first_left_operand_pos = i;
		SME.last_right_operand_pos = _maxPiority.first + 1; //after factorial
	}
	else if (SME._operator != "No Operation")
	{
		//go right first

		 long i = _maxPiority.first; // operator position
		long exprlength = _mathexpr.size();
		i++;//step one
		if (_mathexpr[i] == '-') //negative
		{
			digits.push_back(_mathexpr[i]);
			i++; //step one more
		}
		while (i< exprlength && !isOperation(_mathexpr[i]))
		{
			if(_mathexpr[i] == '.') oneFloatPoint++;
			digits.push_back(_mathexpr[i]);
			i++;
		}
		if(oneFloatPoint > 1) throwInvalid();
		//convert to real number
		SME.rightOperand = convertDigitsToDouble(digits);
		SME.last_right_operand_pos = i;

		digits.clear();
		oneFloatPoint = 0;
		//go left second
		i = _maxPiority.first; // operator position
		i--;//go backward
		while (i >= 0 && !isOperation(_mathexpr[i]))
		{
			if(_mathexpr[i] == '.') oneFloatPoint++;
			digits.push_back(_mathexpr[i]);
			i--;
		}
		if ((i == 0 && _mathexpr[i] == '-')
			|| (i > 0 && SME._operator == "^" &&  _mathexpr[i] == '-' && !is_Digit(_mathexpr[i - 1])) )
		{
			digits.push_back('-');
			i--;
		}

		if(oneFloatPoint > 1) throwInvalid();

		std::reverse(digits.begin(), digits.end());

		SME.leftOperand = convertDigitsToDouble(digits);
		SME.first_left_operand_pos = i;
	}
	else
	{
		//No Operation
		//it's handled in the loop of startCalculation() Function for fast run time result :)
	}

	return SME;
}

pair<unsigned long, unsigned long> getMaxPiority(vector<pair<unsigned long, unsigned long> >& pL)
{
	unsigned long sz = pL.size();
	unsigned long max = pL.at(0).second;
	unsigned long _min_pos = pL.at(0).first;//from left to right
	//find max piority 
	for (unsigned long i = 1; i < sz; i++)
		if (pL.at(i).second > max)
		{
			max = pL.at(i).second;
			_min_pos = pL.at(i).first;
		}
	//if the many operators with the sam piority
	//pick the one with min pos(from left to right) even exponent for simplicity

		for (unsigned long i = 1; i < sz; i++)
			if (max == pL.at(i).second && _min_pos > pL.at(i).first)
					_min_pos = pL.at(i).first;

	return pair<unsigned long, unsigned long>(_min_pos,max);
}

string doSimpleMath(SimpleMathExpression & SME)
{
	if (SME._operator == "x") return double2str(SME.leftOperand * SME.rightOperand);
	else if (SME._operator == "+") return double2str(SME.leftOperand + SME.rightOperand);
	else if (SME._operator == "-") return double2str(SME.leftOperand - SME.rightOperand);
	else if (SME._operator == "!") 
	{
		if (long(SME.leftOperand) < 0) throwInvalid();
		return long2str(factorial(long(SME.leftOperand)));
	}
	else if (SME._operator == "/")
	{
		if (SME.rightOperand == 0) throwInvalid();
		return double2str(SME.leftOperand / SME.rightOperand);
	}
	else if (SME._operator == "%") return long2str(long(SME.leftOperand) % long(SME.rightOperand)); 
	else if (SME._operator == "^") return double2str(pow(SME.leftOperand,SME.rightOperand));
	// you should check that both operands are positive for % in isvalidToCalculate

	return "NULL"; //Should be never returned
}

string replaceWithResult(unsigned long f_pos, unsigned long l_pos, string& actual_str, string& replacement_str)
{
	string new_str(actual_str.substr(0, f_pos + 1));

	new_str.append(replacement_str);
	new_str.append(actual_str.substr(l_pos, actual_str.size()));
	
	return new_str;
}

bool isOnlyDigits(string &mathResult)
{
	// only good for good writing floating number 0.5 not .5 or 5. both will consider be true
	//and for 2.5. will consider it bad which is good thing
	bool isNumber = true;
	bool oneFloatPoint = false;
	for (string::const_iterator k = mathResult.begin(); k != mathResult.end(); ++k)
		if (*k == '+' && k == mathResult.begin()) {/*neglect*/ }
		else if (*k == '-' && k == mathResult.begin()) {/*neglect*/ }
		else if ((*k) == '.' && !oneFloatPoint) oneFloatPoint = true;
		else if (!isdigit(*k)) isNumber = false;
		return isNumber;
}

unsigned long factorial(long n)
{
	long fact = 1;
	for (long i = 1; i <= n; i++)
		fact *= i;
	return fact;
}

bool isBalanced(const string & mathExpr)
{
	//for correct parentheses
	stack<char> mystack;
	string::const_iterator ch_iter = mathExpr.begin();
	char ch;

	while (ch_iter != mathExpr.end())
	{
		if (*ch_iter == '(')
			mystack.push('(');
		if (*ch_iter == ')')
		{
			if (mystack.isEmpty()) return false; //closed bracket without an opening one
			else
				mystack.pop(ch);
		}

		ch_iter++;
	}
	if (!mystack.isEmpty()) return false; //open bracket without a closing one
	//here the parentheses format should be good.
	return true;
}

void deleteLastSpaces(string& str)
{
		size_t len = str.size();
		if(str.at(len - 1) != ' ') return;
		else
		{
			str.erase(str.end() - 1);
			deleteLastSpaces(str);
		}
}
