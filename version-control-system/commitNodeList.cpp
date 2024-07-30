#include <iostream>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <ctime>
#include <vector>
using namespace std;

string gen_random(const int len) // Generates a random string tmp_s
{
    srand((unsigned)time(NULL) * getpid()); //intializes the seed for the 'random' function
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i)
    {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return tmp_s;
}

string get_time()
{
    time_t t = time(0); // get time now
    tm *now = localtime(&t);
    string dateTime = to_string(now->tm_year + 1900) + "/" +
                      to_string(now->tm_mon + 1) + "/" +
                      to_string(now->tm_mday) + " " +
                      to_string(now->tm_hour) + ":" +
                      to_string(now->tm_min) + "\n";

    return dateTime;
}

class commitNode
{
private:
    string commitID;
    string commitMsg;
    string nextCommitID;
    commitNode *next;

    void createCommitNode()
    {
        // create 'commitID' directory to 'commits', add file commitInfo.txt, add commitId,commitMsg,date&time to the file.
        // copy files from staging area to the commitId directory
        // staging area directories -----> commits/commitID/Data
        filesystem::create_directory(filesystem::current_path() / ".git" / "commits" / commitID);
        auto path = filesystem::current_path() / ".git" / "commits" / commitID / "commitInfo.txt";
        ofstream write(path.string());
        write << "1." + commitID + "\n" +
                     "2." + commitMsg + "\n" +
                     "3." + get_time() + "\n";

        auto STAGING_AREA_PATH = filesystem::path(filesystem::current_path() / ".git" / "staging_area");
        const auto copyOptions = filesystem::copy_options::update_existing | filesystem::copy_options::recursive;
        filesystem::copy(STAGING_AREA_PATH, filesystem::current_path() / ".git" / "commits" / commitID / "Data", copyOptions);
    }

public:
    // constructors
    commitNode()
    {
        this->next = NULL;
    }
    commitNode(string commitID, string commitMsg)
    {
        this->commitID = commitID;
        this->commitMsg = commitMsg;
        this->next = NULL;
        createCommitNode();
    }
    commitNode(string commitId)
    {
        this->commitID = commitId;
        next = NULL;
    }


    // revrets the current commit to the state of previous commit
    // All the files from the previous commit are recursively copied to the current commit directory
    void revertCommitNode(string fromHash)
    {
        filesystem::create_directories(filesystem::current_path() / ".git" / "commits" / getCommitID() / "Data");
        auto nextCommitPath = filesystem::current_path() / ".git" / "commits" / getCommitID() / "commitInfo.txt";
        auto copyFrom = filesystem::current_path() / ".git" / "commits" / fromHash / "Data";
        ofstream write(nextCommitPath.string());
        write << "1." + commitID + "\n" +
                     "2." + commitMsg + "\n" +
                     "3." + get_time() + "\n";
        const auto copyOptions = filesystem::copy_options::recursive;
        filesystem::copy(copyFrom, filesystem::current_path() / ".git" / "commits" / getCommitID() / "Data", copyOptions);
    }

    // read info from the commitInfo.txt ---> commitId, commitMsg
    void readNodeInfo()
    {
        string info;
        auto path = filesystem::current_path() / ".git" / "commits" / getCommitID() / "commitInfo.txt";
        ifstream file(path.string());
        while (getline(file, info))
        {
            if (info[0] == '1')
            {
                this->setCommitID(info.substr(2));
            }
            if (info[0] == '2')
            {
                this->setCommitMsg(info.substr(2));
            }
        }
    }

    string getCommitMsg()
    {
        return this->commitMsg;
    }
    void setCommitMsg(string commitMsg)
    {
        this->commitMsg = commitMsg;
    }

    void setCommitID(string commitID)
    {
        this->commitID = commitID;
    }
    string getCommitID()
    {
        return this->commitID;
    }

    void setNext(commitNode *node)
    {
        this->next = node;
    }
    commitNode *getNext()
    {
        return next;
    }

    void setNextCommitID(string nextCommitId)
    {
        this->nextCommitID = nextCommitId;
    }

    // creates 'nextCommitinfo.txt' and writes nextcommitId to it
    void writeNextCommitID(string nextCommitID)
    {
        setNextCommitID(nextCommitID);
        auto path = filesystem::current_path() / ".git" / "commits" / getCommitID() / "nextCommitInfo.txt";
        ofstream write(path.string());
        write << nextCommitID;
    }

