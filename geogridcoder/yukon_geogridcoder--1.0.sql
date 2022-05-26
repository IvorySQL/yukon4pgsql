--complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION yukon_geogridcoder" to load this file. \quit

------------------------------------------------geosotgrid----------------------------------------------------
CREATE OR REPLACE FUNCTION geosotgrid_in(cstring)
	RETURNS geosotgrid
	AS '$libdir/yukon_geogridcoder-1.0','geosotgrid_in'
	LANGUAGE 'c'  IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION geosotgrid_out(geosotgrid)
	RETURNS cstring
	AS '$libdir/yukon_geogridcoder-1.0','geosotgrid_out'
	LANGUAGE 'c'  IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION geosotgrid_recv(internal)
	RETURNS geosotgrid
	AS '$libdir/yukon_geogridcoder-1.0','geosotgrid_recv'
	LANGUAGE 'c'  IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION geosotgrid_send(geosotgrid)
	RETURNS bytea
	AS '$libdir/yukon_geogridcoder-1.0','geosotgrid_send'
	LANGUAGE 'c'  IMMUTABLE STRICT PARALLEL SAFE;

CREATE TYPE geosotgrid(
	internallength = variable,
	input = geosotgrid_in,
	output = geosotgrid_out,
	send = geosotgrid_send,
	receive = geosotgrid_recv,
	alignment = int4,
	storage = main
);

----------------------------------------geosotgrid type----------------------------------------

CREATE OR REPLACE FUNCTION grid_lt(geosotgrid, geosotgrid)
RETURNS bool
AS '$libdir/yukon_geogridcoder-1.0'
LANGUAGE 'c' IMMUTABLE STRICT ;

CREATE OR REPLACE FUNCTION grid_le(geosotgrid, geosotgrid)
RETURNS bool
AS '$libdir/yukon_geogridcoder-1.0'
LANGUAGE 'c' IMMUTABLE STRICT ;

CREATE OR REPLACE FUNCTION grid_gt(geosotgrid, geosotgrid)
RETURNS bool
AS '$libdir/yukon_geogridcoder-1.0'
LANGUAGE 'c' IMMUTABLE STRICT ;

CREATE OR REPLACE FUNCTION grid_ge(geosotgrid, geosotgrid)
RETURNS bool
AS '$libdir/yukon_geogridcoder-1.0'
LANGUAGE 'c' IMMUTABLE STRICT ;

CREATE OR REPLACE FUNCTION grid_eq(geosotgrid, geosotgrid)
RETURNS bool
AS '$libdir/yukon_geogridcoder-1.0'
LANGUAGE 'c' IMMUTABLE STRICT ;

CREATE OR REPLACE FUNCTION grid_cmp(geosotgrid, geosotgrid)
RETURNS integer
AS '$libdir/yukon_geogridcoder-1.0'
LANGUAGE 'c' IMMUTABLE STRICT ;

CREATE OPERATOR < (
LEFTARG = geosotgrid, RIGHTARG = geosotgrid, PROCEDURE = grid_lt,
COMMUTATOR = '>', NEGATOR = '>=',
RESTRICT = contsel, JOIN = contjoinsel
);

CREATE OPERATOR <= (
LEFTARG = geosotgrid, RIGHTARG = geosotgrid, PROCEDURE = grid_le,
COMMUTATOR = '>=', NEGATOR = '>',
RESTRICT = contsel, JOIN = contjoinsel
);

CREATE OPERATOR = (
LEFTARG = geosotgrid, RIGHTARG = geosotgrid, PROCEDURE = grid_eq,
COMMUTATOR = '=', -- we might implement a faster negator here
RESTRICT = contsel, JOIN = contjoinsel
);

CREATE OPERATOR >= (
LEFTARG = geosotgrid, RIGHTARG = geosotgrid, PROCEDURE = grid_ge,
COMMUTATOR = '<=', NEGATOR = '<',
RESTRICT = contsel, JOIN = contjoinsel
);

CREATE OPERATOR > (
LEFTARG = geosotgrid, RIGHTARG = geosotgrid, PROCEDURE = grid_gt,
COMMUTATOR = '<', NEGATOR = '<=',
RESTRICT = contsel, JOIN = contjoinsel
);

