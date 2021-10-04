//
// Created by Jakob Wanger on 2019-09-28.
// Student Number: 250950022
//

#include "mycp.h"
int main(int argc, char *argv[]){
    string newName="";
    if (argc==3){ //check to see if the right number of parameters are passed in
        FileManager manager= FileManager(argv[1]);
        FileManager manager2= FileManager(argv[2]);

        if (manager.getErrorNum()!=0){ //if an error occurred, let the user know and exit softly
            cout<<"Error: "<<manager.getStringError()<<"\n";
            exit(1);
        }

        if (strcmp(manager2.getType().c_str(), "Directory")==0) newName=argv[2]+manager.getName(); //if we are copying to a directory with no new name
        else newName=argv[2]; //else we have a name specified

       fstream newFile(newName, ios_base::out);
       manager.dump(&newFile);
        if (manager.getErrorNum()!=0){ //if an error occurred, let the user know and exit softly
            cout<<"Error: "<<manager.getStringError()<<"\n";
        }
        newFile.close();
    }
    else { //else notify the user
        cout<<"\nError: Incorrect number of arguments provided\n";
    }
}
