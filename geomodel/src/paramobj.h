/*
 *
 * src/paramobj.h
 *
 * Copyright (C)  SuperMap Software Co., Ltd.
 *
 * Yukon is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see <http://www.gnu.org/licenses/>. *
 */

#ifndef GM_PARAMOBJ_H
#define GM_PARAMOBJ_H

#include <string>
#include <regex>
#include "include.h"

extern "C" {
#include "../liblwgeom/liblwgeom.h"
}

/* should be one byte */
#define WKB_CIRCLE_TYPE 1
#define WKB_OVAL_TYPE 2

/**
 * Macros for manipulating the 'typemod' int. An int32_t used as follows:
 * Plus/minus = Top bit.
 * Spare bits = indicate is a parammeter object.
 * SRID = Next 21 bits.
 * TYPE = Next 6 bits.
 * ZM Flags = Z=1,M=0.
 */
#define PO_TYPMOD_GET_PO(typmod) (((typmod) & 0x60000000) >> 29)
#define PO_TYPMOD_SET_PO(typmod) ((typmod) = (typmod & 0x9FFFFFFF) | (0x01 << 29))
#define PO_TYPMOD_GET_SRID(typmod) ((((typmod)&0x0FFFFF00) - ((typmod)&0x10000000)) >> 8)
#define PO_TYPMOD_SET_SRID(typmod, srid) ((typmod) = (((typmod)&0xE00000FF) | ((srid & 0x001FFFFF) << 8)))
#define PO_TYPMOD_GET_TYPE(typmod) ((typmod & 0x000000FC) >> 2)
#define PO_TYPMOD_SET_TYPE(typmod, type) ((typmod) = (typmod & 0xFFFFFF03) | ((type & 0x0000003F) << 2))
#define PO_TYPMOD_GET_Z(typmod) (0)
#define PO_TYPMOD_SET_Z(typmod) ((typmod) |= 1 << 1)
#define PO_TYPMOD_GET_M(typmod) (0)
#define PO_TYPMOD_SET_M(typmod) ((typmod) &= ~(1 << 0))
#define PO_TYPMOD_GET_NDIMS(typmod) (2 + TYPMOD_GET_Z(typmod) + TYPMOD_GET_M(typmod))

#define PO_SRID_MAXIMUM 999999
#define PO_SRID_USER_MAXIMUM 998999
#define PO_SRID_UNKNOWN 0
#define PO_SRID_IS_UNKNOWN(x) ((int)x <= 0)
#define PO_SRID_DEFAULT 4326

#pragma pack(1)
struct PO_CIRCLE {
	uint8_t type;
	double x;
	double y;
	double r;
};

#pragma pack(1)
struct PO_OVAL {
	uint32_t type;
	double x;
	double y;
	double a;
	double b;
};

/* 数据库中存储的序列化数据 */
typedef struct {
	uint32_t size;   /* For PgSQL use only, use VAR* macros to manipulate. */
	uint8_t srid[3]; /* 24 bits of SRID */
	uint8_t gflags;  /* HasZ, HasM, HasBBox, IsGeodetic */
	uint8_t data[1];
} POSERIALIZED;

struct ParamObjTypeStruct {
	char *typename_C;
	int type;
};

void PreProcess(std::string &str);
uint32 poserialized_typmod_in(ArrayType *arr);
bool paramobj_type_from_string(const char *str, uint8_t *type);
const char *paramobj_type_name(uint8_t type);
#endif
