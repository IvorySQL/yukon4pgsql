/*
 *
 * src/paramobj.cpp
 *
 * Copyright (C) 2021 SuperMap Software Co., Ltd.
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

#include "paramobj.h"

extern "C" {
PG_FUNCTION_INFO_V1(paramobj_in);
PG_FUNCTION_INFO_V1(paramobj_out);
PG_FUNCTION_INFO_V1(paramobj_send);
PG_FUNCTION_INFO_V1(paramobj_recv);
PG_FUNCTION_INFO_V1(paramobj_typmod_in);
PG_FUNCTION_INFO_V1(paramobj_typmod_out);
PG_FUNCTION_INFO_V1(paramobj_typmod_dims);
PG_FUNCTION_INFO_V1(paramobj_typmod_srid);
PG_FUNCTION_INFO_V1(paramobj_typmod_type);

Datum paramobj_in(PG_FUNCTION_ARGS);
Datum paramobj_out(PG_FUNCTION_ARGS);
Datum paramobj_send(PG_FUNCTION_ARGS);
Datum paramobj_recv(PG_FUNCTION_ARGS);
Datum paramobj_typmod_in(PG_FUNCTION_ARGS);
Datum paramobj_typmod_out(PG_FUNCTION_ARGS);
Datum paramobj_typmod_dims(PG_FUNCTION_ARGS);
Datum paramobj_typmod_srid(PG_FUNCTION_ARGS);
Datum paramobj_typmod_type(PG_FUNCTION_ARGS);
}

ParamObjTypeStruct paramobjStructArray[] = {{"CIRCLE", WKB_CIRCLE_TYPE}, {"OVAL", WKB_OVAL_TYPE}};

#define PARAMOBJ_STRUCT_ARRAY_LEN (sizeof paramobjStructArray / sizeof(ParamObjTypeStruct))

/**
 * @brief converts data from the paramobj's textual form to its internal form
 *
 * @return Datum
 *
 * @note
 * format 'CIRCLE(x,y,r)' or 'OVAL(x,y,a,b)'
 *
 */
Datum paramobj_in(PG_FUNCTION_ARGS)
{
	char *input = PG_GETARG_CSTRING(0);
	std::string sinput(input);
	PreProcess(sinput);
	char *res = nullptr;

	// circle regex
	// std::regex
	// circlergx(R"&(CIRCLE\((\-?([1-9][\d]*|0)(\.[\d]+)?),(\-?([1-9][\d]*|0)(\.[\d]+)?),(\-?([1-9][\d]*|0)(\.[\d]+)?)\))&");
	// std::smatch circleres;
	// oval regex
	std::regex ovalrgx(
	    R"&(OVAL\((\-?([1-9][\d]*|0)(\.[\d]+)?),(\-?([1-9][\d]*|0)(\.[\d]+)?),(\-?([1-9][\d]*|0)(\.[\d]+)?),(\-?([1-9][\d]*|0)(\.[\d]+)?)\))&");
	std::smatch ovalres;

	//判断输入格式是否正确
	// bool bcircle = regex_match(sinput, circleres, circlergx);
	bool boval = regex_match(sinput, ovalres, ovalrgx);
	if (boval == false)
	{
		ereport(ERROR, (errmsg("parse error - invalid geometry，format should be 'OVAL(x,y,a,b)")));
		PG_RETURN_NULL();
	}

	PO_OVAL oval;
	res = palloc0(8 + sizeof(PO_OVAL));
	SET_VARSIZE(res, 8 + sizeof(PO_OVAL));
	oval.type = WKB_OVAL_TYPE;
	// int size = ovalres.size();
	oval.x = stod(ovalres[1]);
	oval.y = stod(ovalres[2]);
	oval.a = stod(ovalres[3]);
	oval.b = stod(ovalres[4]);
	memcpy(res + 8, &oval, sizeof(PO_OVAL));
	PG_RETURN_POINTER(res);
}
/**
 * @brief  converts data from the paramobj's internal form to its  textual form.
 *
 * @return Datum
 */
