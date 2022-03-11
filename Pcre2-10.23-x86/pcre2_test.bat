@REM This is a generated file.
@echo off
setlocal
SET srcdir="H:\Programming\VSProjects\InfinityProjectsCpp\Pcre2-10.23-x86"
SET pcre2test="H:\Programming\VSProjects\InfinityProjectsCpp\Pcre2-10.23-x86\pcre2test.exe"
if not [%CMAKE_CONFIG_TYPE%]==[] SET pcre2test="H:\Programming\VSProjects\InfinityProjectsCpp\Pcre2-10.23-x86\%CMAKE_CONFIG_TYPE%\pcre2test.exe"
call %srcdir%\RunTest.Bat
if errorlevel 1 exit /b 1
echo RunTest.bat tests successfully completed
