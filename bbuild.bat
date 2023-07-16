@echo off
rem create build dir
mkdir build

rem go to build dir
pushd build

set project_name=main

set cl_path="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30133\bin\Hostx86\x64"

set root=C:\devel

set dsna_path=%root%\c-dsna

set vcpkg_path=%root%\vcpkg\installed\x64-windows

set cfiles=..\source\%project_name%.c^
    ..\source\display.c^
    ..\source\salloc.c^
    ..\source\vector.c^
    ..\source\triangle.c^
    ..\source\mesh.c^
    ..\source\geometry.c^
    %dsna_path%\source\Array.c

set ifiles=/I%vcpkg_path%\include /I%dsna_path%\include

set libs=user32.lib gdi32.lib %vcpkg_path%\lib\SDL2.lib

rem compiler command
%cl_path%\cl.exe /nologo %cfiles% /DEBUG:FULL /Z7 %ifiles% %libs% 
rem %cl_path%\cl.exe /nologo %cfiles% /O2 /Z7 /INCREMENTAL:NO %ifiles% %libs% /link /OPT:REF /OPT:ICF 

rem copy dll to exe location
XCOPY /Q /S /Y %vcpkg_path%\bin\SDL2.dll .

popd
