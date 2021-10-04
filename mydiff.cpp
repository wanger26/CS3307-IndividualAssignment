//
// Created by Jakob Wanger on 2019-09-29.
// Student Number: 250950022
//

#include "mydiff.h"
int main(int argc, char *argv[]){

    if (argc==3){
        FileManager manager1= FileManager(argv[1]);
        if (manager1.getErrorNum()!=0){ //if an error occurred, let the user know and exit softly
            cout<<"Error: "<<manager1.getStringError()<<"\n";
            exit(1);
        }
        else if (access(argv[1],F_OK)==-1){
            cout<<"Error: "<<argv[1]<<" does not exist.\n";
            exit(1);
        }

        FileManager manager2= FileManager(argv[2]);
        if (manager2.getErrorNum()!=0){ //if an error occurred, let the user know and exit softly
            cout<<"Error: "<<manager2.getStringError()<<"\n";
            exit(1);
        }
        else if (access(argv[2],F_OK)==-1){
            cout<<"Error: "<<argv[2]<<" does not exist.\n";
            exit(1);
        }        int result= manager1.compare(manager2);

        if (result==0) cout<<"\n"<<manager1.getName()<<" "<< manager2.getName()<< " are the same.\n";
        else if (result==1) cout<<"\n"<<manager1.getName()<<" "<< manager2.getName()<< " differ.\n";
        else if (manager1.getErrorNum()!=0){
            cout<<manager1.getName()<<" error: "<<manager1.getStringError()<<"\n";
        }
        else{
            cout<<manager2.getName()<<" error: "<<manager2.getStringError()<<"\n";
        }
    }
    else {
        cout<<"\nError: Incorrect number of arguments provided\n";
    }

}