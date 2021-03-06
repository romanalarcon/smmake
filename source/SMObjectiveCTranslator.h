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

#ifndef __SMOBJECTIVECTRANSLATOR_H
#define __SMOBJECTIVECTRANSLATOR_H

#include "SMTranslator.h"

#include <string>

class SMObjectiveCTranslator: public SMTranslator
{
private:
    void createBaseStateHeaderFile() const;
    void createBaseStateImplementationFile() const;

    std::string bodyForEvent(const SMEvent* event) const;
    std::string importFilesForState(const SMState* state) const;
    void createStateHeaderFile(const SMState * state) const;
    void createStateImplementationFile(const SMState * state) const;

    void createActionsProtocolFile() const;

    void createConfiguratorHeaderFile() const;
    void createConfiguratorImplementationFile() const;

private:
    virtual bool handleOption(const std::string& itemName, const std::string& itemValue);
    virtual void continueTranslation();

public:
    SMObjectiveCTranslator():SMTranslator("objc") {}
};

#endif
