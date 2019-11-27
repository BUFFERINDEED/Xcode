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
#include <unordered_set>
#include <regex>

#define TYPE_JPEG 0x1
#define TYPE_PNG  0x2

using namespace std;
namespace fs = std::__fs::filesystem;
// PATH list
fs::path default_path = "/Users/Stori/Documents";
fs::path root[1] = {"/Users/Stori/Downloads/image"};
fs::path jpeg_image_path = "/Users/Stori/Downloads/image/jpeg";
fs::path png_image_path = "/Users/Stori/Downloads/image/png";
fs::path test_file_path = "/Users/Stori/Downloads/image/imageimage283122.jpg";

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

/*
    @Function -> Rename_File
    
    @Para path -> path of the file or folder, auto identified by file type.
    @Para regex -> regular expression used to find a file,
                    notice this regex will be applied to FULL name
                    of the file
    @Para mode -> some specific setting of function
    @Para new_name_format -> new name format of file
                                specific argument:
                                #CTIME -> last status change time
                                #ATIME -> time of last status change
                                #MTIME -> time of last modification
                                #COUNT -> count number of operation order
    @return 0  -> move successfully
            1  -> file don't exist in old
            -1 -> Permission denied
                                                
 */
int8_t Rename_File(const fs::path path, const char *regular_expression, const int8_t mode, const char *new_name_format);

int main(int argc, const char * argv[]) {

    chdir(default_path.c_str());
    cout << "Current Path :" << Get_Current_Path() << endl;
    
    
    
    if (!File_Exist(root[0])){
        cout << "root path didn't exist" << endl;
        return 0;
    }
    
    int16_t count = 0;
    
    DIR *dirp = opendir(root[0].c_str());
    struct dirent *dp;
    
    struct magic_set *magic = magic_open(MAGIC_CHECK|MAGIC_MIME_TYPE);
    magic_load(magic,NULL);
    
    Rename_File(test_file_path, "imageimage283122.jpg", TYPE_JPEG|TYPE_PNG, "#ATIME");
    
    while(0){
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
                break;
            }
                
        } else {
            if (errno == 0) {
                cout << "Mission Complete" << endl;
                break;
            }
            closedir(dirp);
            cout << "Wrong Operation" << endl;
            magic_close(magic);
            return -1;
        }
    }

    //std::cout << i << endl;
    closedir(dirp);
    magic_close(magic);
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

int8_t Rename_File(const fs::path path, const char *regular_expression, const int8_t mode, const char *new_name_format){
    
    // Judge whether the file exists
    if (!File_Exist(path)){
        cout << "File not exist." << endl;
        return 1;
    }
    
    /*
        Variable Declaration
     */
    string new_name(new_name_format);
    
    regex regu(regular_expression);
    
    // libmagic initialize
    struct magic_set *magic_rename = magic_open(MAGIC_CHECK|MAGIC_MIME_TYPE);
    magic_load(magic_rename,NULL);
    
    // unordered_set initialize
    unordered_set <string> TypeSet;
    if (mode & TYPE_JPEG) TypeSet.insert("image/jpeg");
    if (mode & TYPE_PNG)  TypeSet.insert("iamge/png");
    
    // folder case
    if (strcmp(magic_file(magic_rename, path.c_str()), "inode/directory") == 0){
        DIR *dirp = opendir(path.c_str());
        struct dirent *dp;
        int16_t count = 0;
        
        while(dirp){
            struct stat file_status;
            errno = 0;
            
            chdir(path.c_str());
            if ((dp = readdir(dirp)) != NULL) {
                
                if (TypeSet.find(magic_file(magic_rename, dp->d_name)) == TypeSet.end()){
                    cout << "file not supported" << endl;
                    continue; // file type not supported
                }
                
                lstat(dp->d_name, &file_status);
                void *buffer = malloc(30*sizeof(char));
                
                new_name += string(dp->d_name).substr(string(dp->d_name).find_last_of('.'));
                
                strftime((char *)buffer, 30, "%Y_%b_%d_%H_%M_%S", localtime(&file_status.st_atime));
                new_name = regex_replace(new_name, regex("#ATIME"), (char *)buffer);
                strftime((char *)buffer, 30, "%Y_%b_%d_%H_%M_%S", localtime(&file_status.st_ctime));
                new_name = regex_replace(new_name, regex("#CTIME"), (char *)buffer);
                strftime((char *)buffer, 30, "%Y_%b_%d_%H_%M_%S", localtime(&file_status.st_mtime));
                new_name = regex_replace(new_name, regex("#MTIME"), (char *)buffer);
                
                free(buffer);

                new_name = regex_replace(new_name, regex("#COUNT"), to_string(count++));
                
                rename(dp->d_name, new_name.c_str());
                
                cout << "rename succeed." << endl;
            }
        }
    }
    // file case
    else if ((TypeSet.find(magic_file(magic_rename, path.c_str()))) != TypeSet.end()){
        fs::path origin_filepath = path.string().substr(0, path.string().find_last_of('/'));
        struct stat file_status;
        lstat(path.c_str(), &file_status);
        void *buffer = malloc(30*sizeof(char));
        
        new_name += path.string().substr(path.string().find_last_of('.'));
        
        strftime((char *)buffer, 30, "%Y_%b_%d_%H_%M_%S", localtime(&file_status.st_atime));
        new_name = regex_replace(new_name, regex("#ATIME"), (char *)buffer);
        strftime((char *)buffer, 30, "%Y_%b_%d_%H_%M_%S", localtime(&file_status.st_ctime));
        new_name = regex_replace(new_name, regex("#CTIME"), (char *)buffer);
        strftime((char *)buffer, 30, "%Y_%b_%d_%H_%M_%S", localtime(&file_status.st_mtime));
        new_name = regex_replace(new_name, regex("#MTIME"), (char *)buffer);
        
        free(buffer);
        chdir(origin_filepath.c_str());
        rename(path.c_str(), new_name.c_str());
    }
    
    return 0;
}
