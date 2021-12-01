#include <stdio.h> 
#include <stdint.h>

// First 20 bits:
// Test 00000000000000000100111000010010 
// Mask 11111111111111111111000000000000
// Page# 0100 = 4

// Last 12 bits:
// Test & (1 << 12) - 1 = Test & (Mask2 - 1) = Test & Mask = Offset
// Test   100111000010010
// Mask      111111111111 
// Offset    111000010010
void physicalAddrExtraction(unsigned int addr) {
    int m = 20;
    int n = 12;
    unsigned offsetMask = 0xFFF;
    unsigned int pageNum, startAddr, offset;

    // First m digits for page number
    pageNum = addr >> n; 

    // Last n digits for offset
    offset = addr & offsetMask;
 
    // Physical address starts at most significant bit
    int msb = 0;
    int addr2 = addr;
    addr2 = addr2 / 2;
    while (addr2 != 0) {
        addr2 = addr2 / 2;
        msb++;
    }
    msb = 1 << msb;
   
    // Print results
    printf("Physical address = %u\n", addr);
    printf("Page number = %u\n", pageNum); 
    printf("Starting address = 0x%04x\n", msb);
    printf("Offset = %u\n", offset);   

}

int main() {
   	unsigned int addrs[7] = { 1, 256, 32768, 32769, 128, 65534, 33153 };
	physicalAddrExtraction(19986);
	printf("\n");
    for (int i = 0; i < 7; i++) {
        physicalAddrExtraction(addrs[i]);
        printf("\n");
    } 
	
	return 0;
}	
