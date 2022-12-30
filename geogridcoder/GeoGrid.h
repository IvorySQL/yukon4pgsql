/*
 *
 * GeoGrid.h
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

#ifndef GEOGRID_H_
#define GEOGRID_H_

extern "C" {
#include "../libpgcommon/lwgeom_pg.h"
#include "../liblwgeom/liblwgeom.h"
#include "postgres.h"
#include "fmgr.h"
#include "rtpostgis.h"
#include "utils/array.h"
#include "utils/syscache.h"
#include "catalog/pg_type.h"
#include "catalog/pg_namespace_d.h"
}

#include <cmath>
#include "RasterizeHash.h"

#endif