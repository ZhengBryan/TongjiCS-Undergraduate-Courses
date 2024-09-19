#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <sstream>
#include "Shell.h"
#include "SystemCall.h"
using namespace std;

/* �洢�ַ���������fread/fwrite */
unordered_map<string, string> str_set;

Shell::Shell() : commands{
    new Command_ls(),
    new Command_cd(),
    new Command_cp(),
    new Command_mv(),
    new Command_mkdir(),
    new Command_fformat(),
    new Command_fcreat(),
    new Command_fdelete(),
    new Command_fopen(),
    new Command_fclose(),
    new Command_fread(),
    new Command_fwrite(),
    new Command_flseek(),
    new Command_tree()
}{ }

Shell::~Shell()
{
    for (int i = 0; i < NCOMMAND; i++) {
        if (commands[i])
            delete commands[i];
    }
}

void Shell::interface()
{
	while (true) {
        User& u = OSKernel::Instance().GetUser();   /* ����ѭ�����ڣ�fformat�������¼���User */
        cout << u.u_curdir << "$ ";
        string line;
        getline(cin, line);

        istringstream iss(line);
        vector<std::string> words;
        string word;

        /* �ָ������� */
        while (iss >> word) {
            words.push_back(word);
        }

        if (words.size() == 0)
            continue;

        if (words[0] == string("help")) {
            for (int i = 0; i < NCOMMAND; i++) {
                if (commands[i]) {
                    cout << commands[i]->name << "\t<" << commands[i]->description << ">\n";
                }
            }
            cout << "quit\t<�˳�ϵͳ>\n";
            cout << endl;
            continue;
        }
        else if(words[0] == string("quit")) {
            break;
        }

        /* ƥ������ */
        int i;
        for (i = 0; i < NCOMMAND; i++) {
            if (commands[i] == nullptr)
                continue;
            else if (commands[i]->name == words[0]) {
                words.erase(words.begin());
                commands[i]->execute(words);
                handleUserError();
                cout << endl;
                break;
            }
        }
        if (i == NCOMMAND)
            cout << "�������!\n" << endl;
	}
}

void Shell::handleUserError()
{
    User& u = OSKernel::Instance().GetUser();
    switch (u.u_error) {
        case EPERM:           cout << "����������" << endl; break;
        case ENOENT:          cout << "�ļ���Ŀ¼������" << endl; break;
        case ESRCH:           cout << "û�������Ľ���" << endl; break;
        case EINTR:           cout << "���жϵ�ϵͳ����" << endl; break;
        case EIO:             cout << "I/O ����" << endl; break;
        case ENXIO:           cout << "û���������豸���ַ" << endl; break;
        case E2BIG:           cout << "�����б�̫��" << endl; break;
        case ENOEXEC:         cout << "ִ�и�ʽ����" << endl; break;
        case EBADF:           cout << "�ļ�����������ȷ" << endl; break;
        case ECHILD:          cout << "û���ӽ���" << endl; break;
        case EAGAIN:          cout << "��Դ��ʱ������" << endl; break;
        case ENOMEM:          cout << "�ڴ治��" << endl; break;
        case EACCES:          cout << "Ȩ�޲���" << endl; break;
        case EFAULT:          cout << "����ĵ�ַ" << endl; break;
        case EBUSY:           cout << "�豸����Դæ" << endl; break;
        case EEXIST:          cout << "�ļ��Ѵ���" << endl; break;
        case EXDEV:           cout << "���豸����" << endl; break;
        case ENODEV:          cout << "û���������豸" << endl; break;
        case ENOTDIR:         cout << "��Ŀ¼·��" << endl; break;
        case EISDIR:          cout << "��·����Ŀ¼" << endl; break;
        case ENFILE:          cout << "ϵͳ�򿪵��ļ�����" << endl; break;
        case EMFILE:          cout << "�򿪵��ļ�����" << endl; break;
        case ENOTTY:          cout << "���ʵ��� I/O ���Ʋ���" << endl; break;
        case EFBIG:           cout << "�ļ�����" << endl; break;
        case ENOSPC:          cout << "�豸��û�пռ�" << endl; break;
        case ESPIPE:          cout << "�Ƿ��� seek ����" << endl; break;
        case EROFS:           cout << "ֻ���ļ�ϵͳ" << endl; break;
        case EMLINK:          cout << "���ӹ���" << endl; break;
        case EPIPE:           cout << "����Ĺܵ�" << endl; break;
        case EDOM:            cout << "��ѧ������������������" << endl; break;
        case EDEADLK:         cout << "��Դ��������" << endl; break;
        case ENAMETOOLONG:    cout << "�ļ���̫��" << endl; break;
        case ENOLCK:          cout << "�޷���ȡ�ļ���" << endl; break;
        case ENOSYS:          cout << "����δʵ��" << endl; break;
        case ENOTEMPTY:       cout << "Ŀ¼�ǿ�" << endl; break;
    }
    /* ��������� */
    u.u_error = 0;
}

