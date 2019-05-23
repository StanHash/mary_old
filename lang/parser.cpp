// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

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


// First part of user declarations.

#line 37 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.cpp" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "parser.hpp"

// User implementation prologue.

#line 51 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.cpp" // lalr1.cc:412
// Unqualified %code blocks.
#line 133 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy" // lalr1.cc:413


namespace yy { parser::symbol_type yylex(ParseCtx& ctx); }


#line 59 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.cpp" // lalr1.cc:413


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



// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

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
      *yycdebug_ << std::endl;                  \
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
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace yy {
#line 126 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.cpp" // lalr1.cc:479

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
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
              // Fall through.
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


  /// Build a parser object.
  parser::parser (ParseCtx& ctx_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      ctx (ctx_yyarg)
  {}

  parser::~parser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  inline
  parser::by_state::by_state ()
    : state (empty_state)
  {}

  inline
  parser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  parser::by_state::clear ()
  {
    state = empty_state;
  }

  inline
  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  inline
  parser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  parser::symbol_number_type
  parser::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  inline
  parser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  parser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s)
  {
      switch (that.type_get ())
    {
      case 18: // function_decl
        value.move< mary::Function > (that.value);
        break;

      case 20: // function_decl_param
        value.move< mary::TypedName > (that.value);
        break;

      case 4: // INTEGER
        value.move< std::int32_t > (that.value);
        break;

      case 3: // IDENTIFIER
        value.move< std::string > (that.value);
        break;

      case 19: // function_decl_params
        value.move< std::vector<mary::TypedName> > (that.value);
        break;

      case 21: // typename
        value.move< unsigned > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

  inline
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
      switch (that.type_get ())
    {
      case 18: // function_decl
        value.copy< mary::Function > (that.value);
        break;

      case 20: // function_decl_param
        value.copy< mary::TypedName > (that.value);
        break;

      case 4: // INTEGER
        value.copy< std::int32_t > (that.value);
        break;

      case 3: // IDENTIFIER
        value.copy< std::string > (that.value);
        break;

      case 19: // function_decl_params
        value.copy< std::vector<mary::TypedName> > (that.value);
        break;

      case 21: // typename
        value.copy< unsigned > (that.value);
        break;

      default:
        break;
    }

    return *this;
  }


  template <typename Base>
  inline
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " (";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
  parser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  parser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  parser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::parse ()
  {
    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            symbol_type yylookahead (yylex (ctx));
            yyla.move (yylookahead);
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

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
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
        switch (yyr1_[yyn])
    {
      case 18: // function_decl
        yylhs.value.build< mary::Function > ();
        break;

      case 20: // function_decl_param
        yylhs.value.build< mary::TypedName > ();
        break;

      case 4: // INTEGER
        yylhs.value.build< std::int32_t > ();
        break;

      case 3: // IDENTIFIER
        yylhs.value.build< std::string > ();
        break;

      case 19: // function_decl_params
        yylhs.value.build< std::vector<mary::TypedName> > ();
        break;

      case 21: // typename
        yylhs.value.build< unsigned > ();
        break;

      default:
        break;
    }



      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 5:
#line 148 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy" // lalr1.cc:859
    { ctx.result.functions.push_back(std::move(yystack_[0].value.as< mary::Function > ())); }
#line 563 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.cpp" // lalr1.cc:859
    break;

  case 6:
#line 153 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy" // lalr1.cc:859
    {
        yylhs.value.as< mary::Function > () = {
            /* return type */ (unsigned) (yystack_[7].value.as< unsigned > ()),
            /* parameters  */ std::move(yystack_[4].value.as< std::vector<mary::TypedName> > ()),
            /* name        */ std::move(yystack_[6].value.as< std::string > ()),
            /* identifier  */ (unsigned) (yystack_[1].value.as< std::int32_t > ())
        };
    }
#line 576 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.cpp" // lalr1.cc:859
    break;

  case 7:
#line 164 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy" // lalr1.cc:859
    { yylhs.value.as< std::vector<mary::TypedName> > () = {}; }
#line 582 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.cpp" // lalr1.cc:859
    break;

  case 8:
#line 165 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy" // lalr1.cc:859
    { yylhs.value.as< std::vector<mary::TypedName> > () = { std::move(yystack_[0].value.as< mary::TypedName > ()) }; }
#line 588 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.cpp" // lalr1.cc:859
    break;

  case 9:
#line 166 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy" // lalr1.cc:859
    { yylhs.value.as< std::vector<mary::TypedName> > () = std::move(yystack_[2].value.as< std::vector<mary::TypedName> > ()); yylhs.value.as< std::vector<mary::TypedName> > ().push_back(std::move(yystack_[0].value.as< mary::TypedName > ())); }
#line 594 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.cpp" // lalr1.cc:859
    break;

  case 10:
#line 170 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy" // lalr1.cc:859
    { yylhs.value.as< mary::TypedName > () = { yystack_[0].value.as< unsigned > (), {} }; }
#line 600 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.cpp" // lalr1.cc:859
    break;

  case 11:
#line 171 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy" // lalr1.cc:859
    { yylhs.value.as< mary::TypedName > () = { yystack_[1].value.as< unsigned > (), yystack_[0].value.as< std::string > () }; }
#line 606 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.cpp" // lalr1.cc:859
    break;

  case 12:
#line 175 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy" // lalr1.cc:859
    { yylhs.value.as< unsigned > () = mary::get_named_type(yystack_[0].value.as< std::string > ()); }
#line 612 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.cpp" // lalr1.cc:859
    break;


#line 616 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.cpp" // lalr1.cc:859
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
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
        error (yysyntax_error_ (yystack_[0].state, yyla));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
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

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }


      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what());
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

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
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (!yyla.empty ())
      {
        int yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char parser::yypact_ninf_ = -13;

  const signed char parser::yytable_ninf_ = -1;

  const signed char
  parser::yypact_[] =
  {
     -12,   -13,     3,    -5,    -5,   -13,     1,   -13,   -13,     4,
       5,     1,    -6,   -13,     6,    -2,     1,   -13,     7,   -13,
      -1,   -13
  };

  const unsigned char
  parser::yydefact_[] =
  {
       4,     4,     0,     2,     3,     1,     0,     5,    12,     0,
       0,     7,     0,     8,    10,     0,     0,    11,     0,     9,
       0,     6
  };

  const signed char
  parser::yypgoto_[] =
  {
     -13,   -13,    11,   -13,   -13,    -3,     8
  };

  const signed char
  parser::yydefgoto_[] =
  {
      -1,     2,     3,     7,    12,    13,    14
  };

  const unsigned char
  parser::yytable_[] =
  {
      15,    16,     1,     5,     8,     6,    18,    10,    21,    17,
      11,    20,     4,    19,     9
  };

  const unsigned char
  parser::yycheck_[] =
  {
       6,     7,    14,     0,     3,    10,     8,     3,     9,     3,
       5,     4,     1,    16,     6
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    14,    16,    17,    17,     0,    10,    18,     3,    21,
       3,     5,    19,    20,    21,     6,     7,     3,     8,    20,
       4,     9
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    15,    16,    16,    17,    17,    18,    19,    19,    19,
      20,    20,    21
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     1,     2,     0,     2,     9,     0,     1,     3,
       1,     2,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "IDENTIFIER", "INTEGER",
  "\"(\"", "\")\"", "\",\"", "\"=\"", "\";\"", "\"function\"", "\"true\"",
  "\"false\"", "\"script\"", "\"declarations only\"", "$accept", "file",
  "declarations", "function_decl", "function_decl_params",
  "function_decl_param", "typename", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned char
  parser::yyrline_[] =
  {
       0,   142,   142,   143,   147,   148,   152,   164,   165,   166,
     170,   171,   175
  };

  // Print the state stack on the debug stream.
  void
  parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG



} // yy
#line 988 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.cpp" // lalr1.cc:1167
#line 178 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy" // lalr1.cc:1168


template<unsigned Base>
static std::int32_t lex_int_simple(const char* start, const char* end)
{
	std::int32_t result { 0 };

	while (start != end)
		result = ((*start++) - '0') + (result * Base);

	return result;
}

static std::int32_t lex_int_hex(const char* start, const char* end)
{
	std::int32_t result { 0 };

	while (start != end)
	{
		
#line 201 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.yy"
{
	char yych;
	yych = *start;
	switch (yych) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':	goto yy4;
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':	goto yy6;
	default:	goto yy2;
	}
yy2:
	++start;
#line 205 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy"
	{ result = (result * 16) + (start[-1] - '0'); continue; }
#line 224 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.yy"
yy4:
	++start;
#line 207 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy"
	{ result = (result * 16) + 10 + (start[-1] - 'A'); continue; }
#line 229 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.yy"
yy6:
	++start;
#line 206 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy"
	{ result = (result * 16) + 10 + (start[-1] - 'a'); continue; }
#line 234 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.yy"
}
#line 208 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy"

	}

	return result;
}

yy::parser::symbol_type yy::yylex(ParseCtx& ctx)
{
	if (!ctx.guardEmitted)
	{
		ctx.guardEmitted = true;

		if (ctx.config.noScript)
			return yy::parser::make_DECLARATIONS_ONLY();
	}

	ctx.scanner.token = ctx.scanner.cursor;

	
#line 256 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.yy"
{
	char yych;
	unsigned int yyaccept = 0;
	if ((ctx.scanner.limit - ctx.scanner.cursor) < 9) if (!ctx.scanner.fill(9)) throw false;
	yych = *ctx.scanner.cursor;
	switch (yych) {
	case 0x00:	goto yy10;
	case '\t':
	case '\n':
	case '\v':
	case '\r':
	case ' ':	goto yy14;
	case '(':	goto yy17;
	case ')':	goto yy19;
	case ',':	goto yy21;
	case '/':	goto yy23;
	case '0':	goto yy24;
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy26;
	case ';':	goto yy28;
	case '=':	goto yy30;
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '_':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy32;
	case 'f':	goto yy35;
	case 's':	goto yy36;
	case 't':	goto yy37;
	default:	goto yy12;
	}
yy10:
	++ctx.scanner.cursor;
#line 241 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy"
	{ return yy::parser::make_EOF(); }
#line 344 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.yy"
yy12:
	++ctx.scanner.cursor;
yy13:
#line 240 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy"
	{ throw std::runtime_error("ugh"); }
#line 350 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.yy"
yy14:
	yyaccept = 0;
	ctx.scanner.marker = ++ctx.scanner.cursor;
	if (ctx.scanner.limit <= ctx.scanner.cursor) if (!ctx.scanner.fill(1)) throw false;
	yych = *ctx.scanner.cursor;
	switch (yych) {
	case '\t':
	case '\n':
	case '\v':
	case '\r':
	case ' ':	goto yy14;
	case '/':	goto yy38;
	default:	goto yy16;
	}
yy16:
#line 250 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy"
	{ return yylex(ctx); }
#line 368 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.yy"
yy17:
	++ctx.scanner.cursor;
#line 292 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy"
	{ return yy::parser::make_LPAREN(); }
#line 373 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.yy"
yy19:
	++ctx.scanner.cursor;
#line 293 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy"
	{ return yy::parser::make_RPAREN(); }
#line 378 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.yy"
yy21:
	++ctx.scanner.cursor;
#line 294 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy"
	{ return yy::parser::make_COMMA(); }
#line 383 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.yy"
yy23:
	yyaccept = 1;
	yych = *(ctx.scanner.marker = ++ctx.scanner.cursor);
	switch (yych) {
	case '*':	goto yy40;
	case '/':	goto yy42;
	default:	goto yy13;
	}
yy24:
	yyaccept = 2;
	yych = *(ctx.scanner.marker = ++ctx.scanner.cursor);
	switch (yych) {
	case 'B':
	case 'b':	goto yy44;
	case 'O':
	case 'o':	goto yy45;
	case 'X':
	case 'x':	goto yy46;
	default:	goto yy27;
	}
yy25:
#line 275 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy"
	{
			return yy::parser::make_INTEGER(lex_int_simple<10>(ctx.scanner.token, ctx.scanner.cursor));
		}
#line 409 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.yy"
yy26:
	++ctx.scanner.cursor;
	if (ctx.scanner.limit <= ctx.scanner.cursor) if (!ctx.scanner.fill(1)) throw false;
	yych = *ctx.scanner.cursor;
yy27:
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy26;
	default:	goto yy25;
	}
yy28:
	++ctx.scanner.cursor;
#line 295 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy"
	{ return yy::parser::make_SEMICOLON(); }
#line 432 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.yy"
yy30:
	++ctx.scanner.cursor;
#line 296 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy"
	{ return yy::parser::make_EQUAL(); }
#line 437 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.yy"
yy32:
	++ctx.scanner.cursor;
	if (ctx.scanner.limit <= ctx.scanner.cursor) if (!ctx.scanner.fill(1)) throw false;
	yych = *ctx.scanner.cursor;
yy33:
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '_':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy32;
	default:	goto yy34;
	}
yy34:
#line 303 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy"
	{
			return yy::parser::make_IDENTIFIER(std::string(ctx.scanner.token, ctx.scanner.cursor));
		}
#line 514 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.yy"
yy35:
	yych = *++ctx.scanner.cursor;
	switch (yych) {
	case 'a':	goto yy47;
	case 'u':	goto yy48;
	default:	goto yy33;
	}
yy36:
	yych = *++ctx.scanner.cursor;
	switch (yych) {
	case 'c':	goto yy49;
	default:	goto yy33;
	}
yy37:
	yych = *++ctx.scanner.cursor;
	switch (yych) {
	case 'r':	goto yy50;
	default:	goto yy33;
	}
yy38:
	++ctx.scanner.cursor;
	if (ctx.scanner.limit <= ctx.scanner.cursor) if (!ctx.scanner.fill(1)) throw false;
	yych = *ctx.scanner.cursor;
	switch (yych) {
	case '*':	goto yy40;
	case '/':	goto yy42;
	default:	goto yy39;
	}
yy39:
	ctx.scanner.cursor = ctx.scanner.marker;
	switch (yyaccept) {
	case 0: 	goto yy16;
	case 1: 	goto yy13;
	default:	goto yy25;
	}
yy40:
	++ctx.scanner.cursor;
	if (ctx.scanner.limit <= ctx.scanner.cursor) if (!ctx.scanner.fill(1)) throw false;
	yych = *ctx.scanner.cursor;
	switch (yych) {
	case '*':	goto yy51;
	default:	goto yy40;
	}
yy42:
	++ctx.scanner.cursor;
	if (ctx.scanner.limit <= ctx.scanner.cursor) if (!ctx.scanner.fill(1)) throw false;
	yych = *ctx.scanner.cursor;
	switch (yych) {
	case '\n':	goto yy14;
	default:	goto yy42;
	}
yy44:
	yych = *++ctx.scanner.cursor;
	switch (yych) {
	case '0':
	case '1':	goto yy52;
	default:	goto yy39;
	}
yy45:
	yych = *++ctx.scanner.cursor;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':	goto yy55;
	default:	goto yy39;
	}
