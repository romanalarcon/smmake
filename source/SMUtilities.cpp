/*
The MIT License (MIT)

Copyright (c) 2016 Roman Alarcon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "SMUtilities.h"

#include <sstream>

/**
 *
 */
void SMUtilities::replace(std::string& str, const std::string& from, const std::string& to) 
{
    std::string editableString = str;
    std::string resultingString = "";
    
    size_t start_pos;
   
    while(editableString != "")
    {
        size_t start_pos = editableString.find(from);
        if(start_pos != std::string::npos)
        {
            resultingString += editableString.substr(0, start_pos);
            editableString.erase(0, start_pos + from.length());
            resultingString += to; 

        }
        else
        {
            resultingString += editableString;
            editableString = "";
        }
    }

    str = resultingString;    
}

/**
 *
 */
std::string SMUtilities::removeQuotes(const std::string& str) 
{
    std::string result = str;
    
    if(!result.empty() && result[0] == '\"')
    {
        result = result.erase(0, 1);
    }

    if(!result.empty() && result[result.size() - 1] == '\"')
    {
        result = result.erase(result.size() - 1, 1);
    }
   
    return result;
}


/**
 *
 */
std::string SMUtilities::capitalizeFirstChar(const std::string& inputString)
{
    std::string result = inputString;
    
    if(!result.empty())
    {
        if(result[0] >= 'a' && result[0] <= 'z')
        {
            result[0] = char('A' + result[0] - 'a');
        }
    }
    
    return result;
}

/**
 *
 */
std::string SMUtilities::toupper(const std::string& identifier)
{
    std::string uppercaseStr = identifier;
    
    for(std::string::iterator it = uppercaseStr.begin(); it != uppercaseStr.end(); ++it)
    {
        *it = ::toupper(*it);
    }

    return uppercaseStr;
}


/**
 *
 */
std::string SMUtilities::intToString(int value)
{
    std::string indexStr;
    std::stringstream strstr;
    strstr << value;
    strstr >> indexStr;

    return indexStr;
}

/**
 *
 */
std::string SMUtilities::makeLine(char character, unsigned int length)
{
    std::string line = "";
    
    for(unsigned int index = 0; index < length; ++index)
    {
        line += character;
    } 
    
    return line;
}
