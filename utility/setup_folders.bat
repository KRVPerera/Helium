@echo off

set CURRENT_DIR=%CD%
cd ..
mkdir generated_files
mkdir generated_files\output_files
mkdir generated_files\filter_files
mkdir generated_files\halide_files
mkdir generated_files\log

cd %CURRENT_DIR%