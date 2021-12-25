#include <stdlib.h>
#include "list.h"
#include <stdio.h>
#include <string.h>

void stringToNum(element_t* outputv, element_t inputv) {
	char* input = (char*) inputv;
	long* output = (long*) outputv;
	char* end;
	*output = strtol(input, &end, 10);
	if (end == input) {
		*output = -1;
    }
}

void truncate(element_t *ptr, element_t str, element_t num) {
    char* str2 = (char*) str;
    long end_index = (long) num;
    char** ptr2 = (char**) ptr;
    if(strlen(str2) > end_index) {
        str2[end_index] = 0;
    }
    *ptr2 = str2;
}

void turn_to_null(element_t *ptr, element_t str, element_t num) {
    char* str2 = (char*) str;
    char** ptr2 = (char**) ptr;
    long n = (long) num;
    if (n < 0) 
    {
        *ptr2 = str2;
    } else {
        *ptr2 = NULL;
    }
}

void max_length(element_t *pointer, element_t num1, element_t num2) {
    long length1 = (long) num1;
    long length2 = (long) num2;
    long* ptr = (long*) pointer;

    if(length1 >= length2) {
        *ptr = length1;
    } else {
        *ptr = length2;
    }
}

void print_elem(element_t e) {
    char * el =  e;
    printf("%s ", el);
}


void print_with_newline(element_t e) {
    char * el =  e;
    printf("%s\n", el);
}


int num_is_pos(element_t e) {
    long i = (long) e; 
    return i >= 0;
}


int is_not_null(element_t e) {
    char* str = (char*) e;
    return str != NULL;
}

int main (int argc, char* argv[]){

    //step 1
    struct list* commandLineList = list_create();
    list_append_array(commandLineList, (void*) (argv + 1), argc -1);

    //step 2
    struct list* nums_list = list_create();
	list_map1(stringToNum, nums_list, commandLineList);

    //step 3
    struct list* nulls_list = list_create();
    list_map2(turn_to_null, nulls_list, commandLineList, nums_list); 

    //step 4
    struct list* pos_nums_list = list_create();
    list_filter(num_is_pos, pos_nums_list, nums_list);

    //step 5 
    struct list* non_nulls_list = list_create();
    list_filter(is_not_null, non_nulls_list, nulls_list);

    //step 6
    struct list* truncated_elements_list = list_create();
    list_map2(truncate, truncated_elements_list, non_nulls_list, pos_nums_list);

    //step 7
    list_foreach(print_with_newline, truncated_elements_list);
    list_foreach(print_elem, truncated_elements_list);
    printf("\n");


    element_t mx = 0;
    list_foldl(max_length, &mx, nums_list);
    printf("%ld\n", mx);

    list_destroy(nulls_list);
    list_destroy(non_nulls_list);
    list_destroy(truncated_elements_list);
    list_destroy(commandLineList);
    list_destroy(nums_list);
    list_destroy(pos_nums_list);
    
}