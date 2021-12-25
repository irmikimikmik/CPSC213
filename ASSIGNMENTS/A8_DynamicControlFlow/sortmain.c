#include <stdio.h>
#include <stdlib.h>

#include "int_element.h"
#include "str_element.h"
#include "element.h"

/* If the string is numeric, return an int_element. Otherwise return a str_element. */
struct element *parse_string(char *str) {
  char *endp;
  /* strtol returns a pointer to the first non-numeric character in endp.
     If it gets to the end of the string, that character will be the null terminator. */
  int value = strtol(str, &endp, 10);
  if(str[0] != '\0' && endp[0] == '\0') {
    /* String was non-empty and strtol conversion succeeded - integer */
    return (struct element *)int_element_new(value);
  } else {
    return (struct element *)str_element_new(str);
  }
}

int cmp(void* a, void* b) {
  struct element* elm1 = *(struct element**) a;
  struct element* elm2 = *(struct element**) b;

  return elm1->class->compare(elm1, elm2);
}

int main(int argc, char **argv) {
  /* TODO: Read elements into a new array using parse_string */
  int count = argc - 1;
  struct element** list = malloc(sizeof(struct element*) * count);
  
  for (int i=0; i<count; i++) {
    struct element *e = parse_string(argv[i+1]);
    list[i] = e;
  }
  struct element* e = malloc(sizeof(struct element));

  /* TODO: Sort elements with qsort */
  qsort((void**) list, count, sizeof(e), cmp);
  free(e);
  
  printf("Sorted: ");
  /* TODO: Print elements, separated by a space */
  for (int i = 0; i < count; i++)
  {
    list[i]->class->print(list[i]);
    rc_free_ref(list[i]);
    printf(" ");
  }
  printf("\n");
  free(list);
}
