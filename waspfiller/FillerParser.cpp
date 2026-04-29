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
#line 1 "Filler.bison"
 /*** C/C++ Declarations ***/

#include <stdio.h>
#include <vector>

#line 47 "FillerParser.cpp"


#include "FillerParser.hpp"

// Second part of user prologue.
#line 107 "Filler.bison"


#include "FillerInterpreter.h"
#include "FillerLexer.h"

/* this "connects" the Filler parser in the interpreter to the flex FillerLexer class
 * object. it defines the yylex() function call to pull the next token from the
 * current lexer object of the interpreter context. */
#undef yylex
#define yylex lexer->lex


#line 66 "FillerParser.cpp"



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

#line 34 "Filler.bison"
namespace wasp {
#line 160 "FillerParser.cpp"

  /// Build a parser object.
  FillerParser::FillerParser (class AbstractInterpreter& interpreter_yyarg, std::istream &input_stream_yyarg, std::shared_ptr<class FillerLexerImpl> lexer_yyarg)
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

  FillerParser::~FillerParser ()
  {}

  FillerParser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/

  // basic_symbol.
  template <typename Base>
  FillerParser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value (that.value)
    , location (that.location)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  FillerParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_MOVE_REF (location_type) l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  FillerParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_RVREF (value_type) v, YY_RVREF (location_type) l)
    : Base (t)
    , value (YY_MOVE (v))
    , location (YY_MOVE (l))
  {}


  template <typename Base>
  FillerParser::symbol_kind_type
  FillerParser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  FillerParser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  FillerParser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    value = YY_MOVE (s.value);
    location = YY_MOVE (s.location);
  }

  // by_kind.
  FillerParser::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  FillerParser::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  FillerParser::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  FillerParser::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  void
  FillerParser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  FillerParser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  FillerParser::symbol_kind_type
  FillerParser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  FillerParser::symbol_kind_type
  FillerParser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }



  // by_state.
  FillerParser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  FillerParser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  FillerParser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  FillerParser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  FillerParser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  FillerParser::symbol_kind_type
  FillerParser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  FillerParser::stack_symbol_type::stack_symbol_type ()
  {}

  FillerParser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.value), YY_MOVE (that.location))
  {
#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  FillerParser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.value), YY_MOVE (that.location))
  {
    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  FillerParser::stack_symbol_type&
  FillerParser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    return *this;
  }

  FillerParser::stack_symbol_type&
  FillerParser::stack_symbol_type::operator= (stack_symbol_type& that)
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
  FillerParser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    YY_USE (yysym.kind ());
  }

#if YYDEBUG
  template <typename Base>
  void
  FillerParser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
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
  FillerParser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  FillerParser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  FillerParser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  FillerParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  FillerParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  FillerParser::debug_level_type
  FillerParser::debug_level () const
  {
    return yydebug_;
  }

  void
  FillerParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  FillerParser::state_type
  FillerParser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  FillerParser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  FillerParser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  FillerParser::operator() ()
  {
    return parse ();
  }

  int
  FillerParser::parse ()
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
#line 47 "Filler.bison"
{
    // initialize the initial location object
    yyla.location.begin.filename = yyla.location.end.filename = &interpreter.stream_name();
    yyla.location.begin.line = yyla.location.end.line = interpreter.start_line();
    yyla.location.begin.column = yyla.location.end.column = interpreter.start_column();
    lexer = std::make_shared<FillerLexerImpl>(interpreter,&input_stream);
    //lexer->set_debug(true);
    //this->set_debug_level(1);
}

#line 512 "FillerParser.cpp"


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
  case 2: // count: "count"
#line 124 "Filler.bison"
{
    size_t token_index = ((yystack_[0].value.token_index));(yylhs.value.node_index) = interpreter.push_leaf(wasp::INTEGER,"count",token_index);
}
#line 652 "FillerParser.cpp"
    break;

  case 3: // integer: "integer"
#line 129 "Filler.bison"
{
    size_t token_index = ((yystack_[0].value.token_index));(yylhs.value.node_index) = interpreter.push_leaf(wasp::INTEGER,"value",token_index);
}
#line 660 "FillerParser.cpp"
    break;

  case 4: // real: "real"
#line 133 "Filler.bison"
{
    size_t token_index = ((yystack_[0].value.token_index));(yylhs.value.node_index) = interpreter.push_leaf(wasp::REAL,"value",token_index);
}
#line 668 "FillerParser.cpp"
    break;

  case 5: // repeat: count "repeat" value
