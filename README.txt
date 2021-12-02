Note: for all bitwise operations, assumes little endian

Question 1:
My algorithm works by first setting up a page number mask as well as an offset mask.
Using these masks, the page number and offsets are calculated based on the input address
using bitwise operations.
To find the starting address, I locate the most significant bit (assuming little endian).
In my main function, I test out various lengths of 32 bit addresses, with the last address
being the maximum possible unsigned 32 bit number (2^32 - 1).
To build: gcc q1.c
To run: ./a.out

Question 2:
Make sure addresses.txt and BACKING_STORE.bin are in the same working directory as q2.c
My code uses a FIFO replacement strategy for the TLB.
To build: gcc q2.c
To run: ./q2.c addresses.txt
