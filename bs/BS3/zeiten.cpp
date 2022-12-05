#include <cstdlib>
#include <cstring>
#include <sys/time.h>
#include <sys/resource.h>
#include <iostream>

int main(int argc, char* argv[]) {

    timeval start, end;
    int time_ret;
    double start_seconds, end_seconds, runtime;
    double user_time_seconds, system_time_seconds;

    time_ret = gettimeofday(&start, NULL);
    start_seconds = double(start.tv_sec + start.tv_usec*1e-6);
    
    int sys_ret = system(argv[1]);

    time_ret = gettimeofday(&end, NULL);
    end_seconds = double(end.tv_sec + end.tv_usec*1e-6);
    
    runtime = end_seconds - start_seconds; 


    rusage res_usage;
    int res_ret = getrusage(RUSAGE_CHILDREN, &res_usage);

    user_time_seconds = double(res_usage.ru_utime.tv_sec + res_usage.ru_utime.tv_usec*1e-6);
    system_time_seconds = double(res_usage.ru_stime.tv_sec + res_usage.ru_stime.tv_usec*1e-6);
    
    std::cout << "Laufzeit: " << runtime << "sek" << std::endl;
    std::cout << "User-Zeit: " << user_time_seconds << "sek" << std::endl;
    std::cout << "System-Zeit: " << system_time_seconds << "sek" << std::endl;
}