CREATE OPERATOR CLASS btree_grid_ops
DEFAULT FOR TYPE geosotgrid USING btree AS
OPERATOR	1	< ,
OPERATOR	2	<= ,
OPERATOR	3	= ,
OPERATOR	4	>= ,
OPERATOR	5	> ,
FUNCTION	1	grid_cmp (geosotgrid, geosotgrid);

----------------------------------------geosotgrid array type----------------------------------------

CREATE OR REPLACE FUNCTION gridarray_cmp(geosotgrid, geosotgrid)
RETURNS integer
AS '$libdir/yukon_geogridcoder-1.0'
LANGUAGE 'c' IMMUTABLE STRICT ;

CREATE OR REPLACE FUNCTION gridarray_overlap(_geosotgrid, _geosotgrid)
RETURNS bool
AS '$libdir/yukon_geogridcoder-1.0'
LANGUAGE 'c' IMMUTABLE STRICT ;

CREATE OR REPLACE FUNCTION gridarray_contains(_geosotgrid, _geosotgrid)
RETURNS bool
AS '$libdir/yukon_geogridcoder-1.0'
LANGUAGE 'c' IMMUTABLE STRICT ;

CREATE OR REPLACE FUNCTION gridarray_contained(_geosotgrid, _geosotgrid)
RETURNS bool
AS '$libdir/yukon_geogridcoder-1.0'
LANGUAGE 'c' IMMUTABLE STRICT ;

CREATE OR REPLACE FUNCTION gridarray_extract(anyarray, internal, internal)
RETURNS internal
AS '$libdir/yukon_geogridcoder-1.0'
LANGUAGE 'c' IMMUTABLE STRICT ;

CREATE OR REPLACE FUNCTION gridarray_queryextract(_geosotgrid, internal, int2, internal, internal, internal, internal)
RETURNS internal
AS '$libdir/yukon_geogridcoder-1.0'
LANGUAGE 'c' IMMUTABLE STRICT ;

CREATE OR REPLACE FUNCTION gridarray_consistent(internal, int2, _geosotgrid, int4, internal, internal, internal, internal)
RETURNS bool
AS '$libdir/yukon_geogridcoder-1.0'
LANGUAGE 'c' IMMUTABLE STRICT ;


CREATE OPERATOR && (
	LEFTARG = _geosotgrid,
	RIGHTARG = _geosotgrid,
	PROCEDURE = gridarray_overlap,
	COMMUTATOR = '&&',
	RESTRICT = contsel,
	JOIN = contjoinsel
);

CREATE OPERATOR @> (
	LEFTARG = _geosotgrid,
	RIGHTARG = _geosotgrid,
	PROCEDURE = gridarray_contains,
	COMMUTATOR = '<@',
	RESTRICT = contsel,
	JOIN = contjoinsel
);

CREATE OPERATOR <@ (
	LEFTARG = _geosotgrid,
	RIGHTARG = _geosotgrid,
	PROCEDURE = gridarray_contained,
	COMMUTATOR = '@>',
	RESTRICT = contsel,
	JOIN = contjoinsel
);

CREATE OPERATOR @ (
	LEFTARG = _geosotgrid,
	RIGHTARG = _geosotgrid,
	PROCEDURE = gridarray_contains,
	COMMUTATOR = '~',
	RESTRICT = contsel,
	JOIN = contjoinsel
);

CREATE OPERATOR ~ (
	LEFTARG = _geosotgrid,
	RIGHTARG = _geosotgrid,
	PROCEDURE = gridarray_contained,
	COMMUTATOR = '@',
	RESTRICT = contsel,
	JOIN = contjoinsel
);

CREATE OPERATOR CLASS gin_grid_ops
DEFAULT FOR TYPE _geosotgrid USING gin
AS
    OPERATOR	3	&&,
	OPERATOR	6	= (anyarray, anyarray),
	OPERATOR	7	@>,
	OPERATOR	8	<@,
	OPERATOR	13	@,
	OPERATOR	14	~,
    FUNCTION    1   gridarray_cmp(geosotgrid, geosotgrid),
    FUNCTION	2	gridarray_extract (anyarray, internal, internal),
	FUNCTION	3	gridarray_queryextract(_geosotgrid, internal, int2, internal, internal, internal, internal),
	FUNCTION	4	gridarray_consistent(internal, int2, _geosotgrid, int4, internal, internal, internal, internal),
	STORAGE 		geosotgrid;