yy46:
	yych = *++ctx.scanner.cursor;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':	goto yy58;
	default:	goto yy39;
	}
yy47:
	yych = *++ctx.scanner.cursor;
	switch (yych) {
	case 'l':	goto yy61;
	default:	goto yy33;
	}
yy48:
	yych = *++ctx.scanner.cursor;
	switch (yych) {
	case 'n':	goto yy62;
	default:	goto yy33;
	}
yy49:
	yych = *++ctx.scanner.cursor;
	switch (yych) {
	case 'r':	goto yy63;
	default:	goto yy33;
	}
yy50:
	yych = *++ctx.scanner.cursor;
	switch (yych) {
	case 'u':	goto yy64;
	default:	goto yy33;
	}
yy51:
	++ctx.scanner.cursor;
	if (ctx.scanner.limit <= ctx.scanner.cursor) if (!ctx.scanner.fill(1)) throw false;
	yych = *ctx.scanner.cursor;
	switch (yych) {
	case '/':	goto yy14;
	default:	goto yy40;
	}
yy52:
	++ctx.scanner.cursor;
	if (ctx.scanner.limit <= ctx.scanner.cursor) if (!ctx.scanner.fill(1)) throw false;
	yych = *ctx.scanner.cursor;
	switch (yych) {
	case '0':
	case '1':	goto yy52;
	default:	goto yy54;
	}
