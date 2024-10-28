//
// Created by sigsegv on 10/13/24.
//

#ifndef ACTIONCAMS_FFCTX_H
#define ACTIONCAMS_FFCTX_H

#include <string>
#include <memory>
#include <vector>

struct AVFormatContext;
struct AVStream;
struct AVCodec;

class FFCtx;

enum class FFMediaType {
    UNKNOWN,
    VIDEO,
    AUDIO
};

struct FFVideoData {
    uint16_t width;
    uint16_t height;
};

struct FFAudioData {
    uint32_t sampleRate;
    uint8_t channels;
};

class FFCtxStream {
private:
    std::shared_ptr<const FFCtx> ctx;
    AVStream *avStream;
public:
    FFCtxStream(std::shared_ptr<const FFCtx> ctx, int stream);
    FFCtxStream(const FFCtxStream &) = delete;
    FFCtxStream(FFCtxStream &&) = delete;
    FFCtxStream &operator = (const FFCtxStream &) = delete;
    FFCtxStream &operator = (FFCtxStream &&) = delete;
    const AVCodec *FindDecoder() const;
    FFMediaType GetCodecType() const;
    FFVideoData GetVideoData() const;
    FFAudioData GetAudioData() const;
};

class FFCtx : public std::enable_shared_from_this<FFCtx> {
    friend FFCtxStream;
private:
    AVFormatContext *av_ctx;
    bool opened{false};
protected:
    FFCtx();
public:
    static std::shared_ptr<FFCtx> Create();
    FFCtx(const FFCtx &) = delete;
    FFCtx(FFCtx &&) = delete;
    FFCtx &operator =(const FFCtx &) = delete;
    FFCtx &operator =(FFCtx &&) = delete;
    ~FFCtx();
    void OpenFile(const std::string &);
    std::string GetFormatLongName() const;
    long long int GetDuration() const;
    std::vector<std::shared_ptr<FFCtxStream>> GetStreams() const;
};


#endif //ACTIONCAMS_FFCTX_H
