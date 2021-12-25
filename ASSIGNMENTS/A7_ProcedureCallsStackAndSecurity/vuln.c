#include <unistd.h>
#include <stdlib.h>
#include <string.h>

struct myString
{
    int size;
    char *_str;
};

static char _str1[30] = {'W', 'e', 'l', 'c', 'o', 'm', 'e', '!', ' ', ' P', 'l', 'e', 'a', 's', 'e', ' ', 'e', 'n', 't', 'e', 'r', ' ', 'a',
' ', 'n', 'a', 'm', 'e', ':', '\n'};
static char _str2[11] = {'G', 'o', 'o', 'd', ' ', 'l', 'u', 'c', 'k', ',', ' ',};
static char _str3[43] = {'T', 'h', 'e', ' ', 's', 'e', 'c', 'r', 'e', 't', ' ', 'p', 'h', 'r', 'a', 's', 'e', ' ', 'i', 's', '"', 's', 'q',
'u', 'e', 'a', 'm', 'i', 's', 'h', ' ', 'o', 's', 's', 'i', 'f', 'r', 'a', 'g', 'e', '"'};

static struct myString str1 = { .size = 30, ._str = &_str1[0] };
static struct myString str2 = { .size = 11, ._str = &_str2[0] };
static struct myString str3 = { .size = 43, ._str = &_str3[0] };

int main() {

    char input[128];
    print(str1);
    int size = read(0, input, 256); // why do we read 256 instead of 128
    print(str2);
    write(1, input, size);
}

void print(struct myString str){
    write(1, str._str, str.size);
}
