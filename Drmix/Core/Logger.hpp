#pragma once
#include <spdlog/spdlog.h>

#define DRMIX_LOG_INFO(...)		spdlog::info(__VA_ARGS__)
#define DRMIX_LOG_WARN(...)		spdlog::warn(__VA_ARGS__)
#define DRMIX_LOG_ERROR(...)	spdlog::error(__VA_ARGS__)