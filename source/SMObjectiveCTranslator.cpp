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

#include "SMObjectiveCTranslator.h"
#include "SMUtilities.h"
#include "SMObjectiveCSkeletons.h"

#include <set>
#include <cstdlib>

void SMObjectiveCTranslator::createBaseStateHeaderFile() const
{
    std::string baseStateHeaderCode = SMObjectiveCSkeletons::baseStateDeclarationSkeleton(_smRepresentation->getMachineName());

    std::string eventsCode = ""; 
    std::vector<std::string> eventsIdentifiers = getAllEventsIdentifiers();
    
    for(std::vector<std::string>::const_iterator it = eventsIdentifiers.begin(); it != eventsIdentifiers.end(); ++it)
    {
         eventsCode += "-(void)" + *it + ":(NSDictionary *) parameters;\n";
	}
	
    SMUtilities::replace(baseStateHeaderCode, "[EVENTS]", eventsCode); 

    generateFile(_smRepresentation->getMachineName() + "BaseState.h", baseStateHeaderCode);
} 

void SMObjectiveCTranslator::createBaseStateImplementationFile() const
{
    std::string baseStateImplementationCode = SMObjectiveCSkeletons::baseStateImplementationSkeleton(_smRepresentation->getMachineName());
    
    std::set<std::string> eventsSignatures;
    std::string eventsCode = "";
 
    std::vector<std::string> eventsIdentifiers = getAllEventsIdentifiers();
    
    for(std::vector<std::string>::const_iterator it = eventsIdentifiers.begin(); it != eventsIdentifiers.end(); ++it)
    {
         eventsCode += "-(void)" + *it + ":(NSDictionary *) parameters\n";
         eventsCode += "{\n"
		               "    NSLog(@\"%@ - Event '" + *it + ":' not implemented.\", [self class]);\n"
				       "}\n\n\n";
	}

    SMUtilities::replace(baseStateImplementationCode, "[EVENTS]", eventsCode);

    generateFile(_smRepresentation->getMachineName() + "BaseState.m", baseStateImplementationCode);
} 


void SMObjectiveCTranslator::createStateHeaderFile(const SMState * state) const
{
    std::string stateHeaderCode = SMObjectiveCSkeletons::stateHeaderSkeleton(_smRepresentation->getMachineName());

    SMUtilities::replace(stateHeaderCode, "[STATE_NAME]", state->getName());

    generateFile(_smRepresentation->getMachineName() + state->getName() + ".h", stateHeaderCode);
}


std::string SMObjectiveCTranslator::bodyForEvent(const SMEvent* event) const
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
        "    [self changeState:[" + _smRepresentation->getMachineName() + event->getTarget()->getName() + " sharedInstance]];\n";
    }

    std::string action = event->getAction();
     
    if(!action.empty())
    {
        eventBody +=
        "    if([self.delegate respondsToSelector:@selector(" + action + ":)])\n"
        "    {\n";
        
        //if(generateDebugInformation())
        //{
        //    eventBody += "#ifdef STATE_MACHINE_DEBUG\n";
        //    eventBody += "        [self logActionExec:@\"" + event->getAction() + "\"];\n";
        //    eventBody += "#endif //STATE_MACHINE_DEBUG\n\n";
        //}
    
        eventBody += 
        "        [self.delegate " + action + ":parameters];\n"
        "    }\n";
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

std::string SMObjectiveCTranslator::importFilesForState(const SMState* state) const
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
        result += std::string(((result.empty()) ? "" : "\n")) + "#import \"" + _smRepresentation->getMachineName() + *it + ".h\"";
    }
    
    return result;
}

void SMObjectiveCTranslator::createStateImplementationFile(const SMState * state) const
{
    std::string stateImplementationCode = SMObjectiveCSkeletons::stateImplementationSkeleton(_smRepresentation->getMachineName());

    SMUtilities::replace(stateImplementationCode, "[STATE_NAME]", state->getName());
    
    std::vector<const SMEvent *> events = state->getEvents();
    std::string eventsCode = "";
    
    for(std::vector<const SMEvent *>::const_iterator it = events.begin(); it != events.end(); ++it)
    {
        const SMEvent * event = *it;
        eventsCode += "-(void) " + event->getName() + ":(NSDictionary *) parameters\n" + bodyForEvent(event);
    }
    
    SMUtilities::replace(stateImplementationCode, "[EVENTS]", eventsCode);
    SMUtilities::replace(stateImplementationCode, "[IMPORTS]", importFilesForState(state));

    generateFile(_smRepresentation->getMachineName() + state->getName() + ".m", stateImplementationCode);
}


void SMObjectiveCTranslator::createActionsProtocolFile() const
{
    std::string actionsProtocolCode = SMObjectiveCSkeletons::generateDelegateSkeleton(_smRepresentation->getMachineName());

    std::string actionsCode = ""; 
    std::vector<std::string> actionsIdentifiers = getAllActionsIdentifiers();
    
    for(std::vector<std::string>::const_iterator it = actionsIdentifiers.begin(); it != actionsIdentifiers.end(); ++it)
    {
         actionsCode += "-(void)" + *it + ":(NSDictionary *) parameters;\n";
	}
	
    SMUtilities::replace(actionsProtocolCode, "[ACTIONS]", actionsCode); 

    generateFile(_smRepresentation->getMachineName() + "StateDelegate.h", actionsProtocolCode);

}

void SMObjectiveCTranslator::createConfiguratorHeaderFile() const
{
    std::string configuratorCode = SMObjectiveCSkeletons::generateConfiguratorHeaderSkeleton(_smRepresentation->getMachineName());

    generateFile(_smRepresentation->getMachineName() + "Configurator.h", configuratorCode);
}

void SMObjectiveCTranslator::createConfiguratorImplementationFile() const
{
    std::string configuratorCode = SMObjectiveCSkeletons::generateConfiguratorImplementationSkeleton(_smRepresentation->getMachineName());

    std::string importsCode = ""; 
    std::string associationCode = ""; 
    std::vector<std::string> statesIdentifiers = getAllStatesIdentifiers();
    
    for(std::vector<std::string>::const_iterator it = statesIdentifiers.begin(); it != statesIdentifiers.end(); ++it)
    {
         importsCode += "#import \"" + _smRepresentation->getMachineName() + *it + ".h\"\n";
         associationCode += "    [" + _smRepresentation->getMachineName() + *it + " sharedInstance].delegate = delegate;\n";
	}
	
    SMUtilities::replace(configuratorCode, "[IMPORTS]", importsCode); 
    SMUtilities::replace(configuratorCode, "[ASSOCIATIONS]", associationCode); 

    generateFile(_smRepresentation->getMachineName() + "Configurator.m", configuratorCode);
} 

bool SMObjectiveCTranslator::handleOption(const std::string& itemName, const std::string& itemValue)
{
    bool result = true;

    if(itemName == "folder" && !itemValue.empty())
    {
        std::string outputDirectory = getOutputDirectory() + "/" + itemValue;

        setOutputDirectory(outputDirectory);
        
        if(outputDirectory != ".")
        {
            std::system(("rm -Rf " + outputDirectory).c_str());
            std::system(("mkdir " + outputDirectory).c_str());
        }
    }
    else
    {
        result = false;
    }

    return result;
}

void SMObjectiveCTranslator::continueTranslation()
{
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
