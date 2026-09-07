#include <unistd.h> 
#include <string.h> 
#include <sys/wait.h> 


#define CONSOLE		1
#define KEYBOARD 	0 
#define PIPE_WRITE	1
#define PIPE_READ 	0 
#define BUFFER_SIZE 	1024
#define PROMPT		"my_shell -> "
#define PROMPT_LEN	strlen(PROMPT)

int parse_io(char *input_string, char **output_array, char *delimeter)
{
	int counter = 0; 
	char *token; 

	token = strtok(input_string, delimeter); 
	while (token != NULL) {
		output_array[counter++] = token;
		token = strtok(NULL, delimeter);
	}
	output_array[counter] = NULL; 

	return counter;
}

int main(int arugment_number, char **argument_value, char **environment)
{
	char user_input[BUFFER_SIZE]; 
	char *command_list[BUFFER_SIZE]; 
	char *exec_argument[BUFFER_SIZE]; 
	
	int byte_read; 
	int command_number; 
	int pipe_fd[2];
	int pid; 
	int child_status; 
	int i; 

	/* for testing main logic purpose, just make while true loop here */
	while (1) {

	write(CONSOLE, PROMPT, PROMPT_LEN); 
	byte_read = read(KEYBOARD, user_input, BUFFER_SIZE); 
	
	if (user_input[byte_read - 1] == '\n') {
		user_input[byte_read - 1] = '\0'; 
	} else { 
		/* if user somehow able to input stuff without ending \n */
		user_input[byte_read] = '\0'; 
	}
	
	command_number = parse_io(user_input, command_list, "|"); 

	if (command_number > 1) {
		pipe(pipe_fd); 	
		for (i = 0; i < command_number; i++) {
			parse_io(command_list[i], exec_argument, " ");

			pid = fork(); 
			if (pid == 0) {
				if (i == 0) {
					dup2(pipe_fd[PIPE_WRITE], CONSOLE); 
				} else if (i == (command_number - 1)) {
					dup2(pipe_fd[PIPE_READ], KEYBOARD); 
				} else {
					dup2(pipe_fd[PIPE_READ], KEYBOARD); 
					dup2(pipe_fd[PIPE_WRITE], CONSOLE); 
				}
				close(pipe_fd[PIPE_READ]); 
				close(pipe_fd[PIPE_WRITE]); 

				execvp(exec_argument[0], exec_argument); 
				write(CONSOLE, "execvp failed\n", 14);
				_exit(1); 
			} 
		}

		close(pipe_fd[PIPE_READ]); 
		close(pipe_fd[PIPE_WRITE]); 
		for (i = 0; i < command_number; i++) {
			wait(NULL); 
		}

	} else {
		parse_io(command_list[0], exec_argument, " "); 

		pid = fork(); 
		if (pid == 0) {
			execvp(exec_argument[0], exec_argument); 
		} else {
			waitpid(pid, &child_status, 0); 
		}
	}

	}

	return 0; 
}