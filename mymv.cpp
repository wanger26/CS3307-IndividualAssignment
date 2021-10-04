//
// Created by Jakob Wanger on 2019-09-29.
// Student Number: 250950022
//

#include "mymv.h"

int main(int argc, char *argv[]){
    int result=0;
    string newName="";
    if (argc==3){
        FileManager manager= FileManager(argv[1]);
        FileManager manager2= FileManager(argv[2]);

        if (strcmp(manager2.getType().c_str(), "Directory")==0) newName=argv[2]+manager.getName(); //if the destination is dir, then we can keep the name of the file
        else newName=argv[2]; //else take the specified name
        int result=manager.Rename(newName);
        if (result!=0 && result!=EXDEV){ //if an error occurred exit softly
            cout<<"Error: "<<manager.getStringError()<<"\n";
            exit(1);
        }

        if(result==EXDEV){ //if EXDEV is set then we need to perform dump function
            fstream stream=fstream(argv[2], ios_base::out);
            result=manager.dump(&stream);
            if (result!=0){ //if an error occurred let the user know and exit softly
                cout<<"Error: "<<manager.getStringError()<<"\n";
                exit(1);
            }
            result= manager.remove();
            if (result!=0){  //if an error occurred let the user know and exit softly
                cout<<"Error: "<<manager.getStringError()<<"\n";
            }
        }
    }
    else {
        cout<<"\nError: Incorrect number of arguments provided\n";
    }


    return 0;

}