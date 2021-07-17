#include "shader_reader.h"
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

char *read_shader(const char *file_name) {
	int fd;
	ssize_t num_bytes_read = 0;
	ssize_t bytes_read;
	char *ret = NULL;

	fd = open(file_name, O_RDONLY);

	do {
		ret = realloc(ret, num_bytes_read + 5);
		bytes_read = read(fd, ret + num_bytes_read, 5);
		num_bytes_read += bytes_read;
	} while (bytes_read);
	
	ret = realloc(ret, num_bytes_read + 1);
	ret[num_bytes_read] = '\0';
	return ret;
}
