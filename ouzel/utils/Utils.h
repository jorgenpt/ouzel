// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <limits>
#include <functional>
#include <random>

#include "core/CompileConfig.h"
#if OUZEL_PLATFORM_ANDROID
#include <cpu-features.h>
#endif

#define OUZEL_UNUSED(x) (void)(x)

namespace ouzel
{
#if OUZEL_PLATFORM_ANDROID && OUZEL_SUPPORTS_NEON_CHECK
    class AnrdoidNEONChecker
    {
    public:
        AnrdoidNEONChecker()
        {
            if (android_getCpuFamily() == ANDROID_CPU_FAMILY_ARM && (android_getCpuFeatures() & ANDROID_CPU_ARM_FEATURE_NEON) != 0)
                neonAvailable = true;
            else
                neonAvailable = false;
        }

        bool isNEONAvailable() const { return neonAvailable; }

    private:
        bool neonAvailable;
    };

    extern AnrdoidNEONChecker anrdoidNEONChecker;
#endif

    extern std::mt19937 randomEngine;

    inline uint32_t readUInt32Big(const uint8_t* buffer)
    {
        uint32_t result = static_cast<uint32_t>(buffer[3]) |
                          static_cast<uint32_t>(buffer[2])<<8 |
                          static_cast<uint32_t>(buffer[1])<<16 |
                          static_cast<uint32_t>(buffer[0])<<24;

        return result;
    };

    inline uint32_t readUInt32Little(const uint8_t* buffer)
    {
        uint32_t result = static_cast<uint32_t>(buffer[0]) |
                          static_cast<uint32_t>(buffer[1])<<8 |
                          static_cast<uint32_t>(buffer[2])<<16 |
                          static_cast<uint32_t>(buffer[3])<<24;

        return result;
    };

    inline uint16_t readUInt16Big(const uint8_t* buffer)
    {
        uint32_t result = static_cast<uint32_t>(buffer[0]) |
                          static_cast<uint32_t>(buffer[1])<<8;

        return static_cast<uint16_t>(result);
    };

    inline uint16_t readUInt16Little(const uint8_t* buffer)
    {
        uint32_t result = static_cast<uint32_t>(buffer[0]) |
                          static_cast<uint32_t>(buffer[1])<<8;

        return static_cast<uint16_t>(result);
    };

    inline std::vector<uint32_t> utf8to32(const std::string& text)
    {
        std::vector<uint32_t> result;

        for (auto i = text.begin(); i != text.end(); ++i)
        {
            uint32_t cp = *i & 0xff;

            if (cp < 0x80) // length = 1
            {
                // do nothing
            }
            else if ((cp >> 5) == 0x6) // length = 2
            {
                if (++i == text.end()) return result;
                cp = ((cp << 6) & 0x7ff) + (*i & 0x3f);
            }
            else if ((cp >> 4) == 0xe) // length = 3
            {
                if (++i == text.end()) return result;
                cp = ((cp << 12) & 0xffff) + (((*i & 0xff) << 6) & 0xfff);
                if (++i == text.end()) return result;
                cp += *i & 0x3f;
            }
            else if ((cp >> 3) == 0x1e) // length = 4
            {
                if (++i == text.end()) return result;
                cp = ((cp << 18) & 0x1fffff) + (((*i & 0xff) << 12) & 0x3ffff);
                if (++i == text.end()) return result;
                cp += ((*i & 0xff) << 6) & 0xfff;
                if (++i == text.end()) return result;
                cp += (*i) & 0x3f;
            }

            result.push_back(cp);
        }
        
        return result;
    }
}
