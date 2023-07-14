/*
 *
 * GeoGrid.h
 *
 * Copyright (C) 2023 SuperMap Software Co., Ltd.
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
#include "postgres.h"
#include "fmgr.h"
#include "rtpostgis.h"
#include "access/genam.h"
#include "access/table.h"
#include "access/htup_details.h"
#include "utils/array.h"
#include "utils/syscache.h"
#include "utils/fmgroids.h"
#include "utils/regproc.h"
#include "catalog/pg_type.h"
#include "catalog/pg_namespace_d.h"
#include "catalog/pg_extension.h"
#include "catalog/indexing.h"
#include "commands/extension.h"
}

#include <cmath>
#include "RasterizeHash.h"

#endif