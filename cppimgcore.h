#ifndef INC_CPPIMGCORE_H_
#define INC_CPPIMGCORE_H_
/**
@file cppimgcore.h
@author t-sakai
@date 2018/09/13 create

USAGE:
Put '#define CPPIMG_IMPLEMENTATION' before including this file to create the implementation.
*/
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <utility>

namespace cppimg
{
#ifndef CPPIMG_TYPES
#define CPPIMG_TYPES
    typedef int8_t s8;
    typedef int16_t s16;
    typedef int32_t s32;
    typedef int64_t s64;

    typedef uint8_t u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef uint64_t u64;

    typedef float f32;
    typedef double f64;

    typedef char Char;

    typedef ::size_t size_t;
#endif //CPPIMG_TYPES

#ifndef CPPIMG_NULL
#ifdef __cplusplus
#   if 201103L<=__cplusplus || 1900<=_MSC_VER
#       define CPPIMG_CPP11 1
#   endif
#endif

#ifdef __cplusplus
#   ifdef CPPIMG_CPP11
#       define CPPIMG_NULL nullptr
#   else
#       define CPPIMG_NULL 0
#   endif
#else
#   define CPPIMG_NULL (void*)0
#endif
#endif

#ifndef CPPIMG_FUNC
#define CPPIMG_FUNC
    using std::move;

#ifndef CPPIMG_ASSERT
#define CPPIMG_ASSERT(exp) assert(exp)
#endif

#ifdef _MSC_VER

#ifndef CPPIMG_OFF_T
#define CPPIMG_OFF_T
    typedef s64 off_t;
#endif

#ifndef CPPIMG_FSEEK
#define CPPIMG_FSEEK(f,p,o) _fseeki64((f),(p),(o))
#endif

#ifndef CPPIMG_FTELL
#define CPPIMG_FTELL(f) _ftelli64((f))
#endif

#ifndef CPPIMG_SPRINTF
#define CPPIMG_SPRINTF(b,f, ...) sprintf_s(b, f, __VA_ARGS__)
#endif

    //----------------------------------------------------------------
    inline s64 CPPIMG_FSIZE(FILE* file)
    {
        CPPIMG_ASSERT(CPPIMG_NULL != file);
        struct _stat64 stat;
        return (0 == _fstat64(_fileno(file), &stat))? stat.st_size : 0;
    }

    inline FILE* CPPIMG_FOPEN(const Char* filepath, const Char* mode)
    {
        FILE* file = CPPIMG_NULL;
        return 0 == ::fopen_s(&file, filepath, mode) ? file : CPPIMG_NULL;
    }

    inline void CPPIMG_FCLOSE(FILE*& file)
    {
        if(CPPIMG_NULL != file){
            fclose(file);
            file = CPPIMG_NULL;
        }
    }

#else
#ifndef CPPIMG_OFF_T
#define CPPIMG_OFF_T
    typedef s64 off_t;
#endif

#ifndef CPPIMG_FSEEK
#define CPPIMG_FSEEK(f,p,o) fseeko64((f),(p),(o))
#endif

#ifndef CPPIMG_FTELL
#define CPPIMG_FTELL(f) ftello64((f))
#endif

#ifndef CPPIMG_SPRINTF
#define CPPIMG_SPRINTF(b,f, ...) sprintf(b, f, __VA_ARGS__)
#endif

    inline s64 CPPIMG_FSIZE(FILE* file)
    {
        CPPIMG_ASSERT(CPPIMG_NULL != file);
        struct stat64 stat;
        return (0 == fstat64(fileno(file), &stat))? stat.st_size : 0;
    }

    inline FILE* CPPIMG_FOPEN(const Char* filepath, const Char* mode)
    {
        return fopen(filepath, mode);
    }

    inline void CPPIMG_FCLOSE(FILE*& file)
    {
        if(CPPIMG_NULL != file){
            fclose(file);
            file = CPPIMG_NULL;
        }
    }
#endif

#ifndef CPPIMG_MALLOC
#define CPPIMG_MALLOC(size) malloc(size)
#endif

#ifndef CPPIMG_FREE
#define CPPIMG_FREE(ptr) free(ptr); (ptr) = CPPIMG_NULL
#endif

#ifndef CPPIMG_NEW
#define CPPIMG_NEW new
#endif

#ifndef CPPIMG_PLACEMENT_NEW
#define CPPIMG_PLACEMENT_NEW(ptr) new(ptr)
#endif

#ifndef CPPIMG_DELETE
#define CPPIMG_DELETE(ptr) delete (ptr); (ptr) = CPPIMG_NULL
#endif

#ifndef CPPIMG_DELETE_ARRAY
#define CPPIMG_DELETE_ARRAY(ptr) delete[] (ptr); (ptr) = CPPIMG_NULL
#endif

#ifndef CPPIMG_MEMSET
#define CPPIMG_MEMSET(ptr, value, size) memset((ptr), (value), (size))
#endif

    template<class T>
    inline T absolute(T x)
    {
        return 0<=x? x : -x;
    }

    template<class T>
    inline T minimum(T x0, T x1)
    {
        return x0<x1? x0 : x1;
    }

    template<class T>
    inline T maximum(T x0, T x1)
    {
        return x0<x1? x1 : x0;
    }

    template<class T>
    inline T clamp(T x, T minv, T maxv)
    {
        x = maximum(x, minv);
        return minimum(x, maxv);
    }

    template<class T>
    inline void swap(T& x0, T& x1)
    {
        T t = cppimg::move(x0);
        x0 = cppimg::move(x1);
        x1 = cppimg::move(t);
    }
#endif

