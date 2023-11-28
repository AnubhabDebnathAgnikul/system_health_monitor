#ifndef __COMMON_H_
#define __COMMON_H_

int ping(const char *ip);
int check_sha256sum(char *reference, char *path);
int ptp_check();

extern char folder_path[200];
#endif /* __COMMON_H_ */