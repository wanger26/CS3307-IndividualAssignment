//
// Created by Jakob Wanger on 2019-09-29.
// Student Number: 250950022
//

#include "myls.h"
string readableTime(timespec);


int main(int argc, char *argv[]){
    bool containl= false; //used as a flag for -l
    const char separator = ' '; //used for displaying content
    const int width= 12; //used for displaying content
    const int date= 25; //used for displaying content
    int result; //used for error tracking

    //Getting path for the current directory just in case we need it
    string path;
    char temp[PATH_MAX];
    getcwd(temp, PATH_MAX);
    path=string(temp);

    if (argc==2 && (strcmp(argv[1], "-L")==0 || strcmp(argv[1], "-l")==0)){ //if only -l was passed in as parameter we need to print all files with their detailed info
        FileManager manager= FileManager(path);
        if (manager.getErrorNum()!=0){ //if an error occurred then let the user know and exit gently
            cout<<"Error: "<<manager.getStringError()<<"\n";
            exit(1);
        }

        result=manager.expand();
        if (result!=0){ //if an error occurred then let the user know and exit gently
            cout<<"Error: "<<manager.getStringError()<<"\n";
            exit(1);
        }

        vector<FileManager> children=manager.getChildren();
        for (int index=0; index<children.size(); index++){
            FileManager child=children.at(index);
            cout<<left<<setw(width)<<setfill(separator)<<child.getPermission();
            cout<<left<<setw(width)<<setfill(separator)<<child.getType();
            cout<<left<<setw(width)<<setfill(separator)<<child.getStringOwner();
            cout<<left<<setw(width)<<setfill(separator)<<child.getStringGroup();
            cout<<left<<setw(width)<<setfill(separator)<<child.getSize();
            cout<<left<<setw(date)<<setfill(separator)<<readableTime(child.getModTime());
            cout<<left<<setw(width)<<setfill(separator)<<child.getName()<<"\n";
        }
    }
    else if (argc>1){ //if there is more then just a ls -l command or even no -l
        for (int x=1; x<argc; x++){
            if (strcmp(argv[x], "-l")==0 || strcmp(argv[x], "-L")==0) //if there is a -l any argument location then set the flag
                containl= true;
        }

        if (containl){ //if the flag was set then we know that -l is in the arguments
            for (int x=1; x<argc; x++){
                if (!strcmp(argv[x], "-l")==0 && !strcmp(argv[x], "-L")==0 && access(argv[x],F_OK)!=-1){ //if this is not the -l it must be a file name so print its info
                    FileManager file=FileManager(argv[x]);
                    if (file.getErrorNum()!=0){ //if an error occurred then let the user know and exit gently
                        cout<<"Error: "<<file.getStringError()<<"\n";
                        exit(1);
                    }
                    if (strcmp(file.getType().c_str(), "Directory")!=0) { //if its not a directory then just print the basic info
                        cout << left << setw(width) << setfill(separator) << file.getPermission();
                        cout << left << setw(width) << setfill(separator) << file.getType();
                        cout << left << setw(width) << setfill(separator) << file.getStringOwner();
                        cout << left << setw(width) << setfill(separator) << file.getStringGroup();
                        cout << left << setw(width) << setfill(separator) << file.getSize();
                        cout << left << setw(date) << setfill(separator) << readableTime(file.getModTime());
                        cout << left << setw(width) << setfill(separator) << file.getName() << "\n";
                    }
                    else { //else open the directory and print its contents
                        result=file.expand();
                        if (result!=0){ //if an error occurred then let the user know and exit gently
                            cout<<"Error: "<<file.getStringError()<<"\n";
                            exit(1);
                        }
                        vector<FileManager> children= file.getChildren();
                        cout<<"\n"<<file.getName()<<":\n";
                        for (int index=0; index<children.size(); index++){
                            cout << left << setw(width) << setfill(separator) << children.at(index).getPermission();
                            cout << left << setw(width) << setfill(separator) << children.at(index).getType();
                            cout << left << setw(width) << setfill(separator) << children.at(index).getStringOwner();
                            cout << left << setw(width) << setfill(separator) << children.at(index).getStringGroup();
                            cout << left << setw(width) << setfill(separator) << children.at(index).getSize();
                            cout << left << setw(date) << setfill(separator) << readableTime(children.at(index).getModTime());
                            cout << left << setw(width) << setfill(separator) << children.at(index).getName() << "\n";
                        }
                        cout<<"\n";
                    }
                }
                else if (!strcmp(argv[x], "-l")==0 && !strcmp(argv[x], "-L")==0 &&access(argv[x],F_OK)==-1) cout<<"Error: "<<argv[x]<<" does not exist.\n";
            }
        }
        else { //if the flag was not set then just print the arguments as recieved like ls does
            for (int x=1; x<argc; x++){
                FileManager file=FileManager(argv[x]);
                if (access(argv[x],F_OK)==-1){
                    cout<<"Error: "<<argv[x]<<" does not exist.\n";
                }
                else if (strcmp(file.getType().c_str(), "Directory")!=0) //if its not a directory then just print the name
                    cout<<argv[x]<<"\n";
                else { //else open the directory and print its contents
                    result=file.expand();
                    if (result!=0){ //if an error occurred then let the user know and exit gently
                        cout<<"Error: "<<file.getStringError()<<"\n";
                        exit(1);
                    }
                    vector<FileManager> children= file.getChildren();
                    cout<<"\n"<<file.getName()<<":\n";
                    for (int index=0; index<children.size(); index++) cout<<children.at(index).getName()<<"\n";
                    cout<<"\n";
                }
            }
        }
    }
    else { //else just print the items in the directory (just a ls)
        FileManager manager= FileManager(path);
        if (manager.getErrorNum()!=0){ //if an error occurred then let the user know and exit gently
            cout<<"Error: "<<manager.getStringError()<<"\n";
            exit(1);
        }

        result=manager.expand();
        if (result!=0){ //if an error occurred then let the user know and exit gently
            cout<<"Error: "<<manager.getStringError()<<"\n";
            exit(1);
        }

        vector<FileManager> children=manager.getChildren();
        cout<<"\n";
        for (int index=0; index<children.size(); index++){ //print children
            cout<<children.at(index).getName()<<"\n";
        }

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