#line 137 "Filler.bison"
        {
            (yylhs.value.node_index) = interpreter.push_parent(wasp::REPEAT
                                       ,"repeat"
                                       ,{(yystack_[2].value.node_index),(yystack_[0].value.node_index)});
        }
#line 678 "FillerParser.cpp"
    break;

  case 6: // alternate: count "alternate" value
#line 143 "Filler.bison"
        {
            (yylhs.value.node_index) = interpreter.push_parent(wasp::ALTERNATE
                                       ,"alternate"
                                       ,{(yystack_[2].value.node_index),(yystack_[0].value.node_index)});
        }
#line 688 "FillerParser.cpp"
    break;

  case 7: // fill: "fill" value
#line 150 "Filler.bison"
        {
            (yylhs.value.node_index) = interpreter.push_parent(wasp::FILL
                                       ,"fill"
                                       ,{(yystack_[0].value.node_index)});
        }
#line 698 "FillerParser.cpp"
    break;

  case 8: // address: "address" integer
#line 157 "Filler.bison"
        {
            (yylhs.value.node_index) = interpreter.push_parent(wasp::ADDRESS
                                       ,"addr"
                                       ,{(yystack_[0].value.node_index)});
        }
#line 708 "FillerParser.cpp"
    break;

  case 9: // move: count "move"
#line 164 "Filler.bison"
        {            
            (yylhs.value.node_index) = interpreter.push_parent(wasp::MOVE
                                       ,"move"
                                       ,{(yystack_[1].value.node_index)});
        }
#line 718 "FillerParser.cpp"
    break;

  case 10: // previous_repeat: count "previous repeat" integer
#line 171 "Filler.bison"
        {            
            (yylhs.value.node_index) = interpreter.push_parent(wasp::PREVIOUS_REPEAT
                                       ,"prepeat"
                                       ,{(yystack_[2].value.node_index), (yystack_[0].value.node_index)});
        }
#line 728 "FillerParser.cpp"
    break;

  case 11: // previous_repeat: "previous repeat" integer
#line 178 "Filler.bison"
      {
          (yylhs.value.node_index) = interpreter.push_parent(wasp::PREVIOUS_REPEAT
                                       ,"prepeat"
                                       ,{(yystack_[0].value.node_index)});
      }
#line 738 "FillerParser.cpp"
    break;

  case 12: // repeat_zero: count "repeat zero"
#line 185 "Filler.bison"
    {
        (yylhs.value.node_index) = interpreter.push_parent(wasp::REPEAT_ZERO
                                       ,"zrepeat"
                                       ,{(yystack_[1].value.node_index)});
    }
#line 748 "FillerParser.cpp"
    break;

  case 13: // previous_alternate: count "previous alternate" integer
#line 192 "Filler.bison"
        {
            (yylhs.value.node_index) = interpreter.push_parent(wasp::PREVIOUS_ALTERNATE
                                       ,"palternate"
                                       ,{(yystack_[2].value.node_index), (yystack_[0].value.node_index)});
        }
#line 758 "FillerParser.cpp"
    break;

  case 14: // previous_alternate: "previous alternate" integer
#line 199 "Filler.bison"
      {
          (yylhs.value.node_index) = interpreter.push_parent(wasp::PREVIOUS_ALTERNATE
                                       ,"palternate"
                                       ,{(yystack_[0].value.node_index)});
      }
#line 768 "FillerParser.cpp"
    break;

  case 15: // back_previous: count "back previous" integer
#line 205 "Filler.bison"
        {
            (yylhs.value.node_index) = interpreter.push_parent(wasp::BACK_PREVIOUS
                                       ,"bprevious"
                                       ,{(yystack_[2].value.node_index), (yystack_[0].value.node_index)});
        }
#line 778 "FillerParser.cpp"
    break;

  case 16: // back_previous: "back previous" integer
#line 212 "Filler.bison"
      {
            (yylhs.value.node_index) = interpreter.push_parent(wasp::BACK_PREVIOUS
                                       ,"bprevious"
                                       ,{(yystack_[0].value.node_index)});
      }
#line 788 "FillerParser.cpp"
    break;

  case 17: // linear_interpolate: count "linear interpolate" value value
