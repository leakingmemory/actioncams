//
// Created by sigsegv on 10/13/24.
//

#ifndef ACTIONCAMS_FFVIDEO_H
#define ACTIONCAMS_FFVIDEO_H

#include <string>
#include <memory>
#include <vector>

class FFCtx;
class FFCtxStream;

enum class FFVMediaType {
    UNKNOWN,
    VIDEO,
    AUDIO
};

struct FFVVideoData {
    uint16_t width;
    uint16_t height;
};

struct FFVAudioData {
    uint32_t sampleRate;
    uint8_t channels;
};

class FFStream {
private:
    std::shared_ptr<FFCtxStream> stream;
public:
    FFStream(std::shared_ptr<FFCtxStream> stream) : stream(stream) {}
    FFVMediaType GetMediaType() const;
    FFVVideoData GetVideoData() const;
    FFVAudioData GetAudioData() const;
};

class FFVideo {
private:
    std::string path;
    std::shared_ptr<FFCtx> ffCtx{};
public:
    FFVideo(const std::string &rootPath, const std::string &relPath);
    std::string GetFormatLongName() const;
    long long int GetDuration() const;
    std::vector<FFStream> GetStreams() const;
};


#endif //ACTIONCAMS_FFVIDEO_H
