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
void physicalAddrExtraction(int addr) {
    int m = 20;
    int n = 12;
    unsigned pageNumMask = 0xFFFFF000;
    unsigned offsetMask = (1 << n) - 1;
    int pageNum, startAddr, offset;

    // First m digits
    pageNum = addr & pageNumMask;
    pageNum = pageNum >> n; 

    // Physical address starts at most significant bit
    int msb = 0;
    int addr2 = addr;
    addr2 = addr2 / 2;
    while (addr2 != 0) {
        addr2 = addr2 / 2;
        msb++;
    }
    msb = 1 << msb;
 
    // Last n digits
    offset = addr & offsetMask;
    
    // Print results
    printf("Physical address = %d\n", addr);
    printf("Page number = %d\n", pageNum); 
    printf("Starting address = 0x%04x\n", msb);
    printf("Offset = %d\n", offset);   

}

int main() {
    int addrs[7] = { 1, 256, 32768, 32769, 128, 65534, 33153 };
	physicalAddrExtraction(19986);
	printf("\n");
    for (int i = 0; i < 7; i++) {
        physicalAddrExtraction(addrs[i]);
        printf("\n");
    } 
	
	return 0;
}	
