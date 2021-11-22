#include <stdio.h>

// Test & (1 << 12) - 1 = Test & (Mask2 - 1) = Test & Mask = Offset
// Test   100111000010010
// Mask2    1000000000000
// 1                    1
// Mask      111111111111 
// Offset    111000010010

void main() {
    int test = 19986;
    int m = 20;
    int n = 12;
    unsigned mask, mask2; 
    int pageNum, startAddr, offset;

    // First m digits
   
    // Last n digits
    mask = (1 << n) - 1;    
    mask2 = 1 << n;
    offset = test & mask;
    
    // Print results
    printf("Mask = %d\n", mask);
    printf("Mask2 = %d\n", mask2);
    printf("Offset = %d\n", offset);   
}
