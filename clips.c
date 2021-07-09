#include <stdio.h>
#include <stdlib.h>

#include <editline/readline.h>
#include <histedit.h>

int main(int argc, char** argv) {
    puts("clips!");
    puts("soon to be a small lisp, written in c");

    // buffer to store user input
    while (1) {
        char* input = readline("clips> ");
        add_history(input);
        free(input);
    }

    return 0;
}
