#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <map>
#include <vector>
#include <list>


void exec_command(std::vector<std::string>& strings) {
	std::vector<char*> args;
	for(const auto& str : strings) {
		args.push_back(const_cast<char*>(str.c_str()));
	}
	args.push_back(nullptr);

	execvp(args[0], &args[0]);
}

int main() {
	std::string input_line;
	std::map<int, std::string> processes;
	std::vector<int> process_list;
	bool background = false;
	pid_t wait_pid;

	while(true) {
		std::vector<std::string> strings;
		std::cout << "myshell> ";
		std::getline(std::cin, input_line);

		pid_t finished_pid = waitpid(-1, 0, WNOHANG);
		for(auto it = process_list.begin(); it != process_list.end(); ++it) {
			if (finished_pid == *it) {
				std::cout << "Beendet " << processes[*it] << std::endl;
				process_list.erase(it);
				break;
			}
		}

		if (process_list.empty()) {
			processes.clear();
		}

		if (input_line == "") {
			continue;
		}

		std::string tmp_str;
		std::stringstream sstr(input_line);
		while (sstr >> tmp_str) {
			strings.push_back(tmp_str);
		}

		if (strings[0] == "exit") {
			std::cout << "exit" << std::endl;
			exit(0);
		}

		if (strings.back() == "&") {
			strings.pop_back();
			background = true;
		}

		pid_t pid = fork();
		if (pid == 0) { //child
			exec_command(strings);
		} else { //parent
			if (background) {
				waitpid(pid, 0, WNOHANG);
				std::cout << "Gestartet " << input_line << std::endl;
				processes[pid] = input_line;
				process_list.push_back(pid);
				background = false;
			} else {
				waitpid(pid, 0, 0);
			}
		}
	}
}
