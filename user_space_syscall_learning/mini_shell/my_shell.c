#include <unistd.h> 
#include <sys/wait.h> 
#include <string.h> 

#ifndef MY_SHELL_H
#define MY_SHELL_H

#define MESSAGE		"mysh> "
#define MESSAGE_LEN	6

#define CONSOLE		1
#define KEYBOARD	0 

#define BUFFER_SIZE	64

#endif 

/* helper function */
void remove_tail(char *target)
{
	int len = strlen(target) - 1; 
	target[len] = '\0'; 
	return; 
}

void string_parsing(char *input_string, char **output_string)
{
	int counter = 0; 
	char *token = strtok(input_string, " "); 
	
	while (token != NULL && counter < BUFFER_SIZE) {
		output_string[counter] = token; 
		token = strtok(NULL, " "); 
		counter += 1; 
	}
	output_string[counter] = NULL; 

	return; 
}
/* end of helper function */

void my_cd(char **argument)
{
	chdir(argument[1]); 
	return; 
}

void child_process(char **argument, char **environment)
{
	/*execve(argument[0], argument, environment)*/
	execvp(argument[0], argument); 
	_exit(1);  
}

int prompt(char *ui_p)
{
	int byte_read; 
	char current_directory[BUFFER_SIZE];

	getcwd(current_directory, BUFFER_SIZE); 

	write(CONSOLE, current_directory, strlen(current_directory));
	write(CONSOLE, MESSAGE, MESSAGE_LEN);
	byte_read = read(KEYBOARD, ui_p, BUFFER_SIZE); 

	return byte_read;
}

int main(int argument_number, char **argument_value, char **environment)
{	
	char user_input[BUFFER_SIZE]; 
	char *ui_p = user_input;
	char *argument[BUFFER_SIZE];  

	int byte_read; 
	int pid; 
	int child_status; 
	int condition; 

	do{
		byte_read = prompt(ui_p); 
		if (byte_read > 1) 
			remove_tail(user_input); 
		string_parsing(user_input, argument); 

		condition = strcmp(argument[0], "cd"); 
		if (condition == 0) {
			my_cd(argument); 
		} else {
			pid = fork(); 
			if (pid == 0) {
				child_process(argument, environment); 
			} else {
				waitpid(pid, &child_status, 0); 
			}
		} 

	}while(byte_read != 0); 

	return 0;  
}