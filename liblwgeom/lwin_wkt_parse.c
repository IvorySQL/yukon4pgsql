/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         wkt_yyparse
#define yylex           wkt_yylex
#define yyerror         wkt_yyerror
#define yydebug         wkt_yydebug
#define yynerrs         wkt_yynerrs
#define yylval          wkt_yylval
#define yychar          wkt_yychar
#define yylloc          wkt_yylloc

/* First part of user prologue.  */
#line 1 "lwin_wkt_parse.y"


/* WKT Parser */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lwin_wkt.h"
#include "lwin_wkt_parse.h"
#include "lwgeom_log.h"


/* Prototypes to quiet the compiler */
int wkt_yyparse(void);
void wkt_yyerror(const char *str);
int wkt_yylex(void);


/* Declare the global parser variable */
LWGEOM_PARSER_RESULT global_parser_result;

/* Turn on/off verbose parsing (turn off for production) */
int wkt_yydebug = 0;

/*
* Error handler called by the bison parser. Mostly we will be
* catching our own errors and filling out the message and errlocation
* from WKT_ERROR in the grammar, but we keep this one
* around just in case.
*/
void wkt_yyerror(__attribute__((__unused__)) const char *str)
{
	/* If we haven't already set a message and location, let's set one now. */
	if ( ! global_parser_result.message )
	{
		global_parser_result.message = parser_error_messages[PARSER_ERROR_OTHER];
		global_parser_result.errcode = PARSER_ERROR_OTHER;
		global_parser_result.errlocation = wkt_yylloc.last_column;
	}
	LWDEBUGF(4,"%s", str);
}

/**
* Parse a WKT geometry string into an LWGEOM structure. Note that this
* process uses globals and is not re-entrant, so don't call it within itself
* (eg, from within other functions in lwin_wkt.c) or from a threaded program.
* Note that parser_result.wkinput picks up a reference to wktstr.
*/
int lwgeom_parse_wkt(LWGEOM_PARSER_RESULT *parser_result, char *wktstr, int parser_check_flags)
{
	int parse_rv = 0;

	/* Clean up our global parser result. */
	lwgeom_parser_result_init(&global_parser_result);
	/* Work-around possible bug in GNU Bison 3.0.2 resulting in wkt_yylloc
	 * members not being initialized on yyparse() as documented here:
	 * https://www.gnu.org/software/bison/manual/html_node/Location-Type.html
	 * See discussion here:
	 * http://lists.osgeo.org/pipermail/postgis-devel/2014-September/024506.html
	 */
	wkt_yylloc.last_column = wkt_yylloc.last_line = \
		wkt_yylloc.first_column = wkt_yylloc.first_line = 1;

	/* Set the input text string, and parse checks. */
	global_parser_result.wkinput = wktstr;
	global_parser_result.parser_check_flags = parser_check_flags;

	wkt_lexer_init(wktstr); /* Lexer ready */
	parse_rv = wkt_yyparse(); /* Run the parse */
	LWDEBUGF(4,"wkt_yyparse returned %d", parse_rv);
	wkt_lexer_close(); /* Clean up lexer */

	/* A non-zero parser return is an error. */
	if ( parse_rv || global_parser_result.errcode )
	{
		if( ! global_parser_result.errcode )
		{
			global_parser_result.errcode = PARSER_ERROR_OTHER;
			global_parser_result.message = parser_error_messages[PARSER_ERROR_OTHER];
			global_parser_result.errlocation = wkt_yylloc.last_column;
		}
		else if (global_parser_result.geom)
		{
			lwgeom_free(global_parser_result.geom);
			global_parser_result.geom = NULL;
		}

		LWDEBUGF(5, "error returned by wkt_yyparse() @ %d: [%d] '%s'",
		            global_parser_result.errlocation,
		            global_parser_result.errcode,
		            global_parser_result.message);

		/* Copy the global values into the return pointer */
		*parser_result = global_parser_result;
		wkt_yylex_destroy();
		return LW_FAILURE;
	}

	/* Copy the global value into the return pointer */
	*parser_result = global_parser_result;
	wkt_yylex_destroy();
	return LW_SUCCESS;
}

#define WKT_ERROR() { if ( global_parser_result.errcode != 0 ) { YYERROR; } }