void Command::help()
{
    cout << name << " <" << description << ">" << endl;
    cout << "Usage:";
    for (int i = 0; i < usage.size(); i++) {
        cout << "\t" << usage[i] << "\n";
    }
}

void Command_ls::execute(vector<string> args) {
    string path;
    User& u = OSKernel::Instance().GetUser();
    bool hidden = true;
    int mode = 0;

    /* �������� */
    for (int i = 0; i < args.size(); i++) {
        string arg = args[i];
        if (arg == "-a")
            hidden = false;
        else if (arg[0] == '-') {
            help();
            return;
        }
        else {
            if (path == "")
                path = arg;
            /* ������� ���岻�� */
            else {
                help();
                return;
            }
        }
    }

    DiskInode inode;
    Sys_Stat(path.c_str(), &inode);
    if (u.u_error) {
        return;
    }
    else if ((inode.d_mode & Inode::IFMT) != Inode::IFDIR) {
        cout << "����·�������ļ��У�" << endl;
        return;
    }

    int file = Sys_Open(path.c_str(), File::FREAD);

    unsigned char buffer[sizeof(DirectoryEntry)];

    DirectoryEntry temp;

    while (Sys_Read(file, buffer, sizeof(DirectoryEntry)))
    {
        memcpy(&temp, buffer, sizeof(DirectoryEntry));
        if (temp.m_ino == 0)
            continue;
        else if (hidden && temp.m_name[0] == '.')
            continue;
        cout << temp.m_name << "\t";
    }

    Sys_Close(file);

    if (!u.u_error)
        cout << endl;
}

void Command_cd::execute(vector<string> args)
{
    if (args.size() != 1 || (args.size() == 1 && args[0][0] == '-')) {
        help();
        return;
    }

    Sys_ChDir(args[0].c_str());
}

void Command_cp::execute(vector<string> args)
{
    if (args.size() != 2) {
        help();
        return;
    }

    User& u = OSKernel::Instance().GetUser();
    bool s_out, d_out;
    fstream src, dst;
    int s_fd, d_fd;

    /* ����src */
    if (args[0][0] == '$') {
        s_out = true;
        src.open(args[0].substr(1), ios::in | ios::binary);
        if (!src) {
            cout << "src: �ⲿ�ļ�������" << endl;
            return;
        }
    }
    else {
        s_out = false;
        s_fd = Sys_Open(args[0].c_str(), File::FREAD);
        if (u.u_error) {
            cout << "src: ";    /* ���غ�handleUserError�ᴦ�� */
            return;
        }
    }

    /* ����dst */
    if (args[1][0] == '$') {
        d_out = true;
        dst.open(args[1].substr(1), ios::out | ios::trunc | ios::binary);
        if (!dst) {
            cout << "dst: �ⲿ�ļ�����ʧ��" << endl;
            return;
        }
    }
    else {
        d_out = false;
        d_fd = Sys_Creat(args[1].c_str());
        if (u.u_error) {
            cout << "dst: ";    /* ���غ�handleUserError�ᴦ�� */
            return;
        }
    }

    /* һ��BufferΪ��λ���д��� */
    char* buffer = new char[BUF_SIZE];

    while (true) {
        int count;

        if (s_out) {
            src.read(buffer, BUF_SIZE);
            count = src.gcount();
        }
        else {
            count = Sys_Read(s_fd, (unsigned char*)buffer, BUF_SIZE);
        }

        if (count == 0)
            break;

        if (d_out)
            dst.write(buffer, count);
        else
            Sys_Write(d_fd, (unsigned char*)buffer, count);
    }

    if (s_out)
        src.close();
    else
        Sys_Close(s_fd);

    if (d_out)
        dst.close();
    else
        Sys_Close(d_fd);

    delete[] buffer;
    return;
}


