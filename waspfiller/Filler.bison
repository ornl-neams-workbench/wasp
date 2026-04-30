%{ /*** C/C++ Declarations ***/

#include <stdio.h>
#include <vector>
%}

/*** yacc/bison Declarations ***/

%code requires{
#include <memory>
#include "waspcore/TreeNodePool.h"
#include "waspcore/wasp_node.h"
#include "waspcore/decl.h"
}

/* Require bison 3 or later */

%output "FillerParser.cpp"
/* add debug output code to generated parser. disable this for release
 * versions. */
/*%debug*/

/* start symbol is named "start" */
%start start

/* write out a header file containing the token defines */
%defines
%require "3.7"
/* use newer C++ skeleton file */
%skeleton "lalr1.cc"

/* namespace to enclose parser in */
/* %name-prefix "wasp" */
%define api.namespace {wasp}
%define api.location.file "../waspcore/location.hh"

/* set the parser's class identifier */
%define api.parser.class {FillerParser}

/* Silence warning for known shift-reduce conflicts */
%expect 1


/* keep track of the current position within the input */
%locations
%initial-action
{
    // initialize the initial location object
    @$.begin.filename = @$.end.filename = &interpreter.stream_name();
    @$.begin.line = @$.end.line = interpreter.start_line();
    @$.begin.column = @$.end.column = interpreter.start_column();
    lexer = std::make_shared<FillerLexerImpl>(interpreter,&input_stream);
    //lexer->set_debug(true);
    //this->set_debug_level(1);
};

/* The interpreter is passed by reference to the parser and to the FillerLexer. This
 * provides a simple but effective pure interface, not relying on global
 * variables. */
%parse-param { class AbstractInterpreter& interpreter }
             {std::istream &input_stream}
             {std::shared_ptr<class FillerLexerImpl> lexer}
/* verbose error messages */
%define parse.error verbose

 /*** BEGIN EXAMPLE - Change the Filler grammar's tokens below ***/

//%define api.value.type {struct YYSTYPE}
%union{
    std::size_t token_index;
    std::size_t node_index;
    std::vector<size_t>* node_indices;
}
%token                  END           0  "end of file"
%token <token_index>       INTEGER         "integer"
%token <token_index>       COUNT         "count"
%token <token_index>       REAL         "real"
%token <token_index>       REPEAT         "repeat"
%token <token_index>       PREVIOUS_REPEAT         "previous repeat"
%token <token_index>       REPEAT_ZERO "repeat zero"
%token <token_index>       ALTERNATE_NODE "alternate"
%token <token_index>       PREVIOUS_ALTERNATE "previous alternate"
%token <token_index>       BACK_PREVIOUS "back previous"
%token <token_index>       LIN_INTERP "linear interpolate"
%token <token_index>       LOG_INTERP "log interpolate"
%token <token_index>       FILL "fill"
%token <token_index>       ADDRESS "address"
%token <token_index>       MOVE "move"

%type <node_index>  integer
%type <node_index>  real
%type <node_index>  count
%type <node_index>  value
%type <node_index>  repeat
%type <node_index>  alternate
%type <node_index>  fill
%type <node_index>  address
%type <node_index>  move
%type <node_index>  previous_repeat
%type <node_index>  repeat_zero
%type <node_index>  previous_alternate
%type <node_index>  back_previous
%type <node_index>  linear_interpolate
%type <node_index>  log_interpolate


%{

#include "FillerInterpreter.h"
#include "FillerLexer.h"

/* this "connects" the Filler parser in the interpreter to the flex FillerLexer class
 * object. it defines the yylex() function call to pull the next token from the
 * current lexer object of the interpreter context. */
#undef yylex
#define yylex lexer->lex

%}

%% /*** Grammar Rules ***/

 /*** BEGIN - Change the Filler grammar rules below ***/
count : COUNT 
{
    size_t token_index = ($1);$$ = interpreter.push_leaf(wasp::INTEGER,"count",token_index);
}

integer : INTEGER 
{
    size_t token_index = ($1);$$ = interpreter.push_leaf(wasp::INTEGER,"value",token_index);
}
real : REAL 
{
    size_t token_index = ($1);$$ = interpreter.push_leaf(wasp::REAL,"value",token_index);
}
repeat : count REPEAT value
        {
            $$ = interpreter.push_parent(wasp::REPEAT
                                       ,"repeat"
                                       ,{$count,$value});
        }        
