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

#include "SMRepresentation.h"

#include <iostream>

SMRepresentation::~SMRepresentation()
{
}

bool SMRepresentation::isDebugOn() const
{
    return _debugOn;
}

std::string SMRepresentation::getMachineName() const
{
    return _machineName;
}


std::vector<const SMState *> SMRepresentation::getStates() const
{
    return (std::vector<const SMState *>(_states.begin(), _states.end()));
}

const SMState * SMRepresentation::getInitialState() const
{
    const SMState * result = NULL;
    
    if(_states.size() > 0)
    {
        result = _states[0];
    }

    return result;
}


SMState * SMRepresentation::getState(const std::string& name) const
{
    SMState * result = NULL;
    
    std::vector<SMState *>::const_iterator it = _states.begin();
    
    while(it != _states.end() && !result)
    {
        if((*it)->getName() == name)
        {
            result = *it;
        }
        
        ++it;
    }
    
    return result;
}

void SMRepresentation::setStateMachineName(const std::string& machineName)
{
    _machineName = machineName;
}

void SMRepresentation::setDebugStatus(bool debugStatus)
{
    _debugOn = debugStatus;
}

void SMRepresentation::makeState(const std::string& name, bool isContextState)
{
    if(!name.empty())
    {
		SMState * state = getState(name);
	
		if(state == NULL)
		{
			state = new SMState(name);
			_states.push_back(state);
		}

		if(isContextState)
		{
			_stateContext = state;
		}
    }
}

void SMRepresentation::makeEvent(const std::string& name, const std::string& targetStateName)
{
    if(_stateContext)
    {
        // Create a state that is not the "context" state
        makeState(targetStateName, false);

        // Search for the new created state
        SMState * target = getState(targetStateName);
    
        if(target)
        {
            _eventContext = new SMEvent(name, _stateContext, target);
        }
        else
        {
            _eventContext = new SMEvent(name, _stateContext, _stateContext);
        }
        
        _stateContext->addEvent(_eventContext);
    }
}

void SMRepresentation::makeAction(const std::string& name)
{
    if(_eventContext)
    {
        _eventContext->setAction(name);
    }
}

