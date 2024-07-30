#include <iostream>

#include "gitClass.cpp"
using namespace std;


int main(int argc, char *argv[])
{
    gitClass gitClassObj;
    if(argc >= 2)
    {
        string argument = string(argv[1]);
        //git init
        if (argument == "init")
        {
            gitClassObj.gitInit();
            cout << "git repository initialized successfully!" << endl;
            
        }
        //git add
        else if (argument == "add")                                               
        {
            if(argc == 2){     
                cout << "missing arguments!" <<endl;
                cout << "Provide a third argument e.g." << endl;
                cout << "git add <'.' | 'file_name'>" << endl;
            }
            if(argc >= 3){
                if(argc == 3){
                    string argumentC = string(argv[2]);
                    if (argumentC == ".") {
                        gitClassObj.gitAdd();
                    }
                    else
                    {
                        string files[1] = {string(argv[2])};
                        gitClassObj.gitAdd(files, 1);
                    }

                } else {
                    string files[argc-2];
                    for (int i = 0; i < argc-2; i++)
                    {
                        files[i] = string(argv[i]); 
                    }
                    gitClassObj.gitAdd(files, argc-2);
                }
            }
            
        }
        //git commit
        else if (argument == "commit")
        {
            if(argc == 4)    //[ git, commit, -m, "msg" ]
            {
                string argumentC = string(argv[2]);    
                string argumentD = string(argv[3]);      
                if(argumentC == "-m")
                {
                    gitClassObj.gitCommit(argumentD);
                    cout << "files commited successfully" << endl;
                }
            }
            else 
            {
                cout << "missing arguments!" <<endl;
                cout << "Provide with a message field e.g." << endl;
                cout << "git commit -m 'my commit message'" << endl;
            }
        }
        // git revert
        else if(argument == "revert")
        {
            if(argc == 3)
            {
                string argumentC = string(argv[2]);
                if(argumentC == "HEAD")
                {
                    gitClassObj.gitRevert(argumentC);
                    cout << "The project is now at HEAD" << endl;
                }
                else
                {
                    gitClassObj.gitRevert(argumentC);
                    cout << "Reverted to <commit_id> commit" << endl;
                }
            }
            else 
            {
                cout << "invalid arguments, should be like: " << endl;
                cout << "git revert <'HEAD'|'commit_hash'>" << endl;
            }
        }
        // //git log
        else if(argument == "log")
        {
            gitClassObj.gitLog();
        }
        else
        {
            cout << RED "Invalid arguments" END << endl;
        }

    }
    else 
    {

        cout << "Usage: " << endl;
        cout << "git init                           ->   initialize an empty git repository in the current dir" << endl;
        cout << "git add <'.'|'file_name'>          ->   add the files to staging area" << endl;
        cout << "git commit <m 'commit message'>    ->   commit your staging files" << endl;
        cout << "git revert <'HEAD'|'commit_hash'>  ->   rollback to a specific commit" << endl;
    }
}