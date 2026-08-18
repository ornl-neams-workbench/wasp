%{
#include <stdio.h>
#include <string>
#include <vector>
%}

%code requires{
#include <memory>
#include "waspcore/utils.h"
#include "waspcore/decl.h"
}

%output "SIRENParser.cpp"
%start start
%defines
%require "3.7"
%skeleton "lalr1.cc"
%define api.namespace {wasp}
%define api.location.file "../waspcore/location.hh"
%define api.parser.class {SIRENParser}
%define parse.error verbose

%locations
%initial-action
{
    @$.begin.filename = @$.end.filename = &interpreter.stream_name();
    @$.begin.line = @$.end.line = interpreter.start_line();
    @$.begin.column = @$.end.column = interpreter.start_column();
    lexer = std::make_shared<SIRENLexerImpl>(interpreter, &input_stream);
};

%parse-param {class AbstractInterpreter& interpreter}
             {std::istream& input_stream}
             {std::shared_ptr<class SIRENLexerImpl> lexer}

%union {
    std::size_t token_index;
    std::size_t node_index;
    std::vector<std::size_t>* node_indices;
}

%token END 0 "end of file"
%token <token_index> ANY "//"
%token <token_index> MINUS "-"
%token <token_index> LPAREN "("
%token <token_index> RPAREN ")"
%token <token_index> LBRACKET "["
%token <token_index> RBRACKET "]"
%token <token_index> COMMA ","
%token <token_index> COLON ":"
%token <token_index> GTE ">="
%token <token_index> LT "<"
%token <token_index> GT ">"
%token <token_index> LTE "<="
%token <token_index> NEQ "!="
%token <token_index> EQ "="
%token <token_index> AND "&&"
%token <token_index> PARENT ".."
%token <token_index> OR "||"
%token <token_index> BANG "!"
%token <token_index> MULTIPLY "*"
%token <token_index> DIVIDE "div"
%token <token_index> SEPARATOR "/"
%token <token_index> PLUS "+"
%token <token_index> EXPONENT "^"
%token <token_index> MOD "mod"
%token <token_index> UNION_OP "|"
%token <token_index> INTERSECT_OP "intersect"
%token <token_index> EXCEPT_OP "except"
%token <token_index> FOLLOWING_AXIS "following-sibling::"
%token <token_index> PRECEDING_AXIS "preceding-sibling::"
%token <token_index> POSITION_FN "position"
%token <token_index> LAST_FN "last"
%token <token_index> COUNT_FN "count"
%token <token_index> CONTAINS_FN "contains"
%token <token_index> STARTS_WITH_FN "starts-with"
%token <token_index> NOT_FN "not"
%token <token_index> INTEGER "integer"
%token <token_index> DOUBLE "double"
%token <token_index> DECL "decl"
%token <token_index> QSTRING "quoted string"

%type <node_index> any_separator separator parent_step
%type <node_index> lparen rparen lbracket rbracket comma colon
%type <node_index> plus minus multiply divide modulus exponent unary_not
%type <node_index> eq neq gt lt gte lte and or
%type <node_index> comparison_operator
%type <node_index> union_operator intersect_operator except_operator
%type <node_index> path_name predicate_name wildcard_name
%type <node_index> path_base predicate_path_base axis_step predicate_axis_step
%type <node_index> path_step predicate_path_step
%type <node_index> relative_path predicate_relative_path absolute_path
%type <node_index> selection selection_expression union_expression
%type <node_index> intersection_expression
%type <node_index> integer_index index_range predicate_content
%type <node_index> predicate_expression predicate_or predicate_and
%type <node_index> predicate_comparison predicate_additive
%type <node_index> predicate_multiplicative predicate_unary predicate_power
%type <node_index> predicate_primary numeric_value string_value
%type <node_index> function_name function_call function_arguments
%type <node_indices> predicate_clause
%destructor { delete $$; } predicate_clause

%{
#include "SIRENInterpreter.h"
#include "SIRENLexer.h"

#undef yylex
#define yylex lexer->lex
%}

%%

any_separator : ANY
    {
        $$ = interpreter.push_leaf(wasp::ANY, "A", $1);
    }

separator : SEPARATOR
    {
        $$ = interpreter.push_leaf(wasp::SEPARATOR, "/", $1);
    }

parent_step : PARENT
    {
        $$ = interpreter.push_leaf(wasp::PARENT, "P", $1);
    }

lparen : LPAREN
    {
        $$ = interpreter.push_leaf(wasp::LPAREN, "(", $1);
    }

rparen : RPAREN
    {
        $$ = interpreter.push_leaf(wasp::RPAREN, ")", $1);
    }

lbracket : LBRACKET
    {
        $$ = interpreter.push_leaf(wasp::LBRACKET, "[", $1);
    }

