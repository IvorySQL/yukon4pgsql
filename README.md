<img src=./doc/image/Yukon.png height=250% width=25% alt=Yukon-logo align=center>  
  
Yukon（禹贡），基于 PostgreSQL 数据库扩展地理空间数据的存储和管理能力，并提供专业的GIS（Geographic Information System）功能，赋能传统关系型数据库。
> 注：《禹贡》是《尚书》中的一篇，是中国古代文献中最古老和最有系统性地理观念的著作。
## 模块组织结构

目前，Yukon 基于 PostgreSQL 扩展的模块包括：
1. postgis：与 PostgreSQL 适配的 PostGIS 矢量模块；
2. postgis_raster：与 PostgreSQL  适配的 PostGIS 栅格模块；
3. postgis_sfcgal：与 PostgreSQL 适配的 PostGIS 三维算法相关模块；
4. yukon_geomodel：Yukon 的三维模型数据模块；
5. yukon_geogridcoder：Yukon 的网格编码模块。
6. yukon_vector_pyramid：Yukon 的矢量金字塔模块。



帮助文档参见 [Yukon在线文档](https://yukon.supermap.io/)。