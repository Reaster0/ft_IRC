#include <iostream>
#include <stdio.h> 
#include <stdlib.h>
#include <string>
#include <time.h>

std::string random_pwd(const int len) 
{
	srand(time(NULL));
	std::string pwd;
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i)
        pwd += alphanum[rand() % (sizeof(alphanum) - 1)];
    return pwd;
}

// int main(int argc, char **argv)
// {
// 	if (argc != 2)
// 		return 2;
// 	int n = atoi(argv[1]);
// 	std::cout << random_pwd(n) << std::endl;
// 	return 0;
// }
