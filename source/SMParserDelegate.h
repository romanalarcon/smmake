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

#ifndef __SMPARSERDELEGATE_H
#define __SMPARSERDELEGATE_H

#include <string>

class SMParserDelegate
{
public:
    virtual void setStateMachineName(const std::string& machineName) = 0;
    virtual void setDebugStatus(bool debugStatus) = 0;
    virtual void makeState(const std::string& name, bool isContextState = false) = 0;
    virtual void makeEvent(const std::string& name, const std::string& targetStateName) = 0;
    virtual void makeAction(const std::string& name) = 0;
    virtual void makeOption(const std::string& name) = 0;
    virtual void makeItem(const std::string& itemName, const std::string& itemValue) = 0;
    virtual void notifySyntaxError(const std::string& token, unsigned int lineNumber, unsigned int columnNumber) = 0;
};

#endif
