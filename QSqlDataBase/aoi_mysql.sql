use aoi;
-- 创建主表`production_result`
create table if not exists `production_result` (`id` bigint unsigned not null auto_increment primary key, `board` varchar(50), 
	`ele_name` varchar(30), `ele_index` varchar(30), `timestamp` datetime, `is_ng` tinyint(1), `ng_type` varchar(80),
	`ng_triger` varchar(80),  `pic_path` varchar(100), `loc_x` mediumint, `loc_y` mediumint, `width` mediumint, `height` mediumint);
alter table `production_result` add index search_index(`timestamp`, `board`, `ele_index`);
-- 算子分表00-末端突出
create table if not exists `op_ending_extrude` (`id` bigint unsigned not null auto_increment primary key, `board` varchar(50),  
	`ele_index` varchar(30), `timestamp` datetime, `is_ng` tinyint(1), `percent` varchar(10));
alter table `op_ending_extrude` add index search_index(`timestamp`, `board`, `ele_index`);
-- 算子分表01-错料
create table if not exists `op_wrong_element` (`id` bigint unsigned not null auto_increment primary key, `board` varchar(50),  
	`ele_index` varchar(30), `timestamp` datetime, `is_ng` tinyint(1), `character` varchar(30));
alter table `op_wrong_element` add index search_index(`timestamp`, `board`, `ele_index`);
-- 算子分表02-装反
create table if not exists `op_backwards` (`id` bigint unsigned not null auto_increment primary key, `board` varchar(50),  
	`ele_index` varchar(30), `timestamp` datetime, `is_ng` tinyint(1), `similarity` varchar(10));
alter table `op_backwards` add index search_index(`timestamp`, `board`, `ele_index`);
-- 算子分表03-末端重叠
create table if not exists `op_ending_overlap` (`id` bigint unsigned not null auto_increment primary key, `board` varchar(50),  
	`ele_index` varchar(30), `timestamp` datetime, `is_ng` tinyint(1), `percent` varchar(10));
alter table `op_ending_overlap` add index search_index(`timestamp`, `board`, `ele_index`);

