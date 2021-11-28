#include <stdio.h>
#define BUF_SIZE 255

int main() {
	// Read in addresses.txt
	FILE* addresses = fopen("addresses.txt", "r");
	char line[BUF_SIZE];
	while (fgets(line, BUF_SIZE, addresses)) {
		printf("%s\n", line);
	}	
	
	return 0;
}
