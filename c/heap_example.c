#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

int main(int argc, char** argv){
    char* char_pointer; 
    int* int_pointer; 
    int memory_size = 50;

    printf("allocating string pointer");
    char_pointer = malloc(memory_size);
    if (char_pointer == NULL){
        fprintf(stderr, "failed to create string pointer");
        exit(-1);
    }

    strcpy(char_pointer, "this memory is located on the heap");
    printf("location of the heap pointer itself: %p \n", (void*)&char_pointer);
    printf("address that the heap pointer is pointing to: %p \n", (void*)char_pointer);
    printf("the value found on the address: %s \n", char_pointer);

    printf("allocating 12 bytes of memory on the heap for int pointer\n");
    int_pointer = malloc(sizeof(int));
    if (int_pointer == NULL){
        fprintf(stderr, "failed to create heap for int pointer");
        exit(-1);
    }

    *int_pointer = 31337; 
    printf("the address of the int pointer itself: %p", (void*)&int_pointer);
    printf("the address the int pointer is pointing to: %p", (void*)int_pointer);
    printf("the value found on the above address: %d", *int_pointer);

    free(char_pointer);
    free(int_pointer);

    return 0;
}