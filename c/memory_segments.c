#include <stdio.h>
#include <stdlib.h> 
int global_var;
int global_initialized_var = 5;
void function() { // This is just a demo function.
int stack_var; // Notice this variable has the same name as the one in main().

printf("the function's stack_var is at address 0x%p\n", (void *)&stack_var);
}
int main() {
int stack_var; // Same name as the variable in function()
static int static_initialized_var = 5;
static int static_var;
int *heap_var_ptr;
heap_var_ptr = (int *) malloc(4);
// These variables are in the data segment.
printf("global_initialized_var is at address 0x%p\n", (void *)&global_initialized_var);
printf("static_initialized_var is at address 0x%p\n\n", (void *)&static_initialized_var);
// These variables are in the bss segment.
printf("static_var is at address 0x%p\n", (void *)&static_var);
printf("global_var is at address 0x%p\n\n", (void *)&global_var);
// This variable is in the heap segment.

printf("heap_var is at address 0x%p\n\n", (void *)heap_var_ptr);
// These variables are in the stack segment.
printf("stack_var is at address 0x%p\n", (void *)&stack_var);
function();
}