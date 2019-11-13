//
//  main.cpp
//  Test_Func
//
//  Created by Reverse on 2019/11/8.
//  Copyright © 2019 Ayaya. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include </usr/local/Cellar/libmagic/5.37/include/magic.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <filesystem>
#include <ctime>
#include <stdarg.h>

using namespace std;
namespace fs = std::__fs::filesystem;
// PATH list
fs::path root[1] = {"/Users/Stori/Downloads/image"};
fs::path jpeg_image_path = "/Users/Stori/Downloads/image/jpeg";
fs::path png_image_path = "/Users/Stori/Downloads/image/png";
fs::path image_path= "/Users/Stori/Downloads/image/jpeg/imageIMAGE\ 2019-06-30\ 16:09:22.jpg";

void f(char* fmt, ...);

int main(int argc, const char * argv[]) {
    
    struct stat status;
    lstat(image_path.c_str(), &status);
    
    f("%c %s %d %f", 'A', "BBBBB", 100, 1.111);
    cout << sizeof(int) << endl;
    
}


void f(char* fmt, ...)
{
    va_list ap;
    
    printf("Start %p\n", ap);
    va_start(ap, fmt);
    
    char* p = fmt;
    while (*p) {
        if (*p == '%' && *(p+1) == 'd') {
            printf("参数类型为int,值为 %d\n", va_arg(ap, int));
        }
        else if (*p == '%' && *(p+1) == 'f') {
            printf("参数类型为double,值为 %f\n", va_arg(ap, double));
        }
        else if (*p == '%' && *(p+1) == 's') {
            printf("参数类型为char*,值为 %s\n", va_arg(ap, char*));
        }
        else if (*p == '%' && *(p+1) == 'c'){
            printf("Start %p\n", ap);
            printf("char argument, value is %c\n", va_arg(ap, int));
            printf("After %p\n", ap);
        }
        p++;
    }
    va_end(ap);
}