#line 187 "lwin_wkt_parse.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "lwin_wkt_parse.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_POINT_TOK = 3,                  /* POINT_TOK  */
  YYSYMBOL_LINESTRING_TOK = 4,             /* LINESTRING_TOK  */
  YYSYMBOL_POLYGON_TOK = 5,                /* POLYGON_TOK  */
  YYSYMBOL_MPOINT_TOK = 6,                 /* MPOINT_TOK  */
  YYSYMBOL_MLINESTRING_TOK = 7,            /* MLINESTRING_TOK  */
  YYSYMBOL_MPOLYGON_TOK = 8,               /* MPOLYGON_TOK  */
  YYSYMBOL_MSURFACE_TOK = 9,               /* MSURFACE_TOK  */
  YYSYMBOL_MCURVE_TOK = 10,                /* MCURVE_TOK  */
  YYSYMBOL_CURVEPOLYGON_TOK = 11,          /* CURVEPOLYGON_TOK  */
  YYSYMBOL_COMPOUNDCURVE_TOK = 12,         /* COMPOUNDCURVE_TOK  */
  YYSYMBOL_CIRCULARSTRING_TOK = 13,        /* CIRCULARSTRING_TOK  */
  YYSYMBOL_COLLECTION_TOK = 14,            /* COLLECTION_TOK  */
  YYSYMBOL_RBRACKET_TOK = 15,              /* RBRACKET_TOK  */
  YYSYMBOL_LBRACKET_TOK = 16,              /* LBRACKET_TOK  */
  YYSYMBOL_COMMA_TOK = 17,                 /* COMMA_TOK  */
  YYSYMBOL_EMPTY_TOK = 18,                 /* EMPTY_TOK  */
  YYSYMBOL_SEMICOLON_TOK = 19,             /* SEMICOLON_TOK  */
  YYSYMBOL_TRIANGLE_TOK = 20,              /* TRIANGLE_TOK  */
  YYSYMBOL_TIN_TOK = 21,                   /* TIN_TOK  */
  YYSYMBOL_POLYHEDRALSURFACE_TOK = 22,     /* POLYHEDRALSURFACE_TOK  */
  YYSYMBOL_ELLIPTICALSTRING_TOK = 23,      /* ELLIPTICALSTRING_TOK  */
  YYSYMBOL_DOUBLE_TOK = 24,                /* DOUBLE_TOK  */
  YYSYMBOL_DIMENSIONALITY_TOK = 25,        /* DIMENSIONALITY_TOK  */
  YYSYMBOL_SRID_TOK = 26,                  /* SRID_TOK  */
  YYSYMBOL_YYACCEPT = 27,                  /* $accept  */
  YYSYMBOL_geometry = 28,                  /* geometry  */
  YYSYMBOL_geometry_no_srid = 29,          /* geometry_no_srid  */
  YYSYMBOL_geometrycollection = 30,        /* geometrycollection  */
  YYSYMBOL_geometry_list = 31,             /* geometry_list  */
  YYSYMBOL_multisurface = 32,              /* multisurface  */
  YYSYMBOL_surface_list = 33,              /* surface_list  */
  YYSYMBOL_tin = 34,                       /* tin  */
  YYSYMBOL_polyhedralsurface = 35,         /* polyhedralsurface  */
  YYSYMBOL_multipolygon = 36,              /* multipolygon  */
  YYSYMBOL_polygon_list = 37,              /* polygon_list  */
  YYSYMBOL_patch_list = 38,                /* patch_list  */
  YYSYMBOL_polygon = 39,                   /* polygon  */
  YYSYMBOL_polygon_untagged = 40,          /* polygon_untagged  */
  YYSYMBOL_patch = 41,                     /* patch  */
  YYSYMBOL_curvepolygon = 42,              /* curvepolygon  */
  YYSYMBOL_curvering_list = 43,            /* curvering_list  */
  YYSYMBOL_curvering = 44,                 /* curvering  */
  YYSYMBOL_patchring_list = 45,            /* patchring_list  */
  YYSYMBOL_ring_list = 46,                 /* ring_list  */
  YYSYMBOL_patchring = 47,                 /* patchring  */
  YYSYMBOL_ring = 48,                      /* ring  */
  YYSYMBOL_compoundcurve = 49,             /* compoundcurve  */
  YYSYMBOL_compound_list = 50,             /* compound_list  */
  YYSYMBOL_multicurve = 51,                /* multicurve  */
  YYSYMBOL_curve_list = 52,                /* curve_list  */
  YYSYMBOL_multilinestring = 53,           /* multilinestring  */
  YYSYMBOL_linestring_list = 54,           /* linestring_list  */
  YYSYMBOL_circularstring = 55,            /* circularstring  */
  YYSYMBOL_linestring = 56,                /* linestring  */
  YYSYMBOL_linestring_untagged = 57,       /* linestring_untagged  */
  YYSYMBOL_triangle_list = 58,             /* triangle_list  */
  YYSYMBOL_triangle = 59,                  /* triangle  */
  YYSYMBOL_triangle_untagged = 60,         /* triangle_untagged  */
  YYSYMBOL_multipoint = 61,                /* multipoint  */
  YYSYMBOL_point_list = 62,                /* point_list  */
  YYSYMBOL_point_untagged = 63,            /* point_untagged  */
  YYSYMBOL_point = 64,                     /* point  */
  YYSYMBOL_ptarray = 65,                   /* ptarray  */
  YYSYMBOL_coordinate = 66,                /* coordinate  */
  YYSYMBOL_ellipse = 67                    /* ellipse  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  84
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   344

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  27
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  41
/* YYNRULES -- Number of rules.  */
#define YYNRULES  142
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  304

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   281


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
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
      25,    26
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   219,   219,   221,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     243,   245,   247,   249,   253,   255,   259,   261,   263,   265,
     269,   271,   273,   275,   277,   279,   283,   285,   287,   289,
     293,   295,   297,   299,   303,   305,   307,   309,   313,   315,
     319,   321,   325,   327,   329,   331,   335,   337,   341,   344,
     346,   348,   350,   354,   356,   360,   361,   362,   363,   364,
     367,   369,   373,   375,   379,   382,   385,   387,   389,   391,
     395,   397,   399,   401,   403,   405,   407,   409,   413,   415,
     417,   419,   423,   425,   427,   429,   431,   433,   435,   437,
     441,   443,   445,   447,   451,   453,   457,   459,   461,   463,
     467,   469,   471,   473,   477,   479,   483,   485,   489,   491,
     493,   495,   499,   503,   505,   507,   509,   513,   515,   519,
     521,   523,   527,   529,   531,   533,   537,   539,   543,   545,
     547,   550,   552
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "POINT_TOK",
  "LINESTRING_TOK", "POLYGON_TOK", "MPOINT_TOK", "MLINESTRING_TOK",
  "MPOLYGON_TOK", "MSURFACE_TOK", "MCURVE_TOK", "CURVEPOLYGON_TOK",
  "COMPOUNDCURVE_TOK", "CIRCULARSTRING_TOK", "COLLECTION_TOK",
  "RBRACKET_TOK", "LBRACKET_TOK", "COMMA_TOK", "EMPTY_TOK",
  "SEMICOLON_TOK", "TRIANGLE_TOK", "TIN_TOK", "POLYHEDRALSURFACE_TOK",
  "ELLIPTICALSTRING_TOK", "DOUBLE_TOK", "DIMENSIONALITY_TOK", "SRID_TOK",
  "$accept", "geometry", "geometry_no_srid", "geometrycollection",
  "geometry_list", "multisurface", "surface_list", "tin",
  "polyhedralsurface", "multipolygon", "polygon_list", "patch_list",
  "polygon", "polygon_untagged", "patch", "curvepolygon", "curvering_list",
  "curvering", "patchring_list", "ring_list", "patchring", "ring",
  "compoundcurve", "compound_list", "multicurve", "curve_list",
  "multilinestring", "linestring_list", "circularstring", "linestring",
  "linestring_untagged", "triangle_list", "triangle", "triangle_untagged",
  "multipoint", "point_list", "point_untagged", "point", "ptarray",
  "coordinate", "ellipse", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-74)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     175,    23,    55,    77,    81,    82,    87,    93,    99,   103,
     107,   113,   143,   149,   174,   204,    36,   -15,    16,   -74,
     -74,   -74,   -74,   -74,   -74,   -74,   -74,   -74,   -74,   -74,
     -74,   -74,   -74,   -74,   -74,   -74,    -7,   -74,    26,    -7,
     -74,    31,     5,   -74,    44,    41,   -74,    54,    59,   -74,
     117,   121,   -74,   148,    17,   -74,   153,    51,   -74,   184,
      14,   -74,   187,    27,   -74,   188,    -7,   -74,   215,   205,
     -74,   216,    18,   -74,   220,    30,   -74,   221,    60,   -74,
     224,    -7,    65,   205,   -74,    58,   176,   -74,    -7,   -74,
     226,    -7,   -74,    -7,   229,   -74,     5,   -74,    -7,   -74,
     230,   -74,   -74,    41,   -74,    -7,   -74,   233,   -74,    59,
     -74,     5,   -74,   234,   -74,   121,   -74,   237,   -74,   -74,
     -74,    17,   -74,   -74,   238,   -74,   -74,   -74,    51,   -74,
     241,   -74,   -74,   -74,   -74,   -74,   -74,    14,   -74,   242,
     -74,   -74,   -74,   -74,    27,   -74,   245,    -7,   -74,   -74,
     246,   205,   -74,    -7,    88,   -74,    92,   249,   -74,    30,
     -74,    97,   250,   -74,    60,   -74,   110,    -7,   -74,    90,
     -74,    -7,   253,   -74,   254,   257,   -74,     5,   258,   105,
     -74,    41,   261,   262,   -74,    59,   265,   266,   -74,   121,
     269,   -74,    17,   270,   -74,    51,   273,   -74,    14,   274,
     -74,    27,   277,   -74,   278,   -74,   205,   281,   282,    -7,
      -7,   -74,    30,   285,    -7,   286,   -74,   -74,    60,   289,
      -7,   123,   106,   -74,   -74,   -74,   -74,   -74,   -74,   -74,
     -74,   -74,   -74,   -74,   -74,   -74,   -74,   -74,   -74,   -74,
     -74,   -74,   -74,   -74,   -74,   -74,   -74,   -74,   -74,   -74,
     -74,   -74,   -74,   -74,   -74,   -74,   -74,   119,   290,   293,
     -74,   -74,   294,   -74,    97,   -74,   -74,   126,    -7,   -74,
     -74,   140,   141,   -74,   -74,    -7,   159,   -74,   -74,   177,
      -7,   136,   207,   295,   146,   197,   296,   297,   291,   292,
     300,   301,   298,   299,   302,   303,   304,   305,   307,   306,
     308,   -74,   310,   -74
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     2,
      18,    13,    15,    16,    12,     8,     9,     7,    14,    11,
       6,     5,    17,    10,     4,    19,     0,   135,     0,     0,
     113,     0,     0,    55,     0,     0,   126,     0,     0,   103,
       0,     0,    47,     0,     0,    29,     0,     0,    91,     0,
       0,    62,     0,     0,    79,     0,     0,   109,     0,     0,
      23,     0,     0,   121,     0,     0,    39,     0,     0,    43,
       0,     0,     0,     0,     1,     0,     0,   137,     0,   134,
       0,     0,   112,     0,     0,    73,     0,    54,     0,   131,
       0,   128,   129,     0,   125,     0,   115,     0,   105,     0,
     102,     0,    57,     0,    49,     0,    46,     0,    33,    35,
      34,     0,    28,    97,     0,    96,    98,    99,     0,    90,
       0,    64,    67,    68,    66,    65,    69,     0,    61,     0,
      84,    85,    86,    87,     0,    78,     0,     0,   108,    25,
       0,     0,    22,     0,     0,   120,     0,     0,   117,     0,
      38,     0,     0,    51,     0,    42,     0,     0,     3,   138,
     132,     0,     0,   110,     0,     0,    52,     0,     0,     0,
     123,     0,     0,     0,   100,     0,     0,     0,    44,     0,
       0,    26,     0,     0,    88,     0,     0,    59,     0,     0,
      76,     0,     0,   106,     0,    20,     0,     0,     0,     0,
       0,    36,     0,     0,     0,     0,    71,    40,     0,     0,
       0,     0,   139,   136,   133,   111,    75,    72,    53,   130,
     127,   124,   114,   104,   101,    56,    48,    45,    30,    32,
      31,    27,    93,    92,    94,    95,    89,    63,    60,    80,
      81,    82,    83,    77,   107,    24,    21,     0,     0,     0,
     116,    37,     0,    58,     0,    50,    41,     0,     0,   140,
     118,     0,     0,    74,    70,     0,     0,   119,   122,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   141,     0,   142
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -74,   -74,     1,   -74,   179,   -74,   206,   -74,   -74,   -74,
     211,   167,   -30,   -35,   115,   -29,   198,   138,   -74,   -73,
      70,   160,   -49,   194,   -74,   212,   -74,   232,   -54,   -50,
     -43,   180,   -74,   130,   -74,   240,   163,   -74,   -37,   -45,
     -48
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    18,   149,    20,   150,    21,   117,    22,    23,    24,
     113,   162,    25,   114,   163,    26,   130,   131,   215,    94,
     216,    95,    27,   139,    28,   124,    29,   107,    30,    31,
     135,   157,    32,   158,    33,   100,   101,    34,    86,    87,
      35
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     102,    19,    90,   125,    83,   108,   133,   126,   123,   140,
     134,   132,   136,   141,   127,   143,    84,    85,     2,   119,
     142,    93,     3,   178,   118,   120,    10,    11,     9,   146,
     105,     2,   106,   111,   153,   112,   166,    16,   187,    36,
      11,    37,    88,   105,    89,   106,   156,    91,    38,    92,
      16,   172,    81,   179,   174,     2,   175,    98,   102,    99,
      96,    82,    97,    10,    11,    85,   108,   105,   183,   106,
     103,    39,   104,    40,   125,   105,   161,   106,   126,   123,
      41,   167,   169,   133,   168,   127,   119,   134,   132,   136,
     140,   118,   120,    42,   141,    43,   143,    45,    48,    46,
      49,   142,    44,    51,   209,    52,    47,    50,   210,    54,
     204,    55,    53,   214,   222,    57,   208,    58,    56,    60,
     229,    61,   221,    63,    59,    64,   223,   220,    62,    66,
     269,    67,    65,   109,   270,   110,   102,   111,    68,   112,
     268,   243,   233,   275,   133,   244,   242,   249,   134,   132,
     136,   250,   245,   252,   236,   277,   278,   239,   251,    69,
     283,    70,   238,   240,   115,    72,   116,    73,    71,   121,
     286,   122,   258,   259,    74,   267,   280,   262,     1,     2,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      75,   170,    76,   171,   281,    13,    14,    15,    16,    77,
     128,    17,   129,   137,   144,   138,   145,   255,     1,     2,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      78,   287,    79,   276,   284,    13,    14,    15,    16,    80,
     279,   147,   151,   148,   152,   282,   154,   159,   155,   160,
     164,   173,   165,   171,   176,   180,   177,   181,   184,   188,
     185,   189,   191,   194,   192,   195,   197,   200,   198,   201,
     203,   205,   171,   206,   211,   217,   212,   218,   224,   225,
     171,   171,   226,   228,   171,   177,   231,   232,   181,   171,
     234,   235,   185,   177,   237,   241,   189,   192,   246,   248,
     195,   198,   253,   254,   201,   171,   256,   257,   206,   171,
     261,   263,   212,   264,   266,   271,   218,   171,   272,   273,
     171,   171,   285,   288,   289,   290,   291,   292,   293,   296,
     297,   301,   294,   295,   300,   303,   190,   193,   298,   299,
     207,   219,   302,   265,   274,   199,   247,   227,   202,   213,
     196,   186,   260,   182,   230
};

