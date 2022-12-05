#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>

void exec_command(std::vector<std::string>& strings) {
	pid_t pid = fork();
	if (pid == 0) { //child process

		std::vector<char*> args;
		for (size_t i = 0; i < strings.size(); i++) {
			args.push_back(const_cast<char*>(strings[i].c_str()));
		}
		args.push_back(nullptr);

		int status_code = execvp(args[0], &args[0]);
		if (status_code == -1) {
			std::cerr << "Error execvp" << std::endl;
		}

	} else { //parent process
		waitpid(pid, 0, 0);
	}
}

int main() {
	std::string input_line;

	while(true) {
		std::vector<std::string> strings;
		std::cout << "myshell> ";
		std::getline(std::cin, input_line);


		std::string tmp_str;
		std::stringstream sstr(input_line);

		while (sstr >> tmp_str) {
			strings.push_back(tmp_str);
		}

		if (strings[0] == "exit") {
			std::cout << "exit" << std::endl;
			// return 0;
			exit(0);
		}

		int pid_status;
		exec_command(strings);
	}
}
