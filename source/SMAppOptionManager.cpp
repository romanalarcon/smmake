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

#include "SMAppOptionManager.h"

#include <cstring>
#include <iostream>
#include <unistd.h>

/**
 *
 */
SMAppOptionManager::~SMAppOptionManager()
{
    for(std::map<char, const SMAppOption*>::const_iterator it = _options.begin(); it != _options.end(); ++it)
    {
        delete it->second;
    }
}


/**
 *
 */
bool SMAppOptionManager::isValidOption(char option, const char * options)
{
    bool result = false;
    
    unsigned int index = 0;
    while(index < std::strlen(options) && !result)
    {
        if(options[index] != ':' && options[index] == option)
        {
            result = true;
        }
        
        ++index;
    }

    return result;
}

/**
 *
 */
bool SMAppOptionManager::processOptions(int argc, char **argv)
{
    const char * options = _optionList.c_str();
    int optionCharacter;
    opterr = 0;
    bool status = true;
    
    while(status && (optionCharacter = getopt (argc, argv, options)) != -1)
    {
        if(optionCharacter == '?')
        {
            if(isValidOption(optopt, options))
            {
                if(optarg == NULL)
                {
                    std::cerr << "Option '-" << char(optopt) << "' requires an argument." << std::endl;
                    status = false;
                }
            }
            else
            {
                std::cerr << "Unknown option '-" << char(optopt) << "'." << std::endl;
                status = false;
            }
        }
        else
        {
            if(valueForOption(optionCharacter) == NULL)
            {
                _options[optionCharacter] = new SMAppOption(optionCharacter, optarg ? std::string(optarg) : "");
            } 
        }
    }   

    if(status)   
    { 
        for (int index = optind; index < argc; index++)
        {
            _nonOptionsArgs.push_back(std::string(argv[index]));
        }
    }    
    
    return status;
}

/**
 *
 */
const SMAppOption * SMAppOptionManager::valueForOption(char optionCode) const 
{
    const SMAppOption *option = NULL;
    
    std::map<char, const SMAppOption*>::const_iterator it = _options.find(optionCode);
    
    if(it != _options.end())
    {
        option = it->second;        
    } 
    
    return option;
}

/**
 *
 */
std::vector<std::string> SMAppOptionManager::getNonOptionsArgs() const 
{ 
    return _nonOptionsArgs; 
}
