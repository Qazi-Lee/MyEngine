#define STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c"    // 包含 Vorbis 解码实现
#define MA_ENABLE_VORBIS   // 告诉 miniaudio 启用 Vorbis 支持
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
