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
char memory[SIZE][SIZE];
int tlb[TLB_SIZE][2];
int curTlbSize = 0;
int freeFrameIdx = 0;

// Extracts the page number and offset from the passed in logical address.
// Allocates these values to the input pageNum and offset pointers.
void logicalAddrExtraction(unsigned int addr, int* pageNum, int* offset) {
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
	char buffer[SIZE];
	fread(buffer, sizeof(buffer), 1, bsPtr);
	
	// Place 256 byte page into main memory at available memory index.
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
	int count = 0;
	while (fgets(line, SIZE, addresses)) {
		count++;
		unsigned int logicalAddr, physicalAddr;
		int pageNum, frameNum, offset;
		int tlbHit = 0;
		char val;
		
		// Extract page number and offset from logical address.
		logicalAddr = atoi(line);
		logicalAddrExtraction(logicalAddr, &pageNum, &offset);

		// Search through TLB first for frame number.
		for (int i = 0; i < curTlbSize; i++) {
			if (tlb[i][0] == pageNum) {
				tlbHit = 1;
				frameNum = tlb[i][1];
				physicalAddr = (frameNum * SIZE) + offset;
				val = memory[frameNum][offset];
				printf("Logical address: %d; Physical address: %d; Signed byte: %d\n", logicalAddr, physicalAddr, val);
				break;
			}
		}			

		// If we had a TLB miss, search through page table.
		if (tlbHit == 0) {
			// Attempt to get frame number from page table.
			if (pageTable[pageNum] == -1) {
				pageFault(pageNum);
			}	
			frameNum = pageTable[pageNum];	
			// Create physical address and get value. Print results.
			physicalAddr = (frameNum * SIZE) + offset;	
			val = memory[frameNum][offset];	
			printf("Logical address: %d; Physical address: %d; Signed byte: %d\n", logicalAddr, physicalAddr, val);
			
			// Update TLB entries using FIFO 
			if (curTlbSize != TLB_SIZE) {
				// Add current page and frame numbers to current open position in TLB.
				tlb[curTlbSize][0] = pageNum;
				tlb[curTlbSize][1] = frameNum;
				curTlbSize++;
			}
			else {
				// Move every entry in TLB up, removing the first-in entry.
				// Save last position for current page and frame numbers.
				for (int i = 0; i < TLB_SIZE-1; i++) {
					tlb[i][0] = tlb[i+1][0];
					tlb[i][1] = tlb[i+1][1];
				}
				tlb[curTlbSize-1][0] = pageNum;
				tlb[curTlbSize-1][1] = frameNum;
			}
		}
	}	

	fclose(addresses);	
	return 0;
}
