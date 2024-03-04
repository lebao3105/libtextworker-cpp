#pragma once

#include <libtextworker/UI.h>
#include <libtextworker/pch.h>
#include <wx/font.h>
#include <wx/wx.h>

namespace libtextworker::UI
{
    namespace wx
    {
        std::map<std::string, int> FONTWT = {
            {"light", wxFONTWEIGHT_LIGHT},
            {"normal", wxFONTWEIGHT_NORMAL},
            {"semibold", wxFONTWEIGHT_SEMIBOLD},
            {"bold", wxFONTWEIGHT_BOLD},
            {"maxlight", wxFONTWEIGHT_EXTRALIGHT},
            {"maxbold", wxFONTWEIGHT_EXTRABOLD}
        };

        std::map<std::string, int> FONTST = {
            {"normal", wxFONTSTYLE_NORMAL},
            {"italic", wxFONTSTYLE_ITALIC}
        };

        std::map<std::string, int> FONTSZ = {{"normal", 12}, {"small", 8}, {"large", 16}};

        class wxColorManager: public ColorManager
        {
        public:
            bool recursive_configure = true;

            /* Returns a wxFont object based on current settings. */
            wxFont GetFont();
            
            template <typename T>
            /* Configures a wx control. */
            void Configure(T* widget, std::optional<std::string> color, bool recursivelly);
        };
    };
}