----------------------------------------geosotgrid function----------------------------------------

CREATE OR REPLACE FUNCTION ST_GeoSOTGrid(geom geometry, level int)
	RETURNS _geosotgrid
	AS '$libdir/yukon_geogridcoder-1.0' ,'gsg_geosotgrid'
	LANGUAGE 'c'  IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION ST_GeoSOTGridZ(altitude float8, level int)
	RETURNS int4
	AS '$libdir/yukon_geogridcoder-1.0' ,'gsg_geosotgrid_z'
	LANGUAGE 'c' IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION ST_AltitudeFromGeoSOTGridZ(z_num int4, level int)
	RETURNS float8
	AS '$libdir/yukon_geogridcoder-1.0' ,'gsg_as_altitude'
	LANGUAGE 'c' IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION ST_GeoSOTGridFromText(geosotgrid2d cstring)
	RETURNS geosotgrid
	AS '$libdir/yukon_geogridcoder-1.0' ,'gsg_geosotgrid_from_text'
	LANGUAGE 'c' IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION ST_GeoSOTGridFromText(geosotgrid2d cstring, geosotgrid_z cstring)
	RETURNS geosotgrid
	AS '$libdir/yukon_geogridcoder-1.0' ,'gsg_geosotgrid3d_from_text'
	LANGUAGE 'c' IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION ST_AsText(grid geosotgrid)
	RETURNS text
	AS '$libdir/yukon_geogridcoder-1.0' ,'gsg_geosotgrid_as_text'
	LANGUAGE 'c' IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION ST_GeomFromGeoSOTGrid(grid geosotgrid)
	RETURNS geometry
	AS '$libdir/yukon_geogridcoder-1.0' ,'gsg_geom_from_geosotgrid'
	LANGUAGE 'c' IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION ST_GeomFromGeoSOTGrid(gridarray _geosotgrid)
	RETURNS geometry[]
	AS '$libdir/yukon_geogridcoder-1.0' ,'gsg_geom_from_geosotgrid_array'
	LANGUAGE 'c' IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION ST_HasZ(grid geosotgrid)
	RETURNS bool
	AS '$libdir/yukon_geogridcoder-1.0' ,'gsg_has_z'
	LANGUAGE 'c' IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION ST_GetLevel(grid geosotgrid)
	RETURNS int2
	AS '$libdir/yukon_geogridcoder-1.0' ,'gsg_get_level'
	LANGUAGE 'c' IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION ST_Degenerate(grid geosotgrid, level int)
	RETURNS geosotgrid
	AS '$libdir/yukon_geogridcoder-1.0' ,'gsg_degenerate'
	LANGUAGE 'c' IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION ST_Degenerate(gridarray _geosotgrid, level int)
	RETURNS _geosotgrid
	AS '$libdir/yukon_geogridcoder-1.0' ,'gsg_degenerate_array'
	LANGUAGE 'c' IMMUTABLE STRICT PARALLEL SAFE;
----------------------------------------geomhash function----------------------------------------

CREATE OR REPLACE FUNCTION ST_GeoHash(boundary box2d,geom geometry,level int default 1)
	RETURNS int8[]
	AS '$libdir/yukon_geogridcoder-1.0' ,'gsg_geohash'
	LANGUAGE 'c'  IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION ST_BoxGeoHash(bbox box2d,querybox box2d,level int default 1)
	RETURNS int8[]
	AS '$libdir/yukon_geogridcoder-1.0' ,'gsg_boxgeohash'
	LANGUAGE 'c'  IMMUTABLE STRICT;


-- tables 要合并的数据表
-- level hash 划分等级
-- schema 表所在的模式名
create or replace function GSG_HashInit(tables text[], level integer default 1, schema text default 'public')
    RETURNS boolean
AS
$$
DECLARE
    _max          integer;
    _sqls         text;
    kw            text;
    _status       boolean;
    _schema       text;
    _count        integer;
    _globalleft   float8;
    _globalright  float8;
    _globaltop    float8;
    _globalbottom float8;
    _ext          box2d;
