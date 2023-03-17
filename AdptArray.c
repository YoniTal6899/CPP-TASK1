#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "AdptArray.h"

typedef struct AdptArray_{
int length;
PElement* values;
COPY_FUNC ArrCopy;
DEL_FUNC ArrDel;
PRINT_FUNC ArrPrint;
} AdpArr, *PAdpArr;

PAdptArray CreateAdptArray(COPY_FUNC cpy, DEL_FUNC del,PRINT_FUNC print){
    PAdpArr arr= (PAdpArr) malloc(sizeof(AdpArr)); 
    if(!arr){return NULL;}
    arr->length=0;
    arr->ArrCopy=cpy;
    arr->ArrDel=del;
    arr->ArrPrint=print;
    arr->values= (PElement*) malloc(sizeof(PElement));
    return arr;
}

void DeleteAdptArray(PAdptArray arr){
    for(int i=0; i<arr->length;i++){
        if(arr->values[i]!=NULL){arr->ArrDel(arr->values[i]);} // Delete every not NULL value 
    }
    free(arr->values); // free memory for the values array
    free(arr); // free memory for the struct itself
}

Result SetAdptArrayAt(PAdptArray arr, int index, PElement val){
    if(index<arr->length){ // Easy case, just replacing inside the 'arr->values'
       if(arr->values[index] != NULL){
            arr->ArrDel(arr->values[index]); //delete the old value
            if (arr->values[index] != NULL){return FAIL;}
       }
       arr->values[index]=arr->ArrCopy(val); // insert copy of the new value in the right index
    }
    else{ // Complicated case, need to reallocate memory (increase the arr->size)
        int new_length= index+1;
        arr->values= (PElement*) realloc(arr->values,new_length*sizeof(PElement));
        for (int i = arr->length; i < new_length; i++){arr->values[i]=NULL;} // initialize every extra value as NULL 
        arr->length=new_length;
        arr->values[index]=arr->ArrCopy(val);         
    }
    if (arr->values[index] == NULL){return FAIL;} // Validation
    return SUCCESS;      
}

PElement GetAdptArrayAt(PAdptArray arr, int index){
    if(index>=arr->length){return FAIL;} // Validation
    else if (arr->values[index]==NULL){return NULL;} // Validation
    return arr->ArrCopy(arr->values[index]);    
}

int GetAdptArraySize(PAdptArray arr){
    if(arr){return arr->length;}
    return -1;
}

void PrintDB(PAdptArray arr){
    if(arr){
        for(int i=0; i<arr->length; i++){
            if(arr->values[i]!=NULL){arr->ArrPrint(arr->values[i]);}
        }
    }

}