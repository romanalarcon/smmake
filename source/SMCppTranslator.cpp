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

#include "SMCppTranslator.h"
#include "SMUtilities.h"
#include "SMCppSkeletons.h"

#include <set>
#include <vector>


void SMCppTranslator::createBaseStateHeaderFile() const
{
    std::string baseStateHeaderCode = SMCppSkeletons::baseStateDeclarationSkeleton(_smRepresentation->getMachineName());

    std::string eventsCode = ""; 
    std::vector<std::string> eventsIdentifiers = getAllEventsIdentifiers();
    
    for(std::vector<std::string>::const_iterator it = eventsIdentifiers.begin(); it != eventsIdentifiers.end(); ++it)
    {
         eventsCode += "    virtual void " + *it + "(const Parameters& parameters = Parameters());\n";
	}
	
    SMUtilities::replace(baseStateHeaderCode, "[EVENTS]", eventsCode); 
    SMUtilities::replace(baseStateHeaderCode, "[NAMESPACE]", _namespace); 

    generateFile(_smRepresentation->getMachineName() + "BaseState.h", baseStateHeaderCode);
} 

void SMCppTranslator::createBaseStateImplementationFile() const
{
    std::string baseStateImplementationCode = SMCppSkeletons::baseStateImplementationSkeleton(_smRepresentation->getMachineName());
    
    std::set<std::string> eventsSignatures;
    std::string eventsCode = "";
 
    std::vector<std::string> eventsIdentifiers = getAllEventsIdentifiers();
    
    for(std::vector<std::string>::const_iterator it = eventsIdentifiers.begin(); it != eventsIdentifiers.end(); ++it)
    {
         eventsCode += "void " + _smRepresentation->getMachineName() + "BaseState::" + *it + "(const Parameters& parameters)\n";
         eventsCode += "{\n"
		               "    std::cerr << \"Event '" + *it + "' not implemented in state '\" << getStateName() << \"'.\" <<  std::endl;\n"
				       "}\n\n";
	}

    SMUtilities::replace(baseStateImplementationCode, "[EVENTS]", eventsCode);
    SMUtilities::replace(baseStateImplementationCode, "[NAMESPACE]", _namespace); 

    generateFile(_smRepresentation->getMachineName() + "BaseState.cpp", baseStateImplementationCode);
} 


void SMCppTranslator::createStateHeaderFile(const SMState * state) const
{
    std::string stateHeaderCode = SMCppSkeletons::stateHeaderSkeleton(_smRepresentation->getMachineName());
    std::string eventsCode = "";

    std::vector<const SMEvent *> events = state->getEvents();
    for(std::vector<const SMEvent *>::const_iterator it = events.begin(); it != events.end(); ++it)
    {
         eventsCode += "    virtual void " + (*it)->getName() + "(const Parameters& parameters);\n";
	}
    
    SMUtilities::replace(stateHeaderCode, "[STATE_NAME]", state->getName());
    SMUtilities::replace(stateHeaderCode, "[EVENTS]", eventsCode);
    SMUtilities::replace(stateHeaderCode, "[NAMESPACE]", _namespace); 

    generateFile(_smRepresentation->getMachineName() + state->getName() + ".h", stateHeaderCode);
}


std::string SMCppTranslator::bodyForEvent(const SMEvent* event) const
{
    std::string eventBody = "{\n";
    
    if(event->getTarget() == NULL || event->getTarget()->getName() == event->getSource()->getName())
    {
        eventBody +=
        "    //\n"
        "    // Stay in current state\n"
        "    //\n";
    }
    else
    {
        eventBody +=
        "    changeState(" + _smRepresentation->getMachineName() + event->getTarget()->getName() + "::sharedInstance());\n";
    }

    std::string action = event->getAction();
     
    if(!action.empty())
    {
        eventBody +=
        "    _delegate->" + action + "(parameters);\n";
    }
    else
    {
        eventBody +=
        "    //\n"
        "    // Don't run any action\n"
        "    //\n";
    }

    return eventBody + "}\n\n";
}

std::string SMCppTranslator::importFilesForState(const SMState* state) const
{
    std::string result = "";
    std::set<std::string> importsSet;
    importsSet.insert(state->getName());
    
    std::vector<const SMEvent *> events = state->getEvents();

    for(std::vector<const SMEvent *>::const_iterator it = events.begin(); it != events.end(); ++it)
    {
        const SMState * target = (*it)->getTarget(); 
        
        if(target)
        {
            importsSet.insert(target->getName());
        }
    }

    for(std::set<std::string>::const_iterator it = importsSet.begin(); it != importsSet.end(); ++it)
    {
        result += std::string(((result.empty()) ? "" : "\n")) + "#include \"" + _smRepresentation->getMachineName() + *it + ".h\"";
    }
    
    return result;
}

