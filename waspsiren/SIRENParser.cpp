// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.



// First part of user prologue.
#line 1 "SIRENParser.bison"

#include <stdio.h>
#include <string>
#include <vector>

#line 47 "SIRENParser.cpp"


#include "SIRENParser.hpp"

// Second part of user prologue.
#line 104 "SIRENParser.bison"

#include "SIRENInterpreter.h"
#include "SIRENLexer.h"

#undef yylex
#define yylex lexer->lex

#line 61 "SIRENParser.cpp"



#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 18 "SIRENParser.bison"
namespace wasp {
#line 155 "SIRENParser.cpp"

  /// Build a parser object.
  SIRENParser::SIRENParser (class AbstractInterpreter& interpreter_yyarg, std::istream& input_stream_yyarg, std::shared_ptr<class SIRENLexerImpl> lexer_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      interpreter (interpreter_yyarg),
      input_stream (input_stream_yyarg),
      lexer (lexer_yyarg)
  {}

  SIRENParser::~SIRENParser ()
  {}

  SIRENParser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/

  // basic_symbol.
  template <typename Base>
  SIRENParser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value (that.value)
    , location (that.location)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  SIRENParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_MOVE_REF (location_type) l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  SIRENParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_RVREF (value_type) v, YY_RVREF (location_type) l)
    : Base (t)
    , value (YY_MOVE (v))
    , location (YY_MOVE (l))
  {}


  template <typename Base>
  SIRENParser::symbol_kind_type
  SIRENParser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  SIRENParser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  SIRENParser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    value = YY_MOVE (s.value);
    location = YY_MOVE (s.location);
  }

  // by_kind.
  SIRENParser::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  SIRENParser::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  SIRENParser::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  SIRENParser::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  void
  SIRENParser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  SIRENParser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  SIRENParser::symbol_kind_type
  SIRENParser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  SIRENParser::symbol_kind_type
  SIRENParser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }



  // by_state.
  SIRENParser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  SIRENParser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  SIRENParser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  SIRENParser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  SIRENParser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  SIRENParser::symbol_kind_type
  SIRENParser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  SIRENParser::stack_symbol_type::stack_symbol_type ()
  {}

  SIRENParser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.value), YY_MOVE (that.location))
  {
#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  SIRENParser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.value), YY_MOVE (that.location))
  {
    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  SIRENParser::stack_symbol_type&
  SIRENParser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    return *this;
  }

  SIRENParser::stack_symbol_type&
  SIRENParser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  SIRENParser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    switch (yysym.kind ())
    {
      case symbol_kind::S_predicate_clause: // predicate_clause
#line 102 "SIRENParser.bison"
                    { delete (yysym.value.node_indices); }
#line 369 "SIRENParser.cpp"
        break;

      default:
        break;
    }
  }

#if YYDEBUG
  template <typename Base>
  void
  SIRENParser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  SIRENParser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  SIRENParser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  SIRENParser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  SIRENParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  SIRENParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  SIRENParser::debug_level_type
  SIRENParser::debug_level () const
  {
    return yydebug_;
  }

  void
  SIRENParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  SIRENParser::state_type
  SIRENParser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  SIRENParser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  SIRENParser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  SIRENParser::operator() ()
  {
    return parse ();
  }

  int
  SIRENParser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    // User initialization code.
#line 25 "SIRENParser.bison"
{
    yyla.location.begin.filename = yyla.location.end.filename = &interpreter.stream_name();
    yyla.location.begin.line = yyla.location.end.line = interpreter.start_line();
    yyla.location.begin.column = yyla.location.end.column = interpreter.start_column();
    lexer = std::make_shared<SIRENLexerImpl>(interpreter, &input_stream);
}

#line 514 "SIRENParser.cpp"


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.kind_ = yytranslate_ (yylex (&yyla.value, &yyla.location));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;

      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // any_separator: "//"
#line 115 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::ANY, "A", (yystack_[0].value.token_index));
    }
#line 654 "SIRENParser.cpp"
    break;

  case 3: // separator: "/"
#line 120 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::SEPARATOR, "/", (yystack_[0].value.token_index));
    }
#line 662 "SIRENParser.cpp"
    break;

  case 4: // parent_step: ".."
#line 125 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::PARENT, "P", (yystack_[0].value.token_index));
    }
#line 670 "SIRENParser.cpp"
    break;

  case 5: // lparen: "("
#line 130 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::LPAREN, "(", (yystack_[0].value.token_index));
    }
#line 678 "SIRENParser.cpp"
    break;

  case 6: // rparen: ")"
#line 135 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::RPAREN, ")", (yystack_[0].value.token_index));
    }
#line 686 "SIRENParser.cpp"
    break;

  case 7: // lbracket: "["
#line 140 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::LBRACKET, "[", (yystack_[0].value.token_index));
    }
#line 694 "SIRENParser.cpp"
    break;

  case 8: // rbracket: "]"
#line 145 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::RBRACKET, "]", (yystack_[0].value.token_index));
    }
#line 702 "SIRENParser.cpp"
    break;

  case 9: // comma: ","
#line 150 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::WASP_COMMA, ",", (yystack_[0].value.token_index));
    }
#line 710 "SIRENParser.cpp"
    break;

  case 10: // colon: ":"
#line 155 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::COLON, ":", (yystack_[0].value.token_index));
    }
#line 718 "SIRENParser.cpp"
    break;

  case 11: // plus: "+"
#line 160 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::PLUS, "+", (yystack_[0].value.token_index));
    }
#line 726 "SIRENParser.cpp"
    break;

  case 12: // minus: "-"
#line 165 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::MINUS, "-", (yystack_[0].value.token_index));
    }
#line 734 "SIRENParser.cpp"
    break;

  case 13: // multiply: "*"
