//
// Created by Jakob Wanger on 2019-09-29.
// Student Number: 250950022
//

#include "mycat.h"

int main(int argc, char *argv[]){
    if (argc>1){ //if an argument was provided
        for (int index=1; index<argc; index++){
            FileManager toBePrinted= FileManager(argv[index]);
            if (toBePrinted.getErrorNum()!=0){ //if an error occurred, let the user know and exit softly
                cout<<"Error: "<<toBePrinted.getStringError()<<"\n";
                exit(1);
            }
            int result= toBePrinted.dump(&cout);
            if (result!=0) //if an error occurred, let the user know and exit softly
                cout<<"Error: "<<toBePrinted.getStringError()<<"\n";
        }
    }
    else {
        cout<<"\nError: Incorrect number of arguments provided\n";
    }
}

