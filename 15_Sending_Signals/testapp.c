#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>

#define SIGTX 44
#define REGISTER_UAPP _IO('R', 'g')

void signalhandler(int sig) {
	printf("Button was pressed!\n");
}

int main() {
	int fd;
	signal(SIGTX, signalhandler);

	printf("PID: %d\n", getpid());

	/* Open the device file */
	fd = open("/dev/irq_signal", O_RDONLY);
	if(fd < 0) {
		perror("Could not open device file");
		return -1;
	}

	/* Register app to KM */
	if(ioctl(fd, REGISTER_UAPP, NULL)) {
		perror("Error registering app");
		close(fd);
		return -1;
	}


	/* Wait for Signal */
	printf("Wait for signal...\n");
	while(1)
		sleep(1);

	return 0;
}


