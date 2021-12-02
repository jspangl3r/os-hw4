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
int physMemory[SIZE][SIZE];
int freeFrameIdx = 0;
int tlbIdx = 0;

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
	errno = 0;	
	FILE* bsPtr = fopen("BACKING_STORE.bin", "rb");
	
	if (bsPtr == NULL) {
		printf("Error %d\n", errno);
		printf("Couldn't open Backing Store!\n");
		return;
	}	

	// Attempt to find page in backing store
	printf("%d\n", pageNum);
	if (fseek(bsPtr, 256*pageNum, SEEK_SET) != 0) {
		printf("Page not found in Backing Store!\n");
		fclose(bsPtr);
		return;
	}
	
	// Read page into buffer.
	errno = 0;
	unsigned char buffer[SIZE];
	if (fread(buffer, sizeof(buffer), 1, bsPtr) != 0) {
		printf("Error %d\n", errno);
		printf("Error while reading from Backing Store page!\n");
		fclose(bsPtr);
		return;
	}		
	
	// Place 256 byte  page into main memory at available memory index.
	for (int i = 0; i < SIZE; i++) {
		physMemory[freeFrameIdx][i] = buffer[i];
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
	int i = 0;
	char line[SIZE];
	while (fgets(line, SIZE, addresses)) {
		if (i > 0) break;
		int logicalAddr, physicalAddr, pageNum, offset;
		logicalAddr = atoi(line);
		logicalAddrExtraction(logicalAddr, &pageNum, &offset);
		// Attempt to get frame number from page table.
		if (pageTable[pageNum] == -1) {
			printf("Page fault for page %d!\n", pageNum);
			pageFault(pageNum);
		}	
		int frameNum = pageTable[pageNum];	
		printf("Acquired frame number %d for page %d\n\n", frameNum, pageNum);
		i++;
	}	

	fclose(addresses);	
	return 0;
}
