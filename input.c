#include "input.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

/**
 * Determina el comando a partir de una línea del archivo.
 * Ignora los caracteres de separación '\t', '\v', '\n', ' '.
 * 
 * Copia en p la primer posición no reconocida.
 * 
 * Devuelve CMD_UNKNOWN si no se reconoce un comando en la línea.
 */
command_t read_command(const char *line, size_t *p) {
    size_t ptr = 0;
    size_t in_ptr = 0;
    command_t curr_cmd = CMD_UNKNOWN;
    const char *curr_ptr = NULL;

    enum {
        IN_STR,
        OUTSIDE
    } state;

    state = OUTSIDE;

    while (line[ptr]) {
        char c = tolower(line[ptr++]);
        switch(state) {
            case OUTSIDE:
                in_ptr = 1;
                state = IN_STR;
                switch(c) {
                    case 'f':
                        curr_ptr = "flush";
                        curr_cmd = CMD_FLUSH;
                        break;
                    case 'm':
                        curr_ptr = "mr";
                        curr_cmd = CMD_MR;
                        break;
                    case 'r':
                        curr_ptr = "r";
                        curr_cmd = CMD_R;
                        break;
                    case 'w':
                        curr_ptr = "w";
                        curr_cmd = CMD_W;
                        break;
                    default:
                        curr_cmd = CMD_UNKNOWN;
                        state = OUTSIDE;
                        break;
                }
                break;
            case IN_STR:
                if (c == curr_ptr[in_ptr]) {
                    in_ptr++;
                } else if ((curr_ptr[in_ptr] == '\0') && isspace(c)) {
                    *p = ptr;
                    return curr_cmd;
                } else {
                    *p = ptr;
                    return CMD_UNKNOWN;
                }
                break;
        }
    }

    *p = ptr;
    return CMD_UNKNOWN;
}

bool read_addr(const char *buffer, size_t ptr, unsigned int *addr) {
    while (isspace(buffer[ptr])) ptr++;
    if (!buffer[ptr])
        return false;

    char *p = NULL;
    *addr = strtol(buffer + ptr, &p, 10);
    if ((*p) && !isspace(*p))
        return false;
    
    return true;
}

bool read_addr_n_val(const char *buffer, size_t ptr, unsigned int *addr, 
    unsigned char *value) {
    while (isspace(buffer[ptr])) ptr++;
    if (!buffer[ptr])
        return false;

    char *p = NULL;
    *addr = strtol(buffer + ptr, &p, 10);
    if (*p != ',')
        return false;
    ptr = 0;
    while (isspace(p[++ptr]));
    *value = strtol(p + ptr, &p, 10);

    if ((*p) && !isspace(*p))
        return false;

    return true;
}

command_t input_process_line(const char *buffer, unsigned int *addr, unsigned char *value)  {
    size_t ptr = 0;
    command_t cmd = read_command(buffer, &ptr);
    switch(cmd) {
        case CMD_FLUSH:
        case CMD_MR:
            return cmd;
        case CMD_R:
            if (!read_addr(buffer, ptr, addr))
                return CMD_UNKNOWN;
            break;
        case CMD_W:
            if (!read_addr_n_val(buffer, ptr, addr, value))
                return CMD_UNKNOWN;
            break;
        default:
            return CMD_UNKNOWN;
    }

    return cmd;
}