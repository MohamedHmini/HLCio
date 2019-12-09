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
		for(int j = 0; j< df->len_cols;j++){
			arrfree(&df->data[i][j]);
		}
		free(df->data[i]);
	}	
	free(df->data);
	free(df);
}


DF_ELEMENT* df_element_create(){
	return malloc(sizeof(DF_ELEMENT));
}


DF_ELEMENT arrcreate(int size){
	DF_ELEMENT e;
    e.type = DF_ELEMENT_TArray;
    e.node.Arr = malloc(sizeof(Array));
    e.node.Arr->size = size;
    e.node.Arr->data = malloc(sizeof(DF_ELEMENT) * size);
	return e;
}


DF_ELEMENT arrinit(int size, DF_ELEMENT initval){
	DF_ELEMENT arr = arrcreate(size);
	for(int i = 0; i<arr.node.Arr->size; i++){
		DF_ELEMENT copy = df_element_copy(initval);
		arr.node.Arr->data[i] = copy;
	}
	return arr;
}	

void arrpush(DF_ELEMENT *arr, DF_ELEMENT e){
	arr->node.Arr->size++;
	arr->node.Arr->data = realloc(arr->node.Arr->data, sizeof(DF_ELEMENT) * (arr->node.Arr->size));
	arr->node.Arr->data[arr->node.Arr->size - 1] = e;
}

        
void arrfree(DF_ELEMENT *arr){
	if(arr != NULL){
		switch(arr->type){
			case DF_ELEMENT_TArray:{
				for(int i = 0;i<arr->node.Arr->size; i++){
					arrfree(&arr->node.Arr->data[i]);
					// free(&arr->node.Arr->data[i]);
				}
				free(arr->node.Arr->data);
				free(arr->node.Arr);
			}break;
			case DF_ELEMENT_TStr:{
				// free(arr->node.Str);
			}break;
			default:{
				
			}break;
		}

		// free(arr);
	}
}


void arrpop(DF_ELEMENT *arr){
	if(arr->node.Arr->size > 0){
		arr->node.Arr->size--;
		// arr->node.Arr->data[arr->node.Arr->size] = NULL;
	}
}


void arrshow(DF_ELEMENT *arr){
	printf("\n[ ");
	for(int i = 0; i < arr->node.Arr->size; i++){
		printf("%d ", arr->node.Arr->data[i].node.Int);
	}
	printf("]");
}



CMP_RESULT arrcmp(DF_ELEMENT* arr, CMP_RESULT (*cmp)()){
	CMP_RESULT r;
	r.best = arr->node.Arr->data[0];
	r.index = 0;

	for(int i = 1; i < arr->node.Arr->size; i++){
		r = cmp(r.best, arr->node.Arr->data[i], r.index, i);
	}

	return r;
}


bool arrequal(DF_ELEMENT *e1, DF_ELEMENT *e2){
	bool equal = true;

	for(int i = 0; i < e1->node.Arr->size; i++){
		if(e1->node.Arr->data[i].node.Int != e2->node.Arr->data[i].node.Int){
			equal = false;
			break;
		}
	}

	return equal;
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


void df_remove_row(DATAFRAME *df, int index){
	// df->len_rows--;
	// df->data[index]
}


void df_remove_column(DATAFRAME *df, int index){

}


void DF_STR_TO_INT(DF_ELEMENT* df_element){
	int i = atoi(df_element->node.Str);
	df_element->node.Int = i;
	df_element->type = DF_ELEMENT_TInt;
	// free(df_element->node.Str);
}

void DF_INT_TO_STR(DF_ELEMENT* df_element){
    sprintf(df_element->node.Str, "%d", df_element->node.Int);
}

// void DF_INT_TO_STR(DF_ELEMENT* df_element){
// 	itoa(df_element->node.Int, df_element->node.Str, 10);
// }

// this function allows a mapping of elements of a particular dataframe : 
//typedef void (*cfun)();

void df_map(
	DATAFRAME *df, 
	void (*fun)(DF_ELEMENT* df_element),
	int start_from_row
	){

	int i,j;

//	va_list cfuns;
//	va_start(cfuns, cfun);
	
//	cfun p = va_arg(cfuns, cfun);
//	(*p)();

	for(i = start_from_row; i< df->len_rows;i++){
		for(j = 0; j< df->len_cols; j++){
			fun( &df->data[i][j] );
		}
//		cfun p = va_arg(cfuns, cfun);
//		(*p)();
	}

//	cfun p = va_arg(cfuns, cfun);
//	p();
}


void df_retype(DATAFRAME *df, DF_ELEMENT_TYPE type, int start_from_row){
	// if col == -1 then we r retyping the whole dataframe with the same type : 

	switch(type){
		case DF_ELEMENT_TStr:{
			df->type = type;
			df_map(df, DF_INT_TO_STR, start_from_row);
		}break;
		case DF_ELEMENT_TInt:{
			df->type = type;
			df_map(df, DF_STR_TO_INT, start_from_row);
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

void display_df(DATAFRAME *df, int start){
	int i,j;

	printf("ROWS : %d\n", df->len_rows);
	printf("COLS : %d\n", df->len_cols);

	for(i = start; i< df->len_rows;i++){
		printf("[ ");
		for(j = 0; j< df->len_cols; j++){
			if(df->type == DF_ELEMENT_TStr){
				printf("%s ", df->data[i][j].node.Str);
			}
			else if(df->type == DF_ELEMENT_TInt){
				printf("%03d ", df->data[i][j].node.Int);
			}
			
		}
		printf("]\n");
	}

}


void show(DATAFRAME *DF){
	//..
}