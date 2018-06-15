# S19-Extractor
This program extracts hex data from SREC ([Motorola S-record](https://en.wikipedia.org/wiki/SREC_(file_format))) file
  
**Sources:**

* s19Converter.cpp

**Input file** s19 formated with program for example purpose:  

* example.s19

**Output file** with structured data in simple hex arrays:  

* flashProgram.h

## Description  
This program converts binary code s19 formated to simple hex array and allocates data by memory sections with size of 64 bytes each. The output array printed to file flashProgram.h that may be included in any project. 
The generated file contains two arrays:
```c++
blockAddrArray[] = {Address of first block in target memory, size of first block, ...};
blockAddrArray[] = {first byte, ...};
```
**USAGE:** you need only to specify one parameter that is input file name  
**EXAMPLE:**  
```bash
ThisProgram example.s19
```