#line 170 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::MULTIPLY, "*", (yystack_[0].value.token_index));
    }
#line 742 "SIRENParser.cpp"
    break;

  case 14: // divide: "div"
#line 175 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::DIVIDE, "div", (yystack_[0].value.token_index));
    }
#line 750 "SIRENParser.cpp"
    break;

  case 15: // modulus: "mod"
#line 180 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::MODULUS, "mod", (yystack_[0].value.token_index));
    }
#line 758 "SIRENParser.cpp"
    break;

  case 16: // exponent: "^"
#line 185 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::EXPONENT, "^", (yystack_[0].value.token_index));
    }
#line 766 "SIRENParser.cpp"
    break;

  case 17: // unary_not: "!"
#line 190 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::UNARY_NOT, "!", (yystack_[0].value.token_index));
    }
#line 774 "SIRENParser.cpp"
    break;

  case 18: // eq: "="
#line 195 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::EQ, "==", (yystack_[0].value.token_index));
    }
#line 782 "SIRENParser.cpp"
    break;

  case 19: // neq: "!="
#line 200 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::NEQ, "!=", (yystack_[0].value.token_index));
    }
#line 790 "SIRENParser.cpp"
    break;

  case 20: // gte: ">="
#line 205 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::GTE, ">=", (yystack_[0].value.token_index));
    }
#line 798 "SIRENParser.cpp"
    break;

  case 21: // gt: ">"
#line 210 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::GT, ">", (yystack_[0].value.token_index));
    }
#line 806 "SIRENParser.cpp"
    break;

  case 22: // lte: "<="
#line 215 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::LTE, "<=", (yystack_[0].value.token_index));
    }
#line 814 "SIRENParser.cpp"
    break;

  case 23: // lt: "<"
#line 220 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::LT, "<", (yystack_[0].value.token_index));
    }
#line 822 "SIRENParser.cpp"
    break;

  case 24: // and: "&&"
#line 225 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::WASP_AND, "&&", (yystack_[0].value.token_index));
    }
#line 830 "SIRENParser.cpp"
    break;

  case 25: // or: "||"
#line 230 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::WASP_OR, "||", (yystack_[0].value.token_index));
    }
#line 838 "SIRENParser.cpp"
    break;

  case 26: // comparison_operator: eq
#line 234 "SIRENParser.bison"
                      { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 844 "SIRENParser.cpp"
    break;

  case 27: // comparison_operator: neq
#line 234 "SIRENParser.bison"
                           { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 850 "SIRENParser.cpp"
    break;

  case 28: // comparison_operator: gt
#line 234 "SIRENParser.bison"
                                 { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 856 "SIRENParser.cpp"
    break;

  case 29: // comparison_operator: lt
#line 234 "SIRENParser.bison"
                                      { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 862 "SIRENParser.cpp"
    break;

  case 30: // comparison_operator: gte
#line 234 "SIRENParser.bison"
                                           { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 868 "SIRENParser.cpp"
    break;

  case 31: // comparison_operator: lte
#line 234 "SIRENParser.bison"
                                                 { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 874 "SIRENParser.cpp"
    break;

  case 32: // union_operator: "|"
#line 237 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::UNION, "|", (yystack_[0].value.token_index));
    }
#line 882 "SIRENParser.cpp"
    break;

  case 33: // intersect_operator: "intersect"
#line 242 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::INTERSECT, "intersect", (yystack_[0].value.token_index));
    }
#line 890 "SIRENParser.cpp"
    break;

  case 34: // except_operator: "except"
#line 247 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::EXCEPT, "except", (yystack_[0].value.token_index));
    }
#line 898 "SIRENParser.cpp"
    break;

  case 35: // path_name: "decl"
#line 252 "SIRENParser.bison"
    {
        std::string name = wasp::strip_quotes(interpreter.token_data((yystack_[0].value.token_index)));
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::DECL, name.c_str(), (yystack_[0].value.token_index));
    }
#line 907 "SIRENParser.cpp"
    break;

  case 36: // path_name: "quoted string"
#line 257 "SIRENParser.bison"
    {
        std::string name = wasp::strip_quotes(interpreter.token_data((yystack_[0].value.token_index)));
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::DECL, name.c_str(), (yystack_[0].value.token_index));
    }
#line 916 "SIRENParser.cpp"
    break;

  case 37: // predicate_name: "decl"
#line 263 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::DECL,
                                   interpreter.token_data((yystack_[0].value.token_index)), (yystack_[0].value.token_index));
    }
#line 925 "SIRENParser.cpp"
    break;

  case 38: // wildcard_name: "*"
#line 269 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::DECL, "*", (yystack_[0].value.token_index));
    }
#line 933 "SIRENParser.cpp"
    break;

  case 39: // axis_step: "following-sibling::" path_name
#line 274 "SIRENParser.bison"
    {
        std::size_t axis = interpreter.push_leaf(
            wasp::FOLLOWING_SIBLING, "following-sibling::", (yystack_[1].value.token_index));
        (yylhs.value.node_index) = interpreter.push_parent(wasp::FOLLOWING_SIBLING,
                                     "following-sibling", {axis, (yystack_[0].value.node_index)});
    }
#line 944 "SIRENParser.cpp"
    break;

  case 40: // axis_step: "following-sibling::" wildcard_name
#line 281 "SIRENParser.bison"
    {
        std::size_t axis = interpreter.push_leaf(
            wasp::FOLLOWING_SIBLING, "following-sibling::", (yystack_[1].value.token_index));
        (yylhs.value.node_index) = interpreter.push_parent(wasp::FOLLOWING_SIBLING,
                                     "following-sibling", {axis, (yystack_[0].value.node_index)});
    }
#line 955 "SIRENParser.cpp"
    break;

  case 41: // axis_step: "preceding-sibling::" path_name
