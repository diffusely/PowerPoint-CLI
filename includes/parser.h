#pragma once

#include <vector>
#include <string>
#include <memory>
#include "token.h"
#include "lexer.h"

struct Command {
    std::string name;
    std::vector<std::string> args;
    
    Command(const std::string& cmdName) : name(cmdName) {}
    virtual ~Command() = default;
};

struct AddSlideCommand : public Command {
    std::string title;
    std::string content;
    int width = 800;
    int height = 600;
    
    AddSlideCommand() : Command("add") {}
};

struct RemoveSlideCommand : public Command {
    int slideIndex = -1;
    
    RemoveSlideCommand() : Command("remove") {}
};

struct ExportCommand : public Command {
    std::string filename;
    std::string format = "html";
    
    ExportCommand() : Command("export") {}
};

struct ShowCommand : public Command {
    std::string target = "presentation";
    
    ShowCommand() : Command("show") {}
};

class Parser
{
public:
    Parser();
    ~Parser();

    std::unique_ptr<Command> parse(const std::string& input);
    std::unique_ptr<Command> parseTokens(const std::vector<Token>& tokens);
    
 
    std::unique_ptr<AddSlideCommand> parseAddCommand(const std::vector<Token>& tokens);
    std::unique_ptr<RemoveSlideCommand> parseRemoveCommand(const std::vector<Token>& tokens);
    std::unique_ptr<ExportCommand> parseExportCommand(const std::vector<Token>& tokens);
    std::unique_ptr<ShowCommand> parseShowCommand(const std::vector<Token>& tokens);
    

    bool validateCommand(const Command& cmd) const;
    void printParseError(const std::string& error) const;
    
 
    std::string extractString(const Token& token) const;
    int extractNumber(const Token& token) const;
    bool isValidSlideIndex(int index) const;

private:
    Lexer m_lexer;
    

    std::vector<Token> getArgumentTokens(const std::vector<Token>& tokens, size_t startIndex) const;
    std::string joinStringTokens(const std::vector<Token>& tokens, size_t start, size_t end) const;
};