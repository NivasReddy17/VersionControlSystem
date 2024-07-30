#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

#include "commitNodeList.cpp"
using namespace std;

class gitClass
{
public:
    commitNodeList list;
    void gitInit();
    void gitAdd();
    void gitAdd(string files[], int arrSize);
    void gitCommit(string msg);
    void gitRevert(string commitHash);
    void gitLog();
};

void gitClass::gitInit()
{
    filesystem::create_directory(".git");
    filesystem::create_directory(".git/staging_area");
    filesystem::create_directory(".git/commits");
}


// similar to 'git add' command in git
// copies all the files from the given directory recursively to the staging area 
void gitClass::gitAdd()
{
    const auto copyOptions = filesystem::copy_options::update_existing | filesystem::copy_options::recursive;

    for (const auto &dirEntry : filesystem::directory_iterator(filesystem::current_path()))
    {
        auto fileName = filesystem::path(dirEntry).filename();
        if (fileName != "git" && fileName != ".git")
        {
            if (filesystem::is_directory(fileName))
            {
                filesystem::copy(dirEntry, filesystem::current_path() / ".git" / "staging_area" / fileName, copyOptions);
            }
            else
            {
                filesystem::copy(dirEntry, filesystem::current_path() / ".git" / "staging_area" / "", copyOptions);
            }
        }
    }
}


// copies only specific files into the staging area
void gitClass::gitAdd(string files[], int arrSize)
{
    const auto copyOptions = filesystem::copy_options::update_existing | filesystem::copy_options::recursive;

    for (int i = 0; i < arrSize; i++)
    {
        if (filesystem::exists(files[i])) // check whether the file exists
        {
            int last_index = files[i].find_last_of('/'); // check whether its a file or a directory
            if (last_index != string::npos)
            {
                auto newDir = filesystem::current_path() / ".git" / "staging_area" / files[i].substr(0, last_index);
                filesystem::create_directories(newDir);
                filesystem::copy(filesystem::current_path() / files[i], filesystem::current_path() / ".git" / "staging_area" / files[i], copyOptions);
            }
            else
            {
                if (filesystem::is_directory(files[i]))
                {
                    filesystem::copy(filesystem::current_path() / files[i], filesystem::current_path() / ".git" / "staging_area" / files[i], copyOptions);
                }
                else
                {
                    filesystem::copy(filesystem::current_path() / files[i], filesystem::current_path() / ".git" / "staging_area", copyOptions);
                }
            }
        }
        else // file not found
        {
            cout << files[i] << "does not exist!" << endl;
        }
    }
}

void gitClass::gitCommit(string msg)
{
    list.addOnTail(msg);
}

void gitClass::gitRevert(string commitHash)
{
    list.revertCommit(commitHash);
}

void gitClass::gitLog()
{
    list.printCommitList();
}

