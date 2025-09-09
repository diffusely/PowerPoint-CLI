#include "../../includes/cli.h"


CLI::CLI() : m_over(true)
{

}

void CLI::run()
{
	while (m_over)
	{
		input();
		if (is_empty())
			continue;
		parse();
		valid_cmd();
		//print_cmd();
		

		if (!m_cmd.empty())
			m_cmd.clear();
		if (m_input == std::string("exit"))
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
	// add slide -a 500 -b 400
	// remove slide
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
