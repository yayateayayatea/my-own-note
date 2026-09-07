#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 

int main(int argument_number, char** argument_value){
    int file_descriptor; 
    int write_result;
    char* file_buffer;
    char* file_data;

    if(argument_number != 3){
        print("usage: \n");
        print("main.elf note_name.txt your_note_without_space");
        return 1; 
    }
    file_buffer = malloc(100);
    if(file_buffer == NULL){
        printf("faile to create file buffer");
        return 1;
    }

    file_data = malloc(20);
    if(file_data == NULL){
        printf("failed to create file data");
        return 1;
    }

    file_descriptor = open(argument_value[1], O_WRONLY|O_CREAT|O_APPEND);
    if(file_descriptor == -1){
        printf("failed to create or access the file: %s", argument_value[1]);
        return 1; 
    }

    write_result = write()







    return 0; 
} 