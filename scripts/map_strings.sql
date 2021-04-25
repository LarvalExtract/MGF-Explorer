.echo on
.mode csv
.import ../data/ma_crc_strings.csv strings
.import ../data/csv/wdf.csv wdf
.import ../data/csv/mtb.csv mtb
.import ../data/csv/ggf.csv ggf
.import ../data/csv/sdf.csv wdf

CREATE TABLE wdf_crcs AS SELECT DISTINCT Value FROM wdf WHERE Name='CRC_Entity' OR Name='CRC_Attribute';
CREATE TABLE wdf_names AS SELECT wdf_crcs.Value, strings.String FROM wdf_crcs INNER JOIN strings ON wdf_crcs.Value=strings.CRC;
SELECT COUNT (*) FROM wdf_crcs;
SELECT COUNT (*) FROM wdf_names;

.echo off
.output ../data/wdf_names.csv
SELECT DISTINCT * FROM wdf_names;
.output stdout
.echo on

CREATE TABLE mtb_crcs AS SELECT DISTINCT Value FROM mtb WHERE Name='CRC_Entity' OR Name='CRC_Attribute';
CREATE TABLE mtb_names AS SELECT mtb_crcs.Value, strings.String FROM mtb_crcs INNER JOIN strings ON mtb_crcs.Value=strings.CRC;
SELECT COUNT (*) FROM mtb_crcs;
SELECT COUNT (*) FROM mtb_names;

.echo off
.output ../data/mtb_names.csv
SELECT DISTINCT * FROM mtb_names;
.output stdout
.echo on

CREATE TABLE ggf_crcs AS SELECT DISTINCT Value FROM ggf WHERE Name='CRC_Entity' OR Name='CRC_Attribute';
CREATE TABLE ggf_names AS SELECT ggf_crcs.Value, strings.String FROM ggf_crcs INNER JOIN strings ON ggf_crcs.Value=strings.CRC;
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
