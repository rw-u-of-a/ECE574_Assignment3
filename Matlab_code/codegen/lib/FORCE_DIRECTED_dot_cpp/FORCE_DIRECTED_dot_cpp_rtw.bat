@echo off

call "%VS140COMNTOOLS%..\..\VC\vcvarsall.bat" AMD64


call  "D:\Program Files\MATLAB\R2016b\bin\win64\checkMATLABRootForDriveMap.exe" "D:\Program Files\MATLAB" R2016b  > mlEnv.txt
for /f %%a in (mlEnv.txt) do set "%%a"\n
cd .

if "%1"=="" (nmake  -f FORCE_DIRECTED_dot_cpp_rtw.mk all) else (nmake  -f FORCE_DIRECTED_dot_cpp_rtw.mk %1)
@if errorlevel 1 goto error_exit

exit /B 0

:error_exit
echo The make command returned an error of %errorlevel%
An_error_occurred_during_the_call_to_make
