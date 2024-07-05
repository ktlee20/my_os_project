#ifndef __TYPES_H__
#define __TYPES_H__

#define kbyte	unsigned char
#define kword	unsigned short
#define kdword	unsigned int
#define kqword	unsigned long
#define kbool	unsigned char

#define TRUE	1
#define FALSE	0
#define NULL	0

/* between push and pop, members are aligned by a byte */
#pragma pack( push, 1 )
typedef struct video_buffer_character
{
	kbyte character;
	kbyte attribute;
} kvideo_buf_character;
#pragma pack( pop )
#endif /* __TYPES_H__ */
