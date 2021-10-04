//
// Created by jakob on 2019-09-26.
//


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "string"
#include "fstream"
#include <pwd.h>
#include "grp.h"
#include <errno.h>
#include <string.h>
#include <sstream>
#include "vector"
#include <dirent.h>
#include <iostream>
#include <cstring>

using namespace std;

#ifndef INDIVIDUAL_ASSIGNMENT_FILEMANAGER_H
#define INDIVIDUAL_ASSIGNMENT_FILEMANAGER_H

class FileManager {

    private:
        string name, stringOwner, stringGroup;
        string type;
        off_t size;
        uid_t numericOwner;
        gid_t numericGroup;
        string permissions;
        timespec accessTime;
        timespec modTime;
        timespec statTime;
        blksize_t blockSize;
        int errorNumber;
        vector<FileManager> children;



    public:
        FileManager (string);

        ~FileManager();

        int dump(ostream*);

        int Rename(string);

        int remove();

        int compare (FileManager);

        int expand();

        string getName();

        void setName(string);

        string getType();

        off_t getSize();

        string getStringOwner();

        uid_t getNumericOwner();

        string getStringGroup();

        uid_t getNumericGroup();

        string getPermission();

        timespec getAccessTime();

        timespec getModTime();

        timespec getStatusTime();

        vector<FileManager> getChildren();

        blksize_t getBlockSize();

        int getErrorNum();

        string getStringError();

private:
        string octalToSymbolic(mode_t) ;

};


#endif //INDIVIDUAL_ASSIGNMENT_FILEMANAGER_H
