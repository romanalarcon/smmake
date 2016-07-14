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

#include "SMApplication.h"
#include "SMParser.h"
#include "SMRepresentation.h"
#include "SMGVizTranslator.h"
#include "SMObjectiveCTranslator.h"
#include "SMCppTranslator.h"
#include "SMJavaTranslator.h"
#include "SMUtilities.h"
#include "SMSwiftTranslator.h"

#include <iostream>

std::string SMTranslatorFactory::getName() const
{
    return _name;
}

void SMObjectiveCTranslatorFactory::addTranslatorToVector(std::vector<SMTranslator *>& translators) const
{
    translators.push_back(new SMObjectiveCTranslator());
    translators.push_back(new SMGVizTranslator());
}

void SMGVizTranslatorFactory::addTranslatorToVector(std::vector<SMTranslator *>& translators) const
{
    translators.push_back(new SMGVizTranslator());
}

void SMCppTranslatorFactory::addTranslatorToVector(std::vector<SMTranslator *>& translators) const
{
    translators.push_back(new SMCppTranslator());
    translators.push_back(new SMGVizTranslator());
}

void SMJavaTranslatorFactory::addTranslatorToVector(std::vector<SMTranslator *>& translators) const
{
    translators.push_back(new SMJavaTranslator());
    translators.push_back(new SMGVizTranslator());
}

void SMSwiftTranslatorFactory::addTranslatorToVector(std::vector<SMTranslator *>& translators) const
{
    translators.push_back(new SMSwiftTranslator());
    translators.push_back(new SMGVizTranslator());
}

/**
 *
 */
SMApplication::SMApplication()
{
    _optionManager = new SMAppOptionManager("ho:");
    _smRepresentation = new SMRepresentation;

    registerFactories(); 
}


/**
 *
 */
SMApplication::~SMApplication()
{
    delete _optionManager;
}

/**
 *
 */
void SMApplication::registerFactories()
{
    registerFactory(new SMObjectiveCTranslatorFactory()); 
    registerFactory(new SMGVizTranslatorFactory()); 
    registerFactory(new SMCppTranslatorFactory()); 
    registerFactory(new SMJavaTranslatorFactory()); 
    registerFactory(new SMSwiftTranslatorFactory()); 
}

/**
 *
 */
void SMApplication::registerFactory(SMTranslatorFactory * factory) 
{
    _translatorFactories[factory->getName()] = factory;
}

/**
 *
 */
void SMApplication::version()
{
    std::cout << "smmake version 1.0" << std::endl;
}

/**
 *
 */
void SMApplication::usage()
{
    std::string translationOptions = "";

    for(std::map<std::string, SMTranslatorFactory* >::const_iterator it = _translatorFactories.begin(); it != _translatorFactories.end(); ++it)
    {
        if(!translationOptions.empty())
        {
            translationOptions += " | ";    
        } 

        translationOptions += it->second->getName();
    }

    if(!translationOptions.empty())
    {
        translationOptions = "| [ -o <" + translationOptions + ">]"; 
    }

    std::cerr << "usage: smmake [[-h] " << translationOptions << " <filename>]" << std::endl;    
}

/**
 *
 */
std::vector<SMTranslator *> SMApplication::getTranslatorsForOption(const std::string& option) const
{
    std::vector<SMTranslator *> translators;
	
    std::map<std::string, SMTranslatorFactory* >::const_iterator it = _translatorFactories.find(option);

    if(it != _translatorFactories.end())
    {
        SMTranslatorFactory * translatorFactory= it->second;
        translatorFactory->addTranslatorToVector(translators);
    }
	
    return translators;
}


/**
 *
 */
