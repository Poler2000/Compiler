# Compiler
## Table of contents
* [General info](#general-info)
* [Technologies](#technologies)
* [Setup](#setup)

## General info
This project was completed as part of  Formal Languages and Translation Techniques course at Wrocław University of Science and Technology.
This is a compiler of simple imperative language. 
My primary goal was to create compiler that is relatively simple and perform operations such as multiplication, division and modulo efficiently. 
I was also able to optimize usage of registers on virtual machine in order to avoid expensive operations on memory. 
Virtual machine, provided by the lecturer, is included in 'virtual_machine' folder.
Repository also includes example programs and testing scripts.

## Technologies
* c++ 17
* flex 2.6.4
* bison (GNU Bison) 3.5.1

## Setup
You need flex and bison installed in order to compile the compiler. libcln-dev package might be needed for the version of virtual machine that uses big numbers.
After installing these tools, use:
```shell
$ git clone https://github.com/Poler2000/Compiler.git
$ cd Compiler
$ cd virtual_machine && make && cd ..
$ make
```
You can use compiler like:
```shell
./kompilator $source_file $asm_file
```
And then run compiled program:
```shell
./virtual_machine/maszyna-wirtualna-cln $asm_file
```