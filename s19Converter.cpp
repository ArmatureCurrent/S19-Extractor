#include <iostream>
#include <iomanip>
#include <fstream>

typedef unsigned short uint16_t;

#define blockSize (uint16_t)0x40

#define codeSpace (uint16_t)0x2000
#define dataSpace (uint16_t)640

#define codeStartAddr (uint16_t)0x8000
#define dataStartAddr (uint16_t)0x4000

uint16_t **addPtr(uint16_t **ptr, uint16_t size, uint16_t *data, bool isData);
uint16_t getDataBytes(char *ptr);
uint16_t getAddr(char *ptr);

int main(int argc, char *argv[])
{
    char pChar;
    uint16_t strAddr = 0;
    uint16_t countString = 0;
    uint16_t countBytes = 0;
    uint16_t memoryBias = 0x00;
    uint16_t blockID = 0x00;
    uint16_t blockTail = 0x00;
    uint16_t blockCount = 0x00;
    uint16_t **ptrBlocksData = 0x00;
    uint16_t **ptrBlocksAddr = 0x00;

    if(argc != 2)
    {
        std::cout << "This program converts binary code s19 formated\n";
        std::cout << "to simple hex array and allocates code by memory\n";
        std::cout << "sections with size of 64 bytes each.\n";
        std::cout << "The output array printed to file flashProgram.h\n";
        std::cout << "that may be included in any project.\n";
        std::cout << "The generated file contains two arrays:\n";
        std::cout << "blockAddrArray[] = {Address of first block in target memory, size of first block, ...};\n";
        std::cout << "blockAddrArray[] = {byte 1, ...};\n";
        std::cout << "USAGE: you need only to specify one parameter that is input file name\n";
        std::cout << "EXAMPLE: ThisProgram example.s19\n";
        return 0;
    }

    std::ifstream file_in(argv[1], std::fstream::binary);
    std::ofstream file_out("flashProgram.h", std::fstream::trunc | std::fstream::ate);

    if(!file_in.is_open())
    {
        std::cout << "File \'" << argv[1] << "\' couldn't be opened!\n";
        return 0;
    }

    if(!file_out.is_open())
    {
        std::cout << "File tagsFlash.h couldn't be opened!\n";
        return 0;
    }
    std::cout << "Files are opened\n";

    while(!file_in.eof())
    {
        file_in.read(&pChar, 1);

        // -------------------------------------
        // According to s19 format
        // each string should begin
        // with 'S' symbol
        // -------------------------------------
        if(pChar == 'S')
        {
            countString++;

            file_in.read(&pChar, 1);

            // -------------------------------------            
            // Strings beginning with S1 code
            // contain data to be programed
            // into target memory
            // -------------------------------------
            if(pChar == '1')
            {
                // -------------------------------------
                // Read count of bytes
                // in a string of data
                // to be programmed into
                // flash memory
                // -------------------------------------
                char *countBytesInString = new char[2];
                file_in.read(countBytesInString, 2);

                countBytes = getDataBytes(countBytesInString);

                // -------------------------------------
                // String with 3 or less bytes
                // doesn't contain any valuable data
                // -------------------------------------
                if(countBytes <= 3)
                {
                    continue;
                }

                countBytes -= 0x03;

                // -------------------------------------
                // Read adress of the first byte
                // in a string of data
                // to be programmed into
                // flash memory
                // -------------------------------------
                char *address = new char[4];
                file_in.read(address, 4);

                strAddr = getAddr(address);

                // -------------------------------------
                // Read all bytes in a string
                // of data to be programmed into
                // flash memory.
                // Control sum is not checked
                // -------------------------------------
                char *bytesString = new char[countBytes * 2];
                file_in.read(bytesString, countBytes * 2);

                // -------------------------------------
                // Distribute data bytes in blocks
                // -------------------------------------

                // Check location of the data string.
                // EEPROM or PROGRAMM FLASH
                if((strAddr >= codeStartAddr) && (strAddr <= (codeStartAddr + codeSpace)))
                {
                    memoryBias = codeStartAddr;
                }
                else if((strAddr >= dataStartAddr) && (strAddr <= (dataStartAddr + dataSpace)))
                {
                    memoryBias = dataStartAddr;
                }
                else
                {
                    memoryBias = 0x0000;
                }

                // Estimate position of the first byte
                // in string to corresponding BLOCK space
                blockID = (strAddr - memoryBias) / blockSize;
                blockTail = (strAddr - memoryBias) % blockSize;

                // Calculate required number of blocks
                uint16_t blocksInString = (countBytes + blockTail) / blockSize;
                if((countBytes + blockTail) % blockSize != 0x00)
                {
                    blocksInString++;
                }

                // If data in string is allocated already 
                if(blocksInString == 0x00)
                {
                    delete countBytesInString;
                    delete address;
                    delete bytesString;
                    continue;
                }

                uint16_t **tmpAddrBuffer = new uint16_t*[blocksInString];
                uint16_t **tmpDataBuffer = new uint16_t*[blocksInString];
                uint16_t tmpDataOffset = 0x00;

                // Allocate data to memory block
                // Save it start address and size
                for(uint16_t i = 0x00; i < blocksInString; i++)
                {
                    tmpAddrBuffer[i] = new uint16_t[2];

                    tmpAddrBuffer[i][0] = (blockID + i) * blockSize + memoryBias;
                    tmpAddrBuffer[i][1] = blockSize;

                    tmpDataBuffer[i] = new uint16_t[blockSize];

                    for(uint16_t j = 0x00; j < blockSize; j++)
                    {
                        tmpDataBuffer[i][j] = 0x00;
                    }

                    for(uint16_t j = 0; (j < countBytes) && (j < (blockSize - blockTail)); j++)
                    {
                        tmpDataBuffer[i][j + blockTail] = getDataBytes(&bytesString[j * 2 + tmpDataOffset * 2]);
                    }

                    tmpDataOffset += blockSize - blockTail;

                    if(countBytes > (blockSize - blockTail))
                    {
                        countBytes -= (blockSize - blockTail);
                    }

                    blockTail = 0x00;
                }

                // If it is first block of data to be allocated
                if(blockCount == 0x00)
                {
                    // Add new items to data and addresses buffer pointers
                    for(uint16_t i = 0x00; i < blocksInString; i++)
                    {
                        ptrBlocksData = addPtr(ptrBlocksData,
                                               blockCount,
                                               tmpDataBuffer[i],
                                               true);
                        ptrBlocksAddr = addPtr(ptrBlocksAddr,
                                               blockCount,
                                               tmpAddrBuffer[i],
                                               false);

                        blockCount++;
                    }
                }
                else
                {
                    uint16_t tempAddr = 0xFFFF;

                    // Try to find existing blocks with data allocated already
                    // then append new data
                    for(uint16_t i = 0x00; i < blocksInString; i++)
                    {
                        for(uint16_t j = 0x00; j < blockCount; j++)
                        {
                            tempAddr = ptrBlocksAddr[j][0];

                            if(tempAddr == tmpAddrBuffer[i][0])
                            {
                                for(uint16_t k = 0x00; k < blockSize; k++)
                                {
                                    ptrBlocksData[j][k] += tmpDataBuffer[i][k];
                                }
                                j = blockCount;
                            }
                        }

                        if(tempAddr != tmpAddrBuffer[i][0])
                        {
                            ptrBlocksData = addPtr(ptrBlocksData,
                                                   blockCount,
                                                   tmpDataBuffer[i],
                                                   true);
                            ptrBlocksAddr = addPtr(ptrBlocksAddr,
                                                   blockCount,
                                                   tmpAddrBuffer[i],
                                                   false);
                            blockCount++;
                        }
                    }
                }

                // Release memory
                for(uint16_t i = 0; i < blocksInString; i++)
                {
                    delete[] tmpDataBuffer[i];
                    delete[] tmpAddrBuffer[i];
                }

                delete countBytesInString;
                delete address;
                delete bytesString;
            }
        }
    }

    // Print extracted data array to output file
    file_out << "#define blockSize (uint8_t)0x" << std::hex << blockSize << "\n\n";
    file_out << "uint16_t blockAddrArray[] = {";

    for(uint16_t i = 0x00; i < blockCount; i++)
    {
        file_out << "0x" << std::hex << std::uppercase << ptrBlocksAddr[i][0];
        file_out << ", 0x" << ptrBlocksAddr[i][1] << ", ";
    }

    long currentPos = file_out.tellp();
    file_out.seekp(currentPos - 2);
    file_out << "};\n\n";

    file_out << "unsigned char blockDataArray[] = {";
    for(uint16_t i = 0x00; i < blockCount; i++)
        for(uint16_t j = 0x00; j < blockSize; j++)
        {
            if((j % 8) == 0x00)
                file_out << "\\\n";

            if(ptrBlocksData[i][j] > 0x0F)
            {
                file_out << "0x" << std::hex << std::uppercase << ptrBlocksData[i][j];
                file_out << ", ";
            }
            else
            {
                file_out << "0x0" << std::hex << std::uppercase << ptrBlocksData[i][j];
                file_out << ", ";
            }
        }

    currentPos = file_out.tellp();
    file_out.seekp(currentPos - 2);
    file_out << "};\n\n";

    for(uint16_t i = 0; i < blockCount; i++)
    {
        delete[] ptrBlocksAddr[i];
        delete[] ptrBlocksData[i];
    }

    std::cout << "Conversion complete\n";

    file_in.close();
    file_out.close();
    return 0;
}