#line 219 "Filler.bison"
        {
            (yylhs.value.node_index) = interpreter.push_parent(wasp::LINEAR_INTERPOLATE
                            ,"lin_interp"
                            ,{(yystack_[3].value.node_index), (yystack_[1].value.node_index), (yystack_[0].value.node_index)});
        }
#line 798 "FillerParser.cpp"
    break;

  case 18: // log_interpolate: count "log interpolate" value value
#line 226 "Filler.bison"
        {
            (yylhs.value.node_index) = interpreter.push_parent(wasp::LOG_INTERPOLATE
                            ,"log_interp"
                            ,{(yystack_[3].value.node_index), (yystack_[1].value.node_index), (yystack_[0].value.node_index)});
        }
#line 808 "FillerParser.cpp"
    break;

  case 19: // value: integer
#line 233 "Filler.bison"
            {
                (yylhs.value.node_index) = (yystack_[0].value.node_index);
            }
#line 816 "FillerParser.cpp"
    break;

  case 20: // value: real
#line 237 "Filler.bison"
            {
                (yylhs.value.node_index) = (yystack_[0].value.node_index);
            }
#line 824 "FillerParser.cpp"
    break;

  case 22: // start: start value
#line 243 "Filler.bison"
                     {
            interpreter.push_staged_child((yystack_[0].value.node_index));
        }
#line 832 "FillerParser.cpp"
    break;

  case 23: // start: start repeat
#line 247 "Filler.bison"
        {
            interpreter.push_staged_child((yystack_[0].value.node_index));
        }
#line 840 "FillerParser.cpp"
    break;

  case 24: // start: start alternate
#line 251 "Filler.bison"
        {
            interpreter.push_staged_child((yystack_[0].value.node_index));
        }
#line 848 "FillerParser.cpp"
    break;

  case 25: // start: start fill
#line 255 "Filler.bison"
        {
            interpreter.push_staged_child((yystack_[0].value.node_index));
        }
#line 856 "FillerParser.cpp"
    break;

  case 26: // start: start address
#line 259 "Filler.bison"
        {
            interpreter.push_staged_child((yystack_[0].value.node_index));
        }
#line 864 "FillerParser.cpp"
    break;

  case 27: // start: start move
#line 263 "Filler.bison"
        {
            interpreter.push_staged_child((yystack_[0].value.node_index));
        }
#line 872 "FillerParser.cpp"
    break;

  case 28: // start: start previous_repeat
#line 267 "Filler.bison"
        {
            interpreter.push_staged_child((yystack_[0].value.node_index));
        }
#line 880 "FillerParser.cpp"
    break;

  case 29: // start: start previous_alternate
#line 271 "Filler.bison"
        {
            interpreter.push_staged_child((yystack_[0].value.node_index));
        }
#line 888 "FillerParser.cpp"
    break;

  case 30: // start: start back_previous
#line 275 "Filler.bison"
        {
            interpreter.push_staged_child((yystack_[0].value.node_index));
        }
#line 896 "FillerParser.cpp"
    break;

  case 31: // start: start linear_interpolate
#line 279 "Filler.bison"
        {
            interpreter.push_staged_child((yystack_[0].value.node_index));
        }
#line 904 "FillerParser.cpp"
    break;

  case 32: // start: start log_interpolate
#line 283 "Filler.bison"
        {
            interpreter.push_staged_child((yystack_[0].value.node_index));
        }
#line 912 "FillerParser.cpp"
    break;

  case 33: // start: start repeat_zero
#line 287 "Filler.bison"
        {
            interpreter.push_staged_child((yystack_[0].value.node_index));
        }
#line 920 "FillerParser.cpp"
    break;