alternate : count ALTERNATE_NODE value
        {
            $$ = interpreter.push_parent(wasp::ALTERNATE_NODE
                                       ,"alternate"
                                       ,{$count,$value});
        }
        
fill : FILL value
        {
            $$ = interpreter.push_parent(wasp::FILL
                                       ,"fill"
                                       ,{$value});
        }
        
address : ADDRESS integer
        {
            $$ = interpreter.push_parent(wasp::ADDRESS
                                       ,"addr"
                                       ,{$integer});
        }
        
move : count MOVE
        {            
            $$ = interpreter.push_parent(wasp::MOVE
                                       ,"move"
                                       ,{$count});
        }
        
previous_repeat : count PREVIOUS_REPEAT integer
        {            
            $$ = interpreter.push_parent(wasp::PREVIOUS_REPEAT
                                       ,"prepeat"
                                       ,{$count, $integer});
        }
        
      |PREVIOUS_REPEAT integer
      {
          $$ = interpreter.push_parent(wasp::PREVIOUS_REPEAT
                                       ,"prepeat"
                                       ,{$integer});
      }
      
repeat_zero : count REPEAT_ZERO
    {
        $$ = interpreter.push_parent(wasp::REPEAT_ZERO
                                       ,"zrepeat"
                                       ,{$count});
    }
        
previous_alternate : count PREVIOUS_ALTERNATE integer
        {
            $$ = interpreter.push_parent(wasp::PREVIOUS_ALTERNATE
                                       ,"palternate"
                                       ,{$count, $integer});
        }
        
      | PREVIOUS_ALTERNATE integer
      {
          $$ = interpreter.push_parent(wasp::PREVIOUS_ALTERNATE
                                       ,"palternate"
                                       ,{$integer});
      }
back_previous :  count BACK_PREVIOUS integer
        {
            $$ = interpreter.push_parent(wasp::BACK_PREVIOUS
                                       ,"bprevious"
                                       ,{$count, $integer});
        }
        
      | BACK_PREVIOUS integer
      {
            $$ = interpreter.push_parent(wasp::BACK_PREVIOUS
                                       ,"bprevious"
                                       ,{$integer});
      }

linear_interpolate : count LIN_INTERP value value
        {
            $$ = interpreter.push_parent(wasp::LINEAR_INTERPOLATE
                            ,"lin_interp"
                            ,{$count, $3, $4});
        }
        
log_interpolate : count LOG_INTERP value value
        {
            $$ = interpreter.push_parent(wasp::LOG_INTERPOLATE
                            ,"log_interp"
                            ,{$count, $3, $4});
        }
        
value : integer
            {
                $$ = $1;
            }
           | real
            {
                $$ = $1;
            }


start   : /** empty **/
        | start value{
            interpreter.push_staged_child($2);
        }
        | start repeat
        {
            interpreter.push_staged_child($2);
        }
        | start alternate
        {
            interpreter.push_staged_child($2);
        } 
        | start fill
        {
            interpreter.push_staged_child($2);
        }
        | start address
        {
            interpreter.push_staged_child($2);
        }
        | start move
        {
            interpreter.push_staged_child($2);
        }
        | start previous_repeat
        {
            interpreter.push_staged_child($2);
        }
        | start previous_alternate
        {
            interpreter.push_staged_child($2);
        }
        | start back_previous
        {
            interpreter.push_staged_child($2);
        }
        | start linear_interpolate
        {
            interpreter.push_staged_child($2);
        }
        | start log_interpolate
        {
            interpreter.push_staged_child($2);
        }
        | start repeat_zero
        {
            interpreter.push_staged_child($2);
        }
        | END



 /*** END RULES - Change the Filler grammar rules above ***/

%% /*** Additional Code ***/
namespace wasp{
void FillerParser::error(const FillerParser::location_type& l,
                           const std::string& m)
{
    interpreter.error_diagnostic()<<l<<" : "<<m<<"."<<std::endl;
}
} // end of namespace
