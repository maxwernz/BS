#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <string>
#include <sys/mman.h>
using namespace std;

void copy(string from_file, string to_file) {
    int from, to;
    struct stat statbuf;
    char* from_pointer;
    char* to_pointer;
    off_t from_size;

    if ((from = open(from_file.c_str(), O_RDONLY)) == -1) {
        perror("Error opening input file");
        exit(-1);
    }
    if ((to = open(to_file.c_str(), O_CREAT | O_WRONLY, 0644)) == -1) {
        perror("Error opening output file");
        exit(-1);
    }
    if (fstat(from, &statbuf) == -1) {
        perror("Error getting size of input file");
        exit(-1);
    }

    from_size = statbuf.st_size;

    if (ftruncate(to, from_size) == -1) {
        perror("Error setting size of output file");
        exit(-1);
    }
    
    if ((from_pointer = (char *) mmap(nullptr, from_size, PROT_READ, MAP_PRIVATE, from, 0)) == (char *) -1) {
        perror("Error getting pointer to input file");
        exit(-1);
    }
    if ((to_pointer = (char *) mmap(nullptr, from_size, PROT_WRITE, MAP_SHARED, to, 0)) == (void *) -1) {
        perror("Error getting pointer to input file");
        exit(-1);
    }

    // while (from_size > 0) {
    //     *to_pointer = *from_pointer;
    //     to_pointer++; from_pointer++;
    //     from_size -= 1;
    // }

    printf("%s\n", from_pointer);

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