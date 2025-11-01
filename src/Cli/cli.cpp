#include "cli.h"
#include "parser.h"
#include "file_manager.h"
#include <memory>
#include <fstream>

CLI::CLI() : m_over(true)
{
    m_parser = std::make_unique<Parser>();
}

CLI::~CLI() = default;

void CLI::run()
{
	while (m_over)
	{
		input();
		if (is_empty())
			continue;
		
		parse();
		
		if (!m_cmd.empty())
			m_cmd.clear();
		if (m_input == std::string("exit") || m_input == std::string("quit"))
			m_over = false;
	}
}

void CLI::update()
{

}

void CLI::input()
{
	std::cout << "CL>";
	std::getline(std::cin, m_input);
}

void CLI::parse()
{

	auto command = m_parser->parse(m_input);
	
	if (command) {
		exec(*command);
	} else {
		parseOldStyle();
		valid_cmd();
	}
}

void CLI::parseOldStyle()
{
	size_t size = m_input.size();

	for (size_t i = 0; i < size; i++)
	{
		std::string temp;

		while (i < size && std::isspace(m_input[i]))
			++i;
		if (i == size)
			break ;
		while (i < size && !std::isspace(m_input[i]))
			temp.push_back(m_input[i++]);

		m_cmd.push_back(temp);
	}
}

void CLI::exec()
{

}

CmdType CLI::get_type() const
{
	if (m_cmd[0] == std::string("add"))
		return CmdType::ADD;
	if (m_cmd[0] == std::string("remove"))
		return CmdType::REMOVE;
	if (m_cmd[0] == std::string("help"))
		return CmdType::HELP;

	// TODO
	return CmdType::UNKNOWN;
}

void CLI::valid_cmd()
{
	CmdType type = get_type();
	size_t size = m_cmd.size();

	switch (type)
	{
	case CmdType::ADD:
	{
		if (size < 2)
		{
			std::cout << "CL: 'add' requires an argument\n";
			return;
		}
		if (m_cmd[1] != "slide")
		{
			std::cout << "CL: 'add' command: '" << m_cmd[1] << "' not recognized\n";
			return;
		}
		break;
	}
	case CmdType::REMOVE:
	{
		if (size < 2)
		{
			std::cout << "CL: 'remove' requires an argument\n";
			return;
		}
		if (m_cmd[1] != "slide")
		{
			std::cout << "CL: 'remove' command: '" << m_cmd[1] << "' not recognized\n";
			return;
		}
		break;
	}
	case CmdType::HELP:
	{
		if (size != 1)
		{
			std::cout << "CL: 'help' does not take any arguments\n";
			return;
		}
		break;
	}
	case CmdType::EXPORT:
	{
		std::cout << "CL: Export command (handled by parser)" << std::endl;
		break;
	}
	case CmdType::SHOW:
	{
		std::cout << "CL: Show command (handled by parser)" << std::endl;
		break;
	}
	case CmdType::HISTORY:
	{
		std::cout << "CL: History command (handled by parser)" << std::endl;
		break;
	}
	case CmdType::UNKNOWN:
	{
		std::cout << "CL: command '" << m_cmd[0] << "' not recognized\n";
		break;
	}
	}

}

void CLI::print_cmd()
{
	for (const auto &it : m_cmd)
		std::cout << it << "\n";
}

bool CLI::is_empty()
{
	size_t i = 0;
	while (i < m_input.size() && std::isspace(m_input[i]))
		++i;
	if (i == m_input.size())
		return true;
	return false;
}

void CLI::executeAdd(const AddSlideCommand& cmd)
{
	m_slides.emplace_back(cmd.title, cmd.content);
	std::cout << "CL: Added slide #" << (m_slides.size() - 1) 
			  << " - \"" << cmd.title << "\"" << std::endl;
}

void CLI::executeHelp()
{
	std::cout << "\n=== PowerPoint CLI Commands ===" << std::endl;
	std::cout << "add slide \"title\" \"content\" - Add a new slide" << std::endl;
	std::cout << "remove [slide] N             - Remove slide by index" << std::endl;
	std::cout << "show presentation            - Display all slides" << std::endl;
	std::cout << "history                      - Show slide count and titles" << std::endl;
	std::cout << "clear                        - Clear all slides" << std::endl;
	std::cout << "help                         - Show this help" << std::endl;
	std::cout << "exit / quit                  - Exit the program" << std::endl;
	std::cout << "===============================" << std::endl;
}
