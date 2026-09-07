#include <unistd.h> 
#include <sys/wait.h> 

#define READ_FD     0
#define WRITE_FD    1 
#define CONSOLE     1


void parent_process(int *pipe_fd)
{
    close(pipe_fd[READ_FD]);
    write(pipe_fd[WRITE_FD], "hello from parent", sizeof("hello from parent"));
    close(pipe_fd[WRITE_FD]);
    return; 
}

void child_process(int *pipe_fd)
{
    char read_buffer[256]; 
    int byte_read; 

    close(pipe_fd[WRITE_FD]); 
    while((byte_read = read(pipe_fd[READ_FD], read_buffer, sizeof(read_buffer))) > 0){
        write(CONSOLE, read_buffer, byte_read); 
    }

    close(pipe_fd[READ_FD]);
    return; 
}

int main(void)
{
    int pipe_fd[2]; 
    int pid; 
    int child_status; 

    pipe(pipe_fd);

    pid = fork(); 
    if (pid == 0) {
        child_process(pipe_fd); 
    } else {
        parent_process(pipe_fd); 
    }

    close(pipe_fd[READ_FD]); 
    close(pipe_fd[WRITE_FD]); 
    return 0; 
}