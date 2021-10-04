//
// Created by Jakob Wanger on 2019-09-29.
// Student Number: 250950022
//

#include "myrm.h"

int main(int argc, char *argv[]){

    if (argc>1){
        for (int index=1; index<argc; index++){
            FileManager toBeRemoved= FileManager(argv[index]);
            if (toBeRemoved.getErrorNum()!=0){
                cout<<toBeRemoved.getStringError()<<"\n";
                exit(1);
            }
            int result=toBeRemoved.remove();
            if (result!=0) cout<<argv[index]<<": "<<toBeRemoved.getStringError()<<"\n";
        }
    }
    else {
        cout<<"\nError: Incorrect number of arguments provided\n";
    }

}