#!/bin/bash

inputFile="./tests/0-div-mod.imp"
outputFile="./asm.txt"
resultFile="./result.txt"
expectedFile="./tests/0-div-mod-expected.txt"
input="1 0"

title="0-div-mod"
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


inputFile="./tests/1-numbers.imp"
outputFile="./asm.txt"
resultFile="./result.txt"
expectedFile="./tests/1-numbers-expected.txt"
input="-7"

title="1-numbers"
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

inputFile="./tests/2-fib.imp"
outputFile="./asm.txt"
resultFile="./result.txt"
expectedFile="./tests/2-fib-expected.txt"
input="1"

title="2-fib"
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

inputFile="./tests/3-fib-factorial.imp"
outputFile="./asm.txt"
resultFile="./result.txt"
expectedFile="./tests/3-fib-factorial-expected.txt"
input="20"

title="3-fib-factorial"
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

inputFile="./tests/4-factorial.imp"
outputFile="./asm.txt"
resultFile="./result.txt"
expectedFile="./tests/4-factorial-expected.txt"
input="20"

title="4-factorial"
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

inputFile="./tests/5-tab.imp"
outputFile="./asm.txt"
resultFile="./result.txt"
expectedFile="./tests/5-tab-expected.txt"

title="5-tab"
echo "In test: $title:"
./compiler $inputFile $outputFile
if [ $? -eq 1 ] ; then
    echo -e "COMPILATION FAILURE\n"
else
    ./virtual_machine/maszyna-wirtualna $outputFile | sed 's/? //g' | grep ">" | sed 's/> //g' >$resultFile
    diffRes=$(diff $resultFile $expectedFile)
    echo $diffRes
    if [ "$diffRes" ]; then
        echo -e " \033[0;31mFAILED\033[0m"
    else
        echo -e " \033[0;32mPASSED\033[0m"
    fi
fi

rm -f $outputFile $resultFile

inputFile="./tests/6-mod-mult.imp"
outputFile="./asm.txt"
resultFile="./result.txt"
expectedFile="./tests/6-mod-mult-expected.txt"
input="1234567890 1234567890987654321 987654321"

title="6-mod-mult"
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

inputFile="./tests/7-loopiii.imp"
outputFile="./asm.txt"
resultFile="./result.txt"
expectedFile="./tests/7-loopiii-expected.txt"
input="0 0 0"

title="7-loopiii"
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

inputFile="./tests/7-loopiii.imp"
outputFile="./asm.txt"
resultFile="./result.txt"
expectedFile="./tests/7-loopiii-expected2.txt"
input="1 0 2"

title="7-loopiii"
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

inputFile="./tests/8-for.imp"
outputFile="./asm.txt"
resultFile="./result.txt"
expectedFile="./tests/8-for-expected.txt"
input="12 23 34"

title="8-for"
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

inputFile="./tests/9-sort.imp"
outputFile="./asm.txt"
resultFile="./result.txt"
expectedFile="./tests/9-sort-expected.txt"

title="9-sort"
echo "In test: $title:"
./compiler $inputFile $outputFile
if [ $? -eq 1 ] ; then
    echo -e "COMPILATION FAILURE\n"
else
    ./virtual_machine/maszyna-wirtualna $outputFile | sed 's/? //g' | grep ">" | sed 's/> //g' >$resultFile
    diffRes=$(diff $resultFile $expectedFile)
    echo $diffRes
    if [ "$diffRes" ]; then
        echo -e " \033[0;31mFAILED\033[0m"
    else
        echo -e " \033[0;32mPASSED\033[0m"
    fi
fi

rm -f $outputFile $resultFile

inputFile="./tests/program0.imp"
outputFile="./asm.txt"
resultFile="./result.txt"
expectedFile="./tests/program0-expected.txt"
input="18"

title="program0"
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

inputFile="./tests/program1.imp"
outputFile="./asm.txt"
resultFile="./result.txt"
expectedFile="./tests/program1-expected.txt"

title="program1"
echo "In test: $title:"
./compiler $inputFile $outputFile
if [ $? -eq 1 ] ; then
    echo -e "COMPILATION FAILURE\n"
else
    ./virtual_machine/maszyna-wirtualna $outputFile | sed 's/? //g' | grep ">" | sed 's/> //g' >$resultFile
    diffRes=$(diff $resultFile $expectedFile)
    echo $diffRes
    if [ "$diffRes" ]; then
        echo -e " \033[0;31mFAILED\033[0m"
    else
        echo -e " \033[0;32mPASSED\033[0m"
    fi
fi

rm -f $outputFile $resultFile

inputFile="./tests/program2.imp"
outputFile="./asm.txt"
resultFile="./result.txt"
expectedFile="./tests/program2-expected.txt"
input="1234567890"

title="program2"
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

inputFile="./tests/program2.imp"
outputFile="./asm.txt"
resultFile="./result.txt"
expectedFile="./tests/program2-expected2.txt"
input="12345678901"

title="program2"
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

inputFile="./tests/program2.imp"
outputFile="./asm.txt"
resultFile="./result.txt"
expectedFile="./tests/program2-expected3.txt"
input="12345678903"

title="program2"
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

inputFile="./tests/div-mod.imp"
outputFile="./asm.txt"
resultFile="./result.txt"
expectedFile="./tests/div-mod-expected.txt"
input="26 7"

title="div-mod"
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