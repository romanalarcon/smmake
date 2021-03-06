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

#include "SMSwiftSkeletons.h"
#include <iostream>
#include <string>

/**
 *
 */
std::string SMSwiftSkeletons::makeFileHeadingComment(const std::string& filename)
{
    return 
        "//\n"
        "// " + filename + "\n"
        "// File generated by smmake v0.1. Do not edit!\n"
        "//\n";
}

std::string SMSwiftSkeletons::stateImplementationSkeleton(const std::string& prefixAndName)
{
    return 
        SMSwiftSkeletons::makeFileHeadingComment(prefixAndName + "[STATE_NAME].swift") + "\n"
        "import Foundation\n\n"
        "class " + prefixAndName + "[STATE_NAME] : " + prefixAndName + "BaseState\n{\n"
        "    static let sharedInstance : " + prefixAndName + "[STATE_NAME] = " + prefixAndName + "[STATE_NAME]()\n\n"
        "    private override init()\n"
        "    {\n"
        "        // Initializer is hidden to avoid constructing multiple instances of \"" + prefixAndName + "[STATE_NAME]" + "\"\n"
        "        // The single instance must be accessed through 'sharedInstance'\n"
        "    }\n\n"
        "[EVENTS]"
        "    override func getStateName() -> String\n"
        "    {\n"
        "        return \"" + prefixAndName + "[STATE_NAME]\"\n"
        "    }\n\n"
        "}\n";
}

std::string SMSwiftSkeletons::baseStateImplementationSkeleton(const std::string& prefixAndName)
{
    std::string skeleton = 
        SMSwiftSkeletons::makeFileHeadingComment(prefixAndName + "BaseState.swift") + "\n"
        "import Foundation\n\n"
        "class " + prefixAndName + "BaseState\n{\n"
        "    var delegate: " + prefixAndName + "StateDelegate?\n\n"
        "[EVENTS]"
        "    func setDelegate(delegate : " + prefixAndName + "StateDelegate)\n"
        "    {\n"
        "        self.delegate = delegate\n"
        "    }\n\n"
        "    func changeState(newState : " + prefixAndName + "BaseState)\n"
        "    {\n"
        "        self.delegate?." + prefixAndName + "ChangeState(newState)\n"
        "    }\n\n"
        "    func getStateName() -> String\n"
        "    {\n"
        "        return  \"" + prefixAndName + "BaseState\"\n"
        "    }\n\n"
        "}\n";
    
    return skeleton;
}

/**
 *
 */
std::string SMSwiftSkeletons::generateDelegateSkeleton(const std::string& prefixAndName)
{
    std::string skeleton =
        SMSwiftSkeletons::makeFileHeadingComment(prefixAndName + "StateDelegate.swift") + "\n"
        "import Foundation\n\n"
        "protocol " + prefixAndName + "StateDelegate\n{\n"
        "[ACTIONS]"
        "    func " + prefixAndName + "ChangeState(newState : " + prefixAndName + "BaseState)\n"
        "}\n";
  
    return skeleton;
}

std::string SMSwiftSkeletons::generateConfiguratorImplementationSkeleton(const std::string& prefixAndName)
{
    std::string skeleton = 
        SMSwiftSkeletons::makeFileHeadingComment(prefixAndName + "Configurator.swift") + "\n"
        "class " + prefixAndName + "Configurator\n{\n"
         "    static let sharedInstance : " + prefixAndName + "Configurator = " + prefixAndName + "Configurator()\n\n"
         "    func setAllStatesDelegate(delegate : " + prefixAndName + "StateDelegate)\n"
         "    {\n"
         "[ASSOCIATIONS]"
         "    }\n\n"
         "}\n";
    
    return skeleton;
} 
