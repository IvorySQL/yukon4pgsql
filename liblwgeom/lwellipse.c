#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "liblwgeom_internal.h"
#include "lwgeom_log.h"

#define PI 3.1415926

double
lwellipse_area(const LWELLIPSE *ellipse)
{
	return PI * ellipse->data->a * ellipse->data->b;
}
void 
lwellipse_free(LWELLIPSE *e)
{
	if (!e) return;

	if (e->bbox)
		lwfree(e->bbox);
	if (e->data)
		lwfree(e->data);
	lwfree(e);
}