#include <unistd.h> 
#include <signal.h> 

#define CONSOLE		1
#define KEYBOARD	0 

void handle_control_c(int input_signal)
{
	write(CONSOLE, "caught control c\n", sizeof("caught control c\n") - 1);
	return; 
}

int main(void)
{
	char user_input[2];
	struct sigaction signal_action; 

	signal_action.sa_handler = handle_control_c; 

	/* macro sigint -> 2 */
	sigaction(SIGINT, &signal_action, NULL); 

	while (1) {
		write(CONSOLE, "-> ", sizeof("-> ") - 1);
		read(KEYBOARD, user_input, 2);
	}
	return 0; 
}