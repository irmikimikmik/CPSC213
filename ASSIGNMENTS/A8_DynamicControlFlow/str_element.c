#include "str_element.h"
#include "refcount.h"
#include <stdio.h>
#include <string.h>

struct str_element {
    struct str_element_class *class;
    char* word;
};

void str_element_print(struct element *e);
int str_compare(struct element *e1, struct element *e2);
int is_str_element(struct element *e);
char* str_element_get_value(struct str_element *e);
struct str_element *str_element_new(char* s);
void str_finalizer(void* e);

struct str_element_class
{
    void (*print) (struct element *);
    int (*compare) (struct element*, struct element*);
    int (*is_str_element) (struct element *);
    char* (*str_element_get_value) (struct str_element *);
    struct str_element* (*str_element_new) (char*);
    void (*str_finalizer) (void* e);
};

struct str_element_class str_element_class = {str_element_print, str_compare, is_str_element, str_element_get_value, str_element_new, str_finalizer};

void str_element_print(struct element *e) {
    struct str_element* s = (struct str_element*) e;
    printf("%s", s->word);
}

int str_compare(struct element *e1, struct element *e2) {

    // e1 is always a string but e2 can be an integer as well
    // one of them has priority as stated in the problem description
    // same goes for integer_element as well

    if (!is_str_element(e2)){
        return 1;
    }

    struct str_element* s1 = (struct str_element*) e1;
    struct str_element* s2 = (struct str_element*) e2;

    if (strlen(s1->word) < strlen(s2->word)){
        for (unsigned int i = 0; i < strlen(s1->word); i++){
            if (*(s1->word + i) > *(s2->word + i)){
                return 1;
            } else if (*(s1->word + i) < *(s2->word + i)){
                return -1;
            }
        }

        return -1;

    } else
    {

        for (unsigned int i = 0; i < strlen(s2->word); i++){
            if (*(s1->word + i) > *(s2->word + i)){
                return 1;
            } else if (*(s1->word + i) < *(s2->word + i)){
                return -1;
            }
        }
        return (strlen(s1->word) == strlen(s2->word))? 0 : 1;
    }
}

int is_str_element(struct element *e) {
    struct str_element* s = (struct str_element*) e;
    if (s->class == &str_element_class) {
        return 1;
    }
    return 0;
}

char* str_element_get_value(struct str_element *e) {
    struct str_element* strElement = (struct str_element*) e;
    return strElement->word;
}

struct str_element* str_element_new(char* word) {
    struct str_element* s = rc_malloc(sizeof(struct str_element), str_finalizer);
    int dLen = strlen(word) + 1;
    s->word = rc_malloc(dLen, NULL);
    strcpy(s->word, word);
    s->class = &str_element_class;
    return s;
}

void str_finalizer(void* e){
    struct str_element *s = (struct str_element*) e;
    rc_free_ref(s->word);
}



