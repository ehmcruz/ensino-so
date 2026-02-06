#include <stdio.h>
#include <stdint.h>

static inline uint8_t inb (const uint16_t port)
{
	uint8_t ret;

	__asm__ __volatile__ (
		"inb %1, %0"
		: "=a"(ret)
		: "Nd"(port) );
	
	return ret;
}


int main ()
{
	printf("ola\n");
	inb(5);
	printf("tchau\n");

	return 0;
}