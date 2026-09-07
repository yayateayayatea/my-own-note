#include <stdio.h> 
#include <stdlib.h> 

void function_in_text(void){
    printf("hello, i am function in text section \n");
    return;
}

void function_in_heap(void){
    printf("hello, i am function in heap section \n");
    return;
}

int main(void){
    void (*pointer01)(void) = function_in_text;

    // i don know how large we need, do we have a method to figure it out? 
    void (*pointer02)(void) = malloc(100); 
    // now i need to copy the byte (machine instruction of whole function) or the address of function_heap? 
    memcpy(pointer02, );

    // to make the heap section executable 


    /*function name itself is an address*/
    printf("i am in main: address of instruction is: %p \n", main);
    printf("hello, i am main function \n");
    
    printf("-------------------------------\n");

    printf("the stack that hold the address of the function to run: %p \n", &pointer01);
    printf("i am in text function: address of instruction is: %p \n", pointer01);
    pointer01();

    printf("-------------------------------\n");

    printf("the stack that hold the address of the function to run: %p", &pointer02);
    printf("the heap address that contain the instruction is: %p", pointer02);
    printf("the instruction found on the heap: ");
    // run the instruction itself 
    pointer02();

    return 0;
}


