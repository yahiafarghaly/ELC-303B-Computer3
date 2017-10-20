/*
 * reciever.h
 *
 *  Created on: Oct 20, 2017
 *      Author: yahia
 */

#ifndef RECIEVER_H_
#define RECIEVER_H_

#include <iostream>
#include <algorithm>
#include <sstream>
#include <math.h>
#include <vector>
#include "queue.h"

typedef struct windows_struct
{
	unsigned int max_seq_number;
	unsigned window_start_value;
	unsigned window_end_value;
}window;

class RecieverResult
{
public:
    // Recieved
    std::vector<int> R; 
    // Expected
    int E;
    // Waiting
    std::vector<int> W;
    // Dropped
    std::vector<int> D;
};

bool is_next_number_within_window_range(const int& number, const window& w)
{
    int start = w.window_start_value;
    int end = w.window_end_value;

    if (number >= start && number <= end)
        return true;
    if (end < start)
    {
        /*Overlapping conditions*/
        if (number >= start && number >= end)
            return true;
        if (number <= start && number <= end)
            return true;
    }
    else
        return false;
    return false;
}

inline bool is_expected_package(const int& number, const int& E)
{
    return(number == E);
}

void move_window(window& w)
{
    int new_start,new_end;

    new_start = w.window_start_value + 1;
    new_end = w.window_end_value + 1;

    if(new_start > w.max_seq_number)
        new_start = 0;
    if(new_end > w.max_seq_number)
        new_end = 0;

    w.window_start_value = new_start;  
    w.window_end_value = new_end;  
}

void init_window(int initSeq,int maxSeqNumBits,window& w)
{
    w.max_seq_number = pow(2,maxSeqNumBits) - 1;
    w.window_start_value = initSeq;
    w.window_end_value = initSeq + maxSeqNumBits;
    if(w.window_end_value > w.max_seq_number)
        w.window_end_value = w.window_end_value % (w.max_seq_number + 1);
}

void print_output(RecieverResult &RR);

void process(std::vector<int>& inputs)
{
    window w;
    RecieverResult RR;
    int current_package;
    
    init_window(inputs.at(1),inputs.at(0),w);
    RR.E = w.window_start_value;

    for(int i = 2; i < inputs.size();++i)
    {
        current_package = inputs.at(i);
        RR.E = w.window_start_value;
        if(is_next_number_within_window_range(current_package,w))
        {
            if(is_expected_package(current_package,RR.E))
            {
                RR.R.push_back(current_package);
                move_window(w);
                RR.E = w.window_start_value;

                // search if expected exist in waiting.
                if(std::find(RR.W.begin(),RR.W.end(),RR.E) != std::end(RR.W))
                {
                    // found expected package
                    RR.R.push_back(RR.E);
                    move_window(w);
                    // remove it from the list
                    RR.W.erase(std::remove(std::begin(RR.W),std::end(RR.W),RR.E),RR.W.end());
                    // update the list
                    RR.E = w.window_start_value;;
                }
            }
            else
            {
                // search if it was inserted before in the waiting list
                if (std::find(RR.W.begin(), RR.W.end(), current_package) != std::end(RR.W))
                    RR.D.push_back(current_package);
                else
                    RR.W.push_back(current_package);
            }
        }
        else
        {
            RR.D.push_back(current_package);
        }

    }
    print_output(RR);
}



/*helpful functions*/

void print_output(RecieverResult &RR)
{
    //Input Example: 4 3 3 5 4 9 7 7 4 6 10 15 16
    //Output Example: R 3 4 5 6 7 E 8 W 9 10 15 D 7 4 16
    auto print = [](const int& n) { std::cout << " " << n; };
    std::cout << "R";
    std::for_each(RR.R.begin(),RR.R.end(),print);
    std::cout << " ";
    std::cout << "E " << RR.E;
    std::cout << " ";
    std::cout << "W";
    std::for_each(RR.W.begin(),RR.W.end(),print);
    std::cout << " ";

    std::cout << "D";
    std::for_each(RR.D.begin(),RR.D.end(),print);

    std::cout << "\n";
}

inline int string2int(char *str)
{
    int x;
    std::stringstream cont(str);
    cont >> x;
    return x;
}

#endif /* RECIEVER_H_ */
