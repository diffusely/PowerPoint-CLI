#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <memory>

class Parser;
struct Command;
struct AddSlideCommand;
struct RemoveSlideCommand;
struct ExportCommand;
struct ShowCommand;

enum class CmdType{ ADD, REMOVE, HELP, UNKNOWN, HISTORY, EXPORT, SHOW};

class CLI
{
public:
	CLI();
	~CLI();

	void run();
	void update();
	void input();
	void parse();
	void parseOldStyle();
	void exec();
	void exec(const Command& command);

	CmdType get_type() const;
	void valid_cmd();

	void print_cmd();
	bool is_empty();

protected:
	bool m_over;
	std::string m_input;
	std::vector<std::string> m_cmd;
	std::unique_ptr<Parser> m_parser;
	
	void executeAdd(const AddSlideCommand& cmd);
	void executeRemove(const RemoveSlideCommand& cmd);
	void executeExport(const ExportCommand& cmd);
	void executeShow(const ShowCommand& cmd);
	void executeHelp();
	
	std::vector<std::pair<std::string, std::string>> m_slides;
};
