#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;
class Fat12Header;
class DirEntry;
bool stringOnlyContainsChar(const string &s, const char c);
vector<string> stringSplit(const string &s, const char delimeter);
void getHeader(Fat12Header *fat12Header);
vector<DirEntry> getRootDirEntries(Fat12Header &header);
vector<DirEntry> getDirEntries(unsigned int firstCluster);
vector<unsigned short> getFATable(Fat12Header &header);
void handleInsLs(vector<string> dirs);
void printDirContent(string dirName, vector<DirEntry> entries);
void handleInsLs_L(vector<string> dirs);
void printDirContent_L(string dirName, vector<DirEntry> entries);
string getEntryName(DirEntry &dirEntry);
void handleInsCat(vector<string> dirs);
string toLower(string s);
void myPrint(string s, bool isred = false);
void myPrintln(string s, bool isRed = false);
extern "C"
{
    void my_print(const char *s, const int len);
}
#pragma pack(push)
#pragma pack(1)
class Fat12Header
{
public:
    char BS_OEMName[8];            // OEM字符串，必须为8个字符，不足以空格填空8
    unsigned short BPB_BytsPerSec; // 每扇区字节数2
    unsigned char BPB_SecPerClus;  // 每簇占用的扇区数1
    unsigned short BPB_RsvdSecCnt; // Boot占用的扇区数2
    unsigned char BPB_NumFATs;     // FAT表的记录数1
    unsigned short BPB_RootEntCnt; // 最大根目录文件数2
    unsigned short BPB_TotSec16;   // 每个FAT占用扇区数2
    unsigned char BPB_Media;       // 媒体描述符1
    unsigned short BPB_FATSz16;    // 每个FAT占用扇区数2
    unsigned short BPB_SecPerTrk;  // 每个磁道扇区数2
    unsigned short BPB_NumHeads;   // 磁头数2
    unsigned int BPB_HiddSec;      // 隐藏扇区数4
    unsigned int BPB_TotSec32;     // 如果BPB_TotSec16是0，则在这里记录4
    unsigned char BS_DrvNum;       // 中断13的驱动器号1
    unsigned char BS_Reserved1;    // 未使用1
    unsigned char BS_BootSig;      // 扩展引导标志1
    unsigned int BS_VolID;         // 卷序列号4
    char BS_VolLab[11];            // 卷标，必须是11个字符，不足以空格填充11
    char BS_FileSysType[8];        // 文件系统类型，必须是8个字符，不足填充空格8
};

class DirEntry
{
public:
    char DIR_Name[11] = {0};
    unsigned char DIR_Attr;
    unsigned char reverse[10];
    unsigned short DIR_WrtTime;
    unsigned short DIR_Wrtdate;
    unsigned short DIR_FstClus;
    unsigned int DIR_FileSize;
};
#pragma pack(pop)

