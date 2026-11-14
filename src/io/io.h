#ifndef _FICHIER_H_
#define _FICHIER_H_

#include <stdio.h>
#include <stdlib.h>

void flush_stdin(void);
int read_n_carac_and_flush(unsigned long max_length, FILE *stream,
                           char *data_read);

FILE *redirect_stream(FILE *stream, const char *file_name, const char *mode);
FILE *open_file(char const *name, char const *mode);
void close_file(FILE **f);
void read_matrix(FILE *fichier, int *matrice[], int tailleLig, int tailleCol);

#endif
