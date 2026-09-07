#include <unistd.h> 
#include <string.h> 
#include <sys/wait.h> 

#define CONSOLE 	1
#define KEYBOARD 	0
#define BUFFER_SIZE 	1024
#define PROMPT		"-> "
#define PROMPT_LEN 	strlen(PROMPT) 
#define ERROR		"command failed"
#define ERROR_LEN	strlen(ERROR)

void handle_control_c(int signal)
{ 
	return; 
}

void parse_io(char *input_string, char **output_array, char *delimiter)
{
	char *token; 
	int counter = 0; 

	token = strtok(input_string, delimiter); 
	while (token != NULL) {
		output_array[counter++] = token; 
		token = strtok(NULL, delimiter); 
	}
	output_array[counter] = NULL; 

	return; 
}

int main(int argc, char **argv, char **env)
{	
	char user_input[BUFFER_SIZE];
	char *argument[BUFFER_SIZE];

	int pid; 
	int child_status; 
	int byte_read; 

	struct sigaction my_sigaction; 
	
	
	my_sigaction.sa_handler = handle_control_c; 
	sigaction(SIGINT, &my_sigaction, NULL);

	while (1) {
		write(CONSOLE, PROMPT, PROMPT_LEN); 
		byte_read = read(KEYBOARD, user_input, BUFFER_SIZE);

		user_input[byte_read - 1] = '\0';

		parse_io(user_input, argument, " "); 

		pid = fork();
		if (pid == 0) {
			my_sigaction.sa_handler = SIG_DFL; 
			sigaction(SIGINT, &my_sigaction, NULL);

			execvp(argument[0], argument);
			write(CONSOLE, ERROR, ERROR_LEN);
			_exit(1);
		} else {
			waitpid(pid, &child_status, 0);
		}
	}

	return 0; 
}

