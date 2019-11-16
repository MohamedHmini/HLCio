#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include "dataframe.h"




DATAFRAME *Dataframe(
	int len_rows, 
	int len_columns, 
	DF_DATA_CONTAINER data
	){

	DATAFRAME *df = (DATAFRAME*) malloc(sizeof(DATAFRAME));

	df->len_rows = len_rows;
	df->len_cols = len_columns;
	df->data = data;
	
	return df;

}


void df_free(DATAFRAME *df){
	for(int i = 0; i<df->len_rows; i++){
		for(int j = 0; j< df->len_cols ;j++){
			arrfree(&df->data[i][j]);
		}
		free(df->data[i]);
	}	
	free(df);
}


DF_ELEMENT arrcreate(int size){
	DF_ELEMENT e;
    e.type = DF_ELEMENT_TArray;
    e.node.Arr = malloc(sizeof(Array));
    e.node.Arr->size = size;
    e.node.Arr->data = malloc(sizeof(DF_ELEMENT) * size);
	return e;
}


void arrpush(DF_ELEMENT *arr, DF_ELEMENT e){
	arr->node.Arr->size++;
	arr->node.Arr->data = realloc(arr->node.Arr->data, sizeof(DF_ELEMENT) * (arr->node.Arr->size));
	arr->node.Arr->data[arr->node.Arr->size - 1] = e;
}


void arrfree(DF_ELEMENT *arr){
	switch(arr->type){
		case DF_ELEMENT_TArray:{
			for(int i = 0;i<arr->node.Arr->size; i++){
				arrfree(&arr->node.Arr->data[i]);
			}
			free(arr->node.Arr);
		}break;
		default:{
			// ..
		}break;
	}
}


void arrpop(DF_ELEMENT *arr){
	if(arr->node.Arr->size > 0){
		arr->node.Arr->size--;
		// arr->node.Arr->data[arr->node.Arr->size] = NULL;
	}
}


DF_ELEMENT df_element_copy(DF_ELEMENT source_element){
	DF_ELEMENT copy;

	switch(source_element.type){
		case DF_ELEMENT_TArray:{
			copy.type = source_element.type;
			copy.node.Arr = malloc(sizeof(Array));
			copy.node.Arr->size = source_element.node.Arr->size;
			copy.node.Arr->data = malloc(sizeof(DF_ELEMENT) * copy.node.Arr->size);
			
			for(int i = 0; i < copy.node.Arr->size; i++){
				copy.node.Arr->data[i] = df_element_copy(source_element.node.Arr->data[i]);
			}

		}break;
		default:{
			copy = source_element;
		}break;
	}

	return copy;

}


DATAFRAME *df_full(
	int rows, 
	int cols, 
	DF_ELEMENT_TYPE type, 
	DF_ELEMENT init_val
	){

	DATAFRAME *df = Dataframe(rows, cols, NULL);
	df->type = type;
	df->data = (DF_DATA_CONTAINER) malloc(sizeof(DF_ELEMENT_CONTAINER) * rows);

	int j = 0;

	while(j < rows){

		df->data[j] = (DF_ELEMENT_CONTAINER) malloc(sizeof(DF_ELEMENT) * cols);
		int k = 0;

		while(k < cols){
			df->data[j][k] = df_element_copy(init_val);
			k++;
		}	

		j++;
	}

	return df;
}



void df_update(DATAFRAME *df){

}



void DF_STR_TO_INT(DF_ELEMENT* df_element){
	df_element->node.Int = atoi(df_element->node.Str);
}

void DF_INT_TO_STR(DF_ELEMENT* df_element){
	itoa(df_element->node.Int, df_element->node.Str, 10);
}

// this function allows a mapping of elements of a particular dataframe : 
//typedef void (*cfun)();

void df_map(
	DATAFRAME *df, 
	void (*fun)(DF_ELEMENT* df_element),
	...
	){

	int i,j;

//	va_list cfuns;
//	va_start(cfuns, cfun);
	
//	cfun p = va_arg(cfuns, cfun);
//	(*p)();

	for(i = 0; i< df->len_rows;i++){
		for(j = 0; j< df->len_cols; j++){
			fun( &df->data[i][j] );
		}
//		cfun p = va_arg(cfuns, cfun);
//		(*p)();
	}

//	cfun p = va_arg(cfuns, cfun);
//	p();
}


void df_retype(DATAFRAME *df, DF_ELEMENT_TYPE type){
	// if col == -1 then we r retyping the whole dataframe with the same type : 

	switch(type){
		case DF_ELEMENT_TStr:{
			df->type = type;
			df_map(df, DF_INT_TO_STR);
		}break;
		case DF_ELEMENT_TInt:{
			df->type = type;
			df_map(df, DF_STR_TO_INT);
		}break;
		case DF_ELEMENT_TDouble:{

		}break;
		default:{

		}
	}
}


/*
TYEP type_of(char data_element){
	TYPE type;
	//...
	return type;

}



*/

void display_df(DATAFRAME *df){
	int i,j;

	printf("ROWS : %d\n", df->len_rows);
	printf("COLS : %d\n", df->len_cols);

	for(i = 0; i< df->len_rows;i++){
		printf("[ ");
		for(j = 0; j< df->len_cols; j++){
			if(df->type == DF_ELEMENT_TStr){
				printf("%s ", df->data[i][j] );
			}
			else if(df->type == DF_ELEMENT_TInt)
				printf("%d ", df->data[i][j] );
			else
			{
				printf("%d ", df->data[i][j].node.Arr->data[0].node.Arr->data[2].node.Int);
			}
			
		}
		printf(" ]\n");
	}

}


void show(DATAFRAME *DF){
	//..
}