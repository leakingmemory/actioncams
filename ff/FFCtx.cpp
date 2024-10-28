//
// Created by sigsegv on 10/13/24.
//

#include "FFCtx.h"
extern "C" {
#include <libavformat/avformat.h>
}

FFCtxStream::FFCtxStream(std::shared_ptr<const FFCtx> ctx, int stream) {
    avStream = ctx ? ctx->av_ctx->streams[stream] : nullptr;
}

const AVCodec *FFCtxStream::FindDecoder() const {
    if (avStream != nullptr) {
        if (avStream->codecpar != nullptr) {
            return avcodec_find_decoder(avStream->codecpar->codec_id);
        }
    }
    return nullptr;
}

FFMediaType FFCtxStream::GetCodecType() const {
    if (avStream != nullptr) {
        if (avStream->codecpar != nullptr) {
            switch (avStream->codecpar->codec_type) {
                case AVMEDIA_TYPE_VIDEO:
                    return FFMediaType::VIDEO;
                case AVMEDIA_TYPE_AUDIO:
                    return FFMediaType::AUDIO;
            }
        }
    }
    return FFMediaType::UNKNOWN;
}

FFVideoData FFCtxStream::GetVideoData() const {
    if (avStream != nullptr) {
        if (avStream->codecpar != nullptr) {
            if (avStream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
                return {.width = (uint16_t) avStream->codecpar->width, .height = (uint16_t) avStream->codecpar->height};
            }
        }
    }
    return {};
}

FFAudioData FFCtxStream::GetAudioData() const {
    if (avStream != nullptr) {
        if (avStream->codecpar != nullptr) {
            if (avStream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
                return {.sampleRate = (uint32_t) avStream->codecpar->sample_rate, .channels = (uint8_t) avStream->codecpar->ch_layout.nb_channels};
            }
        }
    }
    return {};
}

class FFCtxException : public std::exception {
public:
    FFCtxException();
    const char * what() const noexcept override;
};

FFCtxException::FFCtxException() : std::exception() {}

const char *FFCtxException::what() const noexcept {
    return "FFCtxException";
}

FFCtx::FFCtx() {
    av_ctx = avformat_alloc_context();
}

class FFCtxImpl : public FFCtx {
};

std::shared_ptr<FFCtx> FFCtx::Create() {
    return std::make_shared<FFCtxImpl>();
}

FFCtx::~FFCtx() {
    avformat_free_context(av_ctx);
}

void FFCtx::OpenFile(const std::string &filename) {
    if (opened) {
        throw FFCtxException();
    }
    opened = true;
    if (avformat_open_input(&av_ctx, filename.c_str(), NULL, NULL) != 0) {
        throw FFCtxException();
    }
    avformat_find_stream_info(av_ctx, NULL);
}

std::string FFCtx::GetFormatLongName() const {
    if (av_ctx->iformat != NULL) {
        if (av_ctx->iformat->long_name != NULL) {
            std::string fn{av_ctx->iformat->long_name};
            return fn;
        }
    }
    return {};
}

long long FFCtx::GetDuration() const {
    return av_ctx->duration;
}

std::vector<std::shared_ptr<FFCtxStream>> FFCtx::GetStreams() const {
    std::vector<std::shared_ptr<FFCtxStream>> vec{};
    vec.reserve(av_ctx->nb_streams);
    std::shared_ptr<const FFCtx> slf = shared_from_this();
    for (typeof(av_ctx->nb_streams) i = 0; i < av_ctx->nb_streams; i++) {
        vec.emplace_back(std::make_shared<FFCtxStream>(slf, i));
    }
    return vec;
}