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

#include "SMTranslator.h"

#include <fstream>

void SMTranslator::setOutputDirectory(const std::string& outputDirectory)
{
    _outputDirectory = outputDirectory;
}

std::string SMTranslator::getOutputDirectory() const
{
    return _outputDirectory;
}


void SMTranslator::generateFile(const std::string& fileName, const std::string& content) const
{
    std::ofstream codeFile((_outputDirectory + "/" + fileName).c_str());
    codeFile << content;
    codeFile.close();
}

void SMTranslator::translate(const SMRepresentation* smRepresentation, const std::vector<SMOptionItem>& translationOptions)
{
    _smRepresentation = smRepresentation;
    _translationOptions = translationOptions;

    handleOptions(translationOptions);
    
    std::vector<const SMState*> states = smRepresentation->getStates();
    for(std::vector<const SMState*>::const_iterator statesIt = states.begin(); statesIt != states.end(); ++statesIt)
    {
        const SMState * state = *statesIt;
        
        _statesIdentifiers.insert(state->getName());

		std::vector<const SMEvent*> events = state->getEvents();
		for(std::vector<const SMEvent*>::const_iterator eventsIt = events.begin(); eventsIt != events.end(); ++eventsIt)
		{
			const SMEvent * event = *eventsIt;
			
			_eventsIdentifiers.insert(event->getName());
            
            std::string action = event->getAction();
            
            if(!action.empty())
            {
                _actionsIdentifiers.insert(action);
            }
        }
    }

    continueTranslation();
}

void SMTranslator::handleOptions(const std::vector<SMOptionItem>& translationOptions)
{
    for(std::vector<SMOptionItem>::const_iterator it = translationOptions.begin(); it != translationOptions.end(); ++it)
    {
        SMOptionItem item = *it;

        if(!handleOption(item.first, item.second))
        {
            printf("Ignored option '%s'\n", item.first.c_str());
        }
    } 
}

bool SMTranslator::handleOption(const std::string& itemName, const std::string& itemValue)
{
    return false; 
}


std::vector<std::string> SMTranslator::getAllStatesIdentifiers() const
{
    return std::vector<std::string>(_statesIdentifiers.begin(), _statesIdentifiers.end());
}

std::vector<std::string> SMTranslator::getAllEventsIdentifiers() const
{
    return std::vector<std::string>(_eventsIdentifiers.begin(), _eventsIdentifiers.end());
}

std::vector<std::string> SMTranslator::getAllActionsIdentifiers() const
{
    return std::vector<std::string>(_actionsIdentifiers.begin(), _actionsIdentifiers.end());
}

std::string SMTranslator::getName() const
{
    return _name;
}

