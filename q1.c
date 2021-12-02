#include <stdio.h> 
#include <stdint.h>

// Prints the page number, starting address, and offset for an input physical address.
void physicalAddrExtraction(unsigned int addr) {
	int m = 20;
	int n = 12;
	unsigned pageNumMask = 0xFFFFF;
	unsigned offsetMask = (1 << n) - 1;
	int pageNum, offset;

	// First m digits for page number.
	pageNum = addr & pageNumMask;
	pageNum >>= n;  

	// Last n digits for offset.
	offset = addr & offsetMask;

	// Physical address starts at most significant bit.
	int msb = 0;
	unsigned int addr2 = addr;
	addr2 = addr2 / 2;
	while (addr2 != 0) {
		addr2 = addr2 / 2;
		msb++;
	}
	msb = 1 << msb;

	// Print results.
	printf("Physical address = %u\n", addr);
	printf("Page number = %u\n", pageNum); 
	printf("Starting address = 0x%04x\n", msb);
	printf("Offset = %u\n", offset);   

}

int main() {
	// Setup and run test cases.
	unsigned int addrs[11] = { 1, 256, 19986, 32768, 32769, 128, 65534, 33153, 1234567, 123456789, 4294967295 };
	for (int i = 0; i < 11; i++) {
		physicalAddrExtraction(addrs[i]);
		printf("\n");
	} 

	return 0;
}	
