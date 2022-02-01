#!/bin/bash

inputFile="./tests/error0.imp"
outputFile="./result.txt"

title="error0"
echo "In test: $title:"
./kompilator $inputFile $outputFile
if [ $? -eq 1 ] ; then
    echo -e "ERROR DETECTED (TEST SUCCESS)\n"
else
    echo -e "NO ERRORS DETECTED (TEST FAILED)\n"
fi

rm -f $outputFile

inputFile="./tests/error1.imp"
outputFile="./result.txt"

title="error1"
echo "In test: $title:"
./kompilator $inputFile $outputFile
if [ $? -eq 1 ] ; then
    echo -e "ERROR DETECTED (TEST SUCCESS)\n"
else
    echo -e "NO ERRORS DETECTED (TEST FAILED)\n"
fi

rm -f $outputFile

inputFile="./tests/error2.imp"
outputFile="./result.txt"

title="error2"
echo "In test: $title:"
./kompilator $inputFile $outputFile
if [ $? -eq 1 ] ; then
    echo -e "ERROR DETECTED (TEST SUCCESS)\n"
else
    echo -e "NO ERRORS DETECTED (TEST FAILED)\n"
fi

rm -f $outputFile

inputFile="./tests/error3.imp"
outputFile="./result.txt"

title="error3"
echo "In test: $title:"
./kompilator $inputFile $outputFile
if [ $? -eq 1 ] ; then
    echo -e "ERROR DETECTED (TEST SUCCESS)\n"
else
    echo -e "NO ERRORS DETECTED (TEST FAILED)\n"
fi

rm -f $outputFile

inputFile="./tests/error4.imp"
outputFile="./result.txt"

title="error4"
echo "In test: $title:"
./kompilator $inputFile $outputFile
if [ $? -eq 1 ] ; then
    echo -e "ERROR DETECTED (TEST SUCCESS)\n"
else
    echo -e "NO ERRORS DETECTED (TEST FAILED)\n"
fi

rm -f $outputFile

inputFile="./tests/error5.imp"
outputFile="./result.txt"

title="error5"
echo "In test: $title:"
./kompilator $inputFile $outputFile
if [ $? -eq 1 ] ; then
    echo -e "ERROR DETECTED (TEST SUCCESS)\n"
else
    echo -e "NO ERRORS DETECTED (TEST FAILED)\n"
fi

rm -f $outputFile

inputFile="./tests/error6.imp"
outputFile="./result.txt"

title="error6"
echo "In test: $title:"
./kompilator $inputFile $outputFile
if [ $? -eq 1 ] ; then
    echo -e "ERROR DETECTED (TEST SUCCESS)\n"
else
    echo -e "NO ERRORS DETECTED (TEST FAILED)\n"
fi

rm -f $outputFile

inputFile="./tests/error7.imp"
outputFile="./result.txt"

title="error7"
echo "In test: $title:"
./kompilator $inputFile $outputFile
if [ $? -eq 1 ] ; then
    echo -e "ERROR DETECTED (TEST SUCCESS)\n"
else
    echo -e "NO ERRORS DETECTED (TEST FAILED)\n"
fi

rm -f $outputFile

inputFile="./tests/error8.imp"
outputFile="./result.txt"

title="error8"
echo "In test: $title:"
./kompilator $inputFile $outputFile
if [ $? -eq 1 ] ; then
    echo -e "ERROR DETECTED (TEST SUCCESS)\n"
else
    echo -e "NO ERRORS DETECTED (TEST FAILED)\n"
fi

rm -f $outputFile

inputFile="./tests/error9.imp"
outputFile="./result.txt"

title="error9"
echo "In test: $title:"
./kompilator $inputFile $outputFile
if [ $? -eq 1 ] ; then
    echo -e "ERROR DETECTED (TEST SUCCESS)\n"
else
    echo -e "NO ERRORS DETECTED (TEST FAILED)\n"
fi

rm -f $outputFile