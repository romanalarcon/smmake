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

#include "SMJavaSkeletons.h"
#include <iostream>
#include <string>

/**
 *
 */
std::string SMJavaSkeletons::makeFileHeadingComment(const std::string& filename)
{
    return 
        "//\n"
        "// " + filename + "\n"
        "// File generated by smmake v0.1. Do not edit!\n"
        "//\n";
}

std::string SMJavaSkeletons::stateImplementationSkeleton(const std::string& prefixAndName)
{
    return 
        SMJavaSkeletons::makeFileHeadingComment(prefixAndName + "[STATE_NAME].java") + "\n"
        "\n[PACKAGE]\n"
        "\n"
        "import java.util.HashMap;\n"
        "\n"
        "public final class " + prefixAndName + "[STATE_NAME] extends " + prefixAndName + "BaseState {\n\n"
        "   private static " + prefixAndName + "[STATE_NAME] sharedInstance = null;\n"
        "\n"
        "    private " + prefixAndName + "[STATE_NAME]()\n"
        "    {\n    }"
        "\n\n"
        "    public static " + prefixAndName + "[STATE_NAME] sharedInstance()\n"
        "    {\n"
        "        if(sharedInstance == null)\n"
        "        {\n"
        "            sharedInstance = new " + prefixAndName + "[STATE_NAME]();\n"
        "        }\n"
        "\n"
        "        return sharedInstance;\n"
        "    }\n\n"
        "[EVENTS]\n"
        "    @Override\n"
        "    public String getStateName()\n"
        "    {\n"
        "         return \"" + prefixAndName + "[STATE_NAME]\";\n"
        "    }\n\n"
        "}\n";
}

std::string SMJavaSkeletons::baseStateImplementationSkeleton(const std::string& prefixAndName)
{
    std::string skeleton = 
        SMJavaSkeletons::makeFileHeadingComment(prefixAndName + "BaseState.java") + "\n"
        "\n[PACKAGE]\n"
        "\n"
        "import java.util.HashMap;\n"
        "\n"
        "public class " + prefixAndName + "BaseState\n{\n\n"
        "   public " + prefixAndName + "StateDelegate delegate;"
        "\n\n"
        "[EVENTS]\n"
        "   public void setDelegate(" + prefixAndName + "StateDelegate delegate)\n"
        "   {\n"
        "       this.delegate = delegate;\n"
        "   }\n\n"
        "   public void changeState(" + prefixAndName + "BaseState newState)\n"
        "   {\n"
        "       if(newState == null)\n"
        "       {\n"
        "            System.out.println(\"**** ERROR **** Target state is null\");\n"
        "       }\n"
        "\n"
        "       this.delegate." + prefixAndName + "ChangeState(newState);\n"
        "   }\n\n"
        "   public String getStateName()\n"
        "   {\n"
        "       return  \"" + prefixAndName + "BaseState\";\n"
        "   }\n\n"
        "}\n";
    
    return skeleton;
}

/**
 *
 */
std::string SMJavaSkeletons::generateDelegateSkeleton(const std::string& prefixAndName)
{
    std::string skeleton =
        SMJavaSkeletons::makeFileHeadingComment(prefixAndName + "StateDelegate.java") + "\n"
        "\n[PACKAGE]\n"
        "\n"
        "import java.util.HashMap;\n"
        "\n\n"
        "public interface " + prefixAndName + "StateDelegate\n{"
        "\n"
        "[ACTIONS]\n"
        "    void " + prefixAndName + "ChangeState(" + prefixAndName + "BaseState newState);\n"
        "};\n";
  
    return skeleton;
}

std::string SMJavaSkeletons::generateConfiguratorImplementationSkeleton(const std::string& prefixAndName)
{
    std::string skeleton = 
        SMJavaSkeletons::makeFileHeadingComment(prefixAndName + "Configurator.java") + "\n"
        "\n[PACKAGE]\n"
        "\n"
        "public class " + prefixAndName + "Configurator\n{\n\n"
         "  private static " + prefixAndName + "Configurator sharedInstance = null;\n"
         "\n"
         "  private " + prefixAndName + "Configurator()\n"
         "  {\n  }"
         "\n\n"
         "  public static " + prefixAndName + "Configurator sharedInstance()\n"
         "  {\n"
         "      if(sharedInstance == null)\n"
         "      {\n"
         "          sharedInstance = new " + prefixAndName + "Configurator();\n"
         "      }\n"
         "\n"
         "      return sharedInstance;\n"
         "  }\n"
         "\n"
         "  public void setAllStatesDelegate(" + prefixAndName + "StateDelegate delegate)\n"
         "  {\n"
         "[ASSOCIATIONS]\n"
         "  }\n"
         "\n"
         "}\n";
    
    return skeleton;
} 


