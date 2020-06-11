#ifndef _INPUT_H_
#define _INPUT_H_
#include <ctype.h>

typedef enum {
    CMD_UNKNOWN,
    CMD_FLUSH,
    CMD_R,
    CMD_W,
    CMD_MR
} command_t;

command_t input_process_line(const char *buffer, unsigned int *addr, 
    unsigned char *value);

#endif // _INPUT_H_
