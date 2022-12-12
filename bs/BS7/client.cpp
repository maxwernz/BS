#include <string>
#include <iostream>
#include <limits>
#include <errno.h>
#include <mqueue.h>
#include <unistd.h>

using namespace std;

int main() {
  const int MSG_SIZE = 100;
  const string SERVER = "/server";
  string sqname_client = "/client-" + to_string(getpid());
  mqd_t mq_client, mq_server;
  int bytes_read;

  // Set attributes of client queue
  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = 10;
  attr.mq_msgsize = MSG_SIZE;
  attr.mq_curmsgs = 0;

  // Create client queue
  mq_client = mq_open(sqname_client.c_str(), O_CREAT | O_RDONLY, 0644, &attr);

  // Open server queue
  mq_server = mq_open(SERVER.c_str(), O_WRONLY);

    cout << "CLIENT: Start" << endl;

    
  while(true) {
    string message;
    string input;
    bool correct_input = false;
    while(!correct_input) {
      cout << "CLIENT: Bitte Nachricht eingeben ('x' zum verlassen): ";
      getline(cin, input);
      if(input.size() > MSG_SIZE - sqname_client.size() - 1)  {
         cerr << "Nachricht zu lang!" << endl;
      } else if (input[0] == 'X' or input[0]== 'x') {
         return 0; 
      } else if (input[0] == '+' || input[0] == '-' || input[0] == '*' || input[0] == '/') {
         correct_input = true;
      } else {
         cout << "Falsche eingabe" << endl;
      }
    }

   
      
    message = sqname_client + " " + input;
    cout << "CLIENT: sende " << message << endl;

    /* Nachricht versenden */
    if (mq_send(mq_server, message.c_str(), MSG_SIZE, 0) == -1) {
      perror("CLIENT SEND");
      return -1;
    }

    /* Nachricht mit Ergebnis empfangen */
    char* buffer = new char[MSG_SIZE];
    if (bytes_read = mq_receive(mq_client, buffer, MSG_SIZE, NULL) == -1) {
      perror("CLIENT RECEIVE");
      return -1;
    }
    message = buffer;
    delete buffer;
 
    //      message[bytes_read] = '\0';
    cout << "CLIENT: result " << message << endl;
    break;
  }

  // Clean-up
  mq_close(mq_server);
  mq_close(mq_client);
  mq_unlink(sqname_client.c_str());
}
