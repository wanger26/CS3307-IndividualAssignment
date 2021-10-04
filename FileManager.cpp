//
// Created by Jakob Wanger on 2019-09-26.
// Student Number: 250950022
//
using namespace std;


#include "FileManager.h"
#include <experimental/filesystem>
/**
 * This creates an instance of the File Manager and initiatives it
 * @param fileName the name of the file to create the file manager for
 */
FileManager::FileManager (string fileName){
    struct stat sb;

    stat (fileName.c_str(), &sb);
    this->errorNumber= errno;

    this->name= fileName;
    this->size=sb.st_size;
    this->numericOwner=sb.st_uid;
    this->numericGroup=sb.st_gid;
    this->permissions= octalToSymbolic(sb.st_mode);
    this->accessTime= sb.st_atim;
    this->modTime= sb.st_mtim;
    this->statTime= sb.st_ctim;
    this->blockSize= sb.st_blksize;

    //String Owner Below
    struct passwd *pwd;
    pwd= getpwuid(this->numericOwner);
    this->errorNumber=errno;
    if (pwd!=NULL){
        this->stringOwner=string (pwd->pw_name);
    }

    //String Group Below
    struct group *grp;
    grp= getgrgid(this->numericGroup);
    this->errorNumber=errno;
    if (grp!=NULL){
        this->stringGroup=string (grp->gr_name);
    }

    //Getting the file type
    if (S_ISBLK(sb.st_mode)!=0){
        this->type="Block Device";
    }
    else if (S_ISCHR(sb.st_mode)!=0){
        this->type="Character Device";
    }
    else if (S_ISFIFO(sb.st_mode)!=0){
        this->type="FIFO";
    }
    else if (S_ISREG(sb.st_mode)!=0){
        this->type="Regular";
    }
    else if (S_ISDIR(sb.st_mode)!=0){
        this->type="Directory";
    }
    else if (S_ISSOCK(sb.st_mode)!=0){
        this->type="Socket";
    }

}

/**
 * This destroys and frees up any resources attached to the object the destructor was called on.
 * But in this class it needs to do nothing
 */
FileManager::~FileManager(){
}

/**
 *  This function will take a file stream as a parameter and dump the contents of the named file to that file stream
 * @param stream the stream which to dump the contents
 * @return 0 if successful, Non 0 otherwise
 */
int FileManager:: dump(ostream *stream) {
    if (this->type == "Regular") {
        fstream file1Stream(this->name, ios_base::in);
        this->errorNumber = errno;
        if (!file1Stream) return -1;

        while (file1Stream) {
            char *fileText = new char[this->blockSize];
            file1Stream.read(fileText, this->blockSize);
            stream->write(fileText, file1Stream.gcount());
        }
        file1Stream.close();
        return 0;
    }
    this->errorNumber=ENOTSUP;
    return this->errorNumber;

}

/**
 * This changes the name of the file from its existing name to the new name provided as a parameter to this function
 * @param newName the name for the file
 * @return 0 if successful, -1 otherwise
 */
int FileManager:: Rename(string newName){
    int result= rename(this->name.c_str(), newName.c_str());
    this->errorNumber= errno;

    if (result!=0) {
        return -1;
    }
    this->name=newName;
    return 0;
}

/**
 * This removes the file from the file system.
 * @return 0 if successful, -1 otherwise
 */
int FileManager:: remove() {
    int success= unlink(this->name.c_str());
    this->errorNumber = errno;
    if (success==0) { //if there was no error then reset all fields
        this->name="";
        this->name="";
        this->size = 0;
        this->numericOwner = 0;
        this->numericGroup = 0;
        this->permissions.clear();
        this->accessTime.tv_sec = 0;
        this->modTime.tv_sec = 0;
        this->statTime.tv_sec = 0;
        this->blockSize = 0;
        this->errorNumber = 0;
        return 0;
    }
    else return -1;
}

/**
 * This function takes another one of the file objects as a parameter and will compare the contents of the file object
 * this function was invoked upon to this other file object
 * @param file2
 * @return 0 if the 2 files are the same, 1 if they are different, and -1 if an error occurred
 */
int FileManager:: compare (FileManager file2){

    ifstream file1Stream(this->name);
    this->errorNumber= errno;
    if(!file1Stream) return -1;
    ifstream file2Stream(file2.name);
    this->errorNumber= errno;
    if(!file1Stream) return -1;

    while (file1Stream && file2Stream){
        char *file1Text = new char[this->blockSize];
        char *file2Text= new char [this->blockSize];
        file1Stream.read(file1Text,this->blockSize);
        file2Stream.read(file2Text, this->blockSize);

        if (strcmp(file1Text,file2Text)!=0) return 1;
    }
    file1Stream.close();
    file2Stream.close();
    return 0;
}

