
// This is a personal academic project. Dear PVS-Studio, please check it.
// pvs-studio static code analyzer for c, c++ and c#: http://www.viva64.com

#include <testLib.h>
#include <stddef.h>

static uint32_t m_z = 362436069;
static uint32_t m_w = 521288629;

uint32_t getUInt() {
	m_z = 36969 * ( m_z & 65535 ) + ( m_z >> 16 );
	m_w = 18000 * ( m_w & 65535 ) + ( m_w >> 16 );
	return ( m_z << 16 ) + m_w;
}

uint32_t getUniform ( uint32_t max ) {
	uint32_t u = getUInt();
	return ( u + 1.0 ) * 2.328306435454494e-10 * max;
}


uint8_t memcheck ( void * start, uint8_t value, uint32_t size ) {
	uint8_t * p = ( uint8_t * ) start;
	uint32_t i;

	for ( i = 0; i < size; i++, p++ )
		if ( *p != value )
			return 0;

	return 1;
}

void bussyWait ( uint64_t n ) {
	uint64_t i;
	for ( i = 0; i < n; i++ );
}

void endlessLoop() {
	while ( 1 );
}

void endlessLoopPrintMain(char ** argv, uint64_t argc) {
	if ( argv == NULL || argc != 2 ) {
		fdprintf ( STDERR, "Usage: endless_loop_print <wait>\n" );
		return;
	}

	int64_t satoi_flag;
	uint64_t wait = satoi ( argv[1], &satoi_flag );

	if ( wait <= 0 || !satoi_flag ) {
		fdprintf(STDERR, "Error: <wait> must be a positive integer.\n");
		return;
	}

	endlessLoopPrint(wait);
}

void endlessLoopPrint ( uint64_t wait )
{
	uint16_t pid = getPid();
	while ( 1 ) {
		printf( "%d ", pid );
		bussyWait( wait );
	}
}