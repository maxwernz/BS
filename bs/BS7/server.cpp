#include <iostream>
#include <stdio.h>
#include <string>
#include <mqueue.h>

int main() {
    const int MSG_SIZE = 100;
    const char* SERVER = "/server";
    char buffer[MSG_SIZE] = {};
    mqd_t mq_client, mq_server;

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

        
        

    }

}