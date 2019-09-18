@echo off

@echo Generating visual studio 2019 64Bit solutions
PUSHD %~dp0

@echo Check exist Project folder
IF EXIST "Project" rd "Project" /s /q
@echo Make new Project folder
md Project
cd Project
@echo Run CMake Script
cmake -DBUILD_PLAYER=ON -DBUILD_RUNTIME=ON -G "Visual Studio 16 2019" -A x64 ..\

@echo:
@echo Solution created in %~dp0\Project
@echo:
POPD
PAUSE
