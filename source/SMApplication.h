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

#ifndef __SMAPPLICATION_H
#define __SMAPPLICATION_H

#include "SMAppOptionManager.h"
#include "SMParserDelegate.h"
#include "SMTranslator.h"
#include "SMRepresentation.h"

#include <string>
#include <vector>
#include <map>

typedef std::pair<std::string, std::string> SMOptionItem;

class SMTranslatorFactory
{
private:
    std::string _name;

public:
    SMTranslatorFactory(const std::string& name):_name(name) {}
    virtual void addTranslatorToVector(std::vector<SMTranslator *>& translators) const = 0;
    std::string getName() const;
};


class SMObjectiveCTranslatorFactory: public SMTranslatorFactory
{
public:    
    SMObjectiveCTranslatorFactory():SMTranslatorFactory("objc") {}

private:
    virtual void addTranslatorToVector(std::vector<SMTranslator *>& translators) const;
};


class SMCppTranslatorFactory: public SMTranslatorFactory
{
public:    
    SMCppTranslatorFactory():SMTranslatorFactory("cpp") {}

private:
    virtual void addTranslatorToVector(std::vector<SMTranslator *>& translators) const;
};


class SMJavaTranslatorFactory: public SMTranslatorFactory
{
public:    
    SMJavaTranslatorFactory():SMTranslatorFactory("java") {}

private:
    virtual void addTranslatorToVector(std::vector<SMTranslator *>& translators) const;
};


class SMGVizTranslatorFactory: public SMTranslatorFactory
{
public:    
    SMGVizTranslatorFactory():SMTranslatorFactory("gviz") {}

private:
    virtual void addTranslatorToVector(std::vector<SMTranslator *>& translators) const;
};


class SMSwiftTranslatorFactory: public SMTranslatorFactory
{
public:    
    SMSwiftTranslatorFactory():SMTranslatorFactory("swift") {}

private:
    virtual void addTranslatorToVector(std::vector<SMTranslator *>& translators) const;
};


class SMApplication: public SMParserDelegate
{
private:
    SMAppOptionManager *_optionManager;
    SMRepresentation *_smRepresentation;
    std::vector<SMTranslator *> _translators;
 
    std::string _inputFilename;
    std::map<std::string, SMTranslatorFactory* > _translatorFactories;
    std::map<std::string, std::vector<SMOptionItem>* > _translationOptions;
    std::vector<SMOptionItem>* _optionsContext;
 
private:
    void registerFactories();
    void registerFactory(SMTranslatorFactory * factory);


    void usage();
    void version();

    bool analyzeOptions(); 
    bool analyzeFilename();

    std::vector<SMTranslator *> getTranslatorsForOption(const std::string& option) const;
    void processFile(const std::string& fullFilename);

private:
    virtual void setStateMachineName(const std::string& machineName);
    virtual void setDebugStatus(bool debugStatus);
    virtual void makeState(const std::string& name, bool isContextState = false);
    virtual void makeEvent(const std::string& name, const std::string& targetStateName);
    virtual void makeAction(const std::string& name);
    virtual void makeOption(const std::string& name);
    virtual void makeItem(const std::string& itemName, const std::string& itemValue);
    virtual void notifySyntaxError(const std::string& token, unsigned int lineNumber, unsigned int columnNumber);

public:
    SMApplication();
    ~SMApplication();
    void run(int argc, char **argv);
};

#endif
 