    //-------------------------------------------------------------
    //---
    //--- Color Space
    //---
    //-------------------------------------------------------------
    template<class T>
    void RGBToYCbCr(T* ybr, const T* rgb)
    {
        ybr[0] = static_cast<T>( 0.257f*rgb[0] + 0.504f*rgb[1] + 0.098f*rgb[2] + 16);
        ybr[1] = static_cast<T>(-0.148f*rgb[0] - 0.291f*rgb[1] + 0.439f*rgb[2] + 128);
        ybr[2] = static_cast<T>( 0.439f*rgb[0] - 0.368f*rgb[1] - 0.071f*rgb[2] + 128);
    }

    template<class T>
    void YCbCrToRGB(T* rgb, const T* ybr)
    {
        f32 y = (16<ybr[0])? 1.164f*(ybr[0]-16) : 0;
        f32 Cb = static_cast<f32>(ybr[1]) - 128.0f;
        f32 Cr = static_cast<f32>(ybr[2]) - 128.0f;

        rgb[0] = static_cast<T>(y             + 1.596f*Cb);
        rgb[1] = static_cast<T>(y - 0.391f*Cr - 0.813f*Cr);
        rgb[2] = static_cast<T>(y + 2.018f*Cr);
    }

    template<class T>
    void RGBToYUV(T* yuv, const T* rgb)
    {
        yuv[0] = static_cast<T>( 0.299f*rgb[0] + 0.587f*rgb[1] + 0.114f*rgb[2]);
        yuv[1] = static_cast<T>(-0.169f*rgb[0] - 0.331f*rgb[1] + 0.500f*rgb[2]);
        yuv[2] = static_cast<T>( 0.500f*rgb[0] - 0.419f*rgb[1] - 0.081f*rgb[2]);
    }

    template<class T>
    void YUVToRGB(T* rgb, const T* yuv)
    {
        f32 y = ybr[0];

        rgb[0] = static_cast<T>(y                 + 1.402f*yuv[2]);
        rgb[1] = static_cast<T>(y - 0.344f*yuv[1] - 0.714f*yuv[2]);
        rgb[2] = static_cast<T>(y + 1.772f*yuv[1]);
    }

    //-------------------------------------------------------------
    //---
    //--- Image
    //---
    //-------------------------------------------------------------
    template<class T>
    class Image
    {
    public:
        Image();
        Image(const Image& rhs);
        Image(s32 width, s32 height, s32 channels);
        ~Image();

        inline s32 width() const;
        inline s32 height() const;
        inline s32 channels() const;

        const f32& operator()(s32 x, s32 y) const;
        f32& operator()(s32 x, s32 y);

        inline const f32& operator[](s32 index) const;
        inline f32& operator[](s32 index);

        void swap(Image& rhs);
    private:
        Image(Image&&) = delete;
        Image& operator=(const Image&) = delete;
        Image& operator=(Image&&) = delete;

        s32 width_;
        s32 height_;
        s32 channels_;
        T* data_;
    };

    template<class T>
    inline s32 Image<T>::width() const
    {
        return width_;
    }

    template<class T>
    inline s32 Image<T>::height() const
    {
        return height_;
    }

    template<class T>
    inline s32 Image<T>::channels() const
    {
        return channels_;
    }

    template<class T>
    inline const f32& Image<T>::operator[](s32 index) const
    {
        CPPIMG_ASSERT(0<=index && index<(width_*height_*channels_));
        return data_[index];
    }

    template<class T>
    inline f32& Image<T>::operator[](s32 index)
    {
        CPPIMG_ASSERT(0<=index && index<(width_*height_*channels_));
        return data_[index];
    }

    template<class T>
    Image<T>::Image()
        :width_(0)
        ,height_(0)
        ,channels_(0)
        ,data_(CPPIMG_NULL)
    {
    }

    template<class T>
    Image<T>::Image(const Image& rhs)
        :width_(rhs.width_)
        ,height_(rhs.height_)
        ,channels_(rhs.channels_)
    {
        s32 size = width_*height_*channels_*sizeof(T);
        data_ = reinterpret_cast<T*>(CPPIMG_MALLOC(size));
        memcpy(data_, rhs.data_, size);
    }

    template<class T>
    Image<T>::Image(s32 width, s32 height, s32 channels)
        :width_(width)
        ,height_(height)
        ,channels_(channels)
    {
        CPPIMG_ASSERT(0<=width_);
        CPPIMG_ASSERT(0<=height_);
        CPPIMG_ASSERT(0<=channels_);
        data_ = reinterpret_cast<T*>(CPPIMG_MALLOC(width_*height_*channels_*sizeof(T)));
    }

    template<class T>
    Image<T>::~Image()
    {
        CPPIMG_FREE(data_);
    }

    template<class T>
    const f32& Image<T>::operator()(s32 x, s32 y) const
    {
        CPPIMG_ASSERT(0<=x && x<width_);
        CPPIMG_ASSERT(0<=y && y<height_);
        return data_[(y*width_+x)*channels_];
    }

    template<class T>
    f32& Image<T>::operator()(s32 x, s32 y)
    {
        CPPIMG_ASSERT(0<=x && x<width_);
        CPPIMG_ASSERT(0<=y && y<height_);
        return data_[(y*width_+x)*channels_];
    }

    template<class T>
    void Image<T>::swap(Image& rhs)
    {
        cppimg::swap(width_, rhs.width_);
        cppimg::swap(height_, rhs.height_);
        cppimg::swap(channels_, rhs.channels_);
        cppimg::swap(data_, rhs.data_);
    }
}

#endif //INC_CPPIMGCORE_H_
