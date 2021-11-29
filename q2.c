#include <stdio.h>
#include <stdlib.h>
#define BUF_SIZE 256

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
	// Create page table	
	char* page_table = malloc(BUF_SIZE * BUF_SIZE);

	// unsigned long tlb[BUF_SIZE];	
	// for (int i = 0; i < BUF_SIZE; i++) {
    // 		tlb[i] = i*256;
	//}
	
	//unsigned long thing = (unsigned long) page_table + tlb[0]; 
	
	// Read in addresses.txt
	FILE* addresses = fopen("addresses.txt", "r");
	char line[BUF_SIZE];
	while (fgets(line, BUF_SIZE, addresses)) {
		int logicalAddr, pageNum, offset;
		logicalAddr = atoi(line);
		logicalAddrExtraction(logicalAddr, &pageNum, &offset);
		printf("address: %d\npage number: %d\noffset: %d\n\n", logicalAddr, pageNum, offset);
	}	

	free(page_table);
	fclose(addresses);	
	return 0;
}
