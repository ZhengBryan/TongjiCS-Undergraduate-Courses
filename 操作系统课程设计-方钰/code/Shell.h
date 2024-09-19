#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "OSKernel.h"


class Command {
public:
	string name;
	string description;
	vector<string> usage;

	Command(string n, string d, vector<string> u) :name(n), description(d), usage(u) {};
	virtual ~Command() {};
	virtual void execute(vector<string>) = 0;
	void help();
};


class Shell {
	static const int NCOMMAND = 100;
	Command* commands[NCOMMAND] = {};	/* �������command */
public:
	Shell();
	~Shell();
	void interface();
private:
	void handleUserError();
};


/* ����Command */

class Command_ls: public Command {
public:
	Command_ls() :Command("ls", "�г�·���������ļ�", { "ls (current path)", "ls <path>", "-a ȫ��չʾ"}) {};
	void execute(vector<string>);
};

class Command_cd : public Command {
public:
	Command_cd() :Command("cd", "�л���ǰĿ¼", { "cd <path>" }) {};
	void execute(vector<string>);
};

class Command_cp : public Command {
public:
	Command_cp() :Command("cp", "�����ļ�(֧���ļ�ϵͳ���⸴��)", { "cp <src> <dst>", "Ϊ�����֣������ⲿ·��ǰ��'$'"}) {};
	void execute(vector<string>);
};

class Command_mv : public Command {
public:
	Command_mv() :Command("mv", "�ƶ��ļ�", { "mv <old_path <new_path>" }) {};
	void execute(vector<string>);
};

class Command_mkdir : public Command {
public:
	Command_mkdir() :Command("mkdir", "�½�Ŀ¼��", { "mkdir <path>" }) {};
	void execute(vector<string>);
};

class Command_fformat : public Command{
public:
	Command_fformat() :Command("fformat", "��ʽ������", { "fformat" }) {};
	void execute(vector<string>);
};

class Command_fcreat : public Command {
public:
	Command_fcreat() :Command("fcreat", "�½��ļ�", { "fcreat <path>", "�����ļ�������"}) {};
	void execute(vector<string>);
};

class Command_fdelete : public Command {
public:
	Command_fdelete() :Command("fdelete", "ɾ���ļ�", { "fdelete <path>" }) {};
	void execute(vector<string>);
};

class Command_fopen : public Command {
public:
	Command_fopen() :Command("fopen", "���ļ�", { "fopen <path> -<mode>", "-r ��ģʽ", "-w дģʽ", "�����ļ�������"}) {};
	void execute(vector<string>);
};

class Command_fclose : public Command {
public:
	Command_fclose() :Command("fclose", "�ر��ļ�", { "fclose <fd>"}) {};
	void execute(vector<string>);
};

class Command_fread : public Command {
public:
	Command_fread() :Command("fread", "���ļ�", {"fread <fd> <count> [>> <str> (д�뵽�ַ�����)]"}) {};
	void execute(vector<string>);
};

class Command_fwrite : public Command {
public:
	Command_fwrite() :Command("fwrite", "д�ļ�", { "fwrite <fd> <str>\t�ӱ�׼������д��", "fwrite <fd> << <str>\t���ַ�����д��"}) {};
	void execute(vector<string>);
};

class Command_flseek : public Command {
public:
	Command_flseek() :Command("flseek", "�����ļ���дָ��", { "flseek <fd> <offset> -<mode>", "-b ����ļ�ͷ", "-c ��Ե�ǰλ��", "-e ����ļ�β"}) {};
	void execute(vector<string>);
};

class Command_tree : public Command {
public:
	Command_tree() :Command("tree", "չʾĿ¼��", { "tree <path>" }) {};
	void execute(vector<string>);
};

