//
// Created by Jakob Wanger on 2019-09-29.
// Student Number: 250950022
//
#include "mystat.h"
string readableTime(timespec);

int main(int argc, char *argv[]){
    const char separator = ' '; //Used for formatting text
    const int width= 12; //used for formatting text

    if (argc==2){ //Make sure the user provided the correct number of arguments
        FileManager manager= FileManager(argv[1]);
        if (manager.getErrorNum()!=0){ //if we get an error, notify the user and exit the program gently
            cout<<argv[1]<<": "<<manager.getStringError()<<"\n";
            exit(1);
        }
        else if (access(argv[1],F_OK)==-1){
            cout<<"Error: "<<argv[1]<<" does not exist.\n";
            exit(1);
        }
        cout<<right<<setw(width)<<setfill(separator)<<"File: "<<manager.getName()<<"\n";
        cout<<right<<setw(width)<<setfill(separator)<<"Type: "<<manager.getType()<<"\n";
        cout<<right<<setw(width)<<setfill(separator)<<"Size: "<<manager.getSize()<<"\n";
        cout<<right<<setw(width)<<setfill(separator)<<"Owner: "<<manager.getStringOwner()<<"\n";
        cout<<right<<setw(width)<<setfill(separator)<<"Group: "<<manager.getStringGroup()<<"\n";
        cout<<right<<setw(width)<<setfill(separator)<<"Block Size: "<<manager.getBlockSize()<<"\n";
        cout<<right<<setw(width)<<setfill(separator)<<"Permission: "<<manager.getPermission()<<"\n";
        cout<<right<<setw(width)<<setfill(separator)<<"Access: "<<readableTime(manager.getAccessTime())<<"\n";
        cout<<right<<setw(width)<<setfill(separator)<<"Modify: "<<readableTime(manager.getModTime())<<"\n";
        cout<<right<<setw(width)<<setfill(separator)<<"Status: "<<readableTime(manager.getStatusTime())<<"\n";
    }
    else {
        cout<<"\nError: Incorrect number of arguments provided\n";
    }

    return 0;
}

/**
 * This method is used to convert the timespec into a more readable format
 * @param timespec the time from the FileManager class
 * @return a string with the time in a readable format
 */
string readableTime(timespec timespec){
    struct tm time;
    localtime_r(&(timespec.tv_sec), &time);
    return to_string(time.tm_mday)+"-"+ to_string(time.tm_mon)+"-"+to_string(1900+time.tm_year)+" "+ to_string(time.tm_hour)+":"+ to_string(time.tm_min)+":"+ to_string(time.tm_sec);
}