@echo off

set SCRIPT_DIR=%cd%
set DATA_DIR=../data
set CSV_DIR=%DATA_DIR%/csv

set MA_DIR=D:/Emulators/Xbox1/ISO/MechAssault2LoneWolf_World_En-Ja-Fr-De-Es-It_redump_9236
set MA_FILES=%MA_DIR%/Extracted
set MA_XBE=%MA_DIR%/ma2.xbe

set SQLITE=D:/bin/sqlite/sqlite3.exe
set SYSINTERNALS_STRINGS=D:/bin/SysinternalsSuite/strings.exe

set TEMPLATE_DIR=../templates
set WDF_TEMPLATE=%TEMPLATE_DIR%/wdf.bt
set GGF_TEMPLATE=%TEMPLATE_DIR%/ggf.bt


REM Step 1: Export all strings from MechAssault's XBE using SysInternals' strings.exe
REM echo Exporting strings from %MA_XBE%...
REM %SYSINTERNALS_STRINGS% -a %MA_XBE% > %DATA_DIR%/ma2_strings.txt

REM Step 2: Calculate CRC32/JAM-CRC on exported strings using py script
REM echo Calculating CRCs on strings...
REM python generate_string_crc.py %DATA_DIR%/ma2_strings.txt %DATA_DIR%/ma2_crc_strings.csv

REM Step 3: Run 010editor binary templates on all wdf, mtb, and ggf files to export CRCs to CSV files
REM call :RunTemplate wdf, "%WDF_TEMPLATE%", "%CSV_DIR%/"
REM call :RunTemplate mtb, "%WDF_TEMPLATE%", "%CSV_DIR%/"
REM call :RunTemplate ggf, "%GGF_TEMPLATE%", "%CSV_DIR%/"

REM Step 4: Use SQL to create tables of all exported CRCs and join them together to match CRCs to the source strings from the XBE
%SQLITE% < map_strings.sql

REM Step 6: Write matched strings/CRCs to 010editor script switch statement for binary templates

exit /B 0


:RunTemplate
setlocal enableextensions enabledelayedexpansion
set /A total=0
for /R %MA_FILES% %%f in (*.%~1) do set /A total=total+1
set /A i=0
for /R %MA_FILES% %%f in (*.%~1) do start /wait 010editor "%%f" -script:run_template_export_csv.1sc:(%~2,%~3) -noui && set /A i=i+1 && echo (!i!/%total%) Exported CRCs from %%f
endlocal
cd %CSV_DIR%
echo Concatenating CSVs (%~1)... && type *.%~1.csv > %~1.txt && del *.%~1.csv && ren %~1.txt %~1.csv
cd %SCRIPT_DIR%
exit /B 0