#line 288 "SIRENParser.bison"
    {
        std::size_t axis = interpreter.push_leaf(
            wasp::PRECEDING_SIBLING, "preceding-sibling::", (yystack_[1].value.token_index));
        (yylhs.value.node_index) = interpreter.push_parent(wasp::PRECEDING_SIBLING,
                                     "preceding-sibling", {axis, (yystack_[0].value.node_index)});
    }
#line 966 "SIRENParser.cpp"
    break;

  case 42: // axis_step: "preceding-sibling::" wildcard_name
#line 295 "SIRENParser.bison"
    {
        std::size_t axis = interpreter.push_leaf(
            wasp::PRECEDING_SIBLING, "preceding-sibling::", (yystack_[1].value.token_index));
        (yylhs.value.node_index) = interpreter.push_parent(wasp::PRECEDING_SIBLING,
                                     "preceding-sibling", {axis, (yystack_[0].value.node_index)});
    }
#line 977 "SIRENParser.cpp"
    break;

  case 43: // predicate_axis_step: "following-sibling::" predicate_name
#line 303 "SIRENParser.bison"
    {
        std::size_t axis = interpreter.push_leaf(
            wasp::FOLLOWING_SIBLING, "following-sibling::", (yystack_[1].value.token_index));
        (yylhs.value.node_index) = interpreter.push_parent(wasp::FOLLOWING_SIBLING,
                                     "following-sibling", {axis, (yystack_[0].value.node_index)});
    }
#line 988 "SIRENParser.cpp"
    break;

  case 44: // predicate_axis_step: "following-sibling::" wildcard_name
#line 310 "SIRENParser.bison"
    {
        std::size_t axis = interpreter.push_leaf(
            wasp::FOLLOWING_SIBLING, "following-sibling::", (yystack_[1].value.token_index));
        (yylhs.value.node_index) = interpreter.push_parent(wasp::FOLLOWING_SIBLING,
                                     "following-sibling", {axis, (yystack_[0].value.node_index)});
    }
#line 999 "SIRENParser.cpp"
    break;

  case 45: // predicate_axis_step: "preceding-sibling::" predicate_name
#line 317 "SIRENParser.bison"
    {
        std::size_t axis = interpreter.push_leaf(
            wasp::PRECEDING_SIBLING, "preceding-sibling::", (yystack_[1].value.token_index));
        (yylhs.value.node_index) = interpreter.push_parent(wasp::PRECEDING_SIBLING,
                                     "preceding-sibling", {axis, (yystack_[0].value.node_index)});
    }
#line 1010 "SIRENParser.cpp"
    break;

  case 46: // predicate_axis_step: "preceding-sibling::" wildcard_name
#line 324 "SIRENParser.bison"
    {
        std::size_t axis = interpreter.push_leaf(
            wasp::PRECEDING_SIBLING, "preceding-sibling::", (yystack_[1].value.token_index));
        (yylhs.value.node_index) = interpreter.push_parent(wasp::PRECEDING_SIBLING,
                                     "preceding-sibling", {axis, (yystack_[0].value.node_index)});
    }
#line 1021 "SIRENParser.cpp"
    break;

  case 47: // path_base: path_name
