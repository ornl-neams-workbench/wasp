%top{
/* This disables inclusion of unistd.h, which is not available under Visual C++
 * on Win32. The C++ scanner uses STL streams instead. */
#define YY_NO_UNISTD_H

}
%{ /*** C/C++ Declarations ***/

#include <string>
#include <sstream>
#include "FillerLexer.h"
#include "FillerInterpreter.h"

/* import the parser's token type into a local typedef */
typedef wasp::FillerParser::token token;
typedef wasp::FillerParser::token_type token_type;

/* By default yylex returns int, we use token_type. Unfortunately yyterminate
 * by default returns 0, which is not of token_type. */
#define yyterminate() return token::END

%}

 /*** Flex Declarations and Options ***/

 /* enable c++ scanner class generation */
%option c++

%option prefix="Filler"
%option outfile="FillerLexer.cpp"


 /* enable scanner to generate debug output. disable this for release
 * versions. */
/*%option debug*/

 /* no support for include files is planned */
%option yywrap nounput

 /* enables the use of start condition stacks */

INT [+\-]?[0-9]+([eE]\+?[0-9]+)?
COUNT [+\-]?[0-9]+
EXPONENT [eE][+\-]?{INT}
DOUBLE [+\-]?{INT}?\.{INT}{EXPONENT}?|{INT}\.({INT}{EXPONENT}?)?|[+\-]?[0-9]+([eE]\-[0-9]+)?

REPEAT [\$rR\*]
PREVIOUS_REPEAT [qQ]
REPEAT_ZERO [zZ]
ALTERNATE [pP]
PREVIOUS_ALTERNATE [nN]
BACK_PREVIOUS [bB]
LIN_INTERP [iI]
LOG_INTERP [lL]
FILL [fF]
ADDRESS [aA]
MOVE [sS]


 /* The following paragraph suffices to track locations accurately. Each time
 * yylex is invoked, the begin position is moved onto the end position. */
%{
#define YY_USER_ACTION  yylloc->columns(yyleng); file_offset+=yyleng;
%}

%% /*** Regular Filleressions Part ***/

 /* code to place at the beginning of yylex() */
%{
    // reset location
    yylloc->step();
%}
 /*** BEGIN EXAMPLE - Change the Filler lexer rules below ***/

{COUNT}/[zZbBlLiIqQnNrRpPsS\$\*] {
    capture_token(yylval,wasp::INTEGER);
    return token::COUNT;
}

{INT} {
    capture_token(yylval,wasp::INTEGER);
    return token::INTEGER;
}
{DOUBLE} {
    capture_token(yylval,wasp::REAL);
    return token::REAL;
}

{REPEAT} {
    capture_token(yylval,wasp::REPEAT);
    return token::REPEAT;
}
{PREVIOUS_REPEAT} {
    capture_token(yylval,wasp::PREVIOUS_REPEAT);
    return token::PREVIOUS_REPEAT;
}
{REPEAT_ZERO} {
    capture_token(yylval,wasp::REPEAT_ZERO);
    return token::REPEAT_ZERO;
}
{ALTERNATE} {
    capture_token(yylval,wasp::ALTERNATE);
    return token::ALTERNATE;
}
{PREVIOUS_ALTERNATE} {
    capture_token(yylval,wasp::PREVIOUS_ALTERNATE);
    return token::PREVIOUS_ALTERNATE;
}
{BACK_PREVIOUS} {
    capture_token(yylval,wasp::BACK_PREVIOUS);
    return token::BACK_PREVIOUS;
}
{LIN_INTERP} {
    capture_token(yylval,wasp::LINEAR_INTERPOLATE);
    return token::LIN_INTERP;
}
{LOG_INTERP} {
    capture_token(yylval,wasp::LOG_INTERPOLATE);
    return token::LOG_INTERP;
}
{FILL} {
    capture_token(yylval,wasp::FILL);
    return token::FILL;
}
{ADDRESS} {
    capture_token(yylval,wasp::ADDRESS);
    return token::ADDRESS;
}
{MOVE} {
    capture_token(yylval,wasp::MOVE);
    return token::MOVE;
}

 /* gobble up white-spaces */
[ \t\r]+ {
    yylloc->step();
}

 /* gobble up end-of-lines */
\n {
    yylloc->lines(yyleng); yylloc->step();
    std::size_t offset = yyin.tellg();
    offset-=yyleng;
    interpreter.push_line_offset(offset);
}

 /* pass all other characters up to Filler*/
. {
    return static_cast<token_type>(*yytext);
}

 /*** END EXAMPLE - Change the Filler lexer rules above ***/

%% /*** Additional Code ***/

namespace wasp {

FillerLexerImpl::FillerLexerImpl(
                AbstractInterpreter & interpreter,
                std::istream* in,
                std::ostream* out)
    : FillerFlexLexer(in, out)
    , interpreter(interpreter)
    , file_offset(0)
{
}

FillerLexerImpl::~FillerLexerImpl()
{
}

void FillerLexerImpl::set_debug(bool b)
{
    yy_flex_debug = b;
}
void FillerLexerImpl::rewind()
{
    yyin.seekg(-yyleng,std::ios_base::cur);
    yyless(0);
}
void FillerLexerImpl::capture_token(
        wasp::FillerParser::semantic_type* yylval
        ,wasp::NODE type)
{
    std::size_t offset = file_offset - yyleng;
    yylval->token_index = interpreter.token_count();
    interpreter.push_token(yytext,type,offset);
}
} // end of namespace

/* This implementation of FillerFlexLexer::yylex() is required to fill the
 * vtable of the class FillerFlexLexer. We define the scanner's main yylex
 * function via YY_DECL to reside in the FillerLexerImpl class instead. */

#ifdef yylex
#undef yylex
#endif

int FillerFlexLexer::yylex()
{
    return 0;
}

/* When the scanner receives an end-of-file indication from YY_INPUT, it then
 * checks the yywrap() function. If yywrap() returns false (zero), then it is
 * assumed that the function has gone ahead and set up `yyin' to point to
 * another input file, and scanning continues. If it returns true (non-zero),
 * then the scanner terminates, returning 0 to its caller. */

int FillerFlexLexer::yywrap()
{
    return 1;
}
