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

#include "SMGVizTranslator.h"
#include "SMUtilities.h"

#include <iostream>
#include <fstream>
#include <vector>

void SMGVizTranslator::updateGraphVizMapWithEventsInState(const SMState * state)
{
    std::vector<const SMEvent *> events = state->getEvents();
    
	for(std::vector<const SMEvent *>::const_iterator it = events.begin(); it != events.end(); ++it)
	{
		const SMEvent * event = *it;

		std::string sourceState = event->getSource()->getName();
		std::string targetState = event->getTarget() ? event->getTarget()->getName() : sourceState; // "loop" transition
		std::string action      = "{" + event->getAction() + "}";
		
		StringPair key = StringPair(sourceState, targetState);
		
        std::vector<std::string> * labelInfoVector = _graphVizStateMap[key];
        
        if(!labelInfoVector)
        {
            labelInfoVector = new std::vector<std::string>;
            _graphVizStateMap[key] = labelInfoVector;
        }

        labelInfoVector->push_back(event->getName() + " / " + action);
    }
}


void SMGVizTranslator::fillGraphVizMapWithRepresentation(const SMRepresentation* smRepresentation)
{
    std::vector<const SMState *> states = smRepresentation->getStates();
    
    for(std::vector<const SMState *>::const_iterator it = states.begin(); it != states.end(); ++it)
    {
        updateGraphVizMapWithEventsInState(*it);
    }
}

/**
 *
 */
std::string SMGVizTranslator::splitWordsOfIdentifier(const std::string& identifier) const
{
    std::string uppercaseId = SMUtilities::toupper(identifier);
    std::string substring, result;
    
    size_t leftBound = 0;

    // Skip first character
    for(size_t index = 1; index < identifier.length(); ++index)
    {
        if(identifier[index] == uppercaseId[index])
        {
            substring = identifier.substr(leftBound, index - leftBound);
            
            if(substring.length() > 1)
            {
                substring += "\\n";
            }
             
            result += substring;
            leftBound = index;
        }
    }

    result += identifier.substr(leftBound, std::string::npos);
    
    return result;
}

void SMGVizTranslator::continueTranslation()
{
    std::ofstream graph((_smRepresentation->getMachineName() + ".gv").c_str());

    fillGraphVizMapWithRepresentation(_smRepresentation);
        
    graph << "digraph " << _smRepresentation->getMachineName() << "\n{" << std::endl;
    graph << "    splines=true;\n" << std::endl;
    graph << "    sep=\"+100,100\";\n" << std::endl;
    graph << "    overlap=scalexy;\n" << std::endl;
    graph << "    nodesep=0.96;\n" << std::endl;

    graph << "    node [shape = circle color=orange fontname=Arial fontsize=12 style=filled];" << std::endl;

    std::vector<const SMState *> states = _smRepresentation->getStates();
    const SMState * initialState = _smRepresentation->getInitialState();
    
    for(std::vector<const SMState *>::const_iterator it = states.begin(); it != states.end(); ++it)
    {
        std::string name = (*it)->getName();
        if(name != initialState->getName())
        {
            graph << "        "  << name << " [label = \"" + splitWordsOfIdentifier(name) + "\"];" << std::endl;
        }
    }

    graph << "    node [shape = circle color=yellow fontname=Arial fontsize=12 style=filled];" << std::endl;
    graph << "        "  << initialState->getName() << " [label = \"" + splitWordsOfIdentifier(initialState->getName()) + "\"];" << std::endl;
    graph << std::endl;

    for(std::map<StringPair, std::vector<std::string>*>::const_iterator it = _graphVizStateMap.begin(); it != _graphVizStateMap.end(); ++it)
    {
        StringPair key = it->first;
        std::vector<std::string> *labelInfoVector = it->second;
        
        std::string label = "";

        for(std::vector<std::string>::const_iterator it2 = labelInfoVector->begin(); it2 != labelInfoVector->end(); ++it2)
        {
            label += *it2 + "\\l";
        }

        graph << "    "  << key.first << "->" << key.second << "[label=\"" << label << " \" fontname=Arial fontsize=10];" << std::endl;
    }

    graph << "}" << std::endl;
}