const char fatfilename[] = "a.img";
Fat12Header header;
vector<unsigned short> FATable;
vector<DirEntry> rootDirEntries;
int main(void)
{

    string fatFilename = "a.img";
    // prepare
    // readHeader
    getHeader(&header);
    // read FATable
    FATable = getFATable(header);
    // read root dir entries
    rootDirEntries = getRootDirEntries(header);
    // operation start!
    myPrintln("Please enter your instruction!");
    while (true)
    {
        string userInput;
        getline(cin, userInput);
        vector<string> ins = stringSplit(userInput, ' ');
        bool hasParamL = false;
        bool pathAmbigous = false;
        bool paramWrong = false;
        string path = "";
        if (userInput.substr(0, 2) == "ls")
        {
            // handle input operations!
            for (int i = 1; i < ins.size(); i++)
            {
                string token = ins[i];

                if (token.substr(0, 2) == "-l" && stringOnlyContainsChar(token.substr(1, token.size() - 1), 'l'))
                {

                    // if(path!=""){
                    //     string prompt="if you want -l, you should enter -l, then enter path.";
                    //     myPrintln(prompt);
                    //     continue;
                    // }
                    hasParamL = true;
                }
                else if (token[0] != '-')
                {
                    if (path != "")
                    {
                        pathAmbigous = true;
                        string prompt = "You have entered more than one paths.";
                        myPrintln(prompt);
                        break;
                    }
                    path = token;
                }
                else
                {
                    string prompt = "You have entered a wrong option. We only support -l, and mention that -L is not supported.";
                    myPrintln(prompt);
                    paramWrong = true;
                    break;
                }
            }
            if (!paramWrong)
            {
                // handle path
                vector<string> dirs = stringSplit(path, '/');
                // dirs.assign(dirsAndFileName.begin(),dirsAndFileName.end()-1);
                // string fileName=dirsAndFileName[dirsAndFileName.size()-1];
                if (hasParamL)
                {
                    handleInsLs_L(dirs);
                }
                else
                {
                    handleInsLs(dirs);
                }
            }
        }
        else if (userInput.substr(0, 3) == "cat")
        {
            if (ins.size() > 2)
            {
                myPrintln("you should enter only one file!!");
                continue;
            }
            else if (ins.size() < 2)
            {
                myPrintln("please enter a file to cat!");
                continue;
            }
            path = ins[1];
            vector<string> dirs = stringSplit(path, '/');
            handleInsCat(dirs);
        }
        else if (userInput.substr(0, 4) == "exit")
        {
            string prompt = "Bye!";
            myPrintln(prompt);
            break;
        }
        else if (userInput.substr(0, 4) == "help")
        {
            string prompt = "ls [-l] [dirPath] : ";
            myPrintln(prompt, true);
            prompt = "print dir contents. -l option means to print the direct dir num and direct file num.";
            myPrintln(prompt);
            prompt = "Mention that you can only enter one file path, but many -l option is allowed. and -l must be the lower case.";
            myPrintln(prompt);
            prompt = "cat filePath:";
            myPrintln(prompt, true);
            prompt = "show the content of the file.";
            myPrintln(prompt);

            prompt = "exit:";
            myPrintln(prompt, true);
            prompt = "exit this program.";
            myPrintln(prompt);
            prompt = "CAUTION: file path is UPPER case. ";
            myPrintln(prompt, true);
            continue;
        }
        else
        {
            string prompt = "You have entered a wrong indtruction! You can enter ls [-l] [dirPath],  cat filePath, exit, help.";
            myPrintln(prompt);
            continue;
        }
    }

    return 0;
}
bool stringOnlyContainsChar(const string &s, const char c)
{
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] != c)
        {
            return false;
        }
    }
    return true;
}
void myPrint(string s, bool isred)
{
    if (isred)
    {
        string redSign = "\033[31m";
        string commonColorSign = "\033[0m";
        s = redSign + s + commonColorSign;
    }
    //  cout<<s;
my_print(s.c_str(), s.size());
}
void myPrintln(string s, bool isRed)
{
    if (isRed)
    {
        string redSign = "\033[31m";
        string commonColorSign = "\033[0m";
        s = redSign + s + commonColorSign;
    }
    s += "\n";

    // cout<<s;
    my_print(s.c_str(), s.size());
}
vector<string> stringSplit(const string &s, const char delimeter)
{
    vector<string> ans;
    istringstream iss(s);
    string token;
    while (getline(iss, token, delimeter))
    {
        if (token.size() > 0)
            ans.push_back(token);
    }
    return ans;
}

void getHeader(Fat12Header *fat12Header)
{
    ifstream fin(fatfilename, ios_base::binary);
    if (!fin.is_open())
        myPrintln("open file error in getHeader!");
    fin.seekg(3);
    fin.read((char *)fat12Header, 59);
    int readedBytes = fin.gcount();
    fin.seekg(510);
    unsigned char b510 = 0;
    unsigned char b511 = 0;
    fin.read((char *)(&b510), 1);
    fin.read((char *)(&b511), 1);
    fin.close();
}

