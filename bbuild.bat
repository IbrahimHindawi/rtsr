@echo off
rem create build dir
mkdir build

rem go to build dir
pushd build

rem main exe name
set project_name=main

rem compiler path
set cl_path="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30133\bin\Hostx86\x64"

rem package manager path
set vcpkg_path=%userprofile%\vcpkg\installed\x64-windows

rem compiler command
%cl_path%\cl.exe /nologo ..\src\%project_name%.c ..\src\display.c ..\src\salloc.c ..\src\vector.c ..\src\triangle.c ..\src\mesh.c /DEBUG:FULL /Z7 /I%vcpkg_path%\include user32.lib gdi32.lib %vcpkg_path%\lib\SDL2.lib

rem add dynamic lib with exe
XCOPY /Q /S /Y %vcpkg_path%\bin\SDL2.dll .

popd
