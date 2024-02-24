#pragma once

#include <libtextworker/pch.h>
#include <libtextworker/get_config.h>

namespace libtextworker
{
    /*
        libtextworker namespace for GUIs.
    */
    namespace UI
    {

        /* Available colors. Welp we need more:) */
        std::map<std::string, std::string> colors = {
            {"light", "#ffffff"}, {"dark", "#0f0e0d"},
            {"green", "#00ff00"}, {"red", "#ff0000"},
            {"rose", "#ffaa95"}, {"light_pink", "#ffd1d6"},
            {"light_green", "#95ffaa"}, {"black", "#000000"}
        };

        class ColorManager: public libtextworker::get_config::GetConfig {
        public:
            // std::tuple<std::string, std::string> GetColors();
            /*
                Gets the color from current system scheme
                Warning: no darkdetect for C++ right now.
            */
            std::tuple<std::string, std::string> GetColors(std::string scheme);

            /* Gets current font settings. */
            std::tuple<int, std::string, std::string, std::string> GetFont();

            /* Configures a GUI widget, setting its colors. */
            void Configure(auto* widget, std::string colorScheme = "") {};

            /* Resets the settings file, but do nothing in ColorManager. */
            void Reset(bool restore = false) {};
        };
    }
}