vector<DirEntry> getRootDirEntries(Fat12Header &header)
{
    vector<DirEntry> res;
    // get root dir entries
    ifstream fin(fatfilename, ios_base::binary);
    if (!fin.is_open())
        myPrintln("open file error in getRootDirEntries()!");

    // root dir start at 19th sector
    fin.seekg(19 * header.BPB_BytsPerSec);
    for (int i = 0; i < header.BPB_RootEntCnt; i++)
    {
        DirEntry dirEntry;
        // one dir entry is 32Bytes
        fin.read((char *)(&dirEntry), 32);
        // filter useless entries
        if (dirEntry.DIR_Name[0] == 0xE5 || dirEntry.DIR_Name[0] == 0x0)
            continue;
        int j;
        bool invalidFile = false;
        for (j = 0; j < 11; j++)
        {
            if (!(((dirEntry.DIR_Name[j] >= 48) && (dirEntry.DIR_Name[j] <= 57)) ||
                  ((dirEntry.DIR_Name[j] >= 65) && (dirEntry.DIR_Name[j] <= 90)) ||
                  (dirEntry.DIR_Name[j] == ' ') || (dirEntry.DIR_Name[j] == '_') ||  (dirEntry.DIR_Name[j] == '.')))
            {
                invalidFile = true;
            }
        }
        if (invalidFile)
            continue;

        string fullName;
        int k;
        if (dirEntry.DIR_Attr != 0x10)
        {
            // file
            for (k = 0; k < 11; k++)
            {
                if (dirEntry.DIR_Name[k] != ' ')
                {
                    fullName.push_back(dirEntry.DIR_Name[k]);
                }
                else
                {
                    fullName.push_back('.');
                    while (dirEntry.DIR_Name[k] == ' ')
                        k++;
                    k--;
                }
            }
        }
        else
        {
            // dir
            for (k = 0; k < 11 && dirEntry.DIR_Name[k] != ' '; k++)
            {
                fullName.push_back(dirEntry.DIR_Name[k]);
            }
        }

        res.push_back(dirEntry);
    }
    fin.close();
    return res;
}
vector<DirEntry> getDirEntries(DirEntry currentEntry)
{
    unsigned int firstCluster = currentEntry.DIR_FstClus;
    vector<DirEntry> res;
    // get root dir entries
    ifstream fin(fatfilename, ios_base::binary);
    if (!fin.is_open())
        myPrintln("open file error in getDirEntries()!");

    // root dir start at 19th sector
    fin.seekg((33 + firstCluster - 2) * header.BPB_BytsPerSec);
    int i=0;
    int flag=0;
    for (; i < header.BPB_RootEntCnt && firstCluster < 0xff7; i++)
    {
        DirEntry dirEntry;
        // one dir entry is 32Bytes
        fin.read((char *)(&dirEntry), 32);
        if (res.size() > 0 && getEntryName(dirEntry) == getEntryName(res[0]))
        {
            break;
        }
        // filter useless entries
        if (dirEntry.DIR_Name[0] == 0xE5 || dirEntry.DIR_Name[0] == 0x0)
            continue;
         if (dirEntry.DIR_Name[0] == 0xE5)
            break;   
        int j;
        if(dirEntry.DIR_Name[0]=='.'){
            flag++;
        }
        if(flag>=3) break;
        bool invalidFile = false;
        for (j = 0; j < 11; j++)
        {
            if (!(((dirEntry.DIR_Name[j] >= 48) && (dirEntry.DIR_Name[j] <= 57)) ||
                  ((dirEntry.DIR_Name[j] >= 65) && (dirEntry.DIR_Name[j] <= 90)) ||
                  (dirEntry.DIR_Name[j] == ' ') || (dirEntry.DIR_Name[j] == '_') ||  (dirEntry.DIR_Name[j] == '.')))
            {
                invalidFile = true; //非英文及数字、空格
            }
        }
        if (invalidFile)
            continue;

        string fullName;
        int k;
        if (dirEntry.DIR_Attr != 0x10)
        {
            // file
            for (k = 0; k < 11; k++)
            {
                if (dirEntry.DIR_Name[k] != ' ')
                {
                    fullName.push_back(dirEntry.DIR_Name[k]);
                }
                else
                {
                    fullName.push_back('.');
                    while (dirEntry.DIR_Name[k] == ' ')
                        k++;
                    k--;
                }
            }
        }
        else
        {
            // dir
            for (k = 0; k < 11 && dirEntry.DIR_Name[k] != ' '; k++)
            {
                fullName.push_back(dirEntry.DIR_Name[k]);
            }
        }

        res.push_back(dirEntry);
        if (i >= header.BPB_BytsPerSec - 1)
        {
            firstCluster = FATable[firstCluster];
            fin.seekg((33 + firstCluster - 2) * header.BPB_BytsPerSec);
        }
    }
    fin.close();
    return res;
}
string getEntryName(DirEntry &dirEntry)
{
    if (dirEntry.DIR_Name[0] == '\0')
        return "";
    if (dirEntry.DIR_Name[0] == 0xE5 || dirEntry.DIR_Name[0] == 0x0)
        return "";
    if (dirEntry.DIR_Name[0] == '\0')
        return "";
    int j;
    for (j = 0; j < 11; j++)
    {
        if (!(((dirEntry.DIR_Name[j] >= 48) && (dirEntry.DIR_Name[j] <= 57)) ||
              ((dirEntry.DIR_Name[j] >= 65) && (dirEntry.DIR_Name[j] <= 90)) ||
              (dirEntry.DIR_Name[j] == ' ') || (dirEntry.DIR_Name[j] == '_') ||  (dirEntry.DIR_Name[j] == '.') ))
        {
            return "";
        }
    }

    string fullName = "";
    int k;
    if (dirEntry.DIR_Attr != 0x10)
    {
        // file
        for (k = 0; k < 11; k++)
        {
            if (dirEntry.DIR_Name[k] != ' ')
            {
                fullName.push_back(dirEntry.DIR_Name[k]);
            }
            else
            {
                fullName.push_back('.');
                while (dirEntry.DIR_Name[k] == ' ')
                    k++;
                k--;
            }
        }
    }
    else
    {
        // dir
        for (k = 0; k < 11 && dirEntry.DIR_Name[k] != ' '; k++)
        {
            fullName.push_back(dirEntry.DIR_Name[k]);
        }
    }
    return fullName;
}
vector<unsigned short> getFATable(Fat12Header &header)
{
    vector<unsigned short> res;
    int rawFatSize = 9 * header.BPB_BytsPerSec;
    ifstream fin(fatfilename, ios_base::binary);
    if (!fin.is_open())
        myPrintln("open file error in getEntryName()!");
    fin.seekg(1 * header.BPB_BytsPerSec);
    //  FAT size is 9 clusters
    unsigned char rawFatContent[4608] = {0};
    fin.read((char *)rawFatContent, rawFatSize);
    vector<unsigned short> res2(rawFatSize * 2 / 3, 0xffff);
    for (int i = 0; i < rawFatSize; i++)
    {
        if (i % 3 == 1)
        {
            //
            unsigned short temp1 = rawFatContent[i - 1];
            unsigned short temp2 = rawFatContent[i];
            unsigned short temp3 = rawFatContent[i + 1];
            // unsigned short fatItem1= ((temp1<<4)& 0x0ff0) | ((temp2>>4)&0x0f);
            // unsigned short fatItem2= ((temp2<<8)&0x0f00) | (temp3&0x00ff);
            unsigned short fatItem1 = ((temp2 & 0x0F) << 8) | temp1;
            unsigned short fatItem2 = (temp3 << 4) | ((temp2 >> 4) & 0x0f);
            res.push_back(fatItem1);
            res.push_back(fatItem2);
        }
    }
    for (int i = 0, j = 0; i < rawFatSize; i += 3, j += 2)
    {
        res2[j] = static_cast<unsigned short>((rawFatContent[i + 1] & 0x0F) << 8) | rawFatContent[i];
        res2[j + 1] = static_cast<unsigned short>(rawFatContent[i + 2] << 4) | ((rawFatContent[i + 1] >> 4) & 0x0F);
    }
    fin.close();
    return res;
}

