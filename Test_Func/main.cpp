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

using namespace std;
namespace fs = std::__fs::filesystem;
// PATH list
fs::path root[1] = {"/Users/Stori/Downloads/image"};
fs::path jpeg_image_path = "/Users/Stori/Downloads/image/jpeg";
fs::path png_image_path = "/Users/Stori/Downloads/image/png";

/*
    Pravate Function Declaration
 */
int8_t File_Exist(fs::path path);

/*
    Public Function Declaration
 */
fs::path Get_Current_Path(int16_t name_size=100);

/*
    @Function -> Move_File
    @Para old -> old CONTAINER path contain the file
    @Para target -> new CONTAINER of the file
    @Para FileName -> the name of file
 
    @return 0  -> move successfully
            1  -> file don't exist in old
            -1 -> Permission denied
 */
int8_t Move_File(fs::path old, fs::path target, const char *FileName);

int main(int argc, const char * argv[]) {

    cout << Get_Current_Path() << endl;
    
    
    
    if (!File_Exist(root[0])){
        cout << "root path didn't exist" << endl;
        return 0;
    }
    
    int16_t count = 0;
    
    DIR *dirp = opendir(root[0].c_str());
    struct dirent *dp;
    
    struct magic_set *magic = magic_open(MAGIC_CHECK|MAGIC_MIME_TYPE);
    magic_load(magic,NULL);
    
    while(dirp){
        //cout << Get_Current_Path() << endl;
        errno = 0;
        if ((dp = readdir(dirp)) != NULL) {
            
            chdir(root[0].c_str()); // Change to work folder, because it can simplify magic_file
            
            if (strcmp(magic_file(magic, dp->d_name), "image/jpeg") == 0){
                Move_File(root[0], jpeg_image_path, dp->d_name);
            }
            else if (strcmp(magic_file(magic, dp->d_name), "image/png") == 0){
                Move_File(root[0], png_image_path, dp->d_name);
            }
            else {
                cout << "File not Match" << endl;
            }
            if (count++ == 100){
                cout << "\nPartial Misson Complete, operation count: " << count << endl;
                closedir(dirp);
                return 0;
            }
                
        } else {
            if (errno == 0) {
                closedir(dirp);
                cout << "Mission Complete" << endl;
                return 0;
            }
            closedir(dirp);
            cout << "Wrong Operation" << endl;
            return -1;
        }
    }

    //std::cout << i << endl;
    return 0;
}


/*
    Pravate Funtion implement
 */
int8_t File_Exist(fs::path path){
    
    if (access(path.c_str(), F_OK) == 0){
        return 1;
    }
    else
        return 0;
}


fs::path Get_Current_Path(int16_t name_size){
    
    fs::path ret_val;
    void *buffer = malloc(sizeof(char) * name_size);
    getcwd((char *)buffer, name_size);
    ret_val = (char*)buffer;
    free(buffer);
    return ret_val;
}

int8_t Move_File(fs::path old, fs::path target, const char *FileName){
    
    if (!File_Exist(old/FileName)){
        cout << "File didn't exist." << endl;
        return 1;
    }
    
    if (!File_Exist(target)){
        cout << "Target folder didn't exist" << endl;
        if (mkdir(target.c_str(), S_IRWXU) == 0)
            printf("Create '%s'\n", target.c_str());
    }
        
    printf("File root//%s move to %s", FileName, target.c_str());
    if (rename((old/FileName).c_str(), (target/FileName).c_str()) == 0)
        cout << "---Success" << endl;
    else
        cout << "---Wrong" << endl;
        
    return 0;
}
