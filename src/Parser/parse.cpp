#include "parser.h"
#include <iostream>
#include <algorithm>
#include <sstream>

Parser::Parser() {

}

Parser::~Parser() = default;

std::unique_ptr<Command> Parser::parse(const std::string& input) {
    if (input.empty()) {
        return nullptr;
    }
    

    std::vector<Token> tokens = m_lexer.tokenize(input);
    
    if (tokens.empty()) {
        return nullptr;
    }
    
    return parseTokens(tokens);
}

std::unique_ptr<Command> Parser::parseTokens(const std::vector<Token>& tokens) {
    if (tokens.empty() || !tokens[0].isCommand()) {
        printParseError("Expected command as first token");
        return nullptr;
    }
    
    std::string commandName = tokens[0].getValue();
    std::transform(commandName.begin(), commandName.end(), commandName.begin(), ::tolower);
    
    try {
        if (commandName == "add") {
            return parseAddCommand(tokens);
        } else if (commandName == "remove") {
            return parseRemoveCommand(tokens);
        } else if (commandName == "export") {
            return parseExportCommand(tokens);
        } else if (commandName == "show") {
            return parseShowCommand(tokens);
        } else if (commandName == "help" || commandName == "history" || 
                  commandName == "clear" || commandName == "exit" || commandName == "quit") {

            auto cmd = std::make_unique<Command>(commandName);
            return cmd;
        } else {
            printParseError("Unknown command: " + commandName);
            return nullptr;
        }
    } catch (const std::exception& e) {
        printParseError("Parse error: " + std::string(e.what()));
        return nullptr;
    }
}

std::unique_ptr<AddSlideCommand> Parser::parseAddCommand(const std::vector<Token>& tokens) {
    auto cmd = std::make_unique<AddSlideCommand>();
    
    if (tokens.size() < 2) {
        printParseError("'add' command requires arguments");
        return nullptr;
    }
    
    if (tokens.size() >= 2 && tokens[1].getValue() != "slide") {
        printParseError("'add' command only supports 'slide' argument");
        return nullptr;
    }
    

    size_t argIndex = 2;
    
    if (argIndex < tokens.size()) {
        // Extract title
        if (tokens[argIndex].isString()) {
            cmd->title = extractString(tokens[argIndex]);
        } else {
            cmd->title = tokens[argIndex].getValue();
        }
        argIndex++;
    }
    
    if (argIndex < tokens.size()) {

        if (tokens[argIndex].isString()) {
            cmd->content = extractString(tokens[argIndex]);
        } else {
            std::string content;
            for (size_t i = argIndex; i < tokens.size(); ++i) {
                if (i > argIndex) content += " ";
                content += tokens[i].getValue();
            }
            cmd->content = content;
        }
    }
    

    if (cmd->title.empty()) {
        cmd->title = "New Slide";
    }
    if (cmd->content.empty()) {
        cmd->content = "Slide content";
    }
    
    std::cout << "CL: Parsed ADD command - Title: \"" << cmd->title 
              << "\", Content: \"" << cmd->content << "\"" << std::endl;
    
    return cmd;
}

std::unique_ptr<RemoveSlideCommand> Parser::parseRemoveCommand(const std::vector<Token>& tokens) {
    auto cmd = std::make_unique<RemoveSlideCommand>();
    
    if (tokens.size() < 2) {
        printParseError("'remove' command requires arguments");
        return nullptr;
    }
    
    if (tokens[1].getValue() == "slide") {
        if (tokens.size() >= 3) {
            if (tokens[2].isNumber()) {
                cmd->slideIndex = extractNumber(tokens[2]);
            } else {
                printParseError("'remove slide' requires a number");
                return nullptr;
            }
        } else {
            printParseError("'remove slide' requires slide index");
            return nullptr;
        }
    } else if (tokens[1].isNumber()) {
        // Direct number: remove 5
        cmd->slideIndex = extractNumber(tokens[1]);
    } else {
        printParseError("'remove' command format: 'remove slide N' or 'remove N'");
        return nullptr;
    }
    
    if (!isValidSlideIndex(cmd->slideIndex)) {
        printParseError("Invalid slide index: " + std::to_string(cmd->slideIndex));
        return nullptr;
    }
    
    std::cout << "CL: Parsed REMOVE command - Slide index: " << cmd->slideIndex << std::endl;
    
    return cmd;
}

std::unique_ptr<ExportCommand> Parser::parseExportCommand(const std::vector<Token>& tokens) {
    auto cmd = std::make_unique<ExportCommand>();
    
    if (tokens.size() >= 2) {
        if (tokens[1].isString()) {
            cmd->filename = extractString(tokens[1]);
        } else {
            cmd->filename = tokens[1].getValue();
        }
        
        // Extract format from file extension
        size_t dotPos = cmd->filename.find_last_of('.');
        if (dotPos != std::string::npos) {
            cmd->format = cmd->filename.substr(dotPos + 1);
            std::transform(cmd->format.begin(), cmd->format.end(), cmd->format.begin(), ::tolower);
        }
    } else {
        cmd->filename = "presentation.html";
        cmd->format = "html";
    }
    
    std::cout << "CL: Parsed EXPORT command - File: \"" << cmd->filename 
              << "\", Format: " << cmd->format << std::endl;
    
    return cmd;
}

std::unique_ptr<ShowCommand> Parser::parseShowCommand(const std::vector<Token>& tokens) {
    auto cmd = std::make_unique<ShowCommand>();
    
    if (tokens.size() >= 2) {
        cmd->target = tokens[1].getValue();
    }
    
    std::cout << "CL: Parsed SHOW command - Target: " << cmd->target << std::endl;
    
    return cmd;
}

bool Parser::validateCommand(const Command& cmd) const {
    if (cmd.name.empty()) {
        return false;
    }

    return true;
}

void Parser::printParseError(const std::string& error) const {
    std::cout << "CL: Parse Error - " << error << std::endl;
}

std::string Parser::extractString(const Token& token) const {
    std::string value = token.getValue();
    
    if (value.length() >= 2) {
        if ((value.front() == '"' && value.back() == '"') ||
            (value.front() == '\'' && value.back() == '\'')) {
            return value.substr(1, value.length() - 2);
        }
    }
    
    return value;
}

int Parser::extractNumber(const Token& token) const {
    try {
        return std::stoi(token.getValue());
    } catch (const std::exception&) {
        return -1;
    }
}

bool Parser::isValidSlideIndex(int index) const {
    return index >= 0 && index < 1000;
}

std::vector<Token> Parser::getArgumentTokens(const std::vector<Token>& tokens, size_t startIndex) const {
    std::vector<Token> args;
    
    for (size_t i = startIndex; i < tokens.size(); ++i) {
        args.push_back(tokens[i]);
    }
    
    return args;
}

std::string Parser::joinStringTokens(const std::vector<Token>& tokens, size_t start, size_t end) const {
    std::string result;
    
    for (size_t i = start; i < end && i < tokens.size(); ++i) {
        if (i > start) result += " ";
        result += tokens[i].getValue();
    }
    
    return result;
}
