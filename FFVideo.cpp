//
// Created by sigsegv on 10/13/24.
//

#include "FFVideo.h"
#include "ff/FFCtx.h"
#include <filesystem>

FFVMediaType FFStream::GetMediaType() const {
    switch (stream->GetCodecType()) {
        case FFMediaType::VIDEO:
            return FFVMediaType::VIDEO;
        case FFMediaType::AUDIO:
            return FFVMediaType::AUDIO;
        default:
            return FFVMediaType::UNKNOWN;
    }
}

FFVVideoData FFStream::GetVideoData() const {
    auto vd = stream->GetVideoData();
    return {.width = vd.width, .height = vd.height};
}

FFVAudioData FFStream::GetAudioData() const {
    auto ad = stream->GetAudioData();
    return {.sampleRate = ad.sampleRate, .channels = ad.channels};
}

FFVideo::FFVideo(const std::string &rootPath, const std::string &relPath) : path() {
    {
        std::filesystem::path p = rootPath;
        p = p / relPath;
        path = p;
    }
    ffCtx = FFCtx::Create();
    ffCtx->OpenFile(path);
}

std::string FFVideo::GetFormatLongName() const {
    return ffCtx->GetFormatLongName();
}

long long FFVideo::GetDuration() const {
    return ffCtx->GetDuration();
}

std::vector<FFStream> FFVideo::GetStreams() const {
    std::vector<FFStream> vec{};
    auto streams = ffCtx->GetStreams();
    vec.reserve(streams.size());
    for (const auto &stream : streams) {
        vec.emplace_back(stream);
    }
    return vec;
}
