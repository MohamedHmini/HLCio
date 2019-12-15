#include<stdbool.h>
#include "dataframe.h"


#ifndef ADVIO_H
#define ADVIO_H


typedef struct FILE_CONTENT{
    char** content;
    int r;
}FILE_CONTENT;

// DECLARE DEFINED FUNCTIONS : 


FILE *open_file(char*, char*);
bool close_file(FILE *);
char* get_line(FILE *);
char** read_lines(FILE *, int);
FILE_CONTENT read_lines_in_FC(FILE *, int, int);
void FC_free(FILE_CONTENT);
DF_ELEMENT_CONTAINER tokenize_line(DATAFRAME *, char* , char* );
void tokenize(DATAFRAME *, char** , char* );
DATAFRAME *csv_to_df(FILE *, int, char*);


#endif