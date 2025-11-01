#include "cli.h"
#include "parser.h"
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

void CLI::executeRemove(const RemoveSlideCommand& cmd)
{
	if (cmd.slideIndex >= 0 && static_cast<size_t>(cmd.slideIndex) < m_slides.size()) {
		std::string title = m_slides[cmd.slideIndex].first;
		m_slides.erase(m_slides.begin() + cmd.slideIndex);
		std::cout << "CL: Removed slide #" << cmd.slideIndex 
				  << " - \"" << title << "\"" << std::endl;
	} else {
		std::cout << "CL: Invalid slide index: " << cmd.slideIndex 
				  << " (0-" << (m_slides.size() - 1) << ")" << std::endl;
	}
}

void CLI::executeExport(const ExportCommand& cmd)
{
	std::ofstream file(cmd.filename);
	if (!file.is_open()) {
		std::cout << "CL: Failed to create file: " << cmd.filename << std::endl;
		return;
	}
	
	if (cmd.format == "html") {
		file << "<!DOCTYPE html><html><head><title>PowerPoint CLI</title>";
		file << "<style>.slide{border:1px solid #ccc;margin:20px;padding:15px;}";
		file << ".title{font-size:18px;font-weight:bold;}</style>";
		file << "</head><body><h1>Presentation</h1>";
		
		for (size_t i = 0; i < m_slides.size(); ++i) {
			file << "<div class='slide'><div class='title'>" << m_slides[i].first << "</div>";
			file << "<div>" << m_slides[i].second << "</div></div>";
		}
		file << "</body></html>";
	} else {
		file << "PowerPoint CLI Presentation\n==========================\n\n";
		for (size_t i = 0; i < m_slides.size(); ++i) {
			file << "Slide " << (i+1) << ": " << m_slides[i].first << "\n";
			file << m_slides[i].second << "\n\n";
		}
	}
	
	file.close();
	std::cout << "CL: Exported " << m_slides.size() << " slides to " << cmd.filename << std::endl;
}

void CLI::executeShow(const ShowCommand& cmd)
{
	if (cmd.target == "presentation") {
		std::cout << "\n=== PowerPoint CLI Presentation ===" << std::endl;
		if (m_slides.empty()) {
			std::cout << "(No slides in presentation)" << std::endl;
		} else {
			for (size_t i = 0; i < m_slides.size(); ++i) {
				std::cout << "\n--- Slide " << (i+1) << " ---" << std::endl;
				std::cout << "Title: " << m_slides[i].first << std::endl;
				std::cout << "Content: " << m_slides[i].second << std::endl;
			}
		}
		std::cout << "==================================\n" << std::endl;
	} else {
		std::cout << "CL: Unknown show target: " << cmd.target << std::endl;
	}
}

void CLI::exec(const Command& command)
{
	std::string cmdName = command.name;
	
	if (cmdName == "add") {
		const auto* addCmd = dynamic_cast<const AddSlideCommand*>(&command);
		if (addCmd) executeAdd(*addCmd);
	} else if (cmdName == "remove") {
		const auto* removeCmd = dynamic_cast<const RemoveSlideCommand*>(&command);
		if (removeCmd) executeRemove(*removeCmd);
	} else if (cmdName == "export") {
		const auto* exportCmd = dynamic_cast<const ExportCommand*>(&command);
		if (exportCmd) executeExport(*exportCmd);
	} else if (cmdName == "show") {
		const auto* showCmd = dynamic_cast<const ShowCommand*>(&command);
		if (showCmd) executeShow(*showCmd);
	} else if (cmdName == "help") {
		executeHelp();
	} else if (cmdName == "exit" || cmdName == "quit") {
		std::cout << "CL: Goodbye!" << std::endl;
		m_over = false;
	} else if (cmdName == "clear") {
		m_slides.clear();
		std::cout << "CL: Presentation cleared" << std::endl;
	} else if (cmdName == "history") {
		std::cout << "CL: Slides in presentation: " << m_slides.size() << std::endl;
		for (size_t i = 0; i < m_slides.size(); ++i) {
			std::cout << "  [" << i << "] \"" << m_slides[i].first << "\"" << std::endl;
		}
	}
}

void CLI::executeHelp()
{
	std::cout << "\n=== PowerPoint CLI Commands ===" << std::endl;
	std::cout << "add slide \"title\" \"content\" - Add a new slide" << std::endl;
	std::cout << "remove [slide] N             - Remove slide by index" << std::endl;
	std::cout << "show presentation            - Display all slides" << std::endl;
	std::cout << "export \"file.html\"           - Export to HTML or text" << std::endl;
	std::cout << "history                      - Show slide count" << std::endl;
	std::cout << "clear                        - Clear slides from memory" << std::endl;
	std::cout << "help                         - Show this help" << std::endl;
	std::cout << "exit / quit                  - Exit the program" << std::endl;
	std::cout << "===============================" << std::endl;
}