    // checks whether info about nextCommit exits, return nextCommitId
    string checkNextCommitId()
    {
        filesystem::path tempPath(filesystem::current_path() / ".git" / "commits" / getCommitID() / "nextCommitInfo.txt");
        bool exists = filesystem::exists(tempPath);
        if (exists)
        {
            string info;
            ifstream file(tempPath);
            while (getline(file, info))
            {
                this->nextCommitID = info;
                break;
            }
            file.close();
            return nextCommitID;
        }
        return "";
    }
    string getNextCommitId()
    {
        return this->nextCommitID;
    }
};

// Linked List
class commitNodeList
{
private:
    commitNode *HEAD;
    commitNode *TAIL;

    bool checkHead()
    {
        // check if HEAD commit exists
        auto tempDir = filesystem::current_path() / ".git" / "commits" / "0x1111";
        return filesystem::exists(tempDir);
    }

public:
    commitNodeList()
    {
        setHead(NULL);
        setTail(NULL);
        if (checkHead())
        {
            setHead(new commitNode("0x1111"));
        }
    }

    commitNode *getHead()
    {
        return this->HEAD;
    }
    void setHead(commitNode *HEAD)
    {
        this->HEAD = HEAD;
    }

    commitNode *getTail()
    {
        return this->TAIL;
    }
    void setTail(commitNode *TAIL)
    {
        this->TAIL = TAIL;
    }

    // adds a new node at the tail of the list
    // checks if there is a head and traverses through the list until the tail is found
    void addOnTail(string msg)
    {
        if (!checkHead())
        {
            commitNode *newNode = new commitNode("0x1111", msg);
            setHead(newNode);
        }
        else
        {
            string commitID = gen_random(8);
            commitNode *currNode = getHead();
            while (currNode != NULL)
            {
                string nextCommitId = currNode->checkNextCommitId();

                if (nextCommitId.length() < 8)
                {
                    commitNode *newNode = new commitNode(commitID, msg);
                    currNode->writeNextCommitID(commitID);
                    currNode = NULL;
                }
                else
                {
                    commitNode *newNode = new commitNode();
                    newNode->setCommitID(nextCommitId);
                    currNode = newNode;
                }
            }
        }
    }
    // reverts to the specific commit in the linked list of commits
    // creates a new commit that represents the reverted state
    void revertCommit(string commitHash)
    {
        commitNode *commitNodeToRevert;
        if (!checkHead())
        {
            cout << "Nothing to Revert to " << endl;
        }
        else
        {
            bool error = true; // indicates whether the commitHash is found
            string commitID = gen_random(8);
            commitNode *currNode = getHead();
            while (currNode != NULL)
            {
                string nextCommitId = currNode->checkNextCommitId();
                if (commitHash == currNode->getCommitID())
                {
                    commitNodeToRevert = currNode;
                    error = false;
                }

                if (nextCommitId.length() < 8)
                {
                    if (!error)
                    {
                        commitNodeToRevert->readNodeInfo();
                        commitNode *newNode = new commitNode();
                        newNode->setCommitID(commitID);
                        newNode->setCommitMsg(commitNodeToRevert->getCommitMsg());
                        newNode->revertCommitNode(commitNodeToRevert->getCommitID());

                        currNode->writeNextCommitID(commitID);
                    }
                    currNode = NULL;
                }
                else
                {
                    commitNode *newNode = new commitNode();
                    newNode->setCommitID(nextCommitId);
                    currNode = newNode;
                }
            }

            if (error == true) // commitHash not found
            {
                cout << "does not match any Commit Hash" << endl;
            }
        }
    }


    //print details of all the commits
    void printCommitList()
    {
        commitNode *currNode = getHead();
        while (currNode != NULL)
        {
            string nextCommitId = currNode->checkNextCommitId();
            filesystem::path commitPath(filesystem::current_path() / ".git" / "commits" / currNode->getCommitID() / "commitInfo.txt");
            string info;
            ifstream file(commitPath.string());
            while (getline(file, info))
            {
                if (info[0] == '1')
                {
                    cout << "Commit ID:    " << info.substr(2) << endl;
                }
                if (info[0] == '2')
                {
                    cout << "Commit Msg:   " << info.substr(2) << endl;
                }
                if (info[0] == '3')
                {
                    cout << "Date & Time:  " << info.substr(2) << endl;
                }
            }
            file.close();
            cout << "============================\n\n";

            if (nextCommitId.length() < 8)
            {
                currNode = NULL;
            }
            else
            {
                currNode = currNode->getNext;
            }
        }
    }

};
