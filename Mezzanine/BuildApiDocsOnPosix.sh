#!/bin/bash
cd src
mkdir -p ../../docs/doxygen/html/
doxygen Doxyfile 2> doxyfiles/warnings 1> doxyfiles/creationlog
cp doxyfiles/t*.png ../../docs/doxygen/html/
cp doxyfiles/b*.png ../../docs/doxygen/html/
cp doxyfiles/n*.png ../../docs/doxygen/html/
