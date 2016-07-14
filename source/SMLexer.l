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

#include "SMYaccParser.h"

#include <string>
#include "SMYaccParser.h"

/*
 * "Private" variables
 */ 
static int _lineNumber;
static int _columnNumber;
static std::string _lastToken;

/*
 * Functions able to be accessed from outside
 */
int setInputFile(const char * filename);
std::string getLexLastToken();
int getLexLineNumber();
int getLexColumnNumber();


/*
 * "Private" functions
 */ 
static void updateLastToken(const char * token);
static void skipSingleLineComment();


%}

digit                   [0-9]
letter                  [A-Za-z_]
no_quote_or_backslash   [^\"\\]
utf8char                \\(u|U)[0-9A-Fa-f]+
escapechar              \\[^uU]
string                  \"({no_quote_or_backslash}|{utf8char}|{escapechar})*\"
identifier              {letter}({letter}|{digit})*

%%

[\t]+              { _columnNumber++;                                   }
[\r]+              { _columnNumber = 1;                                 }
[ ]+               { _columnNumber += strlen(yytext);                   }
"\n"               { _lineNumber++; _columnNumber = 1;                  }
"#"                { skipSingleLineComment();                           }

"STATEMACHINE"     {  updateLastToken(yytext); return SM_STATEMACHINE; }
"STATES"           {  updateLastToken(yytext); return SM_STATES;       }
"STATE"            {  updateLastToken(yytext); return SM_STATE;        }
"EVENTS"           {  updateLastToken(yytext); return SM_EVENTS;       }
"END"              {  updateLastToken(yytext); return SM_END;          }  
"MOVETO"           {  updateLastToken(yytext); return SM_MOVETO;       }
"RUN"              {  updateLastToken(yytext); return SM_RUN;          }
"STAY"             {  updateLastToken(yytext); return SM_STAY;         }
"YES"              {  updateLastToken(yytext); return SM_YES;          }
"NO"               {  updateLastToken(yytext); return SM_NO;           }
"DEBUG"            {  updateLastToken(yytext); return SM_DEBUG;        }
"OPTIONS"          {  updateLastToken(yytext); return SM_OPTIONS;      }


{identifier}       {
                       yylval.stringValue = strdup(yytext); 
                       updateLastToken(yytext); 
                       return SM_IDENTIFIER;       
                   }

{string}           {
                       yylval.stringValue = strdup(yytext); 
                       updateLastToken(yytext); 
                       return SM_STRING;       

                   }
.                  {
                       _lastToken = std::string("") + yytext[0]; 
                       _columnNumber++; 
                       return yytext[0]; 
                   } 

%%



/**
 *
 */
static void skipSingleLineComment()
{
    int character;
  
    while((character = yyinput()) != EOF && character != '\n') 
    {
    }
  
    if(character == '\n')
    {
        _lineNumber++;
        _columnNumber = 1;
    }
}

/**
 * setInputFile: sets the file to be scanned
 * Return values: 0 means OK, file is opened to read. 1 means error.
 */
int setInputFile(const char * filename)
{
    yyin = fopen(filename, "r");
    
    _lineNumber = 1;
    _columnNumber = 1;


    return (yyin == NULL) ? 1 : 0;
}

/**
 * getLexLastToken: returns the last token read
 */
std::string getLexLastToken()
{
    return _lastToken;
}

/**
 * getLexLineNumber: returns the current line number.
 */
int getLexLineNumber()
{
    return _lineNumber;
}

/**
 * getLexLineNumber: returns the current column number.
 */
int getLexColumnNumber()
{
    return _columnNumber;
}

/**
 *
 */
static void updateLastToken(const char * token)
{
    _lastToken = std::string(yytext); 
    _columnNumber += _lastToken.size(); 
}

int yywrap()
{
    return 1;
}

