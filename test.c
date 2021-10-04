#include <stdio.h>
#include <stdlib.h>

// notice that array is a pointer to a pointer
int append_array_element(int **array, int *array_length, int element) {
    *array_length += 1;
    *array = realloc(*array, *array_length * sizeof(int));
    (*array)[*array_length - 1] = element; // [] has higher priority
                                           // then * in C's order of
    return 0;                              // operations
}

int main() {
    int *array = NULL;
    int array_length = 0;

    append_array_element(&array, &array_length, 142);
    printf ("Our array with %d elements\n", array_length);
    printf("%d\n", array[0]);

    append_array_element(&array, &array_length, 19);
    printf ("Our array with %d elements\n", array_length);
    printf("%d\n", array[0]);
    printf("%d\n", array[1]);

    return 0;
}