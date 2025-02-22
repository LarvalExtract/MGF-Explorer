.echo on
.mode csv
.import ../data/ma_crc_strings.csv strings
.import ../data/csv/wdf.csv wdf
.import ../data/csv/mtb.csv mtb
.import ../data/csv/ggf.csv ggf
.import ../data/csv/sdf.csv wdf

CREATE TABLE wdf_entity_crcs AS SELECT DISTINCT Value FROM wdf WHERE Name='CRC_Entity';
CREATE TABLE wdf_entity_names AS SELECT DISTINCT wdf_entity_crcs.Value, strings.String FROM wdf_entity_crcs INNER JOIN strings ON wdf_entity_crcs.Value=strings.CRC;
CREATE TABLE wdf_attribute_crcs AS SELECT DISTINCT Value FROM wdf WHERE Name='CRC_Attribute';
CREATE TABLE wdf_attribute_names AS SELECT DISTINCT wdf_attribute_crcs.Value, strings.String FROM wdf_attribute_crcs INNER JOIN strings ON wdf_attribute_crcs.Value=strings.CRC;

.echo off
.output ../data/wdf_entity_names.csv
SELECT DISTINCT * FROM wdf_entity_names;
.output ../data/wdf_attribute_names.csv
SELECT DISTINCT * FROM wdf_attribute_names;
.output stdout
.echo on

CREATE TABLE mtb_class_crcs AS SELECT DISTINCT Value FROM mtb WHERE Name='CRC_Entity';
CREATE TABLE mtb_class_names AS SELECT DISTINCT mtb_class_crcs.Value, strings.String FROM mtb_class_crcs INNER JOIN strings ON mtb_class_crcs.Value=strings.CRC;
CREATE TABLE mtb_attribute_crcs AS SELECT DISTINCT Value FROM mtb WHERE Name='CRC_Attribute';
CREATE TABLE mtb_attribute_names AS SELECT DISTINCT mtb_attribute_crcs.Value, strings.String FROM mtb_attribute_crcs INNER JOIN strings ON mtb_attribute_crcs.Value=strings.CRC;

.echo off
.output ../data/mtb_class_names.csv
SELECT DISTINCT * FROM mtb_class_names;
.output ../data/mtb_attribute_names.csv
SELECT DISTINCT * FROM mtb_attribute_names;
.output stdout
.echo on

CREATE TABLE ggf_crcs AS SELECT DISTINCT Value FROM ggf WHERE Name='CRC_Entity' OR Name='CRC_Attribute';
CREATE TABLE ggf_names AS SELECT DISTINCT ggf_crcs.Value, strings.String FROM ggf_crcs INNER JOIN strings ON ggf_crcs.Value=strings.CRC;
SELECT COUNT (*) FROM ggf_crcs;
SELECT COUNT (*) FROM ggf_names;

.echo off
.output ../data/ggf_names.csv
SELECT DISTINCT * FROM ggf_names;
.output stdout
.echo on

.schema

CREATE TABLE unmatched_crcs (crc);
INSERT INTO unmatched_crcs SELECT Value FROM wdf_names WHERE Value NOT IN wdf_crcs;
INSERT INTO unmatched_crcs SELECT Value FROM mtb_names WHERE Value NOT IN mtb_crcs;
INSERT INTO unmatched_crcs SELECT Value FROM ggf_names WHERE Value NOT IN ggf_crcs;

.echo off
.output ../data/unmatched_crcs.csv
SELECT DISTINCT * FROM unmatched_crcs;
.output stdout
.echo on
