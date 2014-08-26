#include "siox-datatypes-helper-posix.h"

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>


static inline unsigned translateSIOXFlagsToPOSIX( unsigned flags )
{
	return ( ( flags & SIOX_LOW_LEVEL_O_RDONLY )    > 0 ? O_RDONLY    : 0 )
	 	 | ( ( flags & SIOX_LOW_LEVEL_O_WRONLY )    > 0 ? O_WRONLY    : 0 )
		 | ( ( flags & SIOX_LOW_LEVEL_O_RDWR )      > 0 ? O_RDWR      : 0 )
		 | ( ( flags & SIOX_LOW_LEVEL_O_APPEND )    > 0 ? O_APPEND    : 0 )
		 | ( ( flags & SIOX_LOW_LEVEL_O_ASYNC )     > 0 ? O_ASYNC     : 0 )
		 | ( ( flags & SIOX_LOW_LEVEL_O_CLOEXEC )   > 0 ? O_CLOEXEC   : 0 )
		 | ( ( flags & SIOX_LOW_LEVEL_O_CREAT )     > 0 ? O_CREAT     : 0 )
		 | ( ( flags & SIOX_LOW_LEVEL_O_DIRECTORY ) > 0 ? O_DIRECTORY : 0 )
		 | ( ( flags & SIOX_LOW_LEVEL_O_EXCL )      > 0 ? O_EXCL      : 0 )
		 | ( ( flags & SIOX_LOW_LEVEL_O_NOCTTY )    > 0 ? O_NOCTTY    : 0 )
		 | ( ( flags & SIOX_LOW_LEVEL_O_NOFOLLOW )  > 0 ? O_NOFOLLOW  : 0 )
		 | ( ( flags & SIOX_LOW_LEVEL_O_NONBLOCK )  > 0 ? O_NONBLOCK  : 0 )
		 | ( ( flags & SIOX_LOW_LEVEL_O_TRUNC )     > 0 ? O_TRUNC     : 0 );
}


