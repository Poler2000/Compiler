#!/bin/bash
inputFile="./tests/test0.imp"
outputFile="./asm.txt"
input="2 -2 2 -2 2 -2 2 -2"
resultFile="./result.txt"
expectedFile="./tests/test0-expected.txt"

title="test0"
echo "In test: $title:"
./compiler $inputFile $outputFile
if [ $? -eq 1 ] ; then
    echo -e " \033[0;31mCOMPILATION FAILURE\033[0m"
else
    echo "input: $input"
    echo "$input" | ./virtual_machine/maszyna-wirtualna $outputFile | sed 's/? //g' | grep ">" | sed 's/> //g' >$resultFile
    diffRes=$(diff $resultFile $expectedFile)
    echo $diffRes
    if [ "$diffRes" ]; then
        echo -e " \033[0;31mFAILED\033[0m"
    else
        echo -e " \033[0;32mPASSED\033[0m"
    fi
fi

rm -f $outputFile $resultFile

inputFile="./tests/test1a.imp"
outputFile="./asm.txt"
expectedFile="./tests/test1a-expected.txt"
resultFile="./result.txt"
input="27"

title="test1a"
echo "In test: $title:"
./compiler $inputFile $outputFile
if [ $? -eq 1 ] ; then
    echo -e " \033[0;31mCOMPILATION FAILURE\033[0m"
else
    echo "input: $input"
    echo "$input" | ./virtual_machine/maszyna-wirtualna $outputFile | sed 's/? //g' | grep ">" | sed 's/> //g' >$resultFile
    diffRes=$(diff $resultFile $expectedFile)
    echo $diffRes
    if [ "$diffRes" ]; then
        echo -e " \033[0;31mFAILED\033[0m"
    else
        echo -e " \033[0;32mPASSED\033[0m"
    fi
fi

rm -f $outputFile

inputFile="./tests/test1b.imp"
outputFile="./asm.txt"
resultFile="./result.txt"
input="27"

title="test1b"
echo "In test: $title:"
./compiler $inputFile $outputFile
if [ $? -eq 1 ] ; then
    echo -e " \033[0;31mCOMPILATION FAILURE\033[0m"
else
    echo "input: $input"
    echo "$input" | ./virtual_machine/maszyna-wirtualna $outputFile | sed 's/? //g' | grep ">" | sed 's/> //g' >$resultFile
    diffRes=$(diff $resultFile $expectedFile)
    echo $diffRes
    if [ "$diffRes" ]; then
        echo -e " \033[0;31mFAILED\033[0m"
    else
        echo -e " \033[0;32mPASSED\033[0m"
    fi
fi

rm -f $outputFile

inputFile="./tests/test1c.imp"
outputFile="./asm.txt"
resultFile="./result.txt"
input="27"

title="test1c"
echo "In test: $title:"
./compiler $inputFile $outputFile
if [ $? -eq 1 ] ; then
    echo -e " \033[0;31mCOMPILATION FAILURE\033[0m"
else
    echo "input: $input"
    echo "$input" | ./virtual_machine/maszyna-wirtualna $outputFile | sed 's/? //g' | grep ">" | sed 's/> //g' >$resultFile
    diffRes=$(diff $resultFile $expectedFile)
    echo $diffRes
    if [ "$diffRes" ]; then
        echo -e " \033[0;31mFAILED\033[0m"
    else
        echo -e " \033[0;32mPASSED\033[0m"
    fi
fi

rm -f $outputFile

inputFile="./tests/test1d.imp"
outputFile="./asm.txt"
resultFile="./result.txt"
input="27"

title="test1d"
echo "In test: $title:"
./compiler $inputFile $outputFile
if [ $? -eq 1 ] ; then
    echo -e " \033[0;31mCOMPILATION FAILURE\033[0m"
else
    echo "input: $input"
    echo "$input" | ./virtual_machine/maszyna-wirtualna $outputFile | sed 's/? //g' | grep ">" | sed 's/> //g' >$resultFile
    diffRes=$(diff $resultFile $expectedFile)
    echo $diffRes
    if [ "$diffRes" ]; then
        echo -e " \033[0;31mFAILED\033[0m"
    else
        echo -e " \033[0;32mPASSED\033[0m"
    fi
fi

rm -f $outputFile $resultFile

inputFile="./tests/test2.imp"
outputFile="./asm.txt"
resultFile="./result.txt"
expectedFile="./tests/test2-expected.txt"
title="test2"
echo "In test: $title:"
./compiler $inputFile $outputFile
if [ $? -eq 1 ] ; then
    echo -e "COMPILATION FAILURE\n"
else
    ./virtual_machine/maszyna-wirtualna-cln $outputFile | sed 's/? //g' | grep ">" | sed 's/> //g' >$resultFile
    diffRes=$(diff $resultFile $expectedFile)
    echo $diffRes
    if [ "$diffRes" ]; then
        echo -e " \033[0;31mFAILED\033[0m"
    else
        echo -e " \033[0;32mPASSED\033[0m"
    fi
fi

rm -f $outputFile