BEGIN

    _max := array_length(tables, 1);
    -- 判断是否是一个空数组
    if _max is null then
        raise 'no table in the tables';
        return false;
    end if;

    -- 判断所有的表是否都包含 geometry 列
    <<kwloop>>
    FOR x in 1.._max
        LOOP
            kw := quote_literal(tables[x]);
            _count := 0;
            _sqls := 'select count(*) from geometry_columns where f_table_schema= ' || quote_literal(schema) ||
                     ' and f_table_name=' || kw;
            execute _sqls into _count;

            if _count = 0 then
                raise 'the table % not contain a geometry column',kw;
            end if;

        END LOOP kwloop;

    _sqls = 'drop schema if exists yk_gsg cascade';
    execute _sqls;
    _sqls = 'create schema yk_gsg';
    execute _sqls;

    _sqls = 'create table yk_gsg.geohash(id serial,tablename text,smid bigint,geom geometry,hash bigint[])';
    execute _sqls;

    -- 将各个表收集到 geohash
    _sqls := 'select st_extent(smgeometry) from ' || quote_ident(tables[1]);
    execute _sqls into _ext;
    _globalbottom := st_ymin(_ext);
    _globaltop := st_ymax(_ext);
    _globalleft := st_xmin(_ext);
    _globalright := st_xmax(_ext);

    <<kwloop>>
    FOR x in 1.._max
        LOOP
            _count := 0;
            _sqls = 'insert into yk_gsg.geohash (tablename,smid,geom) ' || '(select ' || quote_literal(tables[x]) ||
                    ',smid ,smgeometry from ' || quote_ident(schema) || '.' || quote_ident(tables[x]) || ')';
            execute _sqls;

            -- 计算整体范围
            _sqls = 'select st_extent(smgeometry) from ' || quote_ident(tables[x]);

            execute _sqls into _ext;

            _globalbottom := LEAST(_globalbottom, st_ymin(_ext));
            _globaltop := GREATEST(_globaltop, st_ymax(_ext));
            _globalleft := LEAST(_globalleft, st_xmin(_ext));
            _globalright := GREATEST(_globalright, st_xmax(_ext));


            raise notice 'collect data from :%',kw;

        END LOOP kwloop;

    -- 更新 geohash 中的 hash 列

    _sqls = 'update yk_gsg.geohash set hash = st_geohash(''BOX(' || _globalleft || ' ' || _globalbottom || ',' ||
            _globalright || ' ' || _globaltop || ')'',geom,' || level || ')';

    execute _sqls;


    _sqls = 'alter table yk_gsg.geohash drop column geom';
    execute _sqls;

	_sqls = 'create index geohash_index on yk_gsg.geohash using gin(hash);';
    execute _sqls;

    RETURN TRUE;
END ;
$$ LANGUAGE 'plpgsql' VOLATILE;

create or replace function GSG_HashIntersects(globalbox box2d, querybox box2d, level integer default 10)
    RETURNS json
AS
$$
DECLARE
    _count integer;
    _sql   text;
    _res json;
BEGIN
    -- 检查是否有 yk_gsg.geohash 表
    _sql = 'select count(*) from pg_tables where tablename=''geohash'' and schemaname=''yk_gsg''';
    execute _sql into _count;

    if _count = 0 then
        raise 'you should use GSG_HashInit first';
    end if;

    _sql = 'drop table if exists tempgeohash';

    execute _sql;

    _sql = 'create temporary table tempgeohash as (select tablename,smid from yk_gsg.geohash where ST_BoxGeoHash(' || quote_literal(globalbox) ||
           '::box2d,' || quote_literal(querybox) || '::box2d,' || level || ') && hash)';
    --raise notice '%',_sql;
    execute _sql;

    _sql = 'select json_agg(tempgeohash) from tempgeohash';
    execute _sql into _res;

    return _res;

END ;
$$ LANGUAGE 'plpgsql' VOLATILE;


------------------------------------------------------------------ GeoSOT 接口函数 --------------------------------------------------------------

create or replace function GSG_GeoSOTInit(tables text[], level integer default 9, schema text default 'public')
    RETURNS boolean
AS
$$
DECLARE
    _max          integer;
    _sqls         text;
    kw            text;
    _count        integer;
