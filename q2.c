#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 256
#define pageTableIndex(x) (x * SIZE)


// Assumes little endian
void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

void logicalAddrExtraction(int addr, int* pageNum, int* offset) {
	int m = 16;
	int n = 8;
	unsigned sixteenBitMask = 0x0000FFFF;
	unsigned pageNumMask = 0xFF00;
	unsigned offsetMask = (1 << n) - 1;
	addr = addr & sixteenBitMask;
	*pageNum = addr & pageNumMask;
	*pageNum >>= n;
	*offset = addr & offsetMask; 
}

int main() {
	// Create page table, fill each page with a character of this string	
    char* str = "UeRDK4SrzAcDQV6PgGjsLEf6oTJfENypgAMxdIegMDWbOMt3wfvNjlAoxFV2CbUHoRZZN2cBcUwOAN676KgB3Xb4zJul89VKOQvl8ZjCnxRj8109OoOtZihftshnnKrjmOt8ZkFm8BO9gNGPXLDvlwyGRm4CSPdKaSSitxk30uDEyUZrEc1wfRJLIST7q6SN1F1jbz4IBJI9C4APb4KU6yDPbqovXfgka7ce7C5V6cQdS1z9JSKibN8PTcwgdzuS";
	char* pageTable = malloc(SIZE * SIZE * sizeof(char));
    for (int i = 0; i < SIZE; i++) {
        printf("%p\n", (pageTable + pageTableIndex(i)));
        memset(pageTable + pageTableIndex(i), str[i], sizeof(char));    
    }
     
	// unsigned long tlb[BUF_SIZE];	
	// for (int i = 0; i < BUF_SIZE; i++) {
    // 		tlb[i] = i*256;
	//}
	//unsigned long thing = (unsigned long) page_table + tlb[0]; 
	
	// Read in addresses.txt
	FILE* addresses = fopen("addresses.txt", "r");
	char line[SIZE];
	while (fgets(line, SIZE, addresses)) {
		int logicalAddr, pageNum, offset;
		logicalAddr = atoi(line);
		logicalAddrExtraction(logicalAddr, &pageNum, &offset);
		//printf("address: %d\npage number: %d\noffset: %d\n\n", logicalAddr, pageNum, offset);
        // Extract frame number from page table
            
	}	
    
	free(pageTable);
	fclose(addresses);	
	return 0;
}
