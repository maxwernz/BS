#include <fcntl.h>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h> // open/close
#include <unistd.h>    // read/write
#include <vector>
#include <fstream>
using namespace std;

static string program_name;
static const int default_bs = 10;

void usage(int status) {
  ostream &out = (status == 0) ? cout : cerr;

  out << "Usage: " << program_name << " [OPTION] SOURCE DEST" << endl;
  out << "Copy file SOURCE to file DEST" << endl;
  out << " -bs=BLOCK_SIZE           Copy in blocks of size BLOCK_SIZE "
         "(default: "<<default_bs<<")"
      << endl;
  out << " -fs                      Copy via fstream instead of read/write"
      << endl;
  out << " -h                       Show this help" << endl;
  exit(status);
}

void copy(string from_file, string to_file, int bs) {
  int bytes;
  int to;
  int from;
  vector<char> block(bs);
  char *buffer = &(block[0]);

  if ((from = open(from_file.c_str(), O_RDONLY)) == -1) {
    perror("Error opening input file");
    exit(-1);
  }
  if ((to = open(to_file.c_str(), O_CREAT | O_WRONLY, 0660)) == -1) {
    perror("Error opening output file");
    close(from);
    exit(-1);
  }
  while ((bytes = read(from, buffer, bs)) != 0) {
    write(to, buffer, bytes);
  }

  close(from);
  close(to);
}

void fcopy(string from_file, string to_file, int bs) {
  int bytes;
  ofstream to(to_file);
  ifstream from(from_file);
  vector<char> block(bs);
  char *buffer = &(block[0]);

  from.read(buffer, bs);
  while ((bytes = from.gcount()) != 0) {
    to.write(buffer, bytes);
    from.read(buffer, bs);
  }
}

int main(int argc, char *argv[]) {
  vector<string> filenames;
  int block_size = default_bs;
  bool fs = false;

  program_name = argv[0];
  for (int i = 1; i < argc; ++i) {
    string arg = argv[i];
    if (arg.size() > 0 && arg[0] != '-')
      filenames.push_back(arg);
    else {
      if (arg == "-h")
        usage(0);
      if (arg == "-fs")
        fs = true;
      if (arg.substr(0, 4) == "-bs=") {
        block_size = stoi(arg.substr(4));
      }
    }
  }
  if (filenames.size() != 2)
    usage(-1);
  if(fs) fcopy(filenames[0], filenames[1], block_size);
  else copy(filenames[0], filenames[1], block_size);

}
