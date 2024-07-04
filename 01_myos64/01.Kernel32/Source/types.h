#ifndef __TYPES_H__
#define __TYPES_H__

#define BYTE	unsigned char
#define WORD	unsigned short
#define DWORD	unsigned int
#define QWORD	unsigned long
#define BOOL	unsigned char

#define TRUE	1
#define FALSE	0
#define NULL	0

/* between push and pop, members are aligned by a byte */
#pragma pack( push, 1 )
typedef struct kCharacterStruct
{
	BYTE bCharacter;
	BYTE bAttribute;
} CHARACTER;
#pragma pack( pop )
#endif /* __TYPES_H__ */