Datum paramobj_out(PG_FUNCTION_ARGS)
{
	varlena *buf = PG_GETARG_VARLENA_P(0);
	uint32_t buf_size = VARSIZE(buf);
	char *bufdata = VARDATA(buf);
	char *result;
	PO_OVAL *c = (PO_OVAL *)(bufdata + 4);
	if (c->type == WKB_OVAL_TYPE)
	{

		std::string t = "OVAL(" + std::to_string(c->x) + "," + std::to_string(c->y) + "," +
				std::to_string(c->a) + "," + std::to_string(c->b) + ")";
		result = palloc0(t.size() + 1);
		memcpy(result, t.c_str(), t.size());
		PG_RETURN_CSTRING(result);
	}

	PG_RETURN_NULL();
}
/**
 * @brief converts data from the paramobj's internal form to its external binary form.
 *
 * @return Datum
 * @note wkb https://en.wikipedia.org/wiki/Well-known_text_representation_of_geometry
 */
Datum paramobj_send(PG_FUNCTION_ARGS)
{
	varlena *buf = PG_DETOAST_DATUM_COPY(PG_GETARG_DATUM(0));
	uint32_t buf_size = VARSIZE(buf);
	char *buf_data = VARDATA(buf);
	// buf_size - srid(3) - gflags(1) + little_endin(1)
	char *result = palloc0(buf_size - 3 - 1 + 1);
	memcpy(result + 5, buf_data + 4, buf_size - 4 - 3 - 1);
	SET_VARSIZE(result, buf_size - 3 - 1 + 1);
	PG_RETURN_POINTER(result);
}
/**
 * @brief converts data from the paramobj's  binary form to its internal form.
 *
 * @return Datum
 * @note
 * 1-byte integer： 01 - little endian 小端
 * 4-byte integer:  类型：WKB_OVAL_TYPE
 * 8-byte double: x 坐标
 * 8-byte double: y 坐标
 * 8-byte double: 长轴长度
 * 8-byte double: 短轴长度
 * 共 37 bytes
 */
Datum paramobj_recv(PG_FUNCTION_ARGS)
{
	StringInfo buf = (StringInfo)PG_GETARG_POINTER(0);
	int len = buf->len;
	char *data = buf->data;
	char *res = nullptr;

	// if (data[0] != 0x01 || *(int*)data[1] != WKB_OVAL_TYPE || len != 37)
	// {
	// 	ereport(ERROR, (errmsg("WKB format error!")));
	// 	PG_RETURN_NULL();
	// }

	// 开始填充数据
	PO_OVAL oval;
	res = palloc0(8 + sizeof(PO_OVAL));
	SET_VARSIZE(res, 8 + sizeof(PO_OVAL));
	oval.type = WKB_OVAL_TYPE;
	oval.x = *reinterpret_cast<double *>(data + 5);
	oval.y = *reinterpret_cast<double *>(data + 13);
	oval.a = *reinterpret_cast<double *>(data + 21);
	oval.b = *reinterpret_cast<double *>(data + 29);
	memcpy(res + 8, &oval, sizeof(PO_OVAL));
	buf->cursor = buf->len;
	PG_RETURN_POINTER(res);
}
/**
 * @brief converts an array of modifier(s) for the paramobj into internal form.
 *
 * @return Datum
 */
Datum paramobj_typmod_in(PG_FUNCTION_ARGS)
{
	ArrayType *arr = (ArrayType *)DatumGetPointer(PG_GETARG_DATUM(0));
	uint32 typmod = poserialized_typmod_in(arr);
	int srid = PO_TYPMOD_GET_SRID(typmod);
	PG_RETURN_INT32(typmod);
}

/**
 * @brief converts the internal form of the type's modifier(s) to external textual form.
 *
 * @return Datum
 */