yy54:
#line 260 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy"
	{
			return yy::parser::make_INTEGER(lex_int_simple<2>(ctx.scanner.token + 2, ctx.scanner.cursor));
		}
#line 659 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.yy"
yy55:
	++ctx.scanner.cursor;
	if (ctx.scanner.limit <= ctx.scanner.cursor) if (!ctx.scanner.fill(1)) throw false;
	yych = *ctx.scanner.cursor;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':	goto yy55;
	default:	goto yy57;
	}
yy57:
#line 265 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy"
	{
			return yy::parser::make_INTEGER(lex_int_simple<8>(ctx.scanner.token + 2, ctx.scanner.cursor));
		}
#line 680 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.yy"
yy58:
	++ctx.scanner.cursor;
	if (ctx.scanner.limit <= ctx.scanner.cursor) if (!ctx.scanner.fill(1)) throw false;
	yych = *ctx.scanner.cursor;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':	goto yy58;
	default:	goto yy60;
	}
yy60:
#line 270 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy"
	{
			return yy::parser::make_INTEGER(lex_int_hex(ctx.scanner.token + 2, ctx.scanner.cursor));
		}
#line 715 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.yy"
yy61:
	yych = *++ctx.scanner.cursor;
	switch (yych) {
	case 's':	goto yy65;
	default:	goto yy33;
	}
