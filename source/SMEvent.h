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

#ifndef __SMEVENT_H
#define __SMEVENT_H

#include <string>
#include <vector>

class SMState;

class SMEvent
{
private:
    std::string _name;
    std::string _action;

    const SMState* _source;
    const SMState* _target;

public:
    SMEvent(const std::string& name, const SMState* source, const SMState* target): 
             _name(name), 
             _source(source), 
             _target(target), 
             _action("") 
    {
    }

    void setAction(const std::string& action);
    
    std::string getName() const;
    std::string getAction() const;
    
    const SMState * getSource() const;
    const SMState * getTarget() const;
};

#endif
