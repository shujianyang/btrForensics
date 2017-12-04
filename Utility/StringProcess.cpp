/**
 * \file
 * \author Shujian Yang
 *
 * File containing string processing functions.
 */

#include "StringProcess.h"
#include <iostream>

using namespace std;

namespace btrForensics {

//! Split a string into multiple sub strings by separator
//! 
//! \param input The string to split.
//! \param separator The separator
//!
//! \return Vector conataining splitted strings.
//!
vector<string> strSplit(string input, string separator)
{
    vector<string> subs;
    typedef string::size_type st;
    st begin = 0;
    st end = input.size();
    while(begin < end) {
        st index = input.find(separator, begin);
        if(index == string::npos){
            subs.push_back(input.substr(begin));
            break;
        }
        string s(input, begin, index - begin);
        if(!s.empty())
            subs.push_back(s);
        begin = index + 1;
    }

    return subs;
}

//! Strip spaces at the two ends of a string.
//! 
//! \param input The string to split.
//!
//! \return Vector conataining splitted strings.
//! 
string strStrip(string input)
{
    auto begin = input.find_first_not_of(' ');
    auto end = input.find_last_not_of(' ');
    if(begin != string::npos && end != string::npos && begin <= end)
        return input.substr(begin, end - begin + 1);
    else
        return "";
}

}