yy62:
	yych = *++ctx.scanner.cursor;
	switch (yych) {
	case 'c':	goto yy66;
	default:	goto yy33;
	}
yy63:
	yych = *++ctx.scanner.cursor;
	switch (yych) {
	case 'i':	goto yy67;
	default:	goto yy33;
	}
yy64:
	yych = *++ctx.scanner.cursor;
	switch (yych) {
	case 'e':	goto yy68;
	default:	goto yy33;
	}
yy65:
	yych = *++ctx.scanner.cursor;
	switch (yych) {
	case 'e':	goto yy70;
	default:	goto yy33;
	}
yy66:
	yych = *++ctx.scanner.cursor;
	switch (yych) {
	case 't':	goto yy72;
	default:	goto yy33;
	}
yy67:
	yych = *++ctx.scanner.cursor;
	switch (yych) {
	case 'p':	goto yy73;
	default:	goto yy33;
	}
yy68:
	++ctx.scanner.cursor;
	switch ((yych = *ctx.scanner.cursor)) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '_':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy32;
	default:	goto yy69;
	}
yy69:
#line 286 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy"
	{ return yy::parser::make_TRUE(); }
#line 829 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.yy"
yy70:
	++ctx.scanner.cursor;
	switch ((yych = *ctx.scanner.cursor)) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '_':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy32;
	default:	goto yy71;
	}
