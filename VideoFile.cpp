//
// Created by sigsegv on 9/8/24.
//

#include "VideoFile.h"
#include "DateTime.h"
#include <sys/stat.h>
#include <sstream>

class StatException : public std::exception {
private:
    std::string filename;
    std::string error;
public:
    StatException(const std::string &filename) : filename(filename), error() {
        error.append("Stat failed: ");
        error.append(filename);
    }
    const char * what() const noexcept override;
};

const char *StatException::what() const noexcept {
    return error.c_str();
}

VideoFile::VideoFile(const std::filesystem::path &rootDir, const std::filesystem::path &p) : rootDir(rootDir), path(p) {
    struct stat st;
    std::string fullPath = p;
    if (stat(fullPath.c_str(), &st) != 0) {
        throw StatException(fullPath);
    }
    mtime = st.st_mtim.tv_sec;
}

std::string VideoFile::GetRelativeFilename() const {
    std::string fullPath = path;
    std::string rootPath = rootDir;
    if (fullPath.starts_with(rootPath)) {
        auto remaining = fullPath.substr(rootPath.length());
        if (remaining.starts_with("/")) {
            return remaining.substr(1);
        }
    }
    return fullPath;
}

std::string VideoFile::GetModifiedIso8601() const {
    DateTime dateTime{mtime};
    return dateTime.to_iso8601();
}

std::string VideoFile::GetYearString() const {
    DateTime dateTime{mtime};
    std::stringstream sstr{};
    sstr << dateTime.GetYear();
    return sstr.str();
}

std::string VideoFile::GetDateString() const {
    DateTime dateTime{mtime};
    return dateTime.to_dateiso8601();
}

int VideoFile::GetWeekNumber() const {
    DateTime dateTime{mtime};
    return dateTime.GetWeek();
}

void VideoFile::SetGroup(const std::string &group) {
    this->group = group;
}

std::string VideoFile::GetGroup() const {
    return group;
}