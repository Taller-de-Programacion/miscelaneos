#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>


int main (int argc, char **argv) {
    int verbose_flag;
    int option_index = 0;
    struct option long_options[] = {
        {"verbose", no_argument, &verbose_flag, 1},
        {"input", required_argument, 0, 'i'},
        {"output", required_argument, 0, 'o'},
        {0, 0, 0, 0}
    };
    int opt;
    while ((opt = getopt_long (argc, argv, "vi:o:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'i':
                printf ("Input file: \"%s\"\n", optarg);
                break;
            case 'o':
                printf ("Output file: \"%s\"\n", optarg);
                break;
        }
    }
    if (verbose_flag) {
        printf("verbose flag on\n");
    } else {
        printf("verbose flag off\n");
    }
    return 0;
}
