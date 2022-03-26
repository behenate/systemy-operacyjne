#ifndef SYSOPY_TASK3_H
#define SYSOPY_TASK3_H

#include <dirent.h>

void browse_directory(DIR* dir, char * absolute_path);
void browse_directory_nftw(const char *absolute_path);
void print_cnts();
void reset_cnts();
#endif //SYSOPY_TASK3_H
