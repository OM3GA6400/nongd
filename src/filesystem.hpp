#pragma once

#include <Geode/Geode.hpp>

#ifdef GEODE_IS_MACOS
    namespace fs = ghc::filesystem;
#else
    #include <filesystem>
    namespace fs = std::filesystem;
#endif
