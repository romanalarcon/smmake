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

#ifndef __SMJavaSkeletons_H
#define __SMJavaSkeletons_H

#include <string>
#include <vector>

class SMJavaSkeletons
{
private:
    static std::string makeFileHeadingComment(const std::string& filename);
    static std::string makeIfDefHeading(const std::string& filename);

public:
    static std::string stateHeaderSkeleton(const std::string& stateName);
    static std::string stateImplementationSkeleton(const std::string& stateName);
    static std::string baseStateDeclarationSkeleton(const std::string& prefixAndName);
    static std::string baseStateImplementationSkeleton(const std::string& prefixAndName);
    static std::string generateDelegateSkeleton(const std::string& prefixAndName);
    static std::string generateConfiguratorHeaderSkeleton(const std::string& prefixAndName);
    static std::string generateConfiguratorImplementationSkeleton(const std::string& prefixAndName);
};

#endif
