#! /bin/bash

rm -r cppcheck
cppcheck --enable=all --language=c++ -I include/ -i third_party/ --suppress='*:third_party/*' --force --xml . &> check.xml
cppcheck-htmlreport --file=check.xml --title=cPMML --report-dir=cppcheck reports --source-dir=.
xdg-open cppcheck/index.html
