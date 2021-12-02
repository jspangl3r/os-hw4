#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define SIZE 256 // Common size for page and frame sizes
#define TLB_SIZE 16

// Globals 
int m = 16;
int n = 8;
int pageTable[SIZE];
int memory[SIZE][SIZE];
int freeFrameIdx = 0;
int tlbIdx = 0;

// Extracts the page number and offset from the passed in logical address.
// Allocates these values to the input pageNum and offset pointers.
void logicalAddrExtraction(int addr, int* pageNum, int* offset) {
	unsigned sixteenBitMask = 0x0000FFFF;
	unsigned pageNumMask = 0xFF00;
	unsigned offsetMask = (1 << n) - 1;
	addr = addr & sixteenBitMask;
	*pageNum = addr & pageNumMask;
	*pageNum >>= n;
	*offset = addr & offsetMask; 
}

// Page fault handler.
void pageFault(int pageNum) {
	FILE* bsPtr = fopen("BACKING_STORE.bin", "rb");
	
	if (bsPtr == NULL) {
		printf("Couldn't open Backing Store!\n");
		return;
	}	

	// Attempt to find page in backing store
	if (fseek(bsPtr, pageNum*SIZE, SEEK_SET) != 0) {
		printf("Page not found in Backing Store!\n");
		fclose(bsPtr);
		return;
	}
	
	// Read page into buffer.
	unsigned char buffer[SIZE];
	fread(buffer, sizeof(buffer), 1, bsPtr);
	
	// Place 256 byte  page into main memory at available memory index.
	for (int i = 0; i < SIZE; i++) {
		memory[freeFrameIdx][i] = buffer[i];
	}
	
	// Update page table and freeFrameIdx.
	pageTable[pageNum] = freeFrameIdx;
	freeFrameIdx++;

	fclose(bsPtr);	
}

int main(int argc, char* argv[]) {
	// Handle input arg.
	if (argc == 1) {
		printf("Make sure to pass addresses.txt via command line.\n");
		return -1;
	}

	// Attempt to load addresses.txt.
	char* fileName = argv[1];
	FILE* addresses = fopen(fileName, "r");
	if (addresses == NULL) {
		printf("Couldn't open addresses.txt file!\n");
		return 1;
	}

	// Initialize PageTable to invalid -1 for each page.
	memset(pageTable, -1, sizeof(pageTable));

	// Loop over each address.
	char line[SIZE];
	while (fgets(line, SIZE, addresses)) {
		int logicalAddr, physicalAddr, pageNum, frameNum, offset;
		logicalAddr = atoi(line);
		logicalAddrExtraction(logicalAddr, &pageNum, &offset);
		// Attempt to get frame number from page table.
		if (pageTable[pageNum] == -1) {
			pageFault(pageNum);
		}	
		frameNum = pageTable[pageNum];	
		// Create physical address and get value. Print results.
		physicalAddr = (frameNum * SIZE) + offset;	
		int val = memory[frameNum][offset];	
		printf("Logical address: %d; Physical address: %d; Value: %d\n", logicalAddr, physicalAddr, val);
	}	

	fclose(addresses);	
	return 0;
}
