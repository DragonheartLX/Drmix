#pragma once

#include <fmt/core.h>
#include <fmt/color.h>

namespace Logger 
{
    template<typename... Args>
    inline void debug(std::string_view fmt, Args&&... args)
    {
        fmt::print(fmt::fg(fmt::color::gray), "[Debug]: {}\n", fmt::vformat(fmt, fmt::make_format_args(std::forward<Args>(args)...)));
    }

    template<typename... Args>
    inline void info(std::string_view fmt, Args&&... args)
    {
        fmt::print(fmt::fg(fmt::color::white_smoke), "[Info]: {}\n", fmt::vformat(fmt, fmt::make_format_args(std::forward<Args>(args)...)));
    }

    template<typename... Args>
    inline void warn(std::string_view fmt, Args&&... args)
    {
        fmt::print(fmt::fg(fmt::color::orange), "[Warn]: {}\n", fmt::vformat(fmt, fmt::make_format_args(std::forward<Args>(args)...)));
    }

    template<typename... Args>
    inline void error(std::string_view fmt, Args&&... args)
    {
        fmt::print(fmt::fg(fmt::color::red), "[Error]: {}\n", fmt::vformat(fmt, fmt::make_format_args(std::forward<Args>(args)...)));
    }

    template<typename... Args>
    inline void trace(std::string_view fmt, Args&&... args)
    {
        fmt::print(fmt::fg(fmt::color::gray), "[Trace]: {}\n", fmt::vformat(fmt, fmt::make_format_args(std::forward<Args>(args)...)));
    }
}