void SMCppTranslator::createStateImplementationFile(const SMState * state) const
{
    std::string stateImplementationCode = SMCppSkeletons::stateImplementationSkeleton(_smRepresentation->getMachineName());

    SMUtilities::replace(stateImplementationCode, "[STATE_NAME]", state->getName());
    
    std::vector<const SMEvent *> events = state->getEvents();
    std::string eventsCode = "";
    
    for(std::vector<const SMEvent *>::const_iterator it = events.begin(); it != events.end(); ++it)
    {
        const SMEvent * event = *it;
        eventsCode += "void " + _smRepresentation->getMachineName() + state->getName() + "::" + event->getName() + "(const Parameters& parameters)\n" + 
                                  bodyForEvent(event);
    }

    SMUtilities::replace(stateImplementationCode, "[EVENTS]", eventsCode);
    SMUtilities::replace(stateImplementationCode, "[INCLUDES]", importFilesForState(state));
    SMUtilities::replace(stateImplementationCode, "[NAMESPACE]", _namespace); 

    generateFile(_smRepresentation->getMachineName() + state->getName() + ".cpp", stateImplementationCode);
}


void SMCppTranslator::createActionsProtocolFile() const
{
    std::string actionsProtocolCode = SMCppSkeletons::generateDelegateSkeleton(_smRepresentation->getMachineName());

    std::string actionsCode = ""; 
    std::vector<std::string> actionsIdentifiers = getAllActionsIdentifiers();
    
    for(std::vector<std::string>::const_iterator it = actionsIdentifiers.begin(); it != actionsIdentifiers.end(); ++it)
    {
         actionsCode += "    virtual void " + *it + "(const Parameters& parameters) { /* Method not implemented */ }\n";
	}
	
    SMUtilities::replace(actionsProtocolCode, "[ACTIONS]", actionsCode); 
    SMUtilities::replace(actionsProtocolCode, "[NAMESPACE]", _namespace); 

    generateFile(_smRepresentation->getMachineName() + "StateDelegate.h", actionsProtocolCode);

}

void SMCppTranslator::createConfiguratorHeaderFile() const
{
    std::string configuratorCode = SMCppSkeletons::generateConfiguratorHeaderSkeleton(_smRepresentation->getMachineName());

    SMUtilities::replace(configuratorCode, "[NAMESPACE]", _namespace); 

    generateFile(_smRepresentation->getMachineName() + "Configurator.h", configuratorCode);
}

void SMCppTranslator::createConfiguratorImplementationFile() const
{
    std::string configuratorCode = SMCppSkeletons::generateConfiguratorImplementationSkeleton(_smRepresentation->getMachineName());

    std::string importsCode = ""; 
    std::string associationCode = ""; 
    std::vector<std::string> statesIdentifiers = getAllStatesIdentifiers();
    
    for(std::vector<std::string>::const_iterator it = statesIdentifiers.begin(); it != statesIdentifiers.end(); ++it)
    {
         importsCode += "#include \"" + _smRepresentation->getMachineName() + *it + ".h\"\n";
         associationCode += "    " + _smRepresentation->getMachineName() + *it + "::sharedInstance()->setDelegate(delegate);\n";
	}
	
    SMUtilities::replace(configuratorCode, "[INCLUDES]", importsCode); 
    SMUtilities::replace(configuratorCode, "[ASSOCIATIONS]", associationCode); 
    SMUtilities::replace(configuratorCode, "[NAMESPACE]", _namespace); 

    generateFile(_smRepresentation->getMachineName() + "Configurator.cpp", configuratorCode);
} 

bool SMCppTranslator::handleOption(const std::string& itemName, const std::string& itemValue)
{
    bool result = true;

    if(itemName == "namespace")
    {
        _namespace = itemValue;
    }
    else
    {
        result = false;
    }

    return result;
}

void SMCppTranslator::continueTranslation()
{
    if(_namespace.empty())
    {
        _namespace = _smRepresentation->getMachineName() + "Namespace";
    }

    createBaseStateHeaderFile(); 
    createBaseStateImplementationFile(); 
    
    std::vector<const SMState*> states = _smRepresentation->getStates();

    for(std::vector<const SMState*>::const_iterator it = states.begin(); it != states.end(); ++it)
    {
        createStateHeaderFile(*it);
        createStateImplementationFile(*it);
    }

    createActionsProtocolFile(); 

    createConfiguratorHeaderFile(); 
    createConfiguratorImplementationFile(); 
}
