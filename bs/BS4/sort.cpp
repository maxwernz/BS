#include <iostream>
#include <cstdlib>
#include <thread>
#include <vector>
#include "sort_merge.h"
#include <algorithm>

void sort_par(std::vector<int>& array, std::vector<int>& sorted_array, int mi) {
    std::thread t1, t2;

    t1 = std::thread(quickSort, &array[0], 0, mi);
    t2 = std::thread(quickSort, &array[0], mi+1, array.size()-1);

    t1.join();
    t2.join();
    merge(&array[0], &sorted_array[0], array.size());
}

void sort_seq(std::vector<int>& array, std::vector<int>& sorted_array, int mi) {
    quickSort(&array[0], 0, mi);
    quickSort(&array[0], mi+1, array.size()-1);

    merge(&array[0], &sorted_array[0], array.size());
}

int main(int argc, char* argv[]) {
    std::vector<int> array = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    //std::vector<int> array;

    // for(int i = 0; i < 1e6; ++i)
    //     array.push_back(std::rand());
    std::vector<int> sorted_array(array.size());
    
    int mi = (array.size() / 2) - 1; 

    //sort_par(array, sorted_array, mi);
    sort_seq(array, sorted_array, mi);
    for (int i : sorted_array)
        std::cout << i << std::endl;
    std::cout << std::is_sorted(sorted_array.begin(), sorted_array.end()) << std::endl;   


}
