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

#ifndef __SMREPRESENTATION_H
#define __SMREPRESENTATION_H

#include <string>
#include <vector>
#include <map>

#include "SMState.h"

class SMRepresentation
{
private:
    bool _debugOn;
    std::string _machineName;
    std::vector<SMState *> _states;
    SMState * _stateContext;
    SMEvent * _eventContext;

private:
    SMState * getState(const std::string& name) const;

public:
    SMRepresentation(): _stateContext(NULL), _eventContext(NULL), _debugOn(false), _machineName("Machine") {}
    ~SMRepresentation();

    bool isDebugOn() const;
    std::string getMachineName() const;
    
    std::vector<const SMState *> getStates() const;
    const SMState * getInitialState() const;
    
    void setStateMachineName(const std::string& machineName);
    void setDebugStatus(bool debugStatus);
    void makeState(const std::string& name, bool isContextState = false);
    void makeEvent(const std::string& name, const std::string& targetStateName);
    void makeAction(const std::string& name);
};

#endif
