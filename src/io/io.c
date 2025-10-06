#include <string.h>
#include <errno.h>

#include "io.h"
#include "../logger/logger.h"

void flush_stdin(void)
{
    char c;

    c = getchar();
    while (c != '\n' && c != EOF) {
        c = getchar();
    }
}

int
read_n_carac_and_flush(unsigned long max_length, FILE *stream, char *data_read)
{
    size_t data_read_length;

    fgets(data_read, max_length, stream);

    if (ferror(stream) != 0) {
        logger_error("error detected when reading characters: %s",
                     strerror(errno));
        return -1;
    }

    data_read_length = strlen(data_read);
    if ((data_read_length <= max_length) &&
        (data_read[data_read_length - 1] == '\n'))
    {
        data_read[data_read_length - 1] = '\0';
    } else {
        flush_stdin();
    }

    return 0;
}

FILE *ouv_fichier(char const *name, char const *mode)
{
    FILE* f = fopen(name, mode);

    if (f == NULL) {
        fprintf(stderr, "impossible d'ouvrir le fichier %s\n", name);
        exit(0);
    }
    return f;
}

FILE *redirect_stream(FILE *stream, const char *file_name, const char *mode)
{
    FILE *f = freopen(file_name, mode, stream);

    if (f == NULL) {
        logger_error("error whe redirectting stream to file %s: %s",
                     file_name, strerror(errno));
    }

    return f;
}

void fermer_fichier(FILE **f)
{
    fclose(*f);
}

void lire_matrice(FILE* fichier, unsigned int* matrice[],
                  int const tailleLig, int const tailleCol)
{
    int i, j;

    for (i = 0; i < tailleLig; i++) {
        for (j = 0; j < tailleCol; j++) {
            fscanf(fichier, "%d", &matrice[i][j]);
        }
    }
}
