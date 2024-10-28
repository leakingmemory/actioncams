//
// Created by sigsegv on 9/8/24.
//

#include "VideoDirectory.h"
#include "VideoFile.h"
#include <filesystem>
#include <vector>
#include <iostream>

bool IsVideoFile(const std::filesystem::path &path) {
    // TODO - Needs replacement
    std::string filename = path.filename();
    std::transform(filename.cbegin(), filename.cend(), filename.begin(), [] (char ch) {return std::tolower(ch);});
    return (filename.ends_with(".mp4") || filename.ends_with(".mov") || filename.ends_with(".avi") || filename.ends_with(".mts"));
}

void CollectFiles(std::vector<std::filesystem::path> &files, const std::filesystem::path &dir) {
    std::filesystem::directory_iterator iterator{dir};
    for (const auto &item : std::filesystem::directory_iterator(dir)) {
        std::filesystem::path path = item.path();
        std::string filename = path.filename();
        if (filename == ".." || filename == ".") {
            continue;
        }
        if (is_directory(item)) {
            CollectFiles(files, item);
            continue;
        }
        if (IsVideoFile(item)) {
            files.emplace_back(item);
        }
    }
}

VideoDirectory::VideoDirectory(const std::string &path) {
    std::filesystem::path dir{path};
    std::vector<std::shared_ptr<VideoFile>> videoFiles{};
    {
        std::vector<std::filesystem::path> videoFilePaths{};
        CollectFiles(videoFilePaths, dir);
        for (const auto &vf : videoFilePaths) {
            videoFiles.emplace_back(std::make_shared<VideoFile>(dir, vf));
        }
    }
    for (const auto &vf : videoFiles) {
        auto year = vf->GetYearString();
        auto yearIterator = yearWeekDateFile.find(year);
        if (yearIterator == yearWeekDateFile.end()) {
            yearIterator = yearWeekDateFile.insert_or_assign(year, std::map<int,std::map<std::string,std::vector<std::shared_ptr<VideoFile>>>>()).first;
        }
        auto week = vf->GetWeekNumber();
        auto weekIterator = yearIterator->second.find(week);
        if (weekIterator == yearIterator->second.end()) {
            weekIterator = yearIterator->second.insert_or_assign(week, std::map<std::string,std::vector<std::shared_ptr<VideoFile>>>()).first;
        }
        auto date = vf->GetDateString();
        auto dateIterator = weekIterator->second.find(date);
        if (dateIterator == weekIterator->second.end()) {
            dateIterator = weekIterator->second.insert_or_assign(date, std::vector<std::shared_ptr<VideoFile>>()).first;
        }
        dateIterator->second.emplace_back(vf);
    }
}

std::vector<std::string> VideoDirectory::GetYears() const {
    std::vector<std::string> years{};
    years.reserve(yearWeekDateFile.size());
    for (auto &pair : yearWeekDateFile) {
        years.emplace_back(pair.first);
    }
    return years;
}

std::vector<int> VideoDirectory::GetWeeks(const std::string &year) const {
    auto yearIterator = yearWeekDateFile.find(year);
    if (yearIterator == yearWeekDateFile.end()) {
        return {};
    }
    std::vector<int> weeks{};
    weeks.reserve(yearIterator->second.size());
    for (auto &pair : yearIterator->second) {
        weeks.emplace_back(pair.first);
    }
    return weeks;
}

std::vector<std::string> VideoDirectory::GetDates(const std::string &year, int week) const {
    auto yearIterator = yearWeekDateFile.find(year);
    if (yearIterator == yearWeekDateFile.end()) {
        return {};
    }
    auto weekIterator = yearIterator->second.find(week);
    if (weekIterator == yearIterator->second.end()) {
        return {};
    }
    std::vector<std::string> dates{};
    dates.reserve(weekIterator->second.size());
    for (auto &pair : weekIterator->second) {
        dates.emplace_back(pair.first);
    }
    return dates;
}

std::vector<std::shared_ptr<VideoFile>> VideoDirectory::GetVideoFiles(const std::string &year, int week, const std::string &date) const {
    auto weeks = yearWeekDateFile.find(year);
    if (weeks == yearWeekDateFile.end()) {
        return {};
    }
    auto dates = weeks->second.find(week);
    if (dates == weeks->second.end()) {
        return {};
    }
    auto files = dates->second.find(date);
    if (files == dates->second.end()) {
        return {};
    }
    return files->second;
}
