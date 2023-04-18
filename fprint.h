#ifndef FPRINT_H
# define FPRINT_H
# define __USE_XOPEN2K

# include <unistd.h>
# include <stdarg.h>
# include <unistd.h>
# include <fcntl.h>

/*Prints out a few details of the file content*/
int fprint(char *filename, char *args, ...);

#endif