vector<DirEntry> enterEntryByName(vector<DirEntry> &currentDir, string nextDirName)
{
    vector<DirEntry> res;
    for (int i = 0; i < currentDir.size(); i++)
    {

        DirEntry currentEntry = currentDir[i];
        if (currentEntry.DIR_Name[0] != '0' && toLower(getEntryName(currentEntry)) == toLower(nextDirName) && currentEntry.DIR_Attr == 0x10)
        {
            // find it.
            res = getDirEntries(currentEntry);
            return res;
        }
        else if (currentEntry.DIR_Name[0] != '0' && toLower(getEntryName(currentEntry)) == toLower(nextDirName))
        {
            string prompt = nextDirName + " is not a directory.";
            myPrintln(prompt);
            return res;
        }
    }
    string prompt = nextDirName + " path does not exists.";
    myPrintln(prompt);
    vector<DirEntry> temp;
    return temp;
}
string toLower(string s)
{
    // string s2;
    // for(int i=0;i<s.size();i++){
    //     if(s[i]>=65 && s[i]<=90) s2.push_back(s[i]+32);
    //     else{
    //         s2.push_back(s[i]);
    //     }
    // }
    // return s2;

    return s;
}

char *readFile(vector<DirEntry> entries, string fileName, int &fileLen)
{
    ifstream fin(fatfilename, ios_base::binary);
    if (!fin.is_open())
        myPrintln("open file error in readFile()!");
    char buf[512] = {0};
    int count = 0;
    char *res;
    fileLen = 0;
    string content;
    for (int pos = 0; pos < entries.size(); pos++)
    {
        DirEntry entry = entries[pos];
        string entryName = getEntryName(entry);
        if (toLower(entryName) == toLower(fileName))
        {
            // find it;
            if (entry.DIR_Attr == 0x10)
            {
                myPrintln("this file is not a common file, it is a dir.");
                return "";
            }
            else if (entry.DIR_Attr == 0x02)
            {
                myPrintln("this file is not a common file, it is hidden.");
                return "";
            }
            else if (entry.DIR_Attr == 0x04)
            {
                myPrintln("this file is not a common file, it belongs to system.");
                return "";
            }
            if (entry.DIR_FileSize == 0)
            {

                myPrintln("this file is empty.");
                return "";
            }
            res = new char[entry.DIR_FileSize];
            int j = entry.DIR_FstClus;
            for (int i = 0; j < 0xff8; i += 512, j = FATable[j])
            {
                fin.seekg(header.BPB_BytsPerSec * (j + 33 - 2));
                fin.read(buf, sizeof(buf));

                for (int k = 0; k < sizeof(buf); k++)
                {
                    if (count < entry.DIR_FileSize)
                    {
                        res[i + k] = buf[k];
                        content.push_back(buf[k]);
                        count++;
                    }
                }
            }
            fileLen = entry.DIR_FileSize;
            if(j==0xff7){
                myPrintln("There is a bad cluster while reading file.");
                return "";
            }

            return res;
        }
    }
    myPrintln("there is no such file: " + fileName + ". Maybe you have entered a lower case, but we need upper case.");
    return "";
}
void calculateDirFileAndDirNum(vector<DirEntry> &entries, int &numDir, int &numFile)
{
    int nDir = 0;
    int nFile = 0;
    for (int i = 0; i < entries.size(); i++)
    {
        if (entries[i].DIR_Attr == 0x10)
            nDir++;
        else
            nFile++;
    }
    numDir = nDir;
    numFile = nFile;
}

