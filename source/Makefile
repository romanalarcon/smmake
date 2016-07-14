#The MIT License (MIT)
#
#Copyright (c) 2016 Roman Alarcon
#
#Permission is hereby granted, free of charge, to any person obtaining a copy
#of this software and associated documentation files (the "Software"), to deal
#in the Software without restriction, including without limitation the rights
#to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#copies of the Software, and to permit persons to whom the Software is
#furnished to do so, subject to the following conditions:
#
#The above copyright notice and this permission notice shall be included in all
#copies or substantial portions of the Software.
#
#THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#SOFTWARE.


CC=g++
CFLAGS=-c -Wno-write-strings -Wreturn-type
LDFLAGS=
SOURCES=SMApplication.cpp SMAppOptionManager.cpp main.cpp \
        SMParser.cpp \
        SMLexScanner.cpp SMYaccParser.cpp \
        SMRepresentation.cpp SMState.cpp SMEvent.cpp \
        SMTranslator.cpp SMGVizTranslator.cpp SMObjectiveCSkeletons.cpp SMObjectiveCTranslator.cpp SMCppSkeletons.cpp SMCppTranslator.cpp\
        SMJavaSkeletons.cpp SMJavaTranslator.cpp\
        SMSwiftSkeletons.cpp SMSwiftTranslator.cpp\
        SMUtilities.cpp 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE_PATH=../bin
EXECUTABLE="$(EXECUTABLE_PATH)/smmake"
LIBPATH=
INCPATH=
LIBS=

all: $(SOURCES) $(EXECUTABLE)

install:
	@if [ ! -d "$$HOME/scripts" ]; then \
	   mkdir "$$HOME/scripts"; \
	fi 
	@if [ ! -f "$(EXECUTABLE)" ]; then\
	    echo "Error: File '$(EXECUTABLE)' doesn't exist."; \
	else \
	    cp $(EXECUTABLE) $$HOME/scripts; \
	    echo "Program '$(EXECUTABLE)' installed in $$HOME/scripts."; \
        fi

clean:
	rm -f $(EXECUTABLE)
	rm -f $(OBJECTS)
	rm -f *.cpp~
	rm -f *.h~
	rm -f SMLexScanner.cpp SMYaccParser.cpp SMYaccParser.h

$(EXECUTABLE): $(OBJECTS) 
	@if [ ! -d "$(EXECUTABLE_PATH)" ]; then \
	   mkdir "$(EXECUTABLE_PATH)"; \
	fi
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBPATH) $(LIBS) 
	

SMLexScanner.cpp: SMLexer.l
	flex -oSMLexScanner.cpp SMLexer.l

SMYaccParser.cpp: SMGrammar.y
	bison -d SMGrammar.y
	mv SMGrammar.tab.c SMYaccParser.cpp
	mv SMGrammar.tab.h SMYaccParser.h	

.cpp.o:
	$(CC) $(INCPATH) $(CFLAGS) $< -o $@
