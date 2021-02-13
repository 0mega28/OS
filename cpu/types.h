#ifndef TYPES_H
#define TYPES_H

/* We'll use types with no semantic meaning */
typedef unsigned int u32;
typedef int s32;
typedef unsigned short u16;
typedef short s16;
typedef unsigned char u8;
typedef char s8;

#define low_16(address) (u16)(address & 0xffff)
#define high_16(address) (u16)((address >> 16) & 0xffff)

#endif