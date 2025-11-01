#include "file_manager.h"
#include <iostream>
#include <sstream>
#include <filesystem>

SimpleSlide::SimpleSlide(const std::string& t, const std::string& c, size_t idx) 
    : title(t), content(c), index(idx) {}

SimpleFileManager::SimpleFileManager(const std::string& projectName) 
    : m_projectName(projectName)
	{
    m_projectPath = "./" + m_projectName + "_files";
    m_slidesPath = m_projectPath + "/slides";
}

SimpleFileManager::~SimpleFileManager() {}

bool SimpleFileManager::createProject()
{
    try {
        std::filesystem::create_directories(m_projectPath);
        std::filesystem::create_directories(m_slidesPath);
        std::cout << "Project created: " << m_projectPath << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cout << "Error creating project: " << e.what() << std::endl;
        return false;
    }
}

bool SimpleFileManager::saveSlide(const SimpleSlide& slide)
{
    if (writeSlideFile(slide)) {
        std::cout << "Saved slide " << slide.index << ": " << slide.title << std::endl;
        return true;
    }
    return false;
}

bool SimpleFileManager::removeSlide(size_t index)
{
    std::string filename = getSlideFilename(index);
    try {
        if (std::filesystem::exists(filename)) {
            std::filesystem::remove(filename);
            std::cout << "Removed slide " << index << std::endl;
            return true;
        }
    } catch (const std::exception& e) {
        std::cout << "Error removing slide: " << e.what() << std::endl;
    }
    return false;
}

std::vector<SimpleSlide> SimpleFileManager::loadAllSlides()
{
    std::vector<SimpleSlide> slides;
    
    if (!std::filesystem::exists(m_slidesPath)) {
        return slides;
    }
    
    try {
        for (const auto& entry : std::filesystem::directory_iterator(m_slidesPath)) {
            if (entry.path().extension() == ".txt") {
                SimpleSlide* slide = readSlideFile(entry.path().string());
                if (slide) {
                    slides.push_back(*slide);
                    delete slide;
                }
            }
        }
    } catch (const std::exception& e) {
        std::cout << "Error loading slides: " << e.what() << std::endl;
    }
    
    std::cout << "Loaded " << slides.size() << " slides" << std::endl;
    return slides;
}

bool SimpleFileManager::exportToHTML(const std::vector<SimpleSlide>& slides, const std::string& filename)
{
    std::ofstream file(m_projectPath + "/" + filename);
    if (!file.is_open()) return false;
    
    file << "<!DOCTYPE html><html><head><title>" << m_projectName << "</title>";
    file << "<style>body{font-family:Arial;margin:20px;}";
    file << ".slide{border:1px solid black;margin:10px;padding:15px;}";
    file << ".title{font-size:18px;font-weight:bold;}</style>";
    file << "</head><body><h1>" << m_projectName << "</h1>";
    
    for (const auto& slide : slides) {
        file << "<div class='slide'>";
        file << "<div class='title'>" << slide.title << "</div>";
        file << "<div>" << slide.content << "</div>";
        file << "</div>";
    }
    
    file << "</body></html>";
    file.close();
    std::cout << "Exported to HTML: " << filename << std::endl;
    return true;
}

bool SimpleFileManager::exportToText(const std::vector<SimpleSlide>& slides, const std::string& filename)
{
    std::ofstream file(m_projectPath + "/" + filename);
    if (!file.is_open()) return false;
    
    file << m_projectName << " - Text Export" << std::endl;
    file << "========================" << std::endl << std::endl;
    
    for (const auto& slide : slides) {
        file << "Slide " << (slide.index + 1) << ": " << slide.title << std::endl;
        file << slide.content << std::endl << std::endl;
    }
    
    file.close();
    std::cout << "Exported to text: " << filename << std::endl;
    return true;
}

std::string SimpleFileManager::getProjectPath() const
{
    return m_projectPath;
}

std::string SimpleFileManager::getSlidesPath() const
{
    return m_slidesPath;
}

bool SimpleFileManager::projectExists() const
{
    return std::filesystem::exists(m_projectPath);
}

std::string SimpleFileManager::getSlideFilename(size_t index) const
{
    return m_slidesPath + "/slide_" + std::to_string(index) + ".txt";
}

bool SimpleFileManager::writeSlideFile(const SimpleSlide& slide)
{
    std::string filename = getSlideFilename(slide.index);
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    
    file << "TITLE:" << slide.title << std::endl;
    file << "CONTENT:" << slide.content << std::endl;
    file << "INDEX:" << slide.index << std::endl;
    
    file.close();
    return true;
}

SimpleSlide* SimpleFileManager::readSlideFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) return nullptr;
    
    std::string line, title, content;
    size_t index = 0;
    
    while (std::getline(file, line)) {
        if (line.find("TITLE:") == 0) {
            title = line.substr(6);
        } else if (line.find("CONTENT:") == 0) {
            content = line.substr(8);
        } else if (line.find("INDEX:") == 0) {
            index = std::stoul(line.substr(6));
        }
    }
    
    file.close();
    
    if (!title.empty()) {
        return new SimpleSlide(title, content, index);
    }
    return nullptr;
}
