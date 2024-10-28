//
// Created by sigsegv on 9/8/24.
//

#ifndef ACTIONCAMS_VIDEOFILE_H
#define ACTIONCAMS_VIDEOFILE_H

#include <time.h>
#include <filesystem>

class VideoFile {
private:
    std::filesystem::path rootDir;
    std::filesystem::path path;
    time_t mtime;
    std::string group;
public:
    VideoFile(const std::filesystem::path &rootdir, const std::filesystem::path &);
    [[nodiscard]] std::string GetRelativeFilename() const;
    [[nodiscard]] std::string GetModifiedIso8601() const;
    [[nodiscard]] std::string GetYearString() const;
    [[nodiscard]] std::string GetDateString() const;
    [[nodiscard]] int GetWeekNumber() const;
    void SetGroup(const std::string &group);
    [[nodiscard]] std::string GetGroup() const;
};


#endif //ACTIONCAMS_VIDEOFILE_H