void handleInsLs(vector<string> dirs)
{
    vector<DirEntry> currentDir = rootDirEntries;
    for (int i = 0; i < dirs.size(); i++)
    {
        currentDir = enterEntryByName(currentDir, dirs[i]);
    }
    if (currentDir.size() == 0)
    {

        return;
    }
    if (dirs.size() != 0)
    {
        string fullDirname;
        for (int i = 0; i < dirs.size(); i++)
            fullDirname += ("/" + dirs[i]);
        myPrintln(fullDirname + "/:", false);
        printDirContent(fullDirname, currentDir);
    }
    else
    {
        myPrintln("/:", false);
        printDirContent("", currentDir);
    }
}
void handleInsLs_L(vector<string> dirs)
{
    vector<DirEntry> currentDir = rootDirEntries;
    for (int i = 0; i < dirs.size(); i++)
    {
        currentDir = enterEntryByName(currentDir, dirs[i]);
    }
    if (currentDir.size() == 0)
    {

        return;
    }

    if (dirs.size() != 0)
    {
        string fullDirname;
        for (int i = 0; i < dirs.size(); i++)
            fullDirname += ("/" + dirs[i]);
        int nDir, nFile;
        calculateDirFileAndDirNum(currentDir, nDir, nFile);
        myPrintln(fullDirname + "/ " + std::to_string(nDir - 2) + " " + std::to_string(nFile) + ":", false);
        printDirContent_L(fullDirname, currentDir);
    }
    else
    {
        int nDir, nFile;
        calculateDirFileAndDirNum(currentDir, nDir, nFile);
        myPrintln("/ " + std::to_string(nDir) + " " + std::to_string(nFile) + ":", false);
        printDirContent_L("", currentDir);
    }
}
void printDirContent_L(string dirName, vector<DirEntry> entries)
{

    vector<DirEntry> directories;
    for (int i = 0; i < entries.size(); i++)
    {
        DirEntry entry = entries[i];
        if (entry.DIR_Attr == 0x10)
        {
            directories.push_back(entry);

            vector<DirEntry> thisSubEntries = getDirEntries(entry);
            int nDir, nFile;
            calculateDirFileAndDirNum(thisSubEntries, nDir, nFile);

            string thisEntryName = getEntryName(entry);
            if (thisEntryName[0] == '.')
            {
                myPrintln(thisEntryName, true);
                continue;
            }

            myPrint(thisEntryName, true);
            myPrintln(" " + to_string(nDir - 2) + " " + to_string(nFile), false);
        }
        else
        {
            int fileSize = entry.DIR_FileSize;
            myPrintln(getEntryName(entry) + " " + to_string(fileSize), false);
        }
    }

    myPrintln("", false);
    // handle sub directories.
    for (int i = 0; i < directories.size(); i++)
    {
        DirEntry entry = directories[i];
        if (getEntryName(entry)[0] == '.')
            continue;

        int nDir, nFile;
        vector<DirEntry> thisEntryEntries = getDirEntries(entry);
        calculateDirFileAndDirNum(thisEntryEntries, nDir, nFile);

        string name = dirName + "/" + getEntryName(entry);
        if (name[0] != '/')
            name = "/" + name;
        myPrintln(name + "/ " + to_string(nDir - 2) + " " + to_string(nFile) + ":", false);
        vector<DirEntry> enteredEntries = enterEntryByName(entries, getEntryName(entry));
        if (enteredEntries.size() == 0)
        {
            return;
        }
        printDirContent_L(name, enteredEntries);
    }
}