yy71:
#line 287 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy"
	{ return yy::parser::make_FALSE(); }
#line 901 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.yy"
yy72:
	yych = *++ctx.scanner.cursor;
	switch (yych) {
	case 'i':	goto yy74;
	default:	goto yy33;
	}
yy73:
	yych = *++ctx.scanner.cursor;
	switch (yych) {
	case 't':	goto yy75;
	default:	goto yy33;
	}
yy74:
	yych = *++ctx.scanner.cursor;
	switch (yych) {
	case 'o':	goto yy77;
	default:	goto yy33;
	}
yy75:
	++ctx.scanner.cursor;
	switch ((yych = *ctx.scanner.cursor)) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '_':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy32;
	default:	goto yy76;
	}
yy76:
#line 288 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy"
	{ return yy::parser::make_SCRIPT(); }
#line 991 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.yy"
yy77:
	yych = *++ctx.scanner.cursor;
	switch (yych) {
	case 'n':	goto yy78;
	default:	goto yy33;
	}
yy78:
	++ctx.scanner.cursor;
	switch ((yych = *ctx.scanner.cursor)) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '_':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy32;
	default:	goto yy79;
	}
yy79:
#line 285 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy"
	{ return yy::parser::make_FUNCTION(); }
#line 1069 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.yy"
}
#line 306 "/home/stan/Documents/HMHACK/TOOLS/mary/lang/parser.re.yy"

}

void yy::parser::error(const std::string& msg)
{
    std::cerr << msg << std::endl;
}

namespace mary {

ParseResult parse_string(const std::string& string, const ParseConfig& config)
{
	std::istringstream ss(string);
	ParseCtx ctx { ss, {}, config };

	yy::parser parser(ctx);
	parser.parse();

	return std::move(ctx.result);
}

ParseResult parse_file(const std::string& filename, const ParseConfig& config)
{
	std::ifstream fs(filename);

	if (!fs.is_open())
		throw std::runtime_error(std::string("failed to open file: ") + filename); // TODO: better error

	ParseCtx ctx { fs, {}, config };

	yy::parser parser(ctx);
	parser.parse();

	return std::move(ctx.result);
}

} // namespace mary