#line 924 "FillerParser.cpp"

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
  FillerParser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  FillerParser::yytnamerr_ (const char *yystr)
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
  FillerParser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // FillerParser::context.
  FillerParser::context::context (const FillerParser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  FillerParser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
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
  FillerParser::yy_syntax_error_arguments_ (const context& yyctx,
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
  FillerParser::yysyntax_error_ (const context& yyctx) const
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


  const signed char FillerParser::yypact_ninf_ = -11;

  const signed char FillerParser::yytable_ninf_ = -1;

  const signed char
  FillerParser::yypact_[] =
  {
       6,   -11,     4,   -11,   -11,   -11,   -11,    10,    10,    10,
       7,    10,    32,   -11,   -11,   -11,   -11,   -11,   -11,   -11,
     -11,   -11,   -11,   -11,   -11,   -11,   -11,   -11,   -11,   -11,
     -11,   -11,     7,    10,   -11,     7,    10,    10,     7,     7,
     -11,   -11,   -11,   -11,   -11,   -11,     7,     7,   -11,   -11
  };

  const signed char
  FillerParser::yydefact_[] =
  {
      21,    34,     0,     1,     3,     2,     4,     0,     0,     0,
       0,     0,     0,    19,    20,    23,    24,    25,    26,    27,
      28,    33,    29,    30,    31,    32,    22,    11,    14,    16,
       7,     8,     0,     0,    12,     0,     0,     0,     0,     0,
       9,     5,    10,     6,    13,    15,     0,     0,    17,    18
  };

  const signed char
  FillerParser::yypgoto_[] =
  {
     -11,   -11,    -6,   -11,   -11,   -11,   -11,   -11,   -11,   -11,
     -11,   -11,   -11,   -11,   -11,   -10,   -11
  };

  const signed char
  FillerParser::yydefgoto_[] =
  {
       0,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     2
  };

  const signed char
  FillerParser::yytable_[] =
  {
      30,    27,    28,    29,     3,    31,     1,     4,     5,     6,
       4,     7,     6,     4,     8,     9,     0,     0,    10,    11,
       0,     0,    41,     0,     0,    43,     0,    42,    46,    47,
      44,    45,     0,     0,     0,     0,    48,    49,    32,    33,
      34,    35,    36,    37,    38,    39,     0,     0,    40
  };

  const signed char
  FillerParser::yycheck_[] =
  {
      10,     7,     8,     9,     0,    11,     0,     3,     4,     5,
       3,     7,     5,     3,    10,    11,    -1,    -1,    14,    15,
      -1,    -1,    32,    -1,    -1,    35,    -1,    33,    38,    39,
      36,    37,    -1,    -1,    -1,    -1,    46,    47,     6,     7,
       8,     9,    10,    11,    12,    13,    -1,    -1,    16
  };

  const signed char
  FillerParser::yystos_[] =
  {
       0,     0,    33,     0,     3,     4,     5,     7,    10,    11,
      14,    15,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    19,    19,    19,
      32,    19,     6,     7,     8,     9,    10,    11,    12,    13,
      16,    32,    19,    32,    19,    19,    32,    32,    32,    32
  };

  const signed char
  FillerParser::yyr1_[] =
  {
       0,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    26,    27,    28,    28,    29,    29,    30,    31,    32,
      32,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33
  };

  const signed char
  FillerParser::yyr2_[] =
  {
       0,     2,     1,     1,     1,     3,     3,     2,     2,     2,
       3,     2,     2,     3,     2,     3,     2,     4,     4,     1,
       1,     0,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     1
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const FillerParser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "\"integer\"",
  "\"count\"", "\"real\"", "\"repeat\"", "\"previous repeat\"",
  "\"repeat zero\"", "\"alternate\"", "\"previous alternate\"",
  "\"back previous\"", "\"linear interpolate\"", "\"log interpolate\"",
  "\"fill\"", "\"address\"", "\"move\"", "$accept", "count", "integer",
  "real", "repeat", "alternate", "fill", "address", "move",
  "previous_repeat", "repeat_zero", "previous_alternate", "back_previous",
  "linear_interpolate", "log_interpolate", "value", "start", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  FillerParser::yyrline_[] =
  {
       0,   123,   123,   128,   132,   136,   142,   149,   156,   163,
     170,   177,   184,   191,   198,   204,   211,   218,   225,   232,
     236,   242,   243,   246,   250,   254,   258,   262,   266,   270,
     274,   278,   282,   286,   290
  };

  void
  FillerParser::yy_stack_print_ () const
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
  FillerParser::yy_reduce_print_ (int yyrule) const
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

  FillerParser::symbol_kind_type
  FillerParser::yytranslate_ (int t) YY_NOEXCEPT
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
      15,    16
    };
    // Last valid token kind.
    const int code_max = 271;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

#line 34 "Filler.bison"
} // wasp
#line 1469 "FillerParser.cpp"

#line 296 "Filler.bison"
 /*** Additional Code ***/
namespace wasp{
void FillerParser::error(const FillerParser::location_type& l,
                           const std::string& m)
{
    interpreter.error_diagnostic()<<l<<" : "<<m<<"."<<std::endl;
}
} // end of namespace