BEGIN

    _max := array_length(tables, 1);
    -- 判断是否是一个空数组
    if _max is null then
        raise 'no table in the tables';
    end if;

    -- 判断所有的表是否都包含 geometry 列
    <<tableloop>>
    FOR x in 1.._max
        LOOP
            kw := quote_literal(tables[x]);
            _count := 0;
            _sqls := 'select count(*) from geometry_columns where f_table_schema= ' || quote_literal(schema) ||
                     ' and f_table_name=' || kw || ' and srid=4490';
            execute _sqls into _count;

            if _count = 0 then
                raise 'the table % not contain a geometry column or the srid is not 4490',kw;
            end if;

        END LOOP tableloop;

    _sqls = 'drop schema if exists yk_gsggeosot cascade';
    execute _sqls;
    _sqls = 'create schema yk_gsggeosot';
    execute _sqls;

    _sqls = 'create table yk_gsggeosot.geogrid(id serial,tablename text,smid bigint,geom geometry,grid geosotgrid[])';
    execute _sqls;

    -- 将输入插入总表中
    FOR x in 1.._max
        LOOP
            _count := 0;
            _sqls = 'insert into yk_gsggeosot.geogrid (tablename,smid,geom) ' || '(select ' || quote_literal(tables[x]) ||
                    ',smid ,smgeometry from ' || quote_ident(schema) || '.' || quote_ident(tables[x]) || ')';
            execute _sqls;

            raise notice 'collect data from :%',kw;

        END LOOP ;

    -- 更新 geogrid 中的 grid 列

    _sqls = 'update yk_gsggeosot.geogrid set grid = st_getgrid(geom,' || level || ')';

    execute _sqls;

    _sqls = 'alter table yk_gsggeosot.geogrid drop column geom';
    execute _sqls;

	_sqls = 'create index geogrid_index on yk_gsggeosot.geogrid using gin(grid);';
    execute _sqls;

    RETURN TRUE;
END ;
$$ LANGUAGE 'plpgsql' VOLATILE;


create or replace function GSG_GeoSOTIntersects(querygeom geometry, level integer default 10)
    RETURNS json
AS
$$
DECLARE
    _count integer;
    _sql   text;
    _res   json;
BEGIN
    -- 检查是否有 yk_gsg.geohash 表
    _sql = 'select count(*) from pg_tables where tablename=''geogrid'' and schemaname=''yk_gsggeosot''';
    execute _sql into _count;

    if _count = 0 then
        raise 'you should use GSG_GeoSOTInit first';
    end if;

    _sql = 'drop table if exists tempgeogrid';

    execute _sql;

    _sql := 'create temporary table tempgeogrid as (select tablename,count(smid) as times from yk_gsggeosot.geogrid where ST_GetGrid(' ||
            quote_literal(querygeom::text) || '::geometry,' || level || ') && grid group by (tablename))';
    --raise notice '%',_sql;
    execute _sql;

    _sql = 'select json_agg(tempgeogrid) from tempgeogrid';
    execute _sql into _res;
    return _res;

END ;
$$ LANGUAGE 'plpgsql' VOLATILE;


------------------------------------------------------------------ 绘制网格函数 -----------------------------------------------------------------
create or replace
    function st_makegrid16(_level integer, _left integer, _right integer, _bottom integer, _top integer)
    returns bool
as
$$
declare
    _sql text;
    x1   float8;
    y1   float8;
    x2   float8;
    y2   float8;
    x3   float8;
    y3   float8;
    x4   float8;
    y4   float8;

begin

    if _left > _right
        or _bottom > _top then
        raise notice 'the argument is invalid';

        return false;
    end if;
-- 删除已经存在的表
    _sql := 'drop table if exists sotgrid' || _level;

    execute _sql;
-- 创建新表
    _sql := 'create table sotgrid' || _level || '(id serial,geom geometry(polygon,4490))';

    execute _sql;

    for i in _left.._right - 1
        loop
            for j in _bottom.._top - 1
                loop
                    for m in 0..60 * pow(2, _level - 15)
                        loop
                            for n in 0 .. 60 * pow(2, _level - 15)
                                loop
                                    x1 = i + m / 60.0 / pow(2, _level - 15);

                                    y1 = j + n / 60.0 / pow(2, _level - 15);

                                    x2 = i + (m + 1) / 60.0 / pow(2, _level - 15);

                                    y2 = j + n / 60.0 / pow(2, _level - 15);

                                    x3 = i + (m + 1) / 60.0 / pow(2, _level - 15);

                                    y3 = j + (n + 1) / 60.0 / pow(2, _level - 15);

                                    x4 = i + m / 60.0 / pow(2, _level - 15);

                                    y4 = j + (n + 1) / 60.0 / pow(2, _level - 15);

                                    _sql = 'insert into sotgrid' || _level ||
                                           E'(geom) values(ST_MakePolygon(ST_GeomFromText(\' LINESTRING('
                                               || x1 || ' ' || y1 || ','
                                               || x2 || ' ' || y2 || ','
                                               || x3 || ' ' || y3 || ','
                                               || x4 || ' ' || y4 || ','
                                               || x1 || ' ' || y1
                                        || E')\')))';

                                    execute _sql;
                                end loop;
                        end loop;
                end loop;
        end loop;

    return true;
