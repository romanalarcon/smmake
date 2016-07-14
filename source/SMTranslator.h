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

#ifndef __SMTRANSLATOR_H
#define __SMTRANSLATOR_H

#include "SMRepresentation.h"

#include <string>
#include <vector>
#include <set>

typedef std::pair<std::string, std::string> SMOptionItem;

class SMTranslator
{
private:
    std::string _name;
    std::set<std::string> _statesIdentifiers;
    std::set<std::string> _eventsIdentifiers;
    std::set<std::string> _actionsIdentifiers;
    std::vector<SMOptionItem> _translationOptions;

private:
    void handleOptions(const std::vector<SMOptionItem>& translationOptions);

protected:
    std::string _outputDirectory; 
    const SMRepresentation* _smRepresentation;

protected:
    std::vector<std::string> getAllEventsIdentifiers() const;
    std::vector<std::string> getAllStatesIdentifiers() const;
    std::vector<std::string> getAllActionsIdentifiers() const;

    void setOutputDirectory(const std::string& outputDirectory);
    std::string getOutputDirectory() const;
    void generateFile(const std::string& fileName, const std::string& content) const;

    virtual bool handleOption(const std::string& itemName, const std::string& itemValue);
    virtual void continueTranslation() = 0;

public:
	SMTranslator(const std::string& name):_name(name), _outputDirectory("."){}
	std::string getName() const;

    void translate(const SMRepresentation* smRepresentation, const std::vector<SMOptionItem>& translationOptions);
    
};

#endif