static const yytype_int16 yycheck[] =
{
      45,     0,    39,    57,    19,    48,    60,    57,    57,    63,
      60,    60,    60,    63,    57,    63,     0,    24,     4,    54,
      63,    16,     5,    96,    54,    54,    12,    13,    11,    66,
      16,     4,    18,    16,    16,    18,    81,    23,   111,    16,
      13,    18,    16,    16,    18,    18,    16,    16,    25,    18,
      23,    88,    16,    98,    91,     4,    93,    16,   103,    18,
      16,    25,    18,    12,    13,    24,   109,    16,   105,    18,
      16,    16,    18,    18,   128,    16,    16,    18,   128,   128,
      25,    16,    24,   137,    83,   128,   121,   137,   137,   137,
     144,   121,   121,    16,   144,    18,   144,    16,    16,    18,
      18,   144,    25,    16,    16,    18,    25,    25,    16,    16,
     147,    18,    25,    16,    24,    16,   153,    18,    25,    16,
      15,    18,   167,    16,    25,    18,   171,    17,    25,    16,
      24,    18,    25,    16,    15,    18,   181,    16,    25,    18,
      17,   195,   185,    17,   198,   195,   195,   201,   198,   198,
     198,   201,   195,   201,   189,    15,    15,   192,   201,    16,
      24,    18,   192,   192,    16,    16,    18,    18,    25,    16,
      24,    18,   209,   210,    25,   220,    17,   214,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      16,    15,    18,    17,    17,    20,    21,    22,    23,    25,
      16,    26,    18,    16,    16,    18,    18,   206,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      16,    24,    18,   268,    17,    20,    21,    22,    23,    25,
     275,    16,    16,    18,    18,   280,    16,    16,    18,    18,
      16,    15,    18,    17,    15,    15,    17,    17,    15,    15,
      17,    17,    15,    15,    17,    17,    15,    15,    17,    17,
      15,    15,    17,    17,    15,    15,    17,    17,    15,    15,
      17,    17,    15,    15,    17,    17,    15,    15,    17,    17,
      15,    15,    17,    17,    15,    15,    17,    17,    15,    15,
      17,    17,    15,    15,    17,    17,    15,    15,    17,    17,
      15,    15,    17,    17,    15,    15,    17,    17,    15,    15,
      17,    17,    17,    17,    17,    24,    24,    17,    17,    17,
      17,    15,    24,    24,    17,    15,   115,   121,    24,    24,
     151,   164,    24,   218,   264,   137,   198,   177,   144,   159,
     128,   109,   212,   103,   181
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    20,    21,    22,    23,    26,    28,    29,
      30,    32,    34,    35,    36,    39,    42,    49,    51,    53,
      55,    56,    59,    61,    64,    67,    16,    18,    25,    16,
      18,    25,    16,    18,    25,    16,    18,    25,    16,    18,
      25,    16,    18,    25,    16,    18,    25,    16,    18,    25,
      16,    18,    25,    16,    18,    25,    16,    18,    25,    16,
      18,    25,    16,    18,    25,    16,    18,    25,    16,    18,
      25,    16,    25,    19,     0,    24,    65,    66,    16,    18,
      65,    16,    18,    16,    46,    48,    16,    18,    16,    18,
      62,    63,    66,    16,    18,    16,    18,    54,    57,    16,
      18,    16,    18,    37,    40,    16,    18,    33,    39,    40,
      42,    16,    18,    49,    52,    55,    56,    57,    16,    18,
      43,    44,    49,    55,    56,    57,    67,    16,    18,    50,
      55,    56,    57,    67,    16,    18,    65,    16,    18,    29,
      31,    16,    18,    16,    16,    18,    16,    58,    60,    16,
      18,    16,    38,    41,    16,    18,    66,    16,    29,    24,
      15,    17,    65,    15,    65,    65,    15,    17,    46,    66,
      15,    17,    62,    65,    15,    17,    54,    46,    15,    17,
      37,    15,    17,    33,    15,    17,    52,    15,    17,    43,
      15,    17,    50,    15,    65,    15,    17,    31,    65,    16,
      16,    15,    17,    58,    16,    45,    47,    15,    17,    38,
      17,    66,    24,    66,    15,    15,    15,    48,    15,    15,
      63,    15,    15,    57,    15,    15,    40,    15,    39,    40,
      42,    15,    49,    55,    56,    57,    15,    44,    15,    55,
      56,    57,    67,    15,    15,    29,    15,    15,    65,    65,
      60,    15,    65,    15,    17,    41,    15,    66,    17,    24,
      15,    15,    15,    15,    47,    17,    66,    15,    15,    66,
      17,    17,    66,    24,    17,    17,    24,    24,    17,    17,
      24,    24,    17,    17,    24,    24,    17,    17,    24,    24,
      17,    15,    24,    15
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    27,    28,    28,    29,    29,    29,    29,    29,    29,
      29,    29,    29,    29,    29,    29,    29,    29,    29,    29,
      30,    30,    30,    30,    31,    31,    32,    32,    32,    32,
      33,    33,    33,    33,    33,    33,    34,    34,    34,    34,
      35,    35,    35,    35,    36,    36,    36,    36,    37,    37,
      38,    38,    39,    39,    39,    39,    40,    40,    41,    42,
      42,    42,    42,    43,    43,    44,    44,    44,    44,    44,
      45,    45,    46,    46,    47,    48,    49,    49,    49,    49,
      50,    50,    50,    50,    50,    50,    50,    50,    51,    51,
      51,    51,    52,    52,    52,    52,    52,    52,    52,    52,
      53,    53,    53,    53,    54,    54,    55,    55,    55,    55,
      56,    56,    56,    56,    57,    57,    58,    58,    59,    59,
      59,    59,    60,    61,    61,    61,    61,    62,    62,    63,
      63,    63,    64,    64,    64,    64,    65,    65,    66,    66,
      66,    67,    67
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     5,     3,     2,     3,     1,     4,     5,     3,     2,
       3,     3,     3,     1,     1,     1,     4,     5,     3,     2,
       4,     5,     3,     2,     4,     5,     3,     2,     3,     1,
       3,     1,     4,     5,     3,     2,     3,     1,     3,     4,
       5,     3,     2,     3,     1,     1,     1,     1,     1,     1,
       3,     1,     3,     1,     3,     3,     4,     5,     3,     2,
       3,     3,     3,     3,     1,     1,     1,     1,     4,     5,
       3,     2,     3,     3,     3,     3,     1,     1,     1,     1,
       4,     5,     3,     2,     3,     1,     4,     5,     3,     2,
       4,     5,     3,     2,     3,     1,     3,     1,     6,     7,
       3,     2,     5,     4,     5,     3,     2,     3,     1,     1,
       3,     1,     4,     5,     3,     2,     3,     1,     2,     3,
       4,    18,    19
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
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
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yykind)
    {
    case YYSYMBOL_geometry_no_srid: /* geometry_no_srid  */
#line 196 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1552 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_geometrycollection: /* geometrycollection  */
#line 197 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1558 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_geometry_list: /* geometry_list  */
#line 198 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1564 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_multisurface: /* multisurface  */
#line 205 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1570 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_surface_list: /* surface_list  */
#line 183 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1576 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_tin: /* tin  */
#line 212 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1582 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_polyhedralsurface: /* polyhedralsurface  */
#line 211 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1588 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_multipolygon: /* multipolygon  */
#line 204 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1594 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_polygon_list: /* polygon_list  */
#line 184 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1600 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_patch_list: /* patch_list  */
#line 185 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1606 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_polygon: /* polygon  */
#line 208 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1612 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_polygon_untagged: /* polygon_untagged  */
#line 210 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1618 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_patch: /* patch  */
#line 209 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1624 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_curvepolygon: /* curvepolygon  */
#line 194 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1630 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_curvering_list: /* curvering_list  */
#line 181 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1636 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_curvering: /* curvering  */
#line 195 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1642 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_patchring_list: /* patchring_list  */
#line 191 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1648 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_ring_list: /* ring_list  */
#line 190 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1654 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_patchring: /* patchring  */
#line 180 "lwin_wkt_parse.y"
            { ptarray_free(((*yyvaluep).ptarrayvalue)); }
#line 1660 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_ring: /* ring  */
#line 179 "lwin_wkt_parse.y"
            { ptarray_free(((*yyvaluep).ptarrayvalue)); }
#line 1666 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_compoundcurve: /* compoundcurve  */
#line 193 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1672 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_compound_list: /* compound_list  */
#line 189 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1678 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_multicurve: /* multicurve  */
#line 201 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1684 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_curve_list: /* curve_list  */
#line 188 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1690 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_multilinestring: /* multilinestring  */
#line 202 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1696 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_linestring_list: /* linestring_list  */
#line 187 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1702 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_circularstring: /* circularstring  */
#line 192 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1708 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_linestring: /* linestring  */
#line 199 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1714 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_linestring_untagged: /* linestring_untagged  */
#line 200 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1720 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_triangle_list: /* triangle_list  */
#line 182 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1726 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_triangle: /* triangle  */
#line 213 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1732 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_triangle_untagged: /* triangle_untagged  */
#line 214 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1738 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_multipoint: /* multipoint  */
#line 203 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1744 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_point_list: /* point_list  */
#line 186 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1750 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_point_untagged: /* point_untagged  */
#line 207 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1756 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_point: /* point  */
#line 206 "lwin_wkt_parse.y"
            { lwgeom_free(((*yyvaluep).geometryvalue)); }
#line 1762 "lwin_wkt_parse.c"
        break;

    case YYSYMBOL_ptarray: /* ptarray  */
#line 178 "lwin_wkt_parse.y"
            { ptarray_free(((*yyvaluep).ptarrayvalue)); }
#line 1768 "lwin_wkt_parse.c"
        break;

      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* geometry: geometry_no_srid  */
#line 220 "lwin_wkt_parse.y"
                { wkt_parser_geometry_new((yyvsp[0].geometryvalue), SRID_UNKNOWN); WKT_ERROR(); }
#line 2066 "lwin_wkt_parse.c"
    break;

  case 3: /* geometry: SRID_TOK SEMICOLON_TOK geometry_no_srid  */
#line 222 "lwin_wkt_parse.y"
                { wkt_parser_geometry_new((yyvsp[0].geometryvalue), (yyvsp[-2].integervalue)); WKT_ERROR(); }
#line 2072 "lwin_wkt_parse.c"
    break;

  case 4: /* geometry_no_srid: point  */
#line 225 "lwin_wkt_parse.y"
              { (yyval.geometryvalue) = (yyvsp[0].geometryvalue); }
#line 2078 "lwin_wkt_parse.c"
    break;

  case 5: /* geometry_no_srid: linestring  */
#line 226 "lwin_wkt_parse.y"
                   { (yyval.geometryvalue) = (yyvsp[0].geometryvalue); }
#line 2084 "lwin_wkt_parse.c"
    break;

  case 6: /* geometry_no_srid: circularstring  */
#line 227 "lwin_wkt_parse.y"
                       { (yyval.geometryvalue) = (yyvsp[0].geometryvalue); }
#line 2090 "lwin_wkt_parse.c"
    break;

  case 7: /* geometry_no_srid: compoundcurve  */
#line 228 "lwin_wkt_parse.y"
                      { (yyval.geometryvalue) = (yyvsp[0].geometryvalue); }
#line 2096 "lwin_wkt_parse.c"
    break;

  case 8: /* geometry_no_srid: polygon  */
#line 229 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = (yyvsp[0].geometryvalue); }
#line 2102 "lwin_wkt_parse.c"
    break;

  case 9: /* geometry_no_srid: curvepolygon  */
#line 230 "lwin_wkt_parse.y"
                     { (yyval.geometryvalue) = (yyvsp[0].geometryvalue); }
#line 2108 "lwin_wkt_parse.c"
    break;

  case 10: /* geometry_no_srid: multipoint  */
#line 231 "lwin_wkt_parse.y"
                   { (yyval.geometryvalue) = (yyvsp[0].geometryvalue); }
#line 2114 "lwin_wkt_parse.c"
    break;

  case 11: /* geometry_no_srid: multilinestring  */
#line 232 "lwin_wkt_parse.y"
                        { (yyval.geometryvalue) = (yyvsp[0].geometryvalue); }
#line 2120 "lwin_wkt_parse.c"
    break;

  case 12: /* geometry_no_srid: multipolygon  */
#line 233 "lwin_wkt_parse.y"
                     { (yyval.geometryvalue) = (yyvsp[0].geometryvalue); }
#line 2126 "lwin_wkt_parse.c"
    break;

  case 13: /* geometry_no_srid: multisurface  */
#line 234 "lwin_wkt_parse.y"
                     { (yyval.geometryvalue) = (yyvsp[0].geometryvalue); }
#line 2132 "lwin_wkt_parse.c"
    break;

  case 14: /* geometry_no_srid: multicurve  */
#line 235 "lwin_wkt_parse.y"
                   { (yyval.geometryvalue) = (yyvsp[0].geometryvalue); }
#line 2138 "lwin_wkt_parse.c"
    break;

  case 15: /* geometry_no_srid: tin  */
#line 236 "lwin_wkt_parse.y"
            { (yyval.geometryvalue) = (yyvsp[0].geometryvalue); }
#line 2144 "lwin_wkt_parse.c"
    break;

  case 16: /* geometry_no_srid: polyhedralsurface  */
#line 237 "lwin_wkt_parse.y"
                          { (yyval.geometryvalue) = (yyvsp[0].geometryvalue); }
#line 2150 "lwin_wkt_parse.c"
    break;

  case 17: /* geometry_no_srid: triangle  */
#line 238 "lwin_wkt_parse.y"
                 { (yyval.geometryvalue) = (yyvsp[0].geometryvalue); }
#line 2156 "lwin_wkt_parse.c"
    break;

  case 18: /* geometry_no_srid: geometrycollection  */
#line 239 "lwin_wkt_parse.y"
                           { (yyval.geometryvalue) = (yyvsp[0].geometryvalue); }
#line 2162 "lwin_wkt_parse.c"
    break;

  case 19: /* geometry_no_srid: ellipse  */
#line 240 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = (yyvsp[0].geometryvalue); }
#line 2168 "lwin_wkt_parse.c"
    break;

  case 20: /* geometrycollection: COLLECTION_TOK LBRACKET_TOK geometry_list RBRACKET_TOK  */
#line 244 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(COLLECTIONTYPE, (yyvsp[-1].geometryvalue), NULL); WKT_ERROR(); }
#line 2174 "lwin_wkt_parse.c"
    break;

  case 21: /* geometrycollection: COLLECTION_TOK DIMENSIONALITY_TOK LBRACKET_TOK geometry_list RBRACKET_TOK  */
#line 246 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(COLLECTIONTYPE, (yyvsp[-1].geometryvalue), (yyvsp[-3].stringvalue)); WKT_ERROR(); }
#line 2180 "lwin_wkt_parse.c"
    break;

  case 22: /* geometrycollection: COLLECTION_TOK DIMENSIONALITY_TOK EMPTY_TOK  */
#line 248 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(COLLECTIONTYPE, NULL, (yyvsp[-1].stringvalue)); WKT_ERROR(); }
#line 2186 "lwin_wkt_parse.c"
    break;

  case 23: /* geometrycollection: COLLECTION_TOK EMPTY_TOK  */
#line 250 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(COLLECTIONTYPE, NULL, NULL); WKT_ERROR(); }
#line 2192 "lwin_wkt_parse.c"
    break;

  case 24: /* geometry_list: geometry_list COMMA_TOK geometry_no_srid  */
#line 254 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_add_geom((yyvsp[-2].geometryvalue),(yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2198 "lwin_wkt_parse.c"
    break;

  case 25: /* geometry_list: geometry_no_srid  */
#line 256 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_new((yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2204 "lwin_wkt_parse.c"
    break;

  case 26: /* multisurface: MSURFACE_TOK LBRACKET_TOK surface_list RBRACKET_TOK  */
#line 260 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(MULTISURFACETYPE, (yyvsp[-1].geometryvalue), NULL); WKT_ERROR(); }
#line 2210 "lwin_wkt_parse.c"
    break;

  case 27: /* multisurface: MSURFACE_TOK DIMENSIONALITY_TOK LBRACKET_TOK surface_list RBRACKET_TOK  */
#line 262 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(MULTISURFACETYPE, (yyvsp[-1].geometryvalue), (yyvsp[-3].stringvalue)); WKT_ERROR(); }
#line 2216 "lwin_wkt_parse.c"
    break;

  case 28: /* multisurface: MSURFACE_TOK DIMENSIONALITY_TOK EMPTY_TOK  */
#line 264 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(MULTISURFACETYPE, NULL, (yyvsp[-1].stringvalue)); WKT_ERROR(); }
#line 2222 "lwin_wkt_parse.c"
    break;

  case 29: /* multisurface: MSURFACE_TOK EMPTY_TOK  */
#line 266 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(MULTISURFACETYPE, NULL, NULL); WKT_ERROR(); }
#line 2228 "lwin_wkt_parse.c"
    break;

  case 30: /* surface_list: surface_list COMMA_TOK polygon  */
#line 270 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_add_geom((yyvsp[-2].geometryvalue),(yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2234 "lwin_wkt_parse.c"
    break;

  case 31: /* surface_list: surface_list COMMA_TOK curvepolygon  */
#line 272 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_add_geom((yyvsp[-2].geometryvalue),(yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2240 "lwin_wkt_parse.c"
    break;

  case 32: /* surface_list: surface_list COMMA_TOK polygon_untagged  */
#line 274 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_add_geom((yyvsp[-2].geometryvalue),(yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2246 "lwin_wkt_parse.c"
    break;

  case 33: /* surface_list: polygon  */
#line 276 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_new((yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2252 "lwin_wkt_parse.c"
    break;

  case 34: /* surface_list: curvepolygon  */
#line 278 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_new((yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2258 "lwin_wkt_parse.c"
    break;

  case 35: /* surface_list: polygon_untagged  */
#line 280 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_new((yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2264 "lwin_wkt_parse.c"
    break;

  case 36: /* tin: TIN_TOK LBRACKET_TOK triangle_list RBRACKET_TOK  */
#line 284 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(TINTYPE, (yyvsp[-1].geometryvalue), NULL); WKT_ERROR(); }
#line 2270 "lwin_wkt_parse.c"
    break;

  case 37: /* tin: TIN_TOK DIMENSIONALITY_TOK LBRACKET_TOK triangle_list RBRACKET_TOK  */
#line 286 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(TINTYPE, (yyvsp[-1].geometryvalue), (yyvsp[-3].stringvalue)); WKT_ERROR(); }
#line 2276 "lwin_wkt_parse.c"
    break;

  case 38: /* tin: TIN_TOK DIMENSIONALITY_TOK EMPTY_TOK  */
#line 288 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(TINTYPE, NULL, (yyvsp[-1].stringvalue)); WKT_ERROR(); }
#line 2282 "lwin_wkt_parse.c"
    break;

  case 39: /* tin: TIN_TOK EMPTY_TOK  */
#line 290 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(TINTYPE, NULL, NULL); WKT_ERROR(); }
#line 2288 "lwin_wkt_parse.c"
    break;

  case 40: /* polyhedralsurface: POLYHEDRALSURFACE_TOK LBRACKET_TOK patch_list RBRACKET_TOK  */
#line 294 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(POLYHEDRALSURFACETYPE, (yyvsp[-1].geometryvalue), NULL); WKT_ERROR(); }
#line 2294 "lwin_wkt_parse.c"
    break;

  case 41: /* polyhedralsurface: POLYHEDRALSURFACE_TOK DIMENSIONALITY_TOK LBRACKET_TOK patch_list RBRACKET_TOK  */
#line 296 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(POLYHEDRALSURFACETYPE, (yyvsp[-1].geometryvalue), (yyvsp[-3].stringvalue)); WKT_ERROR(); }
#line 2300 "lwin_wkt_parse.c"
    break;

  case 42: /* polyhedralsurface: POLYHEDRALSURFACE_TOK DIMENSIONALITY_TOK EMPTY_TOK  */
#line 298 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(POLYHEDRALSURFACETYPE, NULL, (yyvsp[-1].stringvalue)); WKT_ERROR(); }
#line 2306 "lwin_wkt_parse.c"
    break;

  case 43: /* polyhedralsurface: POLYHEDRALSURFACE_TOK EMPTY_TOK  */
#line 300 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(POLYHEDRALSURFACETYPE, NULL, NULL); WKT_ERROR(); }
#line 2312 "lwin_wkt_parse.c"
    break;

  case 44: /* multipolygon: MPOLYGON_TOK LBRACKET_TOK polygon_list RBRACKET_TOK  */
#line 304 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(MULTIPOLYGONTYPE, (yyvsp[-1].geometryvalue), NULL); WKT_ERROR(); }
#line 2318 "lwin_wkt_parse.c"
    break;

  case 45: /* multipolygon: MPOLYGON_TOK DIMENSIONALITY_TOK LBRACKET_TOK polygon_list RBRACKET_TOK  */
#line 306 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(MULTIPOLYGONTYPE, (yyvsp[-1].geometryvalue), (yyvsp[-3].stringvalue)); WKT_ERROR(); }
#line 2324 "lwin_wkt_parse.c"
    break;

  case 46: /* multipolygon: MPOLYGON_TOK DIMENSIONALITY_TOK EMPTY_TOK  */
#line 308 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(MULTIPOLYGONTYPE, NULL, (yyvsp[-1].stringvalue)); WKT_ERROR(); }
#line 2330 "lwin_wkt_parse.c"
    break;

  case 47: /* multipolygon: MPOLYGON_TOK EMPTY_TOK  */
#line 310 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(MULTIPOLYGONTYPE, NULL, NULL); WKT_ERROR(); }
#line 2336 "lwin_wkt_parse.c"
    break;

  case 48: /* polygon_list: polygon_list COMMA_TOK polygon_untagged  */
#line 314 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_add_geom((yyvsp[-2].geometryvalue),(yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2342 "lwin_wkt_parse.c"
    break;

  case 49: /* polygon_list: polygon_untagged  */
#line 316 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_new((yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2348 "lwin_wkt_parse.c"
    break;

  case 50: /* patch_list: patch_list COMMA_TOK patch  */
#line 320 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_add_geom((yyvsp[-2].geometryvalue),(yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2354 "lwin_wkt_parse.c"
    break;

  case 51: /* patch_list: patch  */
#line 322 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_new((yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2360 "lwin_wkt_parse.c"
    break;

  case 52: /* polygon: POLYGON_TOK LBRACKET_TOK ring_list RBRACKET_TOK  */
#line 326 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_polygon_finalize((yyvsp[-1].geometryvalue), NULL); WKT_ERROR(); }
#line 2366 "lwin_wkt_parse.c"
    break;

  case 53: /* polygon: POLYGON_TOK DIMENSIONALITY_TOK LBRACKET_TOK ring_list RBRACKET_TOK  */
#line 328 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_polygon_finalize((yyvsp[-1].geometryvalue), (yyvsp[-3].stringvalue)); WKT_ERROR(); }
#line 2372 "lwin_wkt_parse.c"
    break;

  case 54: /* polygon: POLYGON_TOK DIMENSIONALITY_TOK EMPTY_TOK  */
#line 330 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_polygon_finalize(NULL, (yyvsp[-1].stringvalue)); WKT_ERROR(); }
#line 2378 "lwin_wkt_parse.c"
    break;

  case 55: /* polygon: POLYGON_TOK EMPTY_TOK  */
#line 332 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_polygon_finalize(NULL, NULL); WKT_ERROR(); }
#line 2384 "lwin_wkt_parse.c"
    break;

  case 56: /* polygon_untagged: LBRACKET_TOK ring_list RBRACKET_TOK  */
#line 336 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = (yyvsp[-1].geometryvalue); }
#line 2390 "lwin_wkt_parse.c"
    break;

  case 57: /* polygon_untagged: EMPTY_TOK  */
#line 338 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_polygon_finalize(NULL, NULL); WKT_ERROR(); }
#line 2396 "lwin_wkt_parse.c"
    break;

  case 58: /* patch: LBRACKET_TOK patchring_list RBRACKET_TOK  */
#line 341 "lwin_wkt_parse.y"
                                                 { (yyval.geometryvalue) = (yyvsp[-1].geometryvalue); }
#line 2402 "lwin_wkt_parse.c"
    break;

  case 59: /* curvepolygon: CURVEPOLYGON_TOK LBRACKET_TOK curvering_list RBRACKET_TOK  */
#line 345 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_curvepolygon_finalize((yyvsp[-1].geometryvalue), NULL); WKT_ERROR(); }
#line 2408 "lwin_wkt_parse.c"
    break;

  case 60: /* curvepolygon: CURVEPOLYGON_TOK DIMENSIONALITY_TOK LBRACKET_TOK curvering_list RBRACKET_TOK  */
#line 347 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_curvepolygon_finalize((yyvsp[-1].geometryvalue), (yyvsp[-3].stringvalue)); WKT_ERROR(); }
#line 2414 "lwin_wkt_parse.c"
    break;

  case 61: /* curvepolygon: CURVEPOLYGON_TOK DIMENSIONALITY_TOK EMPTY_TOK  */
#line 349 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_curvepolygon_finalize(NULL, (yyvsp[-1].stringvalue)); WKT_ERROR(); }
#line 2420 "lwin_wkt_parse.c"
    break;

  case 62: /* curvepolygon: CURVEPOLYGON_TOK EMPTY_TOK  */
#line 351 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_curvepolygon_finalize(NULL, NULL); WKT_ERROR(); }
#line 2426 "lwin_wkt_parse.c"
    break;

  case 63: /* curvering_list: curvering_list COMMA_TOK curvering  */
#line 355 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_curvepolygon_add_ring((yyvsp[-2].geometryvalue),(yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2432 "lwin_wkt_parse.c"
    break;

  case 64: /* curvering_list: curvering  */
#line 357 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_curvepolygon_new((yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2438 "lwin_wkt_parse.c"
    break;

  case 65: /* curvering: linestring_untagged  */
#line 360 "lwin_wkt_parse.y"
                            { (yyval.geometryvalue) = (yyvsp[0].geometryvalue); }
#line 2444 "lwin_wkt_parse.c"
    break;

  case 66: /* curvering: linestring  */
#line 361 "lwin_wkt_parse.y"
                   { (yyval.geometryvalue) = (yyvsp[0].geometryvalue); }
#line 2450 "lwin_wkt_parse.c"
    break;

  case 67: /* curvering: compoundcurve  */
#line 362 "lwin_wkt_parse.y"
                      { (yyval.geometryvalue) = (yyvsp[0].geometryvalue); }
#line 2456 "lwin_wkt_parse.c"
    break;

  case 68: /* curvering: circularstring  */
#line 363 "lwin_wkt_parse.y"
                       { (yyval.geometryvalue) = (yyvsp[0].geometryvalue); }
#line 2462 "lwin_wkt_parse.c"
    break;

  case 69: /* curvering: ellipse  */
#line 364 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = (yyvsp[0].geometryvalue); }
#line 2468 "lwin_wkt_parse.c"
    break;

  case 70: /* patchring_list: patchring_list COMMA_TOK patchring  */
#line 368 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_polygon_add_ring((yyvsp[-2].geometryvalue),(yyvsp[0].ptarrayvalue),'Z'); WKT_ERROR(); }
#line 2474 "lwin_wkt_parse.c"
    break;

  case 71: /* patchring_list: patchring  */
#line 370 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_polygon_new((yyvsp[0].ptarrayvalue),'Z'); WKT_ERROR(); }
#line 2480 "lwin_wkt_parse.c"
    break;

  case 72: /* ring_list: ring_list COMMA_TOK ring  */
#line 374 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_polygon_add_ring((yyvsp[-2].geometryvalue),(yyvsp[0].ptarrayvalue),'2'); WKT_ERROR(); }
#line 2486 "lwin_wkt_parse.c"
    break;

  case 73: /* ring_list: ring  */
#line 376 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_polygon_new((yyvsp[0].ptarrayvalue),'2'); WKT_ERROR(); }
#line 2492 "lwin_wkt_parse.c"
    break;

  case 74: /* patchring: LBRACKET_TOK ptarray RBRACKET_TOK  */
#line 379 "lwin_wkt_parse.y"
                                          { (yyval.ptarrayvalue) = (yyvsp[-1].ptarrayvalue); }
#line 2498 "lwin_wkt_parse.c"
    break;

  case 75: /* ring: LBRACKET_TOK ptarray RBRACKET_TOK  */
#line 382 "lwin_wkt_parse.y"
                                          { (yyval.ptarrayvalue) = (yyvsp[-1].ptarrayvalue); }
#line 2504 "lwin_wkt_parse.c"
    break;

  case 76: /* compoundcurve: COMPOUNDCURVE_TOK LBRACKET_TOK compound_list RBRACKET_TOK  */
#line 386 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(COMPOUNDTYPE, (yyvsp[-1].geometryvalue), NULL); WKT_ERROR(); }
#line 2510 "lwin_wkt_parse.c"
    break;

  case 77: /* compoundcurve: COMPOUNDCURVE_TOK DIMENSIONALITY_TOK LBRACKET_TOK compound_list RBRACKET_TOK  */
#line 388 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(COMPOUNDTYPE, (yyvsp[-1].geometryvalue), (yyvsp[-3].stringvalue)); WKT_ERROR(); }
#line 2516 "lwin_wkt_parse.c"
    break;

  case 78: /* compoundcurve: COMPOUNDCURVE_TOK DIMENSIONALITY_TOK EMPTY_TOK  */
#line 390 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(COMPOUNDTYPE, NULL, (yyvsp[-1].stringvalue)); WKT_ERROR(); }
#line 2522 "lwin_wkt_parse.c"
    break;

  case 79: /* compoundcurve: COMPOUNDCURVE_TOK EMPTY_TOK  */
#line 392 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(COMPOUNDTYPE, NULL, NULL); WKT_ERROR(); }
#line 2528 "lwin_wkt_parse.c"
    break;

  case 80: /* compound_list: compound_list COMMA_TOK circularstring  */
#line 396 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_compound_add_geom((yyvsp[-2].geometryvalue),(yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2534 "lwin_wkt_parse.c"
    break;

  case 81: /* compound_list: compound_list COMMA_TOK linestring  */
#line 398 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_compound_add_geom((yyvsp[-2].geometryvalue),(yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2540 "lwin_wkt_parse.c"
    break;

  case 82: /* compound_list: compound_list COMMA_TOK linestring_untagged  */
#line 400 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_compound_add_geom((yyvsp[-2].geometryvalue),(yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2546 "lwin_wkt_parse.c"
    break;

  case 83: /* compound_list: compound_list COMMA_TOK ellipse  */
#line 402 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_compound_add_geom((yyvsp[-2].geometryvalue),(yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2552 "lwin_wkt_parse.c"
    break;

  case 84: /* compound_list: circularstring  */
#line 404 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_compound_new((yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2558 "lwin_wkt_parse.c"
    break;

  case 85: /* compound_list: linestring  */
#line 406 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_compound_new((yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2564 "lwin_wkt_parse.c"
    break;

  case 86: /* compound_list: linestring_untagged  */
#line 408 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_compound_new((yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2570 "lwin_wkt_parse.c"
    break;

  case 87: /* compound_list: ellipse  */
#line 410 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_compound_new((yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2576 "lwin_wkt_parse.c"
    break;

  case 88: /* multicurve: MCURVE_TOK LBRACKET_TOK curve_list RBRACKET_TOK  */
#line 414 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(MULTICURVETYPE, (yyvsp[-1].geometryvalue), NULL); WKT_ERROR(); }
#line 2582 "lwin_wkt_parse.c"
    break;

  case 89: /* multicurve: MCURVE_TOK DIMENSIONALITY_TOK LBRACKET_TOK curve_list RBRACKET_TOK  */
#line 416 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(MULTICURVETYPE, (yyvsp[-1].geometryvalue), (yyvsp[-3].stringvalue)); WKT_ERROR(); }
#line 2588 "lwin_wkt_parse.c"
    break;

  case 90: /* multicurve: MCURVE_TOK DIMENSIONALITY_TOK EMPTY_TOK  */
#line 418 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(MULTICURVETYPE, NULL, (yyvsp[-1].stringvalue)); WKT_ERROR(); }
#line 2594 "lwin_wkt_parse.c"
    break;

  case 91: /* multicurve: MCURVE_TOK EMPTY_TOK  */
#line 420 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(MULTICURVETYPE, NULL, NULL); WKT_ERROR(); }
#line 2600 "lwin_wkt_parse.c"
    break;

  case 92: /* curve_list: curve_list COMMA_TOK circularstring  */
#line 424 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_add_geom((yyvsp[-2].geometryvalue),(yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2606 "lwin_wkt_parse.c"
    break;

  case 93: /* curve_list: curve_list COMMA_TOK compoundcurve  */
#line 426 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_add_geom((yyvsp[-2].geometryvalue),(yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2612 "lwin_wkt_parse.c"
    break;

  case 94: /* curve_list: curve_list COMMA_TOK linestring  */
#line 428 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_add_geom((yyvsp[-2].geometryvalue),(yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2618 "lwin_wkt_parse.c"
    break;

  case 95: /* curve_list: curve_list COMMA_TOK linestring_untagged  */
#line 430 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_add_geom((yyvsp[-2].geometryvalue),(yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2624 "lwin_wkt_parse.c"
    break;

  case 96: /* curve_list: circularstring  */
#line 432 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_new((yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2630 "lwin_wkt_parse.c"
    break;

  case 97: /* curve_list: compoundcurve  */
#line 434 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_new((yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2636 "lwin_wkt_parse.c"
    break;

  case 98: /* curve_list: linestring  */
#line 436 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_new((yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2642 "lwin_wkt_parse.c"
    break;

  case 99: /* curve_list: linestring_untagged  */
#line 438 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_new((yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2648 "lwin_wkt_parse.c"
    break;

  case 100: /* multilinestring: MLINESTRING_TOK LBRACKET_TOK linestring_list RBRACKET_TOK  */
#line 442 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(MULTILINETYPE, (yyvsp[-1].geometryvalue), NULL); WKT_ERROR(); }
#line 2654 "lwin_wkt_parse.c"
    break;

  case 101: /* multilinestring: MLINESTRING_TOK DIMENSIONALITY_TOK LBRACKET_TOK linestring_list RBRACKET_TOK  */
#line 444 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(MULTILINETYPE, (yyvsp[-1].geometryvalue), (yyvsp[-3].stringvalue)); WKT_ERROR(); }
#line 2660 "lwin_wkt_parse.c"
    break;

  case 102: /* multilinestring: MLINESTRING_TOK DIMENSIONALITY_TOK EMPTY_TOK  */
#line 446 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(MULTILINETYPE, NULL, (yyvsp[-1].stringvalue)); WKT_ERROR(); }
#line 2666 "lwin_wkt_parse.c"
    break;

  case 103: /* multilinestring: MLINESTRING_TOK EMPTY_TOK  */
#line 448 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(MULTILINETYPE, NULL, NULL); WKT_ERROR(); }
#line 2672 "lwin_wkt_parse.c"
    break;

  case 104: /* linestring_list: linestring_list COMMA_TOK linestring_untagged  */
#line 452 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_add_geom((yyvsp[-2].geometryvalue),(yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2678 "lwin_wkt_parse.c"
    break;

  case 105: /* linestring_list: linestring_untagged  */
#line 454 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_new((yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2684 "lwin_wkt_parse.c"
    break;

  case 106: /* circularstring: CIRCULARSTRING_TOK LBRACKET_TOK ptarray RBRACKET_TOK  */
#line 458 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_circularstring_new((yyvsp[-1].ptarrayvalue), NULL); WKT_ERROR(); }
#line 2690 "lwin_wkt_parse.c"
    break;

  case 107: /* circularstring: CIRCULARSTRING_TOK DIMENSIONALITY_TOK LBRACKET_TOK ptarray RBRACKET_TOK  */
#line 460 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_circularstring_new((yyvsp[-1].ptarrayvalue), (yyvsp[-3].stringvalue)); WKT_ERROR(); }
#line 2696 "lwin_wkt_parse.c"
    break;

  case 108: /* circularstring: CIRCULARSTRING_TOK DIMENSIONALITY_TOK EMPTY_TOK  */
#line 462 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_circularstring_new(NULL, (yyvsp[-1].stringvalue)); WKT_ERROR(); }
#line 2702 "lwin_wkt_parse.c"
    break;

  case 109: /* circularstring: CIRCULARSTRING_TOK EMPTY_TOK  */
#line 464 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_circularstring_new(NULL, NULL); WKT_ERROR(); }
#line 2708 "lwin_wkt_parse.c"
    break;

  case 110: /* linestring: LINESTRING_TOK LBRACKET_TOK ptarray RBRACKET_TOK  */
#line 468 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_linestring_new((yyvsp[-1].ptarrayvalue), NULL); WKT_ERROR(); }
#line 2714 "lwin_wkt_parse.c"
    break;

  case 111: /* linestring: LINESTRING_TOK DIMENSIONALITY_TOK LBRACKET_TOK ptarray RBRACKET_TOK  */
#line 470 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_linestring_new((yyvsp[-1].ptarrayvalue), (yyvsp[-3].stringvalue)); WKT_ERROR(); }
#line 2720 "lwin_wkt_parse.c"
    break;

  case 112: /* linestring: LINESTRING_TOK DIMENSIONALITY_TOK EMPTY_TOK  */
#line 472 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_linestring_new(NULL, (yyvsp[-1].stringvalue)); WKT_ERROR(); }
#line 2726 "lwin_wkt_parse.c"
    break;

  case 113: /* linestring: LINESTRING_TOK EMPTY_TOK  */
#line 474 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_linestring_new(NULL, NULL); WKT_ERROR(); }
#line 2732 "lwin_wkt_parse.c"
    break;

  case 114: /* linestring_untagged: LBRACKET_TOK ptarray RBRACKET_TOK  */
#line 478 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_linestring_new((yyvsp[-1].ptarrayvalue), NULL); WKT_ERROR(); }
#line 2738 "lwin_wkt_parse.c"
    break;

  case 115: /* linestring_untagged: EMPTY_TOK  */
#line 480 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_linestring_new(NULL, NULL); WKT_ERROR(); }
#line 2744 "lwin_wkt_parse.c"
    break;

  case 116: /* triangle_list: triangle_list COMMA_TOK triangle_untagged  */
#line 484 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_add_geom((yyvsp[-2].geometryvalue),(yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2750 "lwin_wkt_parse.c"
    break;

  case 117: /* triangle_list: triangle_untagged  */
#line 486 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_new((yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2756 "lwin_wkt_parse.c"
    break;

  case 118: /* triangle: TRIANGLE_TOK LBRACKET_TOK LBRACKET_TOK ptarray RBRACKET_TOK RBRACKET_TOK  */
#line 490 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_triangle_new((yyvsp[-2].ptarrayvalue), NULL); WKT_ERROR(); }
#line 2762 "lwin_wkt_parse.c"
    break;

  case 119: /* triangle: TRIANGLE_TOK DIMENSIONALITY_TOK LBRACKET_TOK LBRACKET_TOK ptarray RBRACKET_TOK RBRACKET_TOK  */
#line 492 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_triangle_new((yyvsp[-2].ptarrayvalue), (yyvsp[-5].stringvalue)); WKT_ERROR(); }
#line 2768 "lwin_wkt_parse.c"
    break;

  case 120: /* triangle: TRIANGLE_TOK DIMENSIONALITY_TOK EMPTY_TOK  */
#line 494 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_triangle_new(NULL, (yyvsp[-1].stringvalue)); WKT_ERROR(); }
#line 2774 "lwin_wkt_parse.c"
    break;

  case 121: /* triangle: TRIANGLE_TOK EMPTY_TOK  */
#line 496 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_triangle_new(NULL, NULL); WKT_ERROR(); }
#line 2780 "lwin_wkt_parse.c"
    break;

  case 122: /* triangle_untagged: LBRACKET_TOK LBRACKET_TOK ptarray RBRACKET_TOK RBRACKET_TOK  */
#line 500 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_triangle_new((yyvsp[-2].ptarrayvalue), NULL); WKT_ERROR(); }
#line 2786 "lwin_wkt_parse.c"
    break;

  case 123: /* multipoint: MPOINT_TOK LBRACKET_TOK point_list RBRACKET_TOK  */
#line 504 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(MULTIPOINTTYPE, (yyvsp[-1].geometryvalue), NULL); WKT_ERROR(); }
#line 2792 "lwin_wkt_parse.c"
    break;

  case 124: /* multipoint: MPOINT_TOK DIMENSIONALITY_TOK LBRACKET_TOK point_list RBRACKET_TOK  */
#line 506 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(MULTIPOINTTYPE, (yyvsp[-1].geometryvalue), (yyvsp[-3].stringvalue)); WKT_ERROR(); }
#line 2798 "lwin_wkt_parse.c"
    break;

  case 125: /* multipoint: MPOINT_TOK DIMENSIONALITY_TOK EMPTY_TOK  */
#line 508 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(MULTIPOINTTYPE, NULL, (yyvsp[-1].stringvalue)); WKT_ERROR(); }
#line 2804 "lwin_wkt_parse.c"
    break;

  case 126: /* multipoint: MPOINT_TOK EMPTY_TOK  */
#line 510 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_finalize(MULTIPOINTTYPE, NULL, NULL); WKT_ERROR(); }
#line 2810 "lwin_wkt_parse.c"
    break;

  case 127: /* point_list: point_list COMMA_TOK point_untagged  */
#line 514 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_add_geom((yyvsp[-2].geometryvalue),(yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2816 "lwin_wkt_parse.c"
    break;

  case 128: /* point_list: point_untagged  */
#line 516 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_collection_new((yyvsp[0].geometryvalue)); WKT_ERROR(); }
#line 2822 "lwin_wkt_parse.c"
    break;

  case 129: /* point_untagged: coordinate  */
#line 520 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_point_new(wkt_parser_ptarray_new((yyvsp[0].coordinatevalue)),NULL); WKT_ERROR(); }
#line 2828 "lwin_wkt_parse.c"
    break;

  case 130: /* point_untagged: LBRACKET_TOK coordinate RBRACKET_TOK  */
#line 522 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_point_new(wkt_parser_ptarray_new((yyvsp[-1].coordinatevalue)),NULL); WKT_ERROR(); }
#line 2834 "lwin_wkt_parse.c"
    break;

  case 131: /* point_untagged: EMPTY_TOK  */
#line 524 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_point_new(NULL, NULL); WKT_ERROR(); }
#line 2840 "lwin_wkt_parse.c"
    break;

  case 132: /* point: POINT_TOK LBRACKET_TOK ptarray RBRACKET_TOK  */
#line 528 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_point_new((yyvsp[-1].ptarrayvalue), NULL); WKT_ERROR(); }
#line 2846 "lwin_wkt_parse.c"
    break;

  case 133: /* point: POINT_TOK DIMENSIONALITY_TOK LBRACKET_TOK ptarray RBRACKET_TOK  */
#line 530 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_point_new((yyvsp[-1].ptarrayvalue), (yyvsp[-3].stringvalue)); WKT_ERROR(); }
#line 2852 "lwin_wkt_parse.c"
    break;

  case 134: /* point: POINT_TOK DIMENSIONALITY_TOK EMPTY_TOK  */
#line 532 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_point_new(NULL, (yyvsp[-1].stringvalue)); WKT_ERROR(); }
#line 2858 "lwin_wkt_parse.c"
    break;

  case 135: /* point: POINT_TOK EMPTY_TOK  */
#line 534 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_point_new(NULL,NULL); WKT_ERROR(); }
#line 2864 "lwin_wkt_parse.c"
    break;

  case 136: /* ptarray: ptarray COMMA_TOK coordinate  */
#line 538 "lwin_wkt_parse.y"
                { (yyval.ptarrayvalue) = wkt_parser_ptarray_add_coord((yyvsp[-2].ptarrayvalue), (yyvsp[0].coordinatevalue)); WKT_ERROR(); }
#line 2870 "lwin_wkt_parse.c"
    break;

  case 137: /* ptarray: coordinate  */
#line 540 "lwin_wkt_parse.y"
                { (yyval.ptarrayvalue) = wkt_parser_ptarray_new((yyvsp[0].coordinatevalue)); WKT_ERROR(); }
#line 2876 "lwin_wkt_parse.c"
    break;

  case 138: /* coordinate: DOUBLE_TOK DOUBLE_TOK  */
#line 544 "lwin_wkt_parse.y"
                { (yyval.coordinatevalue) = wkt_parser_coord_2((yyvsp[-1].doublevalue), (yyvsp[0].doublevalue)); WKT_ERROR(); }
#line 2882 "lwin_wkt_parse.c"
    break;

  case 139: /* coordinate: DOUBLE_TOK DOUBLE_TOK DOUBLE_TOK  */
#line 546 "lwin_wkt_parse.y"
                { (yyval.coordinatevalue) = wkt_parser_coord_3((yyvsp[-2].doublevalue), (yyvsp[-1].doublevalue), (yyvsp[0].doublevalue)); WKT_ERROR(); }
#line 2888 "lwin_wkt_parse.c"
    break;

  case 140: /* coordinate: DOUBLE_TOK DOUBLE_TOK DOUBLE_TOK DOUBLE_TOK  */
#line 548 "lwin_wkt_parse.y"
                { (yyval.coordinatevalue) = wkt_parser_coord_4((yyvsp[-3].doublevalue), (yyvsp[-2].doublevalue), (yyvsp[-1].doublevalue), (yyvsp[0].doublevalue)); WKT_ERROR(); }
#line 2894 "lwin_wkt_parse.c"
    break;

  case 141: /* ellipse: ELLIPTICALSTRING_TOK LBRACKET_TOK coordinate COMMA_TOK coordinate COMMA_TOK coordinate COMMA_TOK DOUBLE_TOK COMMA_TOK DOUBLE_TOK COMMA_TOK DOUBLE_TOK COMMA_TOK DOUBLE_TOK COMMA_TOK DOUBLE_TOK RBRACKET_TOK  */
#line 551 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_ellipse((yyvsp[-15].coordinatevalue),(yyvsp[-13].coordinatevalue),(yyvsp[-11].coordinatevalue),(yyvsp[-9].doublevalue),(yyvsp[-7].doublevalue),(yyvsp[-5].doublevalue),(yyvsp[-3].doublevalue),(yyvsp[-1].doublevalue),NULL); WKT_ERROR(); }
#line 2900 "lwin_wkt_parse.c"
    break;

  case 142: /* ellipse: ELLIPTICALSTRING_TOK DIMENSIONALITY_TOK LBRACKET_TOK coordinate COMMA_TOK coordinate COMMA_TOK coordinate COMMA_TOK DOUBLE_TOK COMMA_TOK DOUBLE_TOK COMMA_TOK DOUBLE_TOK COMMA_TOK DOUBLE_TOK COMMA_TOK DOUBLE_TOK RBRACKET_TOK  */
#line 553 "lwin_wkt_parse.y"
                { (yyval.geometryvalue) = wkt_parser_ellipse((yyvsp[-15].coordinatevalue),(yyvsp[-13].coordinatevalue),(yyvsp[-11].coordinatevalue),(yyvsp[-9].doublevalue),(yyvsp[-7].doublevalue),(yyvsp[-5].doublevalue),(yyvsp[-3].doublevalue),(yyvsp[-1].doublevalue),(yyvsp[-17].stringvalue)); WKT_ERROR(); }
#line 2906 "lwin_wkt_parse.c"
    break;


#line 2910 "lwin_wkt_parse.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 555 "lwin_wkt_parse.y"


