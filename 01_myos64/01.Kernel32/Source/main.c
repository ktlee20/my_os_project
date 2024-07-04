#include "types.h"

void kPrintString( int iX, int iY, const char *pcString);

// Main function
void Main( void )
{
	kPrintString( 0, 4, "C Language Kernel Started" );

	while ( 1 );
}

// Print string
void kPrintString( int iX, int iY, const char *pcString)
{
	CHARACTER *pstScreen = (CHARACTER *) 0xB8000;
	int i;

	pstScreen += (iY * 80) + iX;
	for (i = 0 ; pcString[i] != 0 ; i++)
	{
		pstScreen[ i ].bCharacter = pcString[ i ];
	}
}