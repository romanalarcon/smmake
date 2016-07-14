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

#include "SMJavaTranslator.h"
#include "SMUtilities.h"
#include "SMJavaSkeletons.h"

#include <set>

void SMJavaTranslator::createBaseStateImplementationFile() const
{
    std::string baseStateImplementationCode = SMJavaSkeletons::baseStateImplementationSkeleton(_smRepresentation->getMachineName());
    
    std::set<std::string> eventsSignatures;
    std::string eventsCode = "";
 
    std::vector<std::string> eventsIdentifiers = getAllEventsIdentifiers();
    
    for(std::vector<std::string>::const_iterator it = eventsIdentifiers.begin(); it != eventsIdentifiers.end(); ++it)
    {
         eventsCode += "   public void " + *it + "(final HashMap<Object,Object> parameters)\n";
         eventsCode += "   {\n"
		               "      System.out.println(\"Event '" + *it + "' not implemented in state '\" + getStateName() + \"'.\");\n"
				       "   }\n\n";
	}

    SMUtilities::replace(baseStateImplementationCode, "[PACKAGE]", _package);
    SMUtilities::replace(baseStateImplementationCode, "[EVENTS]", eventsCode);

    generateFile(_smRepresentation->getMachineName() + "BaseState.java", baseStateImplementationCode);
} 

std::string SMJavaTranslator::bodyForEvent(const SMEvent* event) const
{
    std::string eventBody = "    {\n";
    
    if(event->getTarget() == NULL || event->getTarget()->getName() == event->getSource()->getName())
    {
        eventBody +=
        "       //\n"
        "       // Stay in current state\n"
        "       //\n";
    }
    else
    {
        eventBody +=
        "       changeState(" + _smRepresentation->getMachineName() + event->getTarget()->getName() + ".sharedInstance());\n";
    }

    std::string action = event->getAction();
     
    if(!action.empty())
    {
        eventBody +=
        "       delegate." + action + "(parameters);\n";
    }
    else
    {
        eventBody +=
        "       //\n"
        "       // Don't run any action\n"
        "       //\n";
    }

    return eventBody + "    }\n\n";
}

void SMJavaTranslator::createStateImplementationFile(const SMState * state) const
{
    std::string stateImplementationCode = SMJavaSkeletons::stateImplementationSkeleton(_smRepresentation->getMachineName());

    SMUtilities::replace(stateImplementationCode, "[STATE_NAME]", state->getName());
    
    std::vector<const SMEvent *> events = state->getEvents();
    std::string eventsCode = "";
    
    for(std::vector<const SMEvent *>::const_iterator it = events.begin(); it != events.end(); ++it)
    {
        const SMEvent * event = *it;
        eventsCode += "    @Override\n";
        eventsCode += "    public void " + event->getName() + "(final HashMap<Object,Object> parameters)\n" + 
                                  bodyForEvent(event);
    }

    SMUtilities::replace(stateImplementationCode, "[PACKAGE]", _package);
    SMUtilities::replace(stateImplementationCode, "[EVENTS]", eventsCode);

    generateFile(_smRepresentation->getMachineName() + state->getName() + ".java", stateImplementationCode);
}


void SMJavaTranslator::createActionsProtocolFile() const
{
    std::string actionsProtocolCode = SMJavaSkeletons::generateDelegateSkeleton(_smRepresentation->getMachineName());

    std::string actionsCode = ""; 
    std::vector<std::string> actionsIdentifiers = getAllActionsIdentifiers();
    
    for(std::vector<std::string>::const_iterator it = actionsIdentifiers.begin(); it != actionsIdentifiers.end(); ++it)
    {
         actionsCode += "    void " + *it + "(final HashMap<Object,Object> parameters);\n";
    }
	
    SMUtilities::replace(actionsProtocolCode, "[PACKAGE]", _package);
    SMUtilities::replace(actionsProtocolCode, "[ACTIONS]", actionsCode); 

    generateFile(_smRepresentation->getMachineName() + "StateDelegate.java", actionsProtocolCode);

}

void SMJavaTranslator::createConfiguratorImplementationFile() const
{
    std::string configuratorCode = SMJavaSkeletons::generateConfiguratorImplementationSkeleton(_smRepresentation->getMachineName());

    std::string associationCode = ""; 
    std::vector<std::string> statesIdentifiers = getAllStatesIdentifiers();
    
    for(std::vector<std::string>::const_iterator it = statesIdentifiers.begin(); it != statesIdentifiers.end(); ++it)
    {
         associationCode += "      " + _smRepresentation->getMachineName() + *it + ".sharedInstance().setDelegate(delegate);\n";
	}
	
    SMUtilities::replace(configuratorCode, "[PACKAGE]", _package);
    SMUtilities::replace(configuratorCode, "[ASSOCIATIONS]", associationCode); 

    generateFile(_smRepresentation->getMachineName() + "Configurator.java", configuratorCode);
} 

bool SMJavaTranslator::handleOption(const std::string& itemName, const std::string& itemValue)
{
    bool result = true;
    
    if (itemName == "package")
    {
        _package = "package " + itemValue + ";";
    }
    else
    {
        result = false;
    }

    return result;
}

void SMJavaTranslator::continueTranslation()
{
    createBaseStateImplementationFile(); 
    
    std::vector<const SMState*> states = _smRepresentation->getStates();

    for(std::vector<const SMState*>::const_iterator it = states.begin(); it != states.end(); ++it)
    {
        createStateImplementationFile(*it);
    }

    createActionsProtocolFile(); 

    createConfiguratorImplementationFile(); 
}