rbracket : RBRACKET
    {
        $$ = interpreter.push_leaf(wasp::RBRACKET, "]", $1);
    }

comma : COMMA
    {
        $$ = interpreter.push_leaf(wasp::WASP_COMMA, ",", $1);
    }

colon : COLON
    {
        $$ = interpreter.push_leaf(wasp::COLON, ":", $1);
    }

plus : PLUS
    {
        $$ = interpreter.push_leaf(wasp::PLUS, "+", $1);
    }

minus : MINUS
    {
        $$ = interpreter.push_leaf(wasp::MINUS, "-", $1);
    }

multiply : MULTIPLY
    {
        $$ = interpreter.push_leaf(wasp::MULTIPLY, "*", $1);
    }

divide : DIVIDE
    {
        $$ = interpreter.push_leaf(wasp::DIVIDE, "div", $1);
    }

modulus : MOD
    {
        $$ = interpreter.push_leaf(wasp::MODULUS, "mod", $1);
    }

exponent : EXPONENT
    {
        $$ = interpreter.push_leaf(wasp::EXPONENT, "^", $1);
    }

unary_not : BANG
    {
        $$ = interpreter.push_leaf(wasp::UNARY_NOT, "!", $1);
    }

eq : EQ
    {
        $$ = interpreter.push_leaf(wasp::EQ, "==", $1);
    }

neq : NEQ
    {
        $$ = interpreter.push_leaf(wasp::NEQ, "!=", $1);
    }

gte : GTE
    {
        $$ = interpreter.push_leaf(wasp::GTE, ">=", $1);
    }

gt : GT
    {
        $$ = interpreter.push_leaf(wasp::GT, ">", $1);
    }

lte : LTE
    {
        $$ = interpreter.push_leaf(wasp::LTE, "<=", $1);
    }

lt : LT
    {
        $$ = interpreter.push_leaf(wasp::LT, "<", $1);
    }

and : AND
    {
        $$ = interpreter.push_leaf(wasp::WASP_AND, "&&", $1);
    }

or : OR
    {
        $$ = interpreter.push_leaf(wasp::WASP_OR, "||", $1);
    }

comparison_operator : eq | neq | gt | lt | gte | lte

union_operator : UNION_OP
    {
        $$ = interpreter.push_leaf(wasp::UNION, "|", $1);
    }

intersect_operator : INTERSECT_OP
    {
        $$ = interpreter.push_leaf(wasp::INTERSECT, "intersect", $1);
    }

except_operator : EXCEPT_OP
    {
        $$ = interpreter.push_leaf(wasp::EXCEPT, "except", $1);
    }

path_name : DECL
    {
        std::string name = wasp::strip_quotes(interpreter.token_data($1));
        $$ = interpreter.push_leaf(wasp::DECL, name.c_str(), $1);
    }
    | QSTRING
    {
        std::string name = wasp::strip_quotes(interpreter.token_data($1));
        $$ = interpreter.push_leaf(wasp::DECL, name.c_str(), $1);
    }

predicate_name : DECL
    {
        $$ = interpreter.push_leaf(wasp::DECL,
                                   interpreter.token_data($1), $1);
    }

wildcard_name : MULTIPLY
    {
        $$ = interpreter.push_leaf(wasp::DECL, "*", $1);
    }

axis_step : FOLLOWING_AXIS path_name
    {
        std::size_t axis = interpreter.push_leaf(
            wasp::FOLLOWING_SIBLING, "following-sibling::", $1);
        $$ = interpreter.push_parent(wasp::FOLLOWING_SIBLING,
                                     "following-sibling", {axis, $2});
    }
    | FOLLOWING_AXIS wildcard_name
    {
        std::size_t axis = interpreter.push_leaf(
            wasp::FOLLOWING_SIBLING, "following-sibling::", $1);
        $$ = interpreter.push_parent(wasp::FOLLOWING_SIBLING,
                                     "following-sibling", {axis, $2});
    }
    | PRECEDING_AXIS path_name
    {
        std::size_t axis = interpreter.push_leaf(
            wasp::PRECEDING_SIBLING, "preceding-sibling::", $1);
        $$ = interpreter.push_parent(wasp::PRECEDING_SIBLING,
                                     "preceding-sibling", {axis, $2});
    }
    | PRECEDING_AXIS wildcard_name
    {
        std::size_t axis = interpreter.push_leaf(
            wasp::PRECEDING_SIBLING, "preceding-sibling::", $1);
        $$ = interpreter.push_parent(wasp::PRECEDING_SIBLING,
                                     "preceding-sibling", {axis, $2});
    }

