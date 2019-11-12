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

using namespace std;
//cstring str = "~/Downloads/photo_2019-10-30 05.35.05.jpeg";
const string path = "/Users/Stori/Downloads/";

int main(int argc, const char * argv[]) {

    DIR *dirp;
    struct dirent *dp;
    int16_t count = 0;
    const string judge;
    // insert code here...
    //system("ls ~/Downloads/*.jpeg");
    dirp = opendir((char*)&path[0]);
    
    struct magic_set *magic = magic_open(MAGIC_CHECK|MAGIC_MIME_TYPE);
    magic_load(magic,NULL);
    
    while(dirp){
        errno = 0;
        if ((dp = readdir(dirp)) != NULL) {
            
            chdir(path.c_str());    // Change workplace to path
        
            if (strcmp(magic_file(magic, dp->d_name), "image/jpeg") == 0){
                // loads the default mime type definition
                
                //printf("FOUND\n");
                if (count++ == 50){
                    closedir(dirp);
                    return 1;
                }
                
                printf("image path: '%s'\n", dp->d_name);
            }
        } else {
            if (errno == 0) {
                closedir(dirp);
                return 0;
            }
            closedir(dirp);
            return -1;
        }
    }

    //std::cout << i << endl;
    return 0;
}
