@echo off

set SCRIPT_DIR=%cd%
set DATA_DIR=../data
set CSV_DIR=%DATA_DIR%/csv

set MA1_DIR=D:/Emulators/Xbox1/ISO/MechAssault_USA_En-Fr-De_v108_redump_27163
set MA1_FILES=%MA1_DIR%/extracted
set MA1_XBE=%MA1_DIR%/default.xbe
set MA2_DIR=D:/Emulators/Xbox1/ISO/MechAssault2LoneWolf_World_En-Ja-Fr-De-Es-It_redump_9236
set MA2_FILES=%MA2_DIR%/Extracted
set MA2_XBE=%MA2_DIR%/ma2.xbe

set SQLITE=D:/bin/sqlite/sqlite3.exe
set SYSINTERNALS_STRINGS=D:/bin/SysinternalsSuite/strings.exe

set TEMPLATE_DIR=../templates
set WDF_TEMPLATE=%TEMPLATE_DIR%/wdf.bt
set GGF_TEMPLATE=%TEMPLATE_DIR%/ggf.bt


REM Step 1: Export all strings from MechAssault's XBE using SysInternals' strings.exe
echo Exporting strings from %MA1_XBE%...
%SYSINTERNALS_STRINGS% -a %MA1_XBE% > %DATA_DIR%/ma1_strings.txt

echo Exporting strings from %MA2_XBE%...
%SYSINTERNALS_STRINGS% -a %MA2_XBE% > %DATA_DIR%/ma2_strings.txt

cd %DATA_DIR%
type ma1_strings.txt ma2_strings.txt > ma_strings.txt
cd %SCRIPT_DIR%

REM Step 2: Calculate CRC32/JAM-CRC on exported strings using py script
echo Calculating CRCs on strings...
python generate_string_crc.py %DATA_DIR%/ma_strings.txt %DATA_DIR%/ma_crc_strings.csv

REM Step 3: Run 010editor binary templates on all wdf, mtb, and ggf files to export CRCs to CSV files
call :RunTemplate wdf, %MA1_FILES%, "%WDF_TEMPLATE%", "%CSV_DIR%/", "ma1-"
call :RunTemplate wdf, %MA2_FILES%, "%WDF_TEMPLATE%", "%CSV_DIR%/", "ma2-"
call :Concat wdf

call :RunTemplate sdf, %MA1_FILES%, "%WDF_TEMPLATE%", "%CSV_DIR%/", "ma1-"
call :Concat sdf

call :RunTemplate mtb, %MA1_FILES%, "%WDF_TEMPLATE%", "%CSV_DIR%/", "ma1-"
call :RunTemplate mtb, %MA2_FILES%, "%WDF_TEMPLATE%", "%CSV_DIR%/", "ma2-"
call :Concat mtb

call :RunTemplate ggf, %MA2_FILES%, "%GGF_TEMPLATE%", "%CSV_DIR%/", "ma2-"
call :Concat ggf

REM Step 4: Use SQL to create tables of all exported CRCs and join them together to match CRCs to the source strings from the XBE
%SQLITE% < map_strings.sql

REM Step 5: Write matched strings/CRCs to 010editor scripts
echo Creating binary templates...
python generate_template_crc.py ../data/wdf_names.csv ../templates/get_wdf_string.bt
python generate_template_crc.py ../data/mtb_names.csv ../templates/get_mtb_string.bt
python generate_template_crc.py ../data/ggf_names.csv ../templates/get_ggf_string.bt

exit /B 0


:RunTemplate
setlocal enableextensions enabledelayedexpansion
set /A total=0
for /R %~2 %%f in (*.%~1) do set /A total=total+1
set /A i=0
for /R %~2 %%f in (*.%~1) do start /wait 010editor "%%f" -script:run_template_export_csv.1sc:(%~3,%~4,%~5) -noui && set /A i=i+1 && echo (!i!/%total%) Exported CRCs from %%f
endlocal
exit /B 0

:Concat
cd %CSV_DIR%
echo Concatenating CSVs (%~1)... && type *.%~1.csv > %~1.txt && del *.%~1.csv && ren %~1.txt %~1.csv
cd %SCRIPT_DIR%
exit /B 0
