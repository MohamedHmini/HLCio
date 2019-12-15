#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include "dataframe.h"
#include "advio.h"

// BUILT FUNCTIONS :

FILE *open_file(char* file_path, char* option){

	FILE *file = fopen(file_path, option);
	return file;
}


bool close_file(FILE *file){
	bool isclosed;
	isclosed = !fclose(file);
	return isclosed;
}


char* get_line(FILE *file){

	char* line = NULL;
	char c;
	int buff = 0;

	while(1){
		
		c = fgetc(file);
		// printf("%c",c);
		if(c == EOF || c == '\n')
			break;
		buff++;

		char* checker =  realloc(line, sizeof(char) * (buff + 1));

		if(checker == NULL)
			free(checker);
		else
		{
			line = checker;
		}
		
		line[buff - 1] = c;
	}
	if(line != NULL){
		line[buff] = '\0';
	}
	
	return line;
}



char** read_lines(FILE *file, int starts_from){
	char** lines = NULL;
	char* line = NULL;
	int row_i = 0;

	while((line = get_line(file)) != NULL){
		if(starts_from == 0){
			row_i++;
			char** checker = realloc(lines, sizeof(char*) * (row_i + 1));

			if(checker == NULL){
				free(checker);
				break;
			}
			else
			{
				lines = checker;
			}

			lines[row_i - 1] = strdup(line);
			free(line);
			// lines[row_i - 1] = line;
		}
		else
			starts_from--;
		
	}
	// printf("done\n");
	lines[row_i] = NULL;
	return lines;
}


FILE_CONTENT read_lines_in_FC(FILE *file, int starts_from, int ends_at){
	FILE_CONTENT fc;
    fc.content = NULL;
    fc.r = 0;
	char* line = NULL;
	int counter = starts_from;

	while((line = get_line(file)) != NULL){
		if(counter == 0 && (ends_at - starts_from) >= 0){
			fc.r++;
			char** checker = realloc(fc.content, sizeof(char*) * (fc.r + 1));

			if(checker == NULL){
				free(checker);
				break;
			}
			else
			{
				fc.content = checker;
			}
			
			fc.content[fc.r - 1] = line;
			ends_at--;
		}
		else
			counter--;
		
		
	}

	fc.content[fc.r] = NULL;
	return fc;
}


void FC_free(FILE_CONTENT fc){
    for(int i = 0;i<fc.r; i++){
        free(fc.content[i]);
    }
    free(fc.content);
}


DF_ELEMENT_CONTAINER tokenize_line(DATAFRAME *df, char* line, char* tokenizer){

	DF_ELEMENT_CONTAINER tokenized_line = NULL;
	DF_ELEMENT ch;
	ch.node.Str = NULL;
	ch.node.Str = strtok(line, tokenizer);
	ch.type = DF_ELEMENT_TStr;
	int i = 1;
	while(ch.node.Str != NULL){
		
		DF_ELEMENT_CONTAINER checker = (DF_ELEMENT_CONTAINER) realloc(tokenized_line, sizeof(DF_ELEMENT) * i);
		if(checker == NULL){
			free(checker);
			break;
		}
		else{
			tokenized_line = checker;
		}
		
		tokenized_line[i - 1].type = ch.type;
		tokenized_line[i - 1].node.Str = ch.node.Str;
		ch.node.Str = strtok(NULL, tokenizer);
		i++;
	}
	// printf("\n");
	df->len_cols = i - 1;
	return tokenized_line;
}

void tokenize(DATAFRAME *df, char** lines, char* tokenizer){

	DF_DATA_CONTAINER tokenized_lines = NULL;
	DF_ELEMENT_CONTAINER dec = NULL;
	char* line = strdup(lines[0]);
	int i = 1;
	
	while(line != NULL){

		DF_DATA_CONTAINER checker = (DF_DATA_CONTAINER) realloc(tokenized_lines, sizeof(DF_ELEMENT_CONTAINER) * i);
		if(checker == NULL){
			free(checker);
			break;
		}
		else
		{
			tokenized_lines = checker;
		}

		dec = tokenize_line(df, line, tokenizer);
		tokenized_lines[i - 1] = dec;
		// free(line);
		
		i++;

		if(lines[i - 1] != NULL){
			line = strdup(lines[i - 1]);
			free(lines[i - 1]);
		}
		else
			line = NULL;
		// printf("i:%d, line:%s\n",i - 1,line);
		
	}

	// free(line);
	df->len_rows = i - 1;
	df->type = DF_ELEMENT_TStr;
	df->data = NULL;
	df->data = tokenized_lines;
	free(lines);
}



DATAFRAME *csv_to_df(FILE *csv, int starts_from, char *delimiter){
	DATAFRAME *df = NULL;
	df = Dataframe(0,0,NULL); 
	char** lines = NULL;
	lines = read_lines(csv, starts_from);
	tokenize(df, lines, delimiter);
	return df;
}
