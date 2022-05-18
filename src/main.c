#include <stdio.h>
#include "../headers/main.h"
#include "../headers/asm_descriptor.h"

int main(int argc, char* argv[] ) {
    int i;
    int was_next_line_read;
    asm_descriptor* ds;
    for(i = 1; i < argc; i++) {
        ds = new_asm_descriptor(argv[i]);
        while(!feof(ds->fp)) {
            was_next_line_read = get_next_line(ds);
            if(was_next_line_read) {
                log_error(ds->err_log, ds->line);
            }
        }
        print_all_logger_errors(ds->err_log);
        free_asm_descriptor(ds);
    }
    return 0;
}