void printDirContent(string dirName, vector<DirEntry> entries)
{

    vector<DirEntry> directories;
    for (int i = 0; i < entries.size(); i++)
    {
        DirEntry entry = entries[i];
        if (entry.DIR_Attr == 0x10)
        {
            directories.push_back(entry);
            myPrint(getEntryName(entry), true);
            myPrint("  ", false);
        }
        else
        {
            myPrint(getEntryName(entry), false);
            myPrint("  ", false);
        }
    }

    myPrintln("", false);

    for (int i = 0; i < directories.size(); i++)
    {
        DirEntry entry = directories[i];
        if (getEntryName(entry)[0] == '.')
            continue;
        string name = dirName + "/" + getEntryName(entry);
        if (name[0] != '/')
            name = "/" + name;
        myPrintln(name + "/:", false);
        printDirContent(name, enterEntryByName(entries, getEntryName(entry)));
    }
}

void handleInsCat(vector<string> dirs)
{
    string fileName = dirs[dirs.size() - 1];
    vector<DirEntry> currentDir = rootDirEntries;
    for (int i = 0; i < dirs.size() - 1; i++)
    {
        currentDir = enterEntryByName(currentDir, dirs[i]);
    }

    int fileLen = 0;
    char *fileContentArray = readFile(currentDir, fileName, fileLen);
    string content = "";
    for (int i = 0; i < fileLen; i++)
    {
        content.push_back(fileContentArray[i]);
    }

    myPrintln(content, false);
}
