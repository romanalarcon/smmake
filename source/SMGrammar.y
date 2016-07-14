%{ 

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


#include "SMParserDelegate.h"

#include <cstdio>

/*
 * "Private" variables
 */
static SMParserDelegate *_parserDelegate;
static std::string _firstStateNameSeen;
static std::string _lastStateNameSeen;

/*
 * Imported functions
 */
extern int yylex();
extern std::string getLexLastToken();
extern int getLexLineNumber();
extern int getLexColumnNumber();

/*
 * Exported functions
 */
void setParserDelegate(SMParserDelegate *parserDelegate);

static int yyerror(char *s);
std::string removeDoubleQuote(const std::string& inputString);

%}

%union
{
    bool flag;
    const char* stringValue;
}

%token<stringValue> SM_IDENTIFIER SM_STRING

%token SM_STATEMACHINE SM_STATES SM_STATE 
%token SM_DEBUG
%token SM_YES SM_NO 
%token SM_EVENTS SM_MOVETO SM_RUN SM_STAY 
%token SM_END 
%token SM_OPTIONS

%type <stringValue> stateChange 

%%

statemachine         : SM_STATEMACHINE SM_IDENTIFIER ':'
                     {
                          _parserDelegate->setStateMachineName($2);
                     }
                     declarations states SM_END
                     ;

declarations         : declarations declaration
                     | /* EMPTY */
                     ;

declaration          : SM_DEBUG ':' boolOption ';'
                     | SM_OPTIONS SM_IDENTIFIER ':' 
                     {
                         _parserDelegate->makeOption($2);
                     }
                     optionItems SM_END 
                     ;

optionItems          : optionItems optionItem
                     | optionItem
                     ;

optionItem           : SM_IDENTIFIER '=' SM_STRING ';'
                     {
                         _parserDelegate->makeItem($1, $3);
                     }
                     ;                                             

boolOption           : SM_YES
                     {
                         _parserDelegate->setDebugStatus(true);
                     }
                     | SM_NO
                     {
                         _parserDelegate->setDebugStatus(false);
                     }
                     ;

states               : SM_STATES ':' stateList SM_END
                     ; 

stateList            : stateList stateDecl
                     | stateDecl
                     ;

stateDecl            : SM_STATE 
                       SM_IDENTIFIER ':' 
                     {
                         // This sets the new state as the context state where events
                         // will be added by default  
                         _parserDelegate->makeState($2, true);  
                     }
                     events SM_END
                     ;

events               : SM_EVENTS ':' eventsList 
                     ;
      
eventsList           : eventsList eventDecl
                     | eventDecl
                     ;
          
eventDecl            : SM_IDENTIFIER ':' stateChange
                     {
                         _parserDelegate->makeEvent($1, ($3 ? $3 : ""));
                     }
                     action ';'
                     ;
                    
stateChange          : SM_MOVETO SM_IDENTIFIER
                     {
                         $$ = $2;
                     } 
                     | SM_STAY
                     {
                         $$ = NULL;
                     }
                     ; 
           
action               : SM_RUN SM_IDENTIFIER
                     {
                         _parserDelegate->makeAction($2);
                     } 
                     | /* EMPTY */
                     ;                      
%%

void setParserDelegate(SMParserDelegate *parserDelegate)
{
    _parserDelegate = parserDelegate;
}

int yyerror(char *s)
{
    if(_parserDelegate)
    {
        _parserDelegate->notifySyntaxError(getLexLastToken(), getLexLineNumber(), getLexColumnNumber()); 
    }
    else
    {
        std::printf("Error:%d:%d: Unexpected token '%s'\n", getLexLineNumber(), getLexColumnNumber(), getLexLastToken().c_str());
    }
    
    return 0;
}
