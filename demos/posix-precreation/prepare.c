#include <stdio.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	const char * file = "test.tmp";

	// write to non existing file
	int fd = open(	file,
					O_WRONLY | O_CREAT,
					S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH
				);

	char buf[255] = "";

	// everything ok? read and close
	if ( fd != -1 )
	{
		write(fd, "abc", 3);
		write(fd, "abc", 3);
		write(fd, "abc", 3);
		close(fd);
	}

	//printf("%s written.\n", file);

	return 0;
}
