#ifndef IOCTL_TEST_H
#define IOCTL_TEST_H

struct mystruct{
	int repeat;
	char name[64];
};

#define WR_VALUE _IOW('a', 'a', int32_t *)
#define RD_VALUE _IOR('a', 'b', int32_t *)
#define GREETER  _IOW('a', 'c', struct mystruct *)

#endif