predicate_axis_step : FOLLOWING_AXIS predicate_name
    {
        std::size_t axis = interpreter.push_leaf(
            wasp::FOLLOWING_SIBLING, "following-sibling::", $1);
        $$ = interpreter.push_parent(wasp::FOLLOWING_SIBLING,
                                     "following-sibling", {axis, $2});
    }
    | FOLLOWING_AXIS wildcard_name
    {
        std::size_t axis = interpreter.push_leaf(
            wasp::FOLLOWING_SIBLING, "following-sibling::", $1);
        $$ = interpreter.push_parent(wasp::FOLLOWING_SIBLING,
                                     "following-sibling", {axis, $2});
    }
    | PRECEDING_AXIS predicate_name
    {
        std::size_t axis = interpreter.push_leaf(
            wasp::PRECEDING_SIBLING, "preceding-sibling::", $1);
        $$ = interpreter.push_parent(wasp::PRECEDING_SIBLING,
                                     "preceding-sibling", {axis, $2});
    }
    | PRECEDING_AXIS wildcard_name
    {
        std::size_t axis = interpreter.push_leaf(
            wasp::PRECEDING_SIBLING, "preceding-sibling::", $1);
        $$ = interpreter.push_parent(wasp::PRECEDING_SIBLING,
                                     "preceding-sibling", {axis, $2});
    }

path_base : path_name | wildcard_name | parent_step | axis_step
predicate_path_base : predicate_name | wildcard_name | parent_step
                    | predicate_axis_step

predicate_clause : lbracket predicate_content rbracket
    {
        $$ = new std::vector<std::size_t>({$1, $2, $3});
    }

path_step : path_base
    | path_step predicate_clause
    {
        std::size_t predicate = $2->at(1);
        const char* name = interpreter.type(predicate) == wasp::INDEX
                               ? "ipcs" : "cpcs";
        $$ = interpreter.push_parent(wasp::PREDICATED_CHILD, name,
                                     {$1, $2->at(0), predicate, $2->at(2)});
        delete $2;
    }

predicate_path_step : predicate_path_base
    | predicate_path_step predicate_clause
    {
        std::size_t predicate = $2->at(1);
        const char* name = interpreter.type(predicate) == wasp::INDEX
                               ? "ipcs" : "cpcs";
        $$ = interpreter.push_parent(wasp::PREDICATED_CHILD, name,
                                     {$1, $2->at(0), predicate, $2->at(2)});
        delete $2;
    }

relative_path : path_step
    | path_step separator relative_path
    {
        $$ = interpreter.push_parent(wasp::OBJECT, "O", {$1, $2, $3});
    }
    | path_step any_separator relative_path
    {
        $$ = interpreter.push_parent(wasp::ANY, "A", {$1, $2, $3});
    }
    | path_step any_separator
    {
        $$ = interpreter.push_parent(wasp::ANY, "A", {$1, $2});
    }

predicate_relative_path : predicate_path_step
    | predicate_path_step separator predicate_relative_path
    {
        $$ = interpreter.push_parent(wasp::OBJECT, "O", {$1, $2, $3});
    }
    | predicate_path_step any_separator predicate_relative_path
    {
        $$ = interpreter.push_parent(wasp::ANY, "A", {$1, $2, $3});
    }

absolute_path : separator
    {
        $$ = interpreter.push_parent(wasp::DOCUMENT_ROOT, "R", {$1});
    }
    | separator relative_path
    {
        $$ = interpreter.push_parent(wasp::DOCUMENT_ROOT, "R", {$1, $2});
    }
    | any_separator
    {
        $$ = $1;
    }
    | any_separator relative_path
    {
        $$ = interpreter.push_parent(wasp::ANY, "A", {$1, $2});
    }

integer_index : INTEGER
    {
        $$ = interpreter.push_leaf(wasp::INTEGER, "int", $1);
    }

index_range : integer_index colon integer_index
    {
        $$ = interpreter.push_parent(wasp::INDEX, "I", {$1, $2, $3});
    }
    | integer_index colon integer_index colon integer_index
    {
        $$ = interpreter.push_parent(wasp::INDEX, "I",
                                     {$1, $2, $3, $4, $5});
    }

numeric_value : INTEGER
    {
        $$ = interpreter.push_leaf(wasp::VALUE, "value", $1);
    }
    | DOUBLE
    {
        $$ = interpreter.push_leaf(wasp::VALUE, "value", $1);
    }

string_value : QSTRING
    {
        $$ = interpreter.push_leaf(wasp::VALUE, "value", $1);
    }

function_name : POSITION_FN
    {
        $$ = interpreter.push_leaf(wasp::DECL, "position", $1);
    }
    | LAST_FN
    {
        $$ = interpreter.push_leaf(wasp::DECL, "last", $1);
    }
    | COUNT_FN
    {
        $$ = interpreter.push_leaf(wasp::DECL, "count", $1);
    }
    | CONTAINS_FN
    {
        $$ = interpreter.push_leaf(wasp::DECL, "contains", $1);
    }
    | STARTS_WITH_FN
    {
        $$ = interpreter.push_leaf(wasp::DECL, "starts-with", $1);
    }
    | NOT_FN
    {
        $$ = interpreter.push_leaf(wasp::DECL, "not", $1);
    }

