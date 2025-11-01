#pragma once

#include <string>
#include <vector>
#include <fstream>

struct SimpleSlide
{
    std::string title;
    std::string content;
    size_t index;
    
    SimpleSlide(const std::string& t, const std::string& c, size_t idx);
};

class SimpleFileManager
{
public:
    SimpleFileManager(const std::string& projectName = "presentation");
    ~SimpleFileManager();
    
    bool createProject();
    bool saveSlide(const SimpleSlide& slide);
    bool removeSlide(size_t index);
    std::vector<SimpleSlide> loadAllSlides();
    
    bool exportToHTML(const std::vector<SimpleSlide>& slides, const std::string& filename);
    bool exportToText(const std::vector<SimpleSlide>& slides, const std::string& filename);
    
    std::string getProjectPath() const;
    std::string getSlidesPath() const;
    bool projectExists() const;
    
private:
    std::string m_projectName;
    std::string m_projectPath;
    std::string m_slidesPath;
    
    std::string getSlideFilename(size_t index) const;
    bool writeSlideFile(const SimpleSlide& slide);
    SimpleSlide* readSlideFile(const std::string& filename);
};
