/*
 * From http://www.safemode.org/files/zillion/shellcode/doc/Writing_shellcode.html#process
 *
 * Generic program for testing shell code byte arrays.
 * Created by zillion and EVL
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

/* Print message */

static void
croak(const char *msg) {
    fprintf(stderr, "%s\n", msg);
    fflush(stderr);
}

/* Signal error and bail out */

static void
barf(const char *msg) {
    croak(msg);
    exit(1);
}

/*
 * Main code starts here
 */

int main(int argc, char **argv) {
    FILE *f, *fp;
    void *code;
    int i, l;
    int m = 15;          /* max # of bytes to print on one line */
    struct stat sbuf;
    size_t f_len;        /* Note: assume files are < 2**32 bytes long ;-) */

    if(argc < 2) barf("Binary file must be specified");
    if(stat(argv[1], &sbuf)) barf("failed to stat file");
    f_len = (size_t) sbuf.st_size;
    if(!(code = malloc(f_len))) barf("failed to grab required memory");
    if(!(fp = fopen(argv[1], "rb"))) barf("failed to open file");
    if(fread(code, 1, f_len, fp) != f_len) barf("failed to slurp file");
    if(fclose(fp)) barf("failed to close file");
    f = fopen("./exec_shell_code.c", "w");
    fprintf(f, "#define SHELL_CODE \\\n");
    l = m;
    for(i = 0; i < f_len; ++i) {
        if(l >= m) {
            if(i) fprintf(f, "\" \\\n");
            fprintf(f, "\t\"");
            l = 0;
        }
        ++l;
        fprintf(f, "\\x%02x", ((unsigned char *)code)[i]);
    }
    fprintf(f, "\"\n\n");
    fprintf(f, "int main() {\n"
               "    void (*fptr)(void);\n"
               "    fptr = (void (*)(void))&SHELL_CODE;\n"
               "    (*fptr)();\n"
               "}");
    fclose(f);
    return 0;
}
