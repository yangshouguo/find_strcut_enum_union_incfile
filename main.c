
#include<dirent.h>
#include<stdio.h>
#include<string.h>
#include<fstream>
#include<iostream>
using namespace std;
ofstream outfile;
void find_struct_enum_infile(string);
string buffer_string ;
void writeTofile(string x);
void recurse_folder(string foldername){//递归查找某一目录下所有.c文件
    struct dirent *ent = NULL;
    DIR *dir = opendir(foldername.c_str());
    if(dir!=NULL){
        while ((ent = readdir(dir))!=NULL){
            if (strcmp(ent->d_name,".") && strcmp(ent->d_name,"..")!=0){
                if(ent->d_type == 0x10){//if dir
                    cout<<foldername+ent->d_name<<endl;
                    recurse_folder(foldername+"/"+ent->d_name);
                }else if(ent->d_type==0){//if file
                    cout<<foldername+ent->d_name<<endl;
                    if(ent->d_name[ent->d_namlen-1]=='c' && ent->d_name[ent->d_namlen-2]=='.'){
                        //is c file ?
                        writeTofile("//"+foldername+"/"+ent->d_name);
                        find_struct_enum_infile(foldername+"/"+ent->d_name);
                    }
                }
            }
        }
        closedir(dir);
    }
}
string& trim(std::string &s)
{
    if (s.empty())
    {
        return s;
    }
    s.erase(0,s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

string getwordinstring(string &str , int which){
    int front_black = 0 ;
    int behind_black = -1 ;
    for (int i = 1 ;i<= which ; ++i ){
        front_black = str.find_first_not_of(' ',behind_black+1);
        behind_black = str.find_first_of(' ',front_black)-1;
//        cout<<front_black+1<<" "<<behind_black+1<<endl;
        if(front_black==string::npos){
            return "";
        }
        if(behind_black==string::npos){
            behind_black = str.length();
        }
    }
//    cout<<"return :"<<front_black+1<<" "<<behind_black+1<<endl;
////    cout<<"return value:"<<str.substr(front_black,behind_black+1)<<endl;
    return str.substr(front_black,behind_black+1-front_black);
}
int add_struct_to_buffer(ifstream &cfile,bool is_leftkh){ //is_leftkh  左边的{ 方括号是否已经出现
    string line ;
    int pos = 0;
    bool is_end = false;
    int l ;
    int time = 0;
    while (!cfile.eof()){
        getline(cfile,line);
        buffer_string.append(line+"\n");
        line = trim(line);
        //cout<<"line:"<<line<<endl;
        string firstword = getwordinstring(line,1);
            string secondword = getwordinstring(line,2);
            if ((firstword == "struct" || firstword == "enum"|| firstword == "union" ||secondword == "struct" || secondword == "enum"|| secondword == "union"  )&& line.find(';')==string::npos){
//        if ((firstword == "typedef" || firstword == "struct" || firstword == "enum"|| firstword == "union" )&& line.find(';')==string::npos){
            //cout<<"here"<<endl;
            bool is_zkh = false;
            if(line.find('{')!=string::npos)
                is_zkh = true;
            add_struct_to_buffer(cfile,is_zkh);
            //cout<<"back!!!!!!!!!!!!!!!"<<line<<" "<<is_end<<endl;
            continue;
        }
        if(!is_leftkh && line.find(")")!=string::npos){
            buffer_string.clear();
            return 0;
        }
        if(!is_leftkh ){
            if((pos = line.find("{"))!=string::npos){
                is_leftkh = true;
            }else{
                if(time>1){
                    buffer_string.clear();
                    return 0;
                }else{
                time++;
                }
            }
        }

        if((pos = line.find("}"))!=string::npos ){
            is_end = true;
        }
        //if(line.find(";",pos)!=string::npos){
        if(line.find(";")!=string::npos){
            //cout<<"find ; :"<<line<<" "<<pos<<endl<<"is_end"<<is_end<<endl;
            if(is_end){
          //      cout<<"return "<<endl;
                return 1 ;
            }
        }else{
            pos = 0 ;
        }
    }


}
void find_struct_enum_infile(string cfilename){

    ifstream cfile(cfilename.c_str());
    string line ;
    buffer_string.clear();
    int l ;
    if (cfile.is_open()){
        while (!cfile.eof()){
            getline(cfile,line);
            line = trim(line);
        //cout<<"line:"<<line<<endl;
            string firstword = getwordinstring(line,1);
            string secondword = getwordinstring(line,2);
            if ((firstword == "struct" || firstword == "enum"|| firstword == "union" ||((secondword == "struct" || secondword == "enum"|| secondword == "union") &&firstword=="typedef" && getwordinstring(line,4)=="") )&& line.find(';')==string::npos&&line.find(')')==string::npos){
                buffer_string.clear();
                buffer_string.append(line+"\n");

                bool is_zkh = false;
                if(line.find('{')!=string::npos)
                    is_zkh = true;

                add_struct_to_buffer(cfile,is_zkh);

                //add_struct_to_buffer(cfile);
                writeTofile(buffer_string);
                buffer_string.clear();
            }

        }
    }
    cfile.close();
}
void openfile(string filename){
    outfile.open(filename.c_str());
}
void writeTofile(string x){
    outfile<<(x);
    outfile<<('\n');
}
void closefile(){
    outfile.close();
}
int main(int argc, char ** argv){

    openfile("struct.txt");
    string folder = "G:/漏洞/code/wget/wget-1.17.1";  //需要递归查找的目录
    //recurse_folder("H:/code_shop");
    recurse_folder(folder);
    closefile();
//    recurrent_folder();//递归查找某一目录下所有.c文件

}
