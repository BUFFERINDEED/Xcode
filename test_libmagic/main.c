//
//  main.c
//  test_libmagic
//
//  Created by Reverse on 2019/11/12.
//  Copyright © 2019 Ayaya. All rights reserved.
//
#include </usr/local/Cellar/libmagic/5.37/include/magic.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;
using namespace fs = std::__fs::filesystem;

char path[] = "/Users/Stori/Downloads/Pock-master.zip";
fs::path image_path= "/Users/Stori/Downloads/image/jpeg/imageIMAGE 2019-06-30 16/09/22.jpg";

//#include <sys/tyoes.h>

int main(int argc, const char * argv[]) {
    // insert code here...
    struct magic_set *magic = magic_open(MAGIC_CHECK|MAGIC_MIME_TYPE);
    magic_load(magic,NULL); // loads the default mime type definition
    if (strcmp("application/zip", "image/jpeg") == 0) {
        printf("%d\n", strcmp("application/zip", "image/jpeg"));
    }
    printf("magic output: '%s'\n",magic_file(magic,path));
    l

    return 0;
}