Datum paramobj_typmod_out(PG_FUNCTION_ARGS)
{
	char *s = (char *)palloc0(64);
	char *str = s;
	uint32 typmod = PG_GETARG_INT32(0);
	uint32 srid = PO_TYPMOD_GET_SRID(typmod);
	uint32 type = PO_TYPMOD_GET_TYPE(typmod);
	uint32 hasz = PO_TYPMOD_GET_Z(typmod);
	uint32 hasm = PO_TYPMOD_GET_M(typmod);

	/* 如果没有 SRID ，类型和维度，则直接返回空字符串 */
	if (!(srid || type || hasz || hasm))
	{
		*str = '\0';
		PG_RETURN_CSTRING(str);
	}

	str += sprintf(str, "(");

	if (type)
		str += sprintf(str, "%s", paramobj_type_name(type));
	else if ((!type) && (srid || hasz || hasm))
		str += sprintf(str, "PARAMOBJ");

	// if (hasz)
	//  str += sprintf(str, "%s", "Z");

	// if (hasm)
	//  str += sprintf(str, "%s", "M");

	if (srid)
		str += sprintf(str, ",");

	if (srid)
		str += sprintf(str, "%d", srid);

	str += sprintf(str, ")");

	PG_RETURN_CSTRING(s);
}

/**
 * @brief 根据 typmod 获取维度
 *
 * @return Datum
 */
Datum paramobj_typmod_dims(PG_FUNCTION_ARGS)
{
	int32 typmod = PG_GETARG_INT32(0);
	int32 dims = 2;

	int type = PO_TYPMOD_GET_PO(typmod);

	if (type == 0x1)
	{
		if (typmod < 0)
			PG_RETURN_INT32(0);
		if (PO_TYPMOD_GET_Z(typmod))
			dims++;
		if (PO_TYPMOD_GET_M(typmod))
			dims++;
		PG_RETURN_INT32(dims);
	}
	else
	{
		if (typmod < 0)
			PG_RETURN_NULL(); /* unconstrained */
		if (TYPMOD_GET_Z(typmod))
			dims++;
		if (TYPMOD_GET_M(typmod))
			dims++;
		PG_RETURN_INT32(dims);
	}
}

/**
 * @brief 根据 typmpd 获取 srid
 *
 * @return Datum
 */
Datum paramobj_typmod_srid(PG_FUNCTION_ARGS)
{
	int32 typmod = PG_GETARG_INT32(0);
	if (typmod < 0)
		PG_RETURN_INT32(0);
	PG_RETURN_INT32(PO_TYPMOD_GET_SRID(typmod));
}

/**
 * @brief 根据  typmod 获取 type
 *
 * @return Datum
 */
Datum paramobj_typmod_type(PG_FUNCTION_ARGS)
{
	int32 typmod = PG_GETARG_INT32(0);
	int po = PO_TYPMOD_GET_PO(typmod);
	text *stext;
	char *s = (char *)palloc(64);
	char *ptr = s;

	if (po == 0x1)
	{
		int32 type = PO_TYPMOD_GET_TYPE(typmod);
		if (typmod < 0 || type == 0)
			ptr += sprintf(ptr, "OVAL");
		else
			ptr += sprintf(ptr, "%s", paramobj_type_name(type));
	}
	else
	{
		int32 type = TYPMOD_GET_TYPE(typmod);

		/* Has type? */
		if (typmod < 0 || type == 0)
			ptr += sprintf(ptr, "Geometry");
		else
			ptr += sprintf(ptr, "%s", lwtype_name(type));

		/* Has Z? */
		if (typmod >= 0 && TYPMOD_GET_Z(typmod))
			ptr += sprintf(ptr, "%s", "Z");

		/* Has M? */
		if (typmod >= 0 && TYPMOD_GET_M(typmod))
			ptr += sprintf(ptr, "%s", "M");
	}

	stext = cstring_to_text(s);
	pfree(s);
	PG_RETURN_TEXT_P(stext);
}

void
PreProcess(std::string &str)
{
	for (auto c = str.begin(); c != str.end();)
	{
		if (*c == ' ' || *c == '\t')
		{
			str.erase(c);
			continue;
		}
		c++;
	}
}

