#ifndef GM_INCLUDE_H
#define GM_INCLUDE_H

extern "C" {
#include "postgres.h"
#include "fmgr.h"
#include "utils/palloc.h"
#include "lib/stringinfo.h" /* For binary input */
#include "utils/array.h"    /* for ArrayType */
#include "utils/builtins.h" /* for pg_atoi */
#include "catalog/pg_type.h" /* for all oid */
}
#endif