#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <csignal>
#include <mqueue.h>

#define MSG_SIZE 100
mqd_t mq_server;
const char* SERVER = "/server";

void calculate(mqd_t mq_client, std::vector<std::string> input) { // Warum darf ich keine Referenz uebergeben?
    std::string op = input[0];
    int res;
    int op1 = stoi(input[1]), op2 = stoi(input[2]);

    if (op == "+")
        res = op1 + op2;
    else if (op == "-")
        res = op1 - op2;
    else if (op == "*")
        res = op1 * op2;
    else if (op == "/")
        res = op1 / op2;

    const char* result = const_cast<char*>(std::to_string(res).c_str());

    if (mq_send(mq_client, result, MSG_SIZE, 0) == -1) {
        perror("Send result to client");
        exit(-1);
    }

    mq_close(mq_client);

}

void close_server(int signo) { //kommt man da auch ohne global ran?
    mq_close(mq_server);
    mq_unlink(SERVER);
    std::cout << "Server Closed" << std::endl;
    exit(0);
}

int main() {
    char buffer[MSG_SIZE] = {};
    mqd_t mq_client;
    std::string tmp;
    std::vector<std::string> input;

    // Signal
    signal(SIGINT, close_server);
    signal(SIGTERM, close_server);

    // Attributes of server queue
    struct mq_attr attr = {0, 10, MSG_SIZE, 0};

    // Create server queue
    if ((mq_server = mq_open(SERVER, O_CREAT | O_RDONLY, 0644, &attr)) == -1) {
        perror("Create Server queue");
        return -1;
    }

    std::cout << "SERVER START" << std::endl;
    
    while(true) {

        if (mq_receive(mq_server, buffer, MSG_SIZE, nullptr) == -1) {
            perror("Server receive");
            return -1;
        }

        std::stringstream sstr(buffer);
        sstr >> tmp;
        const std::string msqname_client = tmp;
        while(sstr >> tmp) {
            input.push_back(tmp);
        }

        if((mq_client = mq_open(msqname_client.c_str(), O_WRONLY)) == -1) {
            perror("Open Client queue");
            return -1;
        }
        
        std::thread t = std::thread(calculate, mq_client, input);
        t.detach();

        std::cout << "Calculate: " << input[1] << input[0] << input[2] << std::endl;

        input.clear();
    }

}