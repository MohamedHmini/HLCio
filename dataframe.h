/*
	HIGH-LEVEL-C (HLCio) // pronounced : etch-EL-Cio
	
	THIS LIB IS FOR A LARGE SCOPE REASON, WHICH CONSISTS OF DATA MANUPILATION IN A LOW-LEVEL LANGUAGE LIKE 'C'
	DATAFRAMES ARE A GOOD WAY FOR CRUD OPERATIONS ON A DATASET AS WELL AS MORE COMPLEX/ADVANCED QUERIES.

	MOHAEMD-HMINI

*/



// this lib is based on dataframes as a datastructure to better control and manipulate data/!

#ifndef DATAFRAME_H
#define DATAFRAME_H



struct Array;



typedef enum DF_ELEMENT_TYPE {
	DF_ELEMENT_TStr, 
	DF_ELEMENT_TInt, 
	DF_ELEMENT_TDouble, 
	DF_ELEMENT_TNaN,
	DF_ELEMENT_TArray
} DF_ELEMENT_TYPE;


// the actual element :
typedef union DF_NODE{
	char* Str;
	int Int;
	double Double;
	struct Array *Arr;
} DF_NODE;

typedef struct DF_ELEMENT{
	DF_ELEMENT_TYPE type;
	DF_NODE node;
}DF_ELEMENT;

// plays the role of the element's value holder :
typedef DF_ELEMENT* DF_ELEMENT_CONTAINER;

// plays the role of an array : 
typedef DF_ELEMENT_CONTAINER* DF_DATA_CONTAINER;

typedef struct Array{
	int size;
	DF_ELEMENT_CONTAINER data;
}Array;

typedef struct DATAFRAME{

	int len_rows;
	int len_cols;
	DF_ELEMENT_TYPE type;
	DF_DATA_CONTAINER data;

}DATAFRAME;





DATAFRAME *Dataframe(int,int , DF_DATA_CONTAINER );
void df_free(DATAFRAME *df);
DF_ELEMENT arrcreate(int);
void arrpush(DF_ELEMENT*, DF_ELEMENT);
void arrfree(DF_ELEMENT*);
void arrpop(DF_ELEMENT*);
DF_ELEMENT df_element_copy(DF_ELEMENT);
DATAFRAME *df_full(int, int ,DF_ELEMENT_TYPE ,DF_ELEMENT);
void DF_STR_TO_INT(DF_ELEMENT*);
void DF_INT_TO_STR(DF_ELEMENT*);
void df_map(DATAFRAME *,void (*fun)(DF_ELEMENT* df_element), ...);
void df_retype(DATAFRAME *, DF_ELEMENT_TYPE );
void display_df(DATAFRAME *);
void show(DATAFRAME *);



#endif