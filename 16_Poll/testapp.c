#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <poll.h>

int main() {
	int fd;
	int test;

	struct pollfd my_poll;

	/* Open the device file */
	fd = open("/dev/irq_poll", O_RDONLY);
	if(fd < 0) {
		perror("Could not open device file");
		return -1;
	}

	memset(&my_poll, 0, sizeof(my_poll));
	my_poll.fd = fd;
	my_poll.events = POLLIN;

	/* Wait for Signal */
	printf("Wait for signal...\n");
	poll(&my_poll, 1, -1);
	printf("Button was pressed!\n");

	return 0;
}