#line 331 "SIRENParser.bison"
            { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 1027 "SIRENParser.cpp"
    break;

  case 48: // path_base: wildcard_name
#line 331 "SIRENParser.bison"
                        { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 1033 "SIRENParser.cpp"
    break;

  case 49: // path_base: parent_step
#line 331 "SIRENParser.bison"
                                        { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 1039 "SIRENParser.cpp"
    break;

  case 50: // path_base: axis_step
#line 331 "SIRENParser.bison"
                                                      { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 1045 "SIRENParser.cpp"
    break;

  case 51: // predicate_path_base: predicate_name
#line 332 "SIRENParser.bison"
                      { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 1051 "SIRENParser.cpp"
    break;

  case 52: // predicate_path_base: wildcard_name
#line 332 "SIRENParser.bison"
                                       { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 1057 "SIRENParser.cpp"
    break;

  case 53: // predicate_path_base: parent_step
#line 332 "SIRENParser.bison"
                                                       { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 1063 "SIRENParser.cpp"
    break;

  case 54: // predicate_path_base: predicate_axis_step
#line 333 "SIRENParser.bison"
                      { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 1069 "SIRENParser.cpp"
    break;

  case 55: // predicate_clause: lbracket predicate_content rbracket
#line 336 "SIRENParser.bison"
    {
        (yylhs.value.node_indices) = new std::vector<std::size_t>({(yystack_[2].value.node_index), (yystack_[1].value.node_index), (yystack_[0].value.node_index)});
    }
#line 1077 "SIRENParser.cpp"
    break;

  case 56: // path_step: path_base
#line 340 "SIRENParser.bison"
            { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 1083 "SIRENParser.cpp"
    break;

  case 57: // path_step: path_step predicate_clause
#line 342 "SIRENParser.bison"
    {
        std::size_t predicate = (yystack_[0].value.node_indices)->at(1);
        const char* name = interpreter.type(predicate) == wasp::INDEX
                               ? "ipcs" : "cpcs";
        (yylhs.value.node_index) = interpreter.push_parent(wasp::PREDICATED_CHILD, name,
                                     {(yystack_[1].value.node_index), (yystack_[0].value.node_indices)->at(0), predicate, (yystack_[0].value.node_indices)->at(2)});
        delete (yystack_[0].value.node_indices);
    }
#line 1096 "SIRENParser.cpp"
    break;

  case 58: // predicate_path_step: predicate_path_base
#line 351 "SIRENParser.bison"
                      { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 1102 "SIRENParser.cpp"
    break;

  case 59: // predicate_path_step: predicate_path_step predicate_clause
#line 353 "SIRENParser.bison"
    {
        std::size_t predicate = (yystack_[0].value.node_indices)->at(1);
        const char* name = interpreter.type(predicate) == wasp::INDEX
                               ? "ipcs" : "cpcs";
        (yylhs.value.node_index) = interpreter.push_parent(wasp::PREDICATED_CHILD, name,
                                     {(yystack_[1].value.node_index), (yystack_[0].value.node_indices)->at(0), predicate, (yystack_[0].value.node_indices)->at(2)});
        delete (yystack_[0].value.node_indices);
    }
#line 1115 "SIRENParser.cpp"
    break;

  case 60: // relative_path: path_step
#line 362 "SIRENParser.bison"
                { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 1121 "SIRENParser.cpp"
    break;

  case 61: // relative_path: path_step separator relative_path
#line 364 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_parent(wasp::OBJECT, "O", {(yystack_[2].value.node_index), (yystack_[1].value.node_index), (yystack_[0].value.node_index)});
    }
#line 1129 "SIRENParser.cpp"
    break;

  case 62: // relative_path: path_step any_separator relative_path
#line 368 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_parent(wasp::ANY, "A", {(yystack_[2].value.node_index), (yystack_[1].value.node_index), (yystack_[0].value.node_index)});
    }
#line 1137 "SIRENParser.cpp"
    break;

  case 63: // relative_path: path_step any_separator
#line 372 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_parent(wasp::ANY, "A", {(yystack_[1].value.node_index), (yystack_[0].value.node_index)});
    }
#line 1145 "SIRENParser.cpp"
    break;

  case 64: // predicate_relative_path: predicate_path_step
#line 376 "SIRENParser.bison"
                          { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 1151 "SIRENParser.cpp"
    break;

  case 65: // predicate_relative_path: predicate_path_step separator predicate_relative_path
#line 378 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_parent(wasp::OBJECT, "O", {(yystack_[2].value.node_index), (yystack_[1].value.node_index), (yystack_[0].value.node_index)});
    }
#line 1159 "SIRENParser.cpp"
    break;

  case 66: // predicate_relative_path: predicate_path_step any_separator predicate_relative_path
#line 382 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_parent(wasp::ANY, "A", {(yystack_[2].value.node_index), (yystack_[1].value.node_index), (yystack_[0].value.node_index)});
    }
#line 1167 "SIRENParser.cpp"
    break;

  case 67: // absolute_path: separator
#line 387 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_parent(wasp::DOCUMENT_ROOT, "R", {(yystack_[0].value.node_index)});
    }
#line 1175 "SIRENParser.cpp"
    break;

  case 68: // absolute_path: separator relative_path
#line 391 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_parent(wasp::DOCUMENT_ROOT, "R", {(yystack_[1].value.node_index), (yystack_[0].value.node_index)});
    }
#line 1183 "SIRENParser.cpp"
    break;

  case 69: // absolute_path: any_separator
#line 395 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = (yystack_[0].value.node_index);
    }
#line 1191 "SIRENParser.cpp"
    break;

  case 70: // absolute_path: any_separator relative_path
#line 399 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_parent(wasp::ANY, "A", {(yystack_[1].value.node_index), (yystack_[0].value.node_index)});
    }
#line 1199 "SIRENParser.cpp"
    break;

  case 71: // integer_index: "integer"
#line 404 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::INTEGER, "int", (yystack_[0].value.token_index));
    }
#line 1207 "SIRENParser.cpp"
    break;

  case 72: // index_range: integer_index colon integer_index
#line 409 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_parent(wasp::INDEX, "I", {(yystack_[2].value.node_index), (yystack_[1].value.node_index), (yystack_[0].value.node_index)});
    }
#line 1215 "SIRENParser.cpp"
    break;

  case 73: // index_range: integer_index colon integer_index colon integer_index
#line 413 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_parent(wasp::INDEX, "I",
                                     {(yystack_[4].value.node_index), (yystack_[3].value.node_index), (yystack_[2].value.node_index), (yystack_[1].value.node_index), (yystack_[0].value.node_index)});
    }
#line 1224 "SIRENParser.cpp"
    break;

  case 74: // numeric_value: "integer"
#line 419 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::VALUE, "value", (yystack_[0].value.token_index));
    }
#line 1232 "SIRENParser.cpp"
    break;

  case 75: // numeric_value: "double"
#line 423 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::VALUE, "value", (yystack_[0].value.token_index));
    }
#line 1240 "SIRENParser.cpp"
    break;

  case 76: // string_value: "quoted string"
#line 428 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::VALUE, "value", (yystack_[0].value.token_index));
    }
#line 1248 "SIRENParser.cpp"
    break;

  case 77: // function_name: "position"
#line 433 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::DECL, "position", (yystack_[0].value.token_index));
    }
#line 1256 "SIRENParser.cpp"
    break;

  case 78: // function_name: "last"
#line 437 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::DECL, "last", (yystack_[0].value.token_index));
    }
#line 1264 "SIRENParser.cpp"
    break;

  case 79: // function_name: "count"
#line 441 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::DECL, "count", (yystack_[0].value.token_index));
    }
#line 1272 "SIRENParser.cpp"
    break;

  case 80: // function_name: "contains"
#line 445 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::DECL, "contains", (yystack_[0].value.token_index));
    }
#line 1280 "SIRENParser.cpp"
    break;

  case 81: // function_name: "starts-with"
#line 449 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::DECL, "starts-with", (yystack_[0].value.token_index));
    }
#line 1288 "SIRENParser.cpp"
    break;

  case 82: // function_name: "not"
#line 453 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_leaf(wasp::DECL, "not", (yystack_[0].value.token_index));
    }
#line 1296 "SIRENParser.cpp"
    break;

  case 83: // function_arguments: predicate_expression
