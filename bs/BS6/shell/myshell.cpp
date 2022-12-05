#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <map>
#include <vector>
#include <list>

int pid;

void exec_command(std::vector<std::string>& strings) {
	std::vector<char*> args;
	//signal(SIGINT, SIG_DFL);
	// for (const std::string& str : strings)
	// 	std::cout << str << std::endl;
	for(const std::string& string : strings)
		args.push_back(const_cast<char*>(string.c_str()));
	args.push_back(nullptr);

	execvp(args[0], &args[0]);
}

void sigint_handler(int signo) {
	kill(pid, signo);
}

int main() {
	std::string input_line;
	std::vector<std::string> strings;
	std::map<int, std::string> process_ids;
	std::vector<int> running_process_list;
	std::vector<int> process_lengths(2);
	std::vector<std::string> child1, child2;
	bool background = false;
	bool set_pipe;
	pid_t wait_pid;
	signal(SIGINT, sigint_handler);
	int p_length = 0;
	//signal(SIGINT, SIG_IGN);

	

	while(true) {
		std::cout << "myshell> ";
		std::getline(std::cin, input_line);
		//input_line = "cat /etc/passwd | wc";
		set_pipe = false;
		int fd[2];
		
		//check for running background processes, and delete from list if done
		pid_t finished_pid = waitpid(-1, 0, WNOHANG);
		for(auto it = running_process_list.begin(); it != running_process_list.end(); ++it) {
			if (finished_pid == *it) {
				std::cout << "Beendet " << process_ids[*it] << std::endl;
				running_process_list.erase(it);
				break;
			}
		}

		//clear id list
		if (running_process_list.empty()) {
			process_ids.clear();
		}


		if (input_line == "") {
			continue;
		}

		std::string tmp_str;
		std::stringstream sstr(input_line);
		while (sstr >> tmp_str) {
			if (tmp_str == "|") {
				set_pipe = true;
				process_lengths.push_back(p_length);
				p_length = 0;
				continue;
			}
			//process string length
			p_length += 1;
			strings.push_back(tmp_str);
		}


		//check for background '&'
		if (strings.back() == "&") {
			strings.pop_back();
			background = true;
			p_length -= 1;
		}

		process_lengths.push_back(p_length);


		if (strings[0] == "exit") {
			std::cout << "exit shell" << std::endl;
			exit(0);
		}

		if (set_pipe) {
			if (pipe(fd) < 0)
				exit(-1);
		}

		
		pid = fork();
		if (pid == 0) { //child
			if (set_pipe) {
				if (fork() == 0) { //second child for pipe
					for (int i=process_lengths[0]; i < strings.size(); ++i)
						child2.push_back(strings[i]);

					close(fd[1]);
					dup2(fd[0], STDIN_FILENO); //pipe read
					exec_command(child2);
				} else {
					for (int i=0; i < process_lengths[0]; ++i)
						child1.push_back(strings[i]);

					close(fd[0]);
					dup2(fd[1], STDOUT_FILENO); //pipe write
					exec_command(child1);
				}
			} else 
				exec_command(strings);
		} else { //parent
			if (set_pipe) {
				close(fd[0]);
				close(fd[1]);
			}
			if (background) {
				waitpid(pid, 0, WNOHANG);
				std::cout << "Gestartet " << input_line << std::endl;
				process_ids[pid] = input_line;
				running_process_list.push_back(pid);
				background = false;
			} else {
				waitpid(pid, 0, 0);
			}
		}
		strings.clear();
		process_lengths.clear();
		child1.clear();
		child2.clear();
		p_length = 0;
	}
}
