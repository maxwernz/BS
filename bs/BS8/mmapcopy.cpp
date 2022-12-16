#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <string>
#include <cstring>
#include <sys/mman.h>
using namespace std;



void copy(string from_file, string to_file) {
    int from, to;
    struct stat statbuf;
    char *src, *dest;
    off_t size;

    if ((from = open(from_file.c_str(), O_RDWR)) == -1) {
        perror("Error opening input file");
        exit(-1);
    }
    if ((to = open(to_file.c_str(), O_CREAT | O_RDWR, 0644)) == -1) {
        perror("Error opening output file");
        exit(-1);
    }
    if (fstat(from, &statbuf) == -1) {
        perror("Error getting size of input file");
        exit(-1);
    }

    size = statbuf.st_size;

    if (ftruncate(to, size) == -1) {
        perror("Error setting size of output file");
        exit(-1);
    }
    
    if ((src = (char *) mmap(nullptr, size, PROT_READ, MAP_SHARED, from, 0)) == (void *) -1) { //muss das dann auch als char * sein?, bzw klappt es mit void* oder MAP_FAILED?
        perror("Error getting pointer to input file");
        exit(-1);
    }
    if ((dest = (char *) mmap(nullptr, size, PROT_WRITE, MAP_SHARED, to, 0)) ==  (void *) -1) {
        perror("Error getting pointer to input file");
        exit(-1);
    }

    while (size > 0) {
        *dest = *src;
        dest++; src++;
        size -= 1;
    }


    close(to);
    close(from);

}


int main(int argc, char* argv[]) {
    vector<string> filenames;
    for (int i=1; i < 3; ++i) {
        string tmp = argv[i];
        if (tmp.size() > 0)
            filenames.push_back(tmp);
    }
    copy(filenames[0], filenames[1]);
}