bool SMApplication::analyzeOptions()
{
    bool status = false;
    
    if(_optionManager->valueForOption('h') != NULL)
    {
        usage();
    }
    else
    {
        const SMAppOption * outputLanguage = _optionManager->valueForOption('o');

        if(outputLanguage)
        {
            _translators = getTranslatorsForOption(outputLanguage->value);
            
            if(_translators.size() > 0)
            {
                status = true;
            }
            else
            {
                std::cerr << "Error: Unknown output language ('" << outputLanguage->value << "')." << std::endl;
            }
        }
        else
        {
            // Default translator
            _translators = getTranslatorsForOption("objc");
            std::cerr << "Objective C assumed as default output language." << std::endl;
            status = true;
        }
    } 
    
    return status;
} 


/**
 *
 */
bool SMApplication::analyzeFilename()
{
    bool status = false;

    std::vector<std::string> nonOptionsArgs = _optionManager->getNonOptionsArgs();

    if(nonOptionsArgs.size() > 0)
    {
        _inputFilename = nonOptionsArgs[0];
	    status = true;
    }
    else
    {
        std::cerr << "Error: Input file required." << std::endl;
    }

    return status;
}


/**
 *
 */
void SMApplication::run(int argc, char **argv)
{
    version();
    
    if(!_optionManager->processOptions(argc, argv))
    {
        usage();
    }
    else 
    {
        if(analyzeOptions())
        {
            if(analyzeFilename())
            {
                processFile(_inputFilename);
            }
        }
    }
}

/**
 *
 */
void SMApplication::processFile(const std::string& fullFilename)
{
    SMParser parser = SMParser(this);

    int result = parser.setInputFile(fullFilename);
            
    if(result == 0)
    {
        std::cout << "Parsing..." << std::endl;
        result = parser.parse(); 

        if(result == 0)
        {
             std::cout << "Generating code..." << std::endl;
             
             for(std::vector<SMTranslator*>::iterator it = _translators.begin(); it != _translators.end(); ++it)
             {
                 SMTranslator * translator = *it;
                 std::vector<SMOptionItem>* translationOptions = _translationOptions[translator->getName()];

                 if(!translationOptions)
                 {
                    translationOptions = new std::vector<SMOptionItem>;
                 }

                 translator->translate(_smRepresentation, *translationOptions);
             }

            std::cout << "Done." << std::endl;
        }
    }
    else
    {
        std::cerr << "Error: File '" << fullFilename << "' not found." << std::endl;    
    }
}


void SMApplication::setStateMachineName(const std::string& machineName)
{
    if(_smRepresentation)
    {
        _smRepresentation->setStateMachineName(machineName);
    }
}

void SMApplication::setDebugStatus(bool debugStatus)
{
    if(_smRepresentation)
    {
        _smRepresentation->setDebugStatus(debugStatus);
    }
}

void SMApplication::makeState(const std::string& name, bool isContextState)
{
    if(_smRepresentation)
    {
        _smRepresentation->makeState(name, isContextState);
    }
}

void SMApplication::makeEvent(const std::string& name, const std::string& targetStateName)
{
    if(_smRepresentation)
    {
        _smRepresentation->makeEvent(name, targetStateName);
    }
}

void SMApplication::makeAction(const std::string& name)
{
    if(_smRepresentation)
    {
        _smRepresentation->makeAction(name);
    }
}

void SMApplication::makeOption(const std::string& name)
{
    std::vector<SMOptionItem>* options = _translationOptions[name];

    if(options == NULL)
    {
        options = new std::vector<SMOptionItem>; 
        _translationOptions[name] = options;
    }
    
    _optionsContext = options;
}

void SMApplication::makeItem(const std::string& itemName, const std::string& itemValue)
{
    if(_optionsContext)
    {
        _optionsContext->push_back(SMOptionItem(itemName, SMUtilities::removeQuotes(itemValue)));
    }
}


void SMApplication::notifySyntaxError(const std::string& token, unsigned int lineNumber, unsigned int columnNumber)
{
    std::cerr << "Syntax error:" << lineNumber << ":" << columnNumber << ": Bad token '" << token << "'" << std::endl;
}