uint32
poserialized_typmod_in(ArrayType *arr)
{
	uint32 typmod = 0;
	Datum *elem_values;
	int n = 0;
	int i = 0;

	if (ARR_ELEMTYPE(arr) != CSTRINGOID)
		ereport(ERROR, (errcode(ERRCODE_ARRAY_ELEMENT_ERROR), errmsg("typmod array must be type cstring[]")));

	if (ARR_NDIM(arr) != 1)
		ereport(ERROR,
			(errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR), errmsg("typmod array must be one-dimensional")));

	if (ARR_HASNULL(arr))
		ereport(ERROR,
			(errcode(ERRCODE_NULL_VALUE_NOT_ALLOWED), errmsg("typmod array must not contain nulls")));

	deconstruct_array(arr, CSTRINGOID, -2, false, 'c', &elem_values, NULL, &n);

	/* 先将 SRID 设置为默认值 */
	PO_TYPMOD_SET_SRID(typmod, PO_SRID_DEFAULT);

	for (i = 0; i < n; i++)
	{
		if (i == 0) /* TYPE */
		{
			char *s = DatumGetCString(elem_values[i]);
			uint8_t type = 0;

			if (paramobj_type_from_string(s, &type) == false)
			{
				ereport(ERROR,
					(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
					 errmsg("Invalid paramobject type modifier: %s", s)));
			}
			else
			{
				PO_TYPMOD_SET_TYPE(typmod, type);
				// 固定为 1
				PO_TYPMOD_SET_Z(typmod);
				// 固定为 0
				PO_TYPMOD_SET_M(typmod);
			}
		}
		if (i == 1) /* SRID */
		{
			int srid = pg_atoi(DatumGetCString(elem_values[i]), sizeof(int32), '\0');
			// srid = clamp_srid(srid);
			if (srid != PO_SRID_UNKNOWN)
			{
				PO_TYPMOD_SET_SRID(typmod, srid);
			}
		}
	}

	pfree(elem_values);
	//设置参数化对象标识
	PO_TYPMOD_SET_PO(typmod);
	return typmod;
}

bool
paramobj_type_from_string(const char *str, uint8_t *type)
{
	char *tmpstr;
	int tmpstartpos, tmpendpos;
	int i;

	if (str == nullptr || type == nullptr)
	{
		return false;
	}

	*type = 0;

	/* 删除首部和尾部的空格 */
	tmpstartpos = 0;
	for (i = 0; i < strlen(str); i++)
	{
		if (str[i] != ' ')
		{
			tmpstartpos = i;
			break;
		}
	}

	tmpendpos = strlen(str) - 1;
	for (i = strlen(str) - 1; i >= 0; i--)
	{
		if (str[i] != ' ')
		{
			tmpendpos = i;
			break;
		}
	}

	/* 将字符串拷贝出来，并转换未大写 */
	tmpstr = palloc(tmpendpos - tmpstartpos + 2);
	for (i = tmpstartpos; i <= tmpendpos; i++)
		tmpstr[i - tmpstartpos] = pg_toupper(str[i]);

	/* 添加结束符 */
	tmpstr[i - tmpstartpos] = '\0';

	/* 检查类型 */
	for (i = 0; i < PARAMOBJ_STRUCT_ARRAY_LEN; i++)
	{
		if (!strcmp(tmpstr, paramobjStructArray[i].typename_C))
		{
			*type = paramobjStructArray[i].type;
			pfree(tmpstr);
			return true;
		}
	}

	pfree(tmpstr);

	return false;
}

/**
 * @brief 根据 @type 返回具体的类型
 */
const char *
paramobj_type_name(uint8_t type)
{
	if (type > PARAMOBJ_STRUCT_ARRAY_LEN)
	{
		/* assert(0); */
		return "Invalid type";
	}
	return paramobjStructArray[(int)type - 1].typename_C;
}
