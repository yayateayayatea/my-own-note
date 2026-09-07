#include <unistd.h> 
#include <fcntl.h> 

#define CONSOLE     1 

static const char *file_name = "output.txt"; 

int main(void)
{
    int fd; 
    char *argument[] = {
        "/usr/bin/ls", 
        NULL
    }; 

    fd = open(file_name, O_WRONLY | O_CREAT, S_IRWXU); 
    dup2(fd, CONSOLE);
    execve(argument[0], argument, NULL); 
    close(fd); 
    return 0; 
}