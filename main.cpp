#include <iostream>
#include <string.h>
#include <dirent.h> // struct dirent is defined in this header
#include <errno.h>
#include <stack> 
#include <sys/stat.h>
#include <sys/types.h>
#include <algorithm>
#include <fstream>

using namespace std;

/**
* function that replicates the contents of 
* normal file into the bizaro file 
**/
void create_biz_file(string og_file_path, string biz_file_path)
{

    ofstream file_biz;
    ifstream file_og;

    file_biz.open(biz_file_path.c_str());    //open the biz file
    file_og.open(og_file_path.c_str());


    string line;
    
    if (file_og.is_open())
    {
        while ( getline (file_og,line) )
        {
            string biz_line = line;
            reverse(biz_line.begin(), biz_line.end());
            file_biz << biz_line << endl;
        }
        file_og.close();
        file_biz.close();
    }

    else cout << "Unable to open file"; 
}


/**
* function to make the bizaro version of the dir
**/
void make_bizarro(string dir_name)
{

    string loc = ".";
    string parent = "..";
    
    DIR *dir;
    struct dirent *entry;

    // DIR *dir2;
    // struct dirent *entry2;

    //stack to store original file and dir names
    stack<string> stk_og;

    //stack for biz file name and dir names 
    stack<string> stk_biz;

    stk_og.push(dir_name);


    errno = 0;

    while (!stk_og.empty())
    {
        string temp_og = stk_og.top();
        stk_og.pop();

        string temp_biz;

        if(!stk_biz.empty())
        {
            temp_biz = stk_biz.top();
            stk_biz.pop();
            //dir2 = opendir(temp_biz.c_str());
        }
        else
        {
            temp_biz = temp_og;

        }
        
        dir = opendir(temp_og.c_str()); // string to c string conversion

        if (errno != 0) 
        {
            cout << "temp: " << temp_og << endl; //debugging statement 
            cout << "opendir error. " << strerror(errno) << endl;
        }
        else if(dir) 
        {
            errno = 0;
            while((entry = readdir(dir)) != NULL) 
            {
                if (entry->d_name != loc && entry->d_name != parent)
                {
                    auto type =  entry->d_type;
                    if(type == DT_DIR)
                    {
                        string path = temp_og + "/" + entry->d_name; 
                        //cout << "this is the path name: " << path <<  endl;

                        // stk.push(entry->d_name);
                        stk_og.push(path);

                        //creating Bizar's version
                        int check;
                        string bizaro_name = entry->d_name;
                        reverse(bizaro_name.begin(), bizaro_name.end());
                        string path2 = temp_biz + "/" + bizaro_name;
                        check = mkdir(path2.c_str(),0777);
                        stk_biz.push(path2);
                        cout << "check mkdir: " << check << endl; 
                    }

                    //if regular file; create files in the given directory
                    else if (type == DT_REG)
                    {
                        // og file's name 
                        
                        fstream file_biz;
                        string biz_name = entry->d_name;
                        string og_path = temp_og + "/" + entry->d_name;
                        reverse(biz_name.begin(), biz_name.end());
                        string file_path = temp_biz + "/" + biz_name;
                        file_biz.open(file_path.c_str(),std::ios_base::app);
                        file_biz.close();

                        
                        //fill in the files 
                        create_biz_file(og_path, file_path);

                    }
                }
            }
        }
        closedir(dir);
    }
}

int main()
{
    string dir_path;
    cout << "Provide the path to the directory to convert" << endl;
    cin >> dir_path;
    make_bizarro(dir_path);

}

    



