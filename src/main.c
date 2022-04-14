#include <stdio.h>
#include "../headers/main.h"

int main(int argc, char* argv[] ) {
    int i;
    for(i = 1; i < argc; i++) {
        generate_spread_file_from(argv[i]);
    }
    return 0;
}
