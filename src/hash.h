#ifndef HASH_H

#include <stdint.h>

uint32_t fnvla(const char *p)
{
	size_t i = 0;
	uint32_t hash = 2166136261u;
	while (*(p + i) != '\0')
	{
		hash ^= (uint8_t)*(p + i);
		hash *= 16777619u;
		i++;
	}
	return hash;
}

uint16_t fnvla16(const char *p)
{
	size_t i = 0;
	uint16_t hash = 0x811Cu;
	while (*(p + i) != '\0')
	{
		hash ^= (uint8_t)*(p + i);
		hash *= 0x0101u;
		i++;
	}
	return hash;
}

#define HASH_H
#endif