#line 457 "SIRENParser.bison"
                     { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 1302 "SIRENParser.cpp"
    break;

  case 84: // function_arguments: predicate_expression comma predicate_expression
#line 459 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_parent(wasp::EXPRESSION, "arguments",
                                     {(yystack_[2].value.node_index), (yystack_[1].value.node_index), (yystack_[0].value.node_index)});
    }
#line 1311 "SIRENParser.cpp"
    break;

  case 85: // function_call: function_name lparen rparen
#line 465 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_parent(wasp::FUNCTION, interpreter.name((yystack_[2].value.node_index)),
                                     {(yystack_[2].value.node_index), (yystack_[1].value.node_index), (yystack_[0].value.node_index)});
    }
#line 1320 "SIRENParser.cpp"
    break;

  case 86: // function_call: function_name lparen function_arguments rparen
#line 470 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_parent(wasp::FUNCTION, interpreter.name((yystack_[3].value.node_index)),
                                     {(yystack_[3].value.node_index), (yystack_[2].value.node_index), (yystack_[1].value.node_index), (yystack_[0].value.node_index)});
    }
#line 1329 "SIRENParser.cpp"
    break;

  case 87: // predicate_primary: numeric_value
#line 475 "SIRENParser.bison"
                    { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 1335 "SIRENParser.cpp"
    break;

  case 88: // predicate_primary: string_value
#line 476 "SIRENParser.bison"
      { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 1341 "SIRENParser.cpp"
    break;

  case 89: // predicate_primary: predicate_relative_path
#line 477 "SIRENParser.bison"
      { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 1347 "SIRENParser.cpp"
    break;

  case 90: // predicate_primary: function_call
#line 478 "SIRENParser.bison"
      { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 1353 "SIRENParser.cpp"
    break;

  case 91: // predicate_primary: lparen predicate_expression rparen
#line 480 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_parent(wasp::PARENTHESIS, "value",
                                     {(yystack_[2].value.node_index), (yystack_[1].value.node_index), (yystack_[0].value.node_index)});
    }
#line 1362 "SIRENParser.cpp"
    break;

  case 92: // predicate_power: predicate_primary
#line 485 "SIRENParser.bison"
                  { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 1368 "SIRENParser.cpp"
    break;

  case 93: // predicate_power: predicate_primary exponent predicate_power
#line 487 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_parent(wasp::EXPONENT, "value", {(yystack_[2].value.node_index), (yystack_[1].value.node_index), (yystack_[0].value.node_index)});
    }
#line 1376 "SIRENParser.cpp"
    break;

  case 94: // predicate_unary: predicate_power
#line 491 "SIRENParser.bison"
                  { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 1382 "SIRENParser.cpp"
    break;

  case 95: // predicate_unary: minus predicate_unary
#line 493 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_parent(wasp::MINUS, "value", {(yystack_[1].value.node_index), (yystack_[0].value.node_index)});
    }
#line 1390 "SIRENParser.cpp"
    break;

  case 96: // predicate_unary: unary_not predicate_unary
#line 497 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_parent(wasp::UNARY_NOT, "value", {(yystack_[1].value.node_index), (yystack_[0].value.node_index)});
    }
#line 1398 "SIRENParser.cpp"
    break;

  case 97: // predicate_multiplicative: predicate_unary
#line 501 "SIRENParser.bison"
                           { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 1404 "SIRENParser.cpp"
    break;

  case 98: // predicate_multiplicative: predicate_multiplicative multiply predicate_unary
#line 503 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_parent(wasp::MULTIPLY, "value", {(yystack_[2].value.node_index), (yystack_[1].value.node_index), (yystack_[0].value.node_index)});
    }
#line 1412 "SIRENParser.cpp"
    break;

  case 99: // predicate_multiplicative: predicate_multiplicative divide predicate_unary
#line 507 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_parent(wasp::DIVIDE, "value", {(yystack_[2].value.node_index), (yystack_[1].value.node_index), (yystack_[0].value.node_index)});
    }
#line 1420 "SIRENParser.cpp"
    break;

  case 100: // predicate_multiplicative: predicate_multiplicative modulus predicate_unary
#line 511 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_parent(wasp::MODULUS, "value", {(yystack_[2].value.node_index), (yystack_[1].value.node_index), (yystack_[0].value.node_index)});
    }
#line 1428 "SIRENParser.cpp"
    break;

  case 101: // predicate_additive: predicate_multiplicative
#line 515 "SIRENParser.bison"
                     { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 1434 "SIRENParser.cpp"
    break;

  case 102: // predicate_additive: predicate_additive plus predicate_multiplicative
#line 517 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_parent(wasp::PLUS, "value", {(yystack_[2].value.node_index), (yystack_[1].value.node_index), (yystack_[0].value.node_index)});
    }
#line 1442 "SIRENParser.cpp"
    break;

  case 103: // predicate_additive: predicate_additive minus predicate_multiplicative
#line 521 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_parent(wasp::MINUS, "value", {(yystack_[2].value.node_index), (yystack_[1].value.node_index), (yystack_[0].value.node_index)});
    }
#line 1450 "SIRENParser.cpp"
    break;

  case 104: // predicate_comparison: predicate_additive