/**
 * This function operates on directories only and fills the children of the file object this function was invoked upon
 * @return 0 if everything was succesfull, a non 0 number if an error occurred
 */
int FileManager:: expand() {
    if (this->type=="Directory") {
        DIR *dir = opendir(this->name.c_str());
        this->errorNumber = errno;
        if (dir == NULL)
            return -1;

        struct dirent *dirReader;
        dirReader = readdir(dir);

        while (dirReader != nullptr) {
            if (strcmp(dirReader->d_name, ".")!=0 && strcmp(dirReader->d_name, "..")!=0){ //getting rid of all default hard links the directory is containing
                string filePath=dirReader->d_name;
                this->children.push_back(FileManager(&filePath[0]));
                this->errorNumber=errno;
            }
            dirReader = readdir(dir);
        }
        closedir(dir);
        return 0;
    }
    else {
        this->errorNumber=ENOTSUP;
        return this->errorNumber;
    }
}

//---------------------------------------------Setters------------------------------------------------------------------
/**
 * Sets the name of the file
 * @param newName the name of the file
 */
void FileManager:: setName(string newName){
    this->name=newName;
}

//---------------------------------------------Getters------------------------------------------------------------------
/**
 * Gets the name of the file
 * @return the name of the file
 */
string FileManager:: getName(){
    return this->name;
}

/**
 * Get the vector of the children
 * @return vector filled with children
 */
vector<FileManager> FileManager::getChildren() {
    return this->children;
}

/**
 * Gets the type of the file
 * @return the type of the file
 */
string FileManager:: getType(){
    return this->type;
}

/**
 * Gets the size of the file
 * @return the size of the file
 */
off_t FileManager:: getSize() {
    return this->size;
}

/**
 * Gets the string rep. of the owner of the file
 * @return the string rep. of the owner of the file
 */
string FileManager:: getStringOwner(){
    return this->stringOwner;
}

/**
 * Gets the numeric rep. of the owner of the file
 * @return the numeric rep. of the owner of the file
 */
uid_t FileManager:: getNumericOwner(){
    return this->numericOwner;
}

/**
 * Gets the string rep. of the group of the file
 * @return the string rep. of the group of the file
 */
string FileManager:: getStringGroup(){
    return this->stringGroup;
}

/**
 * Gets the numeric rep. of the group of the file
 * @return the numeric rep. group of the file
 */
uid_t FileManager:: getNumericGroup(){
    return this->numericGroup;
}

/**
 * Gets the permission of the file in symbolic format
 * @return the permission of the file in symbolic format
 */
string FileManager:: getPermission(){
    return this->permissions;
}

/**
 * Gets the access time of the file
 * @return the access time of the file
 */
timespec FileManager:: getAccessTime(){
    return this->accessTime;
}

/**
 * Gets the modification time of the file
 * @return the modification of the file
 */
timespec FileManager::getModTime(){
    return this->modTime;
}

/**
 * Gets the status time of the file
 * @return the status time of the file
 */
timespec FileManager:: getStatusTime(){
    return this->statTime;
}

/**
 * Gets the block size of the file
 * @return the block size of the file
 */
blksize_t FileManager:: getBlockSize(){
    return this->blockSize;
}

/**
 * Gets the error number of the file
 * @return the error number of the file
 */
int FileManager:: getErrorNum(){
    return this->errorNumber;
};

/**
 * Gets the error string rep. of the file
 * @return the error string rep. of the file
 */
string FileManager:: getStringError(){
    return strerror(this->errorNumber);
}

//---------------------------------------------Helpers------------------------------------------------------------------
/**
 * This method converts the mod_t's file permission to the symbolic format
 * @param code the mod_t version of the file permission
 * @return file permission in the symbolic format
 */
string FileManager::octalToSymbolic(mode_t code) {
    stringstream ss;
    string octalCode;

    ss<<oct<<(code&0777); //get rid of the file type number and convert to a 3 number octal number
    octalCode=ss.str(); //take that result and convert it to a string

    string result = "";
    for (int index = 0; index < octalCode.length(); index++) {
        if (octalCode.at(index) == '0') result=result+"---";
        else if (octalCode.at(index) == '1') result=result+"--x";
        else if (octalCode.at(index) == '2') result=result+"-w-";
        else if (octalCode.at(index) == '3') result=result+"-wx";
        else if (octalCode.at(index) == '4') result=result+"r--";
        else if (octalCode.at(index) == '5') result=result+"r-x";
        else if (octalCode.at(index) == '6') result=result+"rw-";
        else if (octalCode.at(index) == '7') result=result+"rwx";
    }
    return result;
}