function_arguments : predicate_expression
    | predicate_expression comma predicate_expression
    {
        $$ = interpreter.push_parent(wasp::EXPRESSION, "arguments",
                                     {$1, $2, $3});
    }

function_call : function_name lparen rparen
    {
        $$ = interpreter.push_parent(wasp::FUNCTION, interpreter.name($1),
                                     {$1, $2, $3});
    }
    | function_name lparen function_arguments rparen
    {
        $$ = interpreter.push_parent(wasp::FUNCTION, interpreter.name($1),
                                     {$1, $2, $3, $4});
    }

predicate_primary : numeric_value
    | string_value
    | predicate_relative_path
    | function_call
    | lparen predicate_expression rparen
    {
        $$ = interpreter.push_parent(wasp::PARENTHESIS, "value",
                                     {$1, $2, $3});
    }

predicate_power : predicate_primary
    | predicate_primary exponent predicate_power
    {
        $$ = interpreter.push_parent(wasp::EXPONENT, "value", {$1, $2, $3});
    }

predicate_unary : predicate_power
    | minus predicate_unary
    {
        $$ = interpreter.push_parent(wasp::MINUS, "value", {$1, $2});
    }
    | unary_not predicate_unary
    {
        $$ = interpreter.push_parent(wasp::UNARY_NOT, "value", {$1, $2});
    }

predicate_multiplicative : predicate_unary
    | predicate_multiplicative multiply predicate_unary
    {
        $$ = interpreter.push_parent(wasp::MULTIPLY, "value", {$1, $2, $3});
    }
    | predicate_multiplicative divide predicate_unary
    {
        $$ = interpreter.push_parent(wasp::DIVIDE, "value", {$1, $2, $3});
    }
    | predicate_multiplicative modulus predicate_unary
    {
        $$ = interpreter.push_parent(wasp::MODULUS, "value", {$1, $2, $3});
    }

predicate_additive : predicate_multiplicative
    | predicate_additive plus predicate_multiplicative
    {
        $$ = interpreter.push_parent(wasp::PLUS, "value", {$1, $2, $3});
    }
    | predicate_additive minus predicate_multiplicative
    {
        $$ = interpreter.push_parent(wasp::MINUS, "value", {$1, $2, $3});
    }

predicate_comparison : predicate_additive
    | predicate_additive comparison_operator predicate_additive
    {
        if (interpreter.type($1) == wasp::DECL)
        {
            std::string name = wasp::strip_quotes(interpreter.data($1));
            $$ = interpreter.push_parent(wasp::KEYED_VALUE, name.c_str(),
                                         {$1, $2, $3});
        }
        else
        {
            $$ = interpreter.push_parent(interpreter.type($2), "value",
                                         {$1, $2, $3});
        }
    }

predicate_and : predicate_comparison
    | predicate_and and predicate_comparison
    {
        $$ = interpreter.push_parent(wasp::WASP_AND, "value", {$1, $2, $3});
    }

predicate_or : predicate_and
    | predicate_or or predicate_and
    {
        $$ = interpreter.push_parent(wasp::WASP_OR, "value", {$1, $2, $3});
    }

predicate_expression : predicate_or

predicate_content : index_range
    | predicate_expression
    {
        if (interpreter.type($1) == wasp::VALUE &&
            interpreter.node_token_type($1) == wasp::INTEGER)
        {
            interpreter.set_name($1, "int");
            interpreter.set_type($1, wasp::INTEGER);
            $$ = interpreter.push_parent(wasp::INDEX, "I", {$1});
        }
        else
        {
            $$ = $1;
        }
    }

selection : absolute_path | relative_path

intersection_expression : selection
    | intersection_expression intersect_operator selection
    {
        $$ = interpreter.push_parent(wasp::INTERSECT, "intersect",
                                     {$1, $2, $3});
    }
    | intersection_expression except_operator selection
    {
        $$ = interpreter.push_parent(wasp::EXCEPT, "except", {$1, $2, $3});
    }

union_expression : intersection_expression
    | union_expression union_operator intersection_expression
    {
        $$ = interpreter.push_parent(wasp::UNION, "union", {$1, $2, $3});
    }

selection_expression : union_expression

start : selection_expression
    {
        interpreter.push_staged_child($1);
    }

%%

void wasp::SIRENParser::error(const SIRENParser::location_type& location,
                              const std::string& message)
{
    interpreter.error_diagnostic() << location << ": " << message << std::endl;
}
