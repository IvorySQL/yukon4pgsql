#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "liblwgeom_internal.h"
#include "lwgeom_log.h"

#define PI 3.141592653589793238462643383
#define DTOR 0.017453292519943295

#ifndef ROUNDLONG
#define ROUNDLONG(x) ((x > 0) ? long(x + 0.5) : long(x - 0.5))
#endif

// 函数前置声明

/**
 * @brief 根据参数化对象返回拟合的点数组
 *
 * @param arr 返回的点数组
 * @param len 数组长度
 * @param x 椭圆的 x 坐标
 * @param y 椭圆的 y 坐标
 * @param a 椭圆的长轴
 * @param b 椭圆的短轴
 * @param dRotation 椭圆的旋转角度
 * @param dRadianBegin 椭圆的起始角
 * @param dRadianEnd 椭圆的终止角
 * @param step 步长
 * @return true
 * @return false
 */
bool BuildArc(POINT2D **arr,
	      unsigned long *len,
	      double x,
	      double y,
	      double a,
	      double b,
	      double dRotation,
	      double dRadianBegin,
	      double dRadianEnd,
	      double step);

double CalcEllipseRadian(const double dPntRadian, const double dRreAxis, const double dSemiMinorAxis);

double
lwellipse_area(const LWELLIPSE *ellipse)
{
	if (ellipse->data->startangle == 0 && ellipse->data->endangle == 360)
	{
		return PI * ellipse->data->a * ellipse->data->b;
	}

	return -1;
}
void
lwellipse_free(LWELLIPSE *e)
{
	if (!e)
	{
		return;
	}

	if (e->bbox)
	{
		lwfree(e->bbox);
	}
	if (e->data)
	{
		lwfree(e->data);
	}
	lwfree(e);
}

LWGEOM *
lwellipse_get_spatialdata(LWELLIPSE *geom, unsigned int segment)
{
	LWLINE *lwgeom = NULL;
	// 检查长短半轴是否合法
	if (geom->data->a <= 0 || geom->data->b <= 0)
	{
		return NULL;
	}

	if (segment == 0)
	{
		segment = 72;
	}

	double dRadian = geom->data->angle * DTOR;
	double dRadianBegin = geom->data->startangle * DTOR;
	double dRadianEnd = geom->data->endangle * DTOR;
	double dStep = (dRadianBegin - dRadianEnd) / segment;
	POINT2D *poarr = NULL;
	unsigned long len;
	bool res = BuildArc(&poarr,
			    &len,
			    geom->data->x,
			    geom->data->y,
			    geom->data->a,
			    geom->data->b,
			    dRadian,
			    dRadianBegin,
			    dRadianEnd,
			    dStep);
	if (res)
	{
		//创建 POINTARRAY
		POINTARRAY *parr = ptarray_construct_copy_data(0, 0, len, (uint8_t *)poarr);
		lwgeom = lwline_construct(4326, NULL, parr);
	}
	return lwgeom;
}

bool
BuildArc(POINT2D **arr,
	 unsigned long *len,
	 double x,
	 double y,
	 double a,
	 double b,
	 double dRotation,
	 double dRadianBegin,
	 double dRadianEnd,
	 double step)
{
	a = fabs(a);
	b = fabs(b);
	step = fabs(step);

	// step 不能等于 0
	if (step <= 1e-15)
	{
		return false;
	}

	while (dRadianEnd < dRadianBegin)
	{
		dRadianEnd += 2 * PI;
	}

	while (dRadianEnd > (dRadianBegin + PI * 2))
	{
		dRadianBegin += PI * 2;
	}

	double dSin_a_Pri = 0, dCos_a_Pri = 0, dSin_a_Sec = 0, dCos_a_Sec = 0;

	dCos_a_Pri = cos(dRotation) * a;
	dSin_a_Pri = sin(dRotation) * a;
	dCos_a_Sec = cos(dRotation) * b;
	dSin_a_Sec = sin(dRotation) * b;

	double dRadianBeginT = CalcEllipseRadian(dRadianBegin, a, b);
	double dRadianEndT = CalcEllipseRadian(dRadianEnd, a, b);

	if (fabs(dRadianEndT - dRadianBeginT) <= 1e-15)
	{
		dRadianEndT += 2 * PI;
	}

	*len = (long)fabs((dRadianEndT - dRadianBeginT) / step) + 2;

	if (*len < 2)
	{
		return false;
	}

	*arr = malloc(sizeof(POINT2D) * (*len));

	if (arr != NULL)
	{
		int i = 0;
		for (; i < *len - 1; dRadianBeginT += step, i++)
		{
			(*arr)[i].x = x + dCos_a_Pri * cos(dRadianBeginT) - dSin_a_Sec * sin(dRadianBeginT);
			(*arr)[i].y = y + dSin_a_Pri * cos(dRadianBeginT) + dCos_a_Sec * sin(dRadianBeginT);
		}

		(*arr)[i].x = x + dCos_a_Pri * cos(dRadianEndT) - dSin_a_Sec * sin(dRadianEndT);
		(*arr)[i].y = y + dSin_a_Pri * cos(dRadianEndT) + dCos_a_Sec * sin(dRadianEndT);
		return true;
	}
	return false;
}

double
CalcEllipseRadian(const double dPntRadian, const double dPreAxis, const double dSemiMinorAxis)
{
	double dTempPntRadian = dPntRadian;
	double dRSinB = dPreAxis * sin(dPntRadian);
	double dRCosB = dSemiMinorAxis * cos(dPntRadian);
	double dRadianT = atan2(dRSinB, dRCosB);

	if (dPntRadian > PI)
	{
		while (dTempPntRadian > PI)
		{
			dTempPntRadian -= 2 * PI;
			dRadianT += 2 * PI;
		}
	}
	else if (dPntRadian < -PI)
	{
		while (dTempPntRadian < -PI)
		{
			dTempPntRadian += 2 * PI;
			dRadianT -= 2 * PI;
		}
	}

	return dRadianT;
}