end ;

$$ language 'plpgsql' volatile;


create or replace function st_makegrid10_15(_level integer, _left integer, _right integer, _bottom integer,
                                            _top integer)
    RETURNS bool
AS
$$
DECLARE
    _sql text;
    step float8;
    x1   float8;
    y1   float8;
    x2   float8;
    y2   float8;
    x3   float8;
    y3   float8;
    x4   float8;
    y4   float8;

BEGIN

    if _left > _right or _bottom > _top then
        raise notice 'the argument is invalid';
        return false;
    end if;

    -- 删除已经存在的表
    _sql := 'drop table if exists sotgrid' || _level;
    execute _sql;

    -- 创建新表
    _sql := 'create table sotgrid' || _level || '(id serial,geom geometry(polygon,4490))';
    execute _sql;

    for i in _left.._right - 1
        loop
            for j in _bottom.._top -1
                loop
                    for m in 0..pow(2, (_level - 9))
                        loop
                            for n in 0 .. pow(2, (_level - 9))
                                loop
                                    step = 64 / (pow(2, _level - 9)) / 60;
                                    --x1 x2 x3 x4
                                    if m * step < 1 then
                                        x1 = m * step + i;
                                    else
                                        x1 = i + 1;
                                    end if;
                                    x2 := x1;
                                    if (m + 1) * step < 1 then
                                        x3 = (m + 1) * step + i;
                                    else
                                        x3 = i + 1;
                                    end if;
                                    x4 := x3;
                                    -- y1 y2 y3 y4
                                    if n * step < 1 then
                                        y1 = n * step + j;
                                    else
                                        y1 = j + 1;
                                    end if;
                                    y4 := y1;
                                    if (n + 1) * step < 1 then
                                        y2 = (n + 1) * step + j;
                                    else
                                        y2 = j + 1;
                                    end if;
                                    y3 := y2;
                                    _sql = 'insert into sotgrid' || _level ||
                                           E'(geom) values(ST_MakePolygon(ST_GeomFromText(\'LINESTRING('
                                               || x1 || ' ' || y1 || ','
                                               || x2 || ' ' || y2 || ','
                                               || x3 || ' ' || y3 || ','
                                               || x4 || ' ' || y4 || ','
                                               || x1 || ' ' || y1 || E')\')))';
                                    --raise notice '%',_sql;
                                    execute _sql;
                                end loop;
                        end loop;
                end loop;
        end loop;
    return true;

END ;
$$ LANGUAGE 'plpgsql' VOLATILE;

create or replace function st_makegrid9(_left integer, _right integer, _bottom integer, _top integer)
    RETURNS bool
AS
$$
DECLARE
    _sql text;
BEGIN

    if _left > _right or _bottom > _top then
        raise notice 'the argument is invalid';
        return false;
    end if;

    -- 删除已经存在的表
    _sql := 'drop table if exists sotgrid9';
    execute _sql;

    -- 创建新表
    _sql := 'create table sotgrid9(id serial,geom geometry(polygon,4490))';
    execute _sql;

    for i in _left.._right - 1
        loop
            for j in _bottom.._top -1
                loop
                    _sql = E'insert into sotgrid9 (geom) values(ST_MakePolygon(ST_GeomFromText(\'LINESTRING('
                               || i || ' ' || j || ','
                               || i || ' ' || j + 1 || ','
                               || i + 1 || ' ' || j + 1 || ','
                               || i + 1 || ' ' || j || ','
                               || i || ' ' || j
                        || E')\')))';
                    --raise notice '%',_sql;
                    execute _sql;
                end loop;
        end loop;


    return true;

END ;
$$ LANGUAGE 'plpgsql' VOLATILE;