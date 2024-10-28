//
// Created by sigsegv on 9/8/24.
//

#ifndef ACTIONCAMS_VIDEODIRECTORY_H
#define ACTIONCAMS_VIDEODIRECTORY_H

#include <string>
#include <memory>
#include <map>
#include <vector>

class VideoFile;

class VideoDirectory {
private:
    std::map<std::string,std::map<int,std::map<std::string,std::vector<std::shared_ptr<VideoFile>>>>> yearWeekDateFile{};
public:
    VideoDirectory() = delete;
    VideoDirectory(const std::string &path);
    [[nodiscard]] std::vector<std::string> GetYears() const;
    [[nodiscard]] std::vector<int> GetWeeks(const std::string &year) const;
    [[nodiscard]] std::vector<std::string> GetDates(const std::string &year, int week) const;
    [[nodiscard]] std::vector<std::shared_ptr<VideoFile>> GetVideoFiles(const std::string &year, int week, const std::string &date) const;
};


#endif //ACTIONCAMS_VIDEODIRECTORY_H
