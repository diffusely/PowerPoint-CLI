#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <list>

enum class CmdType{ ADD, REMOVE, HELP, UNKNOWN};

class CLI
{
public:
	CLI();

	void run();
	void update();
	void input();
	void parse();
	void exec();

	CmdType get_type() const;
	void valid_cmd();

	void print_cmd();
	bool is_empty();

private:
	bool m_over;

	std::string m_input;
	std::vector<std::string> m_cmd;
};
