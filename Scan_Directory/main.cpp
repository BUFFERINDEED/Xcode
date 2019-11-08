//
//  main.cpp
//  Scan_Directory
//
//  Created by Reverse on 2019/11/8.
//  Copyright © 2019 Ayaya. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sys/dir.h>
#include <sys/stat.h>
using namespace std;


bool isDir(const char* path);
void myFind(const string& filename, const string& tofind);
void findInDir(const char *path, const string& tofind);
void findInDir(const char *path, const string& tofind, int recursive);
 
int main()
{
    findInDir("/Users/Stori/Documents/GitHub/Code/C/Xcode", "5");
    return 0;
}
 
void myFind(const string& filename, const string& tofind)
{
    ifstream fin(filename.c_str(),ios_base::binary);
    string str(1024*1024*2,0);
    fin.read(&str[0],2*1024*1024);
    fin.close();
    
    string::size_type beg = 0;
    string::size_type pos = -1;
    string::size_type find_size = tofind.size();
    if(0 == find_size)
    {
        cout << "no such file or no content" << endl;
        return;
    }
    
    unsigned count = 0;
    while((pos=str.find(tofind,beg))!=string::npos)
    {
        beg=pos+find_size;
        count++;
    }
    cout << "find matched number:" << count << endl;
}
//遍历文件夹的驱动函数
void findInDir(const char *path, const string& tofind)
{
    unsigned long len;
    char temp[256];
    //去掉末尾的'/'
    len = strlen(path);
    strcpy(temp, path);
    if(temp[len - 1] == '/') temp[len -1] = '\0';
    
    if(isDir(temp))
    {
        //处理目录
        int recursive = 1;
        findInDir(temp, tofind, recursive);
    }
    else   //输出文件
    {
        printf("path:%s\n", path);
        myFind(path, tofind);
    }
}
 
 
 
 
//遍历文件夹de递归函数
void findInDir(const char *path, const string& tofind, int recursive)
{
    DIR *pdir;
    struct dirent *pdirent;
    string name;
    char temp[256];
    try {
        pdir = opendir(path);
    }
    catch(const char *str)
    {printf("failed open dir");}
    
    if(pdir)
    {
        while((pdirent = readdir(pdir)))
        {
            name = pdirent->d_name;
            //跳过"."和".."
            if(strcmp(pdirent->d_name, ".") == 0
               || strcmp(pdirent->d_name, "..") == 0)
                continue;
            sprintf(temp, "%s/%s", path, pdirent->d_name);
            
            //当temp为目录并且recursive为1的时候递归处理子目录
            if(isDir(temp) && recursive && (name != ".git") && (name.find_last_not_of(".xcodeproj") == string::npos))
            {
                findInDir(temp, tofind, recursive);
            }
            else
            {
                printf("path:%s\n", temp);
                myFind(temp, tofind);
            }
        }
    }
    else
    {
        printf("opendir error:%s\n", path);
    }
    closedir(pdir);
}
 
bool isDir(const char* path)
{
    struct stat st;
    lstat(path, &st);
    return 0 != S_ISDIR(st.st_mode);
}

