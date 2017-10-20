/*
 * a4.cpp
 *
 *  Created on: Oct 14, 2017
 *      Author: yahia
 */

#include <iostream>
#include <vector>
#include "reciever.h"


int main(int argc,char **argv)
{
	std::vector<int> inputs;

	// Input: a4 seqNumbits initSeq pid1 pid2
	if(argc > 3)
	{
		for(int i = 1 ; i < argc;i++)
			inputs.push_back(string2int(argv[i]));
		process(inputs);
	}
	else
		std::cout << "Not enough arguments\n";

	return 0;
}