#line 525 "SIRENParser.bison"
                       { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 1456 "SIRENParser.cpp"
    break;

  case 105: // predicate_comparison: predicate_additive comparison_operator predicate_additive
#line 527 "SIRENParser.bison"
    {
        if (interpreter.type((yystack_[2].value.node_index)) == wasp::DECL)
        {
            std::string name = wasp::strip_quotes(interpreter.data((yystack_[2].value.node_index)));
            (yylhs.value.node_index) = interpreter.push_parent(wasp::KEYED_VALUE, name.c_str(),
                                         {(yystack_[2].value.node_index), (yystack_[1].value.node_index), (yystack_[0].value.node_index)});
        }
        else
        {
            (yylhs.value.node_index) = interpreter.push_parent(interpreter.type((yystack_[1].value.node_index)), "value",
                                         {(yystack_[2].value.node_index), (yystack_[1].value.node_index), (yystack_[0].value.node_index)});
        }
    }
#line 1474 "SIRENParser.cpp"
    break;

  case 106: // predicate_and: predicate_comparison
#line 541 "SIRENParser.bison"
                { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 1480 "SIRENParser.cpp"
    break;

  case 107: // predicate_and: predicate_and and predicate_comparison
#line 543 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_parent(wasp::WASP_AND, "value", {(yystack_[2].value.node_index), (yystack_[1].value.node_index), (yystack_[0].value.node_index)});
    }
#line 1488 "SIRENParser.cpp"
    break;

  case 108: // predicate_or: predicate_and
#line 547 "SIRENParser.bison"
               { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 1494 "SIRENParser.cpp"
    break;

  case 109: // predicate_or: predicate_or or predicate_and
#line 549 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_parent(wasp::WASP_OR, "value", {(yystack_[2].value.node_index), (yystack_[1].value.node_index), (yystack_[0].value.node_index)});
    }
#line 1502 "SIRENParser.cpp"
    break;

  case 110: // predicate_expression: predicate_or
#line 553 "SIRENParser.bison"
                       { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 1508 "SIRENParser.cpp"
    break;

  case 111: // predicate_content: index_range
#line 555 "SIRENParser.bison"
                    { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 1514 "SIRENParser.cpp"
    break;

  case 112: // predicate_content: predicate_expression
#line 557 "SIRENParser.bison"
    {
        if (interpreter.type((yystack_[0].value.node_index)) == wasp::VALUE &&
            interpreter.node_token_type((yystack_[0].value.node_index)) == wasp::INTEGER)
        {
            interpreter.set_name((yystack_[0].value.node_index), "int");
            interpreter.set_type((yystack_[0].value.node_index), wasp::INTEGER);
            (yylhs.value.node_index) = interpreter.push_parent(wasp::INDEX, "I", {(yystack_[0].value.node_index)});
        }
        else
        {
            (yylhs.value.node_index) = (yystack_[0].value.node_index);
        }
    }
#line 1532 "SIRENParser.cpp"
    break;

  case 113: // selection: absolute_path
#line 571 "SIRENParser.bison"
            { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 1538 "SIRENParser.cpp"
    break;

  case 114: // selection: relative_path
#line 571 "SIRENParser.bison"
                            { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 1544 "SIRENParser.cpp"
    break;

  case 115: // intersection_expression: selection
#line 573 "SIRENParser.bison"
                          { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 1550 "SIRENParser.cpp"
    break;

  case 116: // intersection_expression: intersection_expression intersect_operator selection
#line 575 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_parent(wasp::INTERSECT, "intersect",
                                     {(yystack_[2].value.node_index), (yystack_[1].value.node_index), (yystack_[0].value.node_index)});
    }
#line 1559 "SIRENParser.cpp"
    break;

  case 117: // intersection_expression: intersection_expression except_operator selection
#line 580 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_parent(wasp::EXCEPT, "except", {(yystack_[2].value.node_index), (yystack_[1].value.node_index), (yystack_[0].value.node_index)});
    }
#line 1567 "SIRENParser.cpp"
    break;

  case 118: // union_expression: intersection_expression
#line 584 "SIRENParser.bison"
                   { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 1573 "SIRENParser.cpp"
    break;

  case 119: // union_expression: union_expression union_operator intersection_expression
#line 586 "SIRENParser.bison"
    {
        (yylhs.value.node_index) = interpreter.push_parent(wasp::UNION, "union", {(yystack_[2].value.node_index), (yystack_[1].value.node_index), (yystack_[0].value.node_index)});
    }
#line 1581 "SIRENParser.cpp"
    break;

  case 120: // selection_expression: union_expression
#line 590 "SIRENParser.bison"
                       { (yylhs.value.node_index) = (yystack_[0].value.node_index); }
#line 1587 "SIRENParser.cpp"
    break;

  case 121: // start: selection_expression
#line 593 "SIRENParser.bison"
    {
        interpreter.push_staged_child((yystack_[0].value.node_index));
    }
#line 1595 "SIRENParser.cpp"
    break;


#line 1599 "SIRENParser.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  SIRENParser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  SIRENParser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

  std::string
  SIRENParser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // SIRENParser::context.
  SIRENParser::context::context (const SIRENParser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  SIRENParser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    const int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        const int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        const int yychecklim = yylast_ - yyn + 1;
        const int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }






  int
  SIRENParser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  SIRENParser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char SIRENParser::yypact_ninf_ = -96;

  const signed char SIRENParser::yytable_ninf_ = -72;

  const short
  SIRENParser::yypact_[] =
  {
       6,   -96,   -96,   -96,   -96,   -15,   -15,   -96,   -96,    -8,
      -8,   -96,   -96,   -96,   -96,   -96,    67,   -96,   -96,   -96,
       2,   -23,   -96,     7,   -96,   -96,   -96,   -96,   -96,   -96,
     -96,    -8,    -8,    98,   -96,   -96,   -96,     6,     6,   -96,
       6,   -96,   -96,   -96,   -96,   -96,   -96,    -2,    -2,   -96,
     -96,   -96,   -96,   -96,   -96,     8,   -96,   -96,   -96,   -96,
     122,   122,   122,   -96,   -96,   -96,   -96,    67,   -96,    24,
     -96,   -96,   -96,    47,   -96,    40,   -96,   -96,    55,   191,
     -96,    51,    52,   -96,    76,   -96,   -96,     2,   -96,   -96,
     -96,   -96,   -96,    83,   -96,   -96,   170,   170,   -96,   -96,
      53,    74,   -96,   146,   -96,   -96,   -96,   122,   122,   122,
     -96,   -96,   -96,   -96,   -96,   -96,   -96,   122,   122,   -96,
     -96,   -96,   -96,   -96,   -96,   122,   -96,   122,   -96,   122,
     -96,   -96,   -96,   -96,   -96,   -96,   -96,    24,   -96,    83,
      89,   -96,   -96,   -96,   -96,    55,    55,     4,   -96,    51,
      53,   -96,   -96,   122,   -96,   -96
  };

  const signed char
  SIRENParser::yydefact_[] =
  {
       0,     2,     4,    38,     3,     0,     0,    35,    36,    69,
      67,    49,    47,    48,    50,    56,    60,   114,   113,   115,
     118,   120,   121,     0,    39,    40,    41,    42,    70,    68,
       7,    63,     0,     0,    57,    33,    34,     0,     0,    32,
       0,     1,    62,    61,    12,     5,    17,     0,     0,    77,
      78,    79,    80,    81,    82,    74,    75,    37,    76,    53,
       0,     0,     0,    51,    52,    54,    58,    64,    89,     0,
     111,    87,    88,     0,    90,    92,    94,    97,   101,   104,
     106,   108,   110,   112,     0,   116,   117,   119,    43,    44,
      45,    46,    74,     0,    95,    96,     0,     0,    59,    10,
       0,     0,    16,     0,    13,    14,    15,     0,     0,     0,
      20,    23,    21,    22,    19,    18,    11,     0,     0,    26,
      27,    30,    28,    31,    29,     0,    24,     0,    25,     0,
       8,    55,     6,    91,    66,    65,    71,    72,    85,     0,
      83,    93,    98,    99,   100,   102,   103,   105,   107,   109,
       0,    86,     9,     0,    73,    84
  };

  const signed char
  SIRENParser::yypgoto_[] =
  {
     -96,   -14,   -13,    35,    26,   -89,   -96,   -96,   -96,   -37,
     -96,   -78,   -96,   -96,   -96,   -96,   -96,   -96,   -96,   -96,
     -96,   -96,   -96,   -96,   -96,   -96,   -96,   -96,   -96,    34,
       9,    11,   -96,   -96,   -96,   -96,    50,   -96,   -96,    54,
      -9,   -96,   -95,   -96,   -96,   -96,   -96,   -96,   -96,   -96,
      17,   -47,   -35,   -24,    -6,    -7,   -96,   -60,   -96,    59,
      84,   -96,   -96,   -96
  };

  const unsigned char
  SIRENParser::yydefgoto_[] =
  {
       0,     9,    10,    59,    60,   133,    33,   131,   153,   100,
     117,    61,   107,   108,   109,   103,    62,   119,   120,   121,
     122,   123,   124,   127,   129,   125,    40,    37,    38,    12,
      63,    64,    14,    65,    15,    66,    34,    16,    67,    17,
      68,    18,    69,    70,    71,    72,    73,   139,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    19,
      20,    21,    22,    23
  };

  const short
  SIRENParser::yytable_[] =
  {
      93,   118,    31,    32,    39,   137,     3,    41,    44,     1,
       2,    13,   138,     3,    94,    95,    25,    27,   -71,     3,
      13,    13,     5,     6,     2,     7,     8,     3,   116,     4,
      35,    36,     7,     8,    99,    11,     5,     6,    57,    24,
      26,   140,    13,    13,    11,    11,     7,     8,    13,    13,
     151,    13,    45,    96,    97,   154,    88,    90,    89,    91,
     142,   143,   144,    28,    29,   102,    11,    11,   126,   118,
       1,   128,    11,    11,    30,    11,   104,   105,    44,    45,
     132,   106,   145,   146,   130,    42,    43,   134,   135,   132,
       4,   136,     2,   155,    46,     3,    85,    86,   152,   101,
     150,   147,    44,    45,    47,    48,    49,    50,    51,    52,
      53,    54,    92,    56,    57,    58,     2,    98,    46,     3,
     141,   148,   149,     0,    87,     0,    44,    45,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
       2,     0,    46,     3,     0,     0,     0,     0,     0,     0,
       0,    45,    47,    48,    49,    50,    51,    52,    53,    54,
      92,    56,    57,    58,     2,     0,     0,     3,     0,     0,
       0,     0,     0,     0,     0,     0,    47,    48,    49,    50,
      51,    52,    53,    54,    92,    56,    57,    58,     2,     0,
       0,     3,     0,     0,     0,    44,     0,     0,     0,     0,
      47,    48,   110,   111,   112,   113,   114,   115,     0,     0,
      57,     0,     0,     0,     0,   116
  };

  const short
  SIRENParser::yycheck_[] =
  {
      60,    79,    16,    16,    27,   100,    21,     0,     4,     3,
      18,     0,   101,    21,    61,    62,     5,     6,    10,    21,
       9,    10,    30,    31,    18,    40,    41,    21,    24,    23,
      28,    29,    40,    41,    10,     0,    30,    31,    40,     5,
       6,   101,    31,    32,     9,    10,    40,    41,    37,    38,
     139,    40,     5,    67,    67,   150,    47,    48,    47,    48,
     107,   108,   109,     9,    10,    25,    31,    32,    17,   147,
       3,    19,    37,    38,     7,    40,    21,    22,     4,     5,
       6,    26,   117,   118,     8,    31,    32,    96,    97,     6,
      23,    38,    18,   153,    20,    21,    37,    38,     9,    73,
     137,   125,     4,     5,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    18,    67,    20,    21,
     103,   127,   129,    -1,    40,    -1,     4,     5,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      18,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     5,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    18,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    18,    -1,
      -1,    21,    -1,    -1,    -1,     4,    -1,    -1,    -1,    -1,
      30,    31,    11,    12,    13,    14,    15,    16,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    24
  };

  const signed char
  SIRENParser::yystos_[] =
  {
       0,     3,    18,    21,    23,    30,    31,    40,    41,    43,
      44,    45,    71,    73,    74,    76,    79,    81,    83,   101,
     102,   103,   104,   105,    71,    73,    71,    73,    81,    81,
       7,    43,    44,    48,    78,    28,    29,    69,    70,    27,
      68,     0,    81,    81,     4,     5,    20,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    45,
      46,    53,    58,    72,    73,    75,    77,    80,    82,    84,
      85,    86,    87,    88,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   101,   102,    72,    73,
      72,    73,    38,    99,    93,    93,    43,    44,    78,    10,
      51,    46,    25,    57,    21,    22,    26,    54,    55,    56,
      11,    12,    13,    14,    15,    16,    24,    52,    53,    59,
      60,    61,    62,    63,    64,    67,    17,    65,    19,    66,
       8,    49,     6,    47,    82,    82,    38,    84,    47,    89,
      99,    92,    93,    93,    93,    94,    94,    95,    96,    97,
      51,    47,     9,    50,    84,    99
  };

  const signed char
  SIRENParser::yyr1_[] =
  {
       0,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    67,    67,    67,
      67,    67,    68,    69,    70,    71,    71,    72,    73,    74,
      74,    74,    74,    75,    75,    75,    75,    76,    76,    76,
      76,    77,    77,    77,    77,    78,    79,    79,    80,    80,
      81,    81,    81,    81,    82,    82,    82,    83,    83,    83,
      83,    84,    85,    85,    86,    86,    87,    88,    88,    88,
      88,    88,    88,    89,    89,    90,    90,    91,    91,    91,
      91,    91,    92,    92,    93,    93,    93,    94,    94,    94,
      94,    95,    95,    95,    96,    96,    97,    97,    98,    98,
      99,   100,   100,   101,   101,   102,   102,   102,   103,   103,
     104,   105
  };

  const signed char
  SIRENParser::yyr2_[] =
  {
       0,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     2,     2,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     2,     1,     2,
       1,     3,     3,     2,     1,     3,     3,     1,     2,     1,
       2,     1,     3,     5,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     4,     1,     1,     1,
       1,     3,     1,     3,     1,     2,     2,     1,     3,     3,
       3,     1,     3,     3,     1,     3,     1,     3,     1,     3,
       1,     1,     1,     1,     1,     1,     3,     3,     1,     3,
       1,     1
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const SIRENParser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "\"//\"", "\"-\"",
  "\"(\"", "\")\"", "\"[\"", "\"]\"", "\",\"", "\":\"", "\">=\"", "\"<\"",
  "\">\"", "\"<=\"", "\"!=\"", "\"=\"", "\"&&\"", "\"..\"", "\"||\"",
  "\"!\"", "\"*\"", "\"div\"", "\"/\"", "\"+\"", "\"^\"", "\"mod\"",
  "\"|\"", "\"intersect\"", "\"except\"", "\"following-sibling::\"",
  "\"preceding-sibling::\"", "\"position\"", "\"last\"", "\"count\"",
  "\"contains\"", "\"starts-with\"", "\"not\"", "\"integer\"",
  "\"double\"", "\"decl\"", "\"quoted string\"", "$accept",
  "any_separator", "separator", "parent_step", "lparen", "rparen",
  "lbracket", "rbracket", "comma", "colon", "plus", "minus", "multiply",
  "divide", "modulus", "exponent", "unary_not", "eq", "neq", "gte", "gt",
  "lte", "lt", "and", "or", "comparison_operator", "union_operator",
  "intersect_operator", "except_operator", "path_name", "predicate_name",
  "wildcard_name", "axis_step", "predicate_axis_step", "path_base",
  "predicate_path_base", "predicate_clause", "path_step",
  "predicate_path_step", "relative_path", "predicate_relative_path",
  "absolute_path", "integer_index", "index_range", "numeric_value",
  "string_value", "function_name", "function_arguments", "function_call",
  "predicate_primary", "predicate_power", "predicate_unary",
  "predicate_multiplicative", "predicate_additive", "predicate_comparison",
  "predicate_and", "predicate_or", "predicate_expression",
  "predicate_content", "selection", "intersection_expression",
  "union_expression", "selection_expression", "start", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  SIRENParser::yyrline_[] =
  {
       0,   114,   114,   119,   124,   129,   134,   139,   144,   149,
     154,   159,   164,   169,   174,   179,   184,   189,   194,   199,
     204,   209,   214,   219,   224,   229,   234,   234,   234,   234,
     234,   234,   236,   241,   246,   251,   256,   262,   268,   273,
     280,   287,   294,   302,   309,   316,   323,   331,   331,   331,
     331,   332,   332,   332,   333,   335,   340,   341,   351,   352,
     362,   363,   367,   371,   376,   377,   381,   386,   390,   394,
     398,   403,   408,   412,   418,   422,   427,   432,   436,   440,
     444,   448,   452,   457,   458,   464,   469,   475,   476,   477,
     478,   479,   485,   486,   491,   492,   496,   501,   502,   506,
     510,   515,   516,   520,   525,   526,   541,   542,   547,   548,
     553,   555,   556,   571,   571,   573,   574,   579,   584,   585,
     590,   592
  };

  void
  SIRENParser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  SIRENParser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  SIRENParser::symbol_kind_type
  SIRENParser::yytranslate_ (int t) YY_NOEXCEPT
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41
    };
    // Last valid token kind.
    const int code_max = 296;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

#line 18 "SIRENParser.bison"
} // wasp
#line 2265 "SIRENParser.cpp"

#line 597 "SIRENParser.bison"


void wasp::SIRENParser::error(const SIRENParser::location_type& location,
                              const std::string& message)
{
    interpreter.error_diagnostic() << location << ": " << message << std::endl;
}
