#include <fcntl.h> 
#include <unistd.h> 
#include <sys/stat.h> 
#include <stdlib.h> 

#define CONSOLE     1

int main(void)
{
    int fd; 
    int byte_read; 
    int byte_write; 
    int file_size; 
    char *read_buffer; 
    struct stat file_information; 
    const char *file_name = "file.txt"; 


    fd = open(file_name, O_RDONLY); 

    fstat(fd, &file_information); 
    file_size = file_information.st_size;
    read_buffer = malloc(file_size); 

    byte_read = read(fd, read_buffer, file_size); 
    byte_write = write(CONSOLE, read_buffer, file_size); 

    free(read_buffer);
    close(fd); 
    return 0; 
}