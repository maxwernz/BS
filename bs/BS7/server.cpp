#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <mqueue.h>

void calculate(std::vector<std::string>& input, char*& result) {
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

    result = const_cast<char*>(std::to_string(res).c_str());

}

int main() {
    const int MSG_SIZE = 100;
    const char* SERVER = "/server";
    char buffer[MSG_SIZE] = {};
    mqd_t mq_client, mq_server;
    std::string tmp;
    std::vector<std::string> input;
    char* result;

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

        calculate(input, result);

        if((mq_client = mq_open(msqname_client.c_str(), O_WRONLY)) == -1) {
            perror("Open Client queue");
            return -1;
        }

        if (mq_send(mq_client, result, MSG_SIZE, 0) == -1) {
            perror("Send result to client");
            return -1;
        }

        input.clear();
    }

}