void Command_mv::execute(vector<string> args)
{
    if (args.size() != 2) {
        help();
        return;
    }

    if (args[0] == args[1])
        return;

    // ��ִ��cp
    Command_cp cp;
    cp.execute(args);

    // ��cp��������ɾ��
    User& u = OSKernel::Instance().GetUser();
    if (u.u_error)
        return;

    // cp�ɹ���ɾ��ԭ�ļ�
    Command_fdelete fdelete;
    fdelete.execute({ vector<string>(1, args[0]) });
}



void Command_mkdir::execute(vector<string> args)
{
    if (args.size() != 1 || (args.size() == 1 && args[0][0] == '-')) {
        help();
        return;
    }

    Sys_MkNod(args[0].c_str());
}

void Command_fformat::execute(vector<string> args)
{
    OSKernel& o = OSKernel::Instance();

    o.GetFileSystem().FormatDisk();
    o.Restart();        /* ��������Kernel�������ܹرմ򿪵�Openfile�ṹ������curdir��*/
}

void Command_fcreat::execute(vector<string> args)
{
    if (args.size() != 1 || (args.size() == 1 && args[0][0] == '-')) {
        help();
        return;
    }

    int fd = Sys_Creat(args[0].c_str());

    User& u = OSKernel::Instance().GetUser();
    if (!u.u_error)
        cout << "fd: " << fd << endl;
}


void Command_fdelete::execute(vector<string> args)
{
    if (args.size() != 1 || (args.size() == 1 && args[0][0] == '-')) {
        help();
        return;
    }

    Sys_Unlink(args[0].c_str());

    return;
}

void Command_fopen::execute(vector<string> args)
{
    string path;
    int mode = 0;

    /* �������� */
    for (int i = 0; i < args.size(); i++) {
        string arg = args[i];

        if (arg == "-r") 
            mode |= File::FREAD;
        else if (arg == "-w") 
            mode |= File::FWRITE;
        else if(arg[0] == '-') {
            help();
            return;
        }
        else {
            if (path == "")
                path = arg;
            /* ������� ���岻�� */
            else {
                help();
                return;
            }
        }
    }

    if (path == "") {
        help();
        return;
    }

    if (mode == 0) {
        cout << "����ָ������һ�ִ򿪷�ʽ" << endl;
        return;
    }

    int fd = Sys_Open(path.c_str(), mode);

    User& u = OSKernel::Instance().GetUser();
    if (!u.u_error)
        cout << "fd: " << fd << endl;
}

void Command_fclose::execute(vector<string> args)
{
    if (args.size() != 1 || (args.size() == 1 && args[0][0] == '-')) {
        help();
        return;
    }

    int fd;
    istringstream ss1 = istringstream(args[0]);
    ss1 >> fd;
    if (ss1.fail()) {
        cout << "�ļ�������fd�Ƿ�" << endl;
        return;
    }

    Sys_Close(fd);
}

void Command_fread::execute(vector<string> args)
{
    if (!(args.size() == 2 || (args.size() == 4 && args[2] == ">>"))) {
        help();
        return;
    }

    int fd, count;

    istringstream ss1 = istringstream(args[0]);
    ss1 >> fd;
    if (ss1.fail()) {
        cout << "�ļ�������fd�Ƿ�" << endl;
        return;
    }
    istringstream ss2 = istringstream(args[1]);
    ss2 >> count;
    if (ss2.fail()) {
        cout << "�����ֽ����Ƿ�" << endl;
        return;
    }
    
    unsigned char* buffer = new unsigned char[count + 1];
    count = Sys_Read(fd, buffer, count);

    User& u = OSKernel::Instance().GetUser();
    if (!u.u_error) {
        buffer[count] = 0;

        if (!count) {
            delete[] buffer;
            return;
        }

        if (args.size() == 2)
            cout << buffer << endl;
        else {
            str_set[args[3]] = string((char *)buffer);
        }
    }

    delete[] buffer;
}