// -------------------------------------
//    Extract address bytes from buffer
// -------------------------------------
uint16_t getAddr(char *ptr)
{
    uint16_t tempAddr = 0x00;

    for(short i = 0x00; i < 0x04; i++)
    {
        if(((uint16_t)ptr[i] - (uint16_t)0x30) <= (uint16_t)0x09)
            tempAddr += (((uint16_t)ptr[i] - 0x30) << (0x03 - i) * 0x04);
        else
            tempAddr += (((uint16_t)ptr[i] - 0x37) << (0x03 - i) * 0x04);
    }

    return tempAddr;
}

// -------------------------------------
//    Extract data bytes from buffer
// -------------------------------------
uint16_t getDataBytes(char *ptr)
{
    uint16_t tempData = 0x00;
    for(short i = 0x00; i < 0x02; i++)
    {
        if(((uint16_t)ptr[i] - (uint16_t)0x30) <= (uint16_t)0x09)
            tempData += (((uint16_t)ptr[i] - 0x30) << (0x01 - i) * 0x04);
        else
            tempData += (((uint16_t)ptr[i] - 0x37) << (0x01 - i) * 0x04);
    }

    return tempData;
}

// -------------------------------------
//    Add new items to dynamic buffer
// -------------------------------------
uint16_t **addPtr(uint16_t **ptr, uint16_t size, uint16_t *data, bool isData)
{
    if(size == 0)
    {
        ptr = new uint16_t*[size + 1];
    }
    else
    {
        uint16_t **copy = new uint16_t*[size + 1];
        for(uint16_t i = 0x00; i < size; i++)
        {
            copy[i] = ptr[i];
        }
        delete [] ptr;
        ptr = copy;
    }

    if(isData)
    {
        ptr[size] = new uint16_t[blockSize];
        for(uint16_t i = 0x00; i < blockSize; i++)
        {
            ptr[size][i] = data[i];
        }
    }
    else
    {
        ptr[size] = new uint16_t[2];
        for(uint16_t i = 0x00; i < 2; i++)
        {
            ptr[size][i] = data[i];
        }
    }

    return ptr;
}