void Command_fwrite::execute(vector<string> args)
{
    if (!(args.size() == 2 || (args.size() == 3 && args[1] == "<<"))) {
        help();
        return;
    }

    int fd, count;
    istringstream ss1 = istringstream(args[0]);
    ss1 >> fd;
    if (ss1.fail()) {
        cout << "�ļ�������fd�Ƿ�" << endl;
        return;
    }

    char* buffer;
    if (args.size() == 2) {
        count = args[1].size();
        buffer = new char[count + 1]; // ��������β�� ���ǲ�д���ļ�
        strcpy(buffer, args[1].c_str());
    }
    else{
        if (str_set.count(args[2]) == 0) {
            cout << "������ַ��������ڣ�����ͨ��fread����" << endl;
            return;
        }
        string str = str_set[args[2]];
        count = str.size();
        buffer = new char[count + 1]; // ��������β�� ���ǲ�д���ļ�
        strcpy(buffer, str.c_str());
    }

    Sys_Write(fd, (unsigned char *)buffer, count);

    delete[] buffer;
}


void Command_flseek::execute(vector<string> args)
{
    /* ������ fd offset �� ģʽ */
    if (args.size() != 3) {
        help();
        return;
    }

    int fd, offset;
    istringstream ss1 = istringstream(args[0]);
    ss1 >> fd;
    if (ss1.fail()) {
        cout << "�ļ�������fd�Ƿ�" << endl;
        return;
    }
    istringstream ss2 = istringstream(args[1]);
    ss2 >> offset;
    if (ss2.fail()) {
        cout << "�����ֽ����Ƿ�" << endl;
        return;
    }

    int mode;
    if (args[2] == "-b")
        mode = 0;
    else if (args[2] == "-c")
        mode = 1;
    else if (args[2] == "-e")
        mode = 2;
    else {
        help();
        return;
    }

    Sys_Seek(fd, offset, mode);
}


void _traverse_tree(vector<bool> pre_print, string path)
{
    int file = Sys_Open(path.c_str(), File::FREAD);

    unsigned char buffer[sizeof(DirectoryEntry)];
    DirectoryEntry temp;

    vector<string> files;

    while (Sys_Read(file, buffer, sizeof(DirectoryEntry)))
    {
        memcpy(&temp, buffer, sizeof(DirectoryEntry));
        if (temp.m_ino == 0)
            continue;
        else if (temp.m_name[0] == '.')
            continue;
        files.push_back(temp.m_name);
    }

    for (int i = 0; i < files.size(); i++) {
        string file = files[i];
        DiskInode inode;
        for (auto pre : pre_print) {
            cout << (pre ? "��" : " ") << "  ";
        }
        cout << ((i == files.size()-1) ? "����" : "����") << file << endl;
        Sys_Stat((path + "/" + file).c_str(), &inode);
        if ((inode.d_mode & Inode::IFMT) == Inode::IFDIR) {
            vector<bool> p = pre_print;
            p.push_back(i != files.size() - 1);
            _traverse_tree(p, path + "/" + file);
        }
    }

    Sys_Close(file);
}

void Command_tree::execute(vector<string> args)
{
    string path = ".";
    User& u = OSKernel::Instance().GetUser();
    
    if (args.size() > 1) {
        help();
        return;
    }
    else if (args.size() == 1) {
        path = args[0];
    }

    DiskInode inode;
    Sys_Stat(path.c_str(), &inode);
    if (u.u_error) {
        return;
    }
    else if ((inode.d_mode & Inode::IFMT) != Inode::IFDIR) {
        cout << "����·�������ļ��У�" << endl;
        return;
    }

    cout << path << endl;
    _traverse_tree({}, path);

    if (!u.u_error)
        cout << endl;
}
