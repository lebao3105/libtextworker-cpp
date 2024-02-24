#include <libtextworker/UI.h>
#include <libtextworker/general.h>

using namespace libtextworker::UI;
using namespace libtextworker::general;

std::tuple<std::string, std::string> ColorManager::GetColors(std::string scheme)
{
    // Get current settings
    auto back = std::any_cast<std::string>(this->Get("color", "background"));
    auto fore = std::any_cast<std::string>(this->Get("color", "foreground"));
    std::map<std::string, std::string> reserve_for_fore = {{"light", "dark"}, {"dark", "light"}};

    if (!scheme.empty())
        back = scheme;
    else if (back[0] == '#')
        throw libTewException(_file, "->color: Background must not be a hex string");
    else
        scheme = back;
    
    if (fore == "default")
        fore = colors[reserve_for_fore[scheme]];
    else if (colors.count(fore))
        fore = colors[fore];
    else if (fore[0] != '#')
        throw libTewException(_file, "->color: Unknown foreground color passed");

    // No darkdetect for now
    // auto autoc = this->Get("color", "auto");

    // Prefer color for specific modes first
    auto test_back = this->Get("color", "background-" + scheme);
    auto test_fore = this->Get("color", "foreground-" + scheme);

    if (!test_back.has_value())
        test_back = colors[scheme];
    
    if (!test_fore.has_value())
        if (fore[0] == '#') // hex colors
            test_fore = fore;
        else
            test_fore = colors[fore];
    else if (colors.count(std::any_cast<std::string>(test_fore)))
        test_fore = colors[std::any_cast<std::string>(test_fore)];
    else
        throw libTewException(_file, "->color: Unknown foreground color for current system color scheme passed");

    return std::tuple<std::string, std::string>{std::any_cast<std::string>(test_back), std::any_cast<std::string>(test_fore)};
}

std::tuple<int, std::string, std::string, std::string> ColorManager::GetFont()
{
    std::string family;
    int size;
    std::string weight;
    std::string style;

    try {
        family = std::any_cast<std::string>(this->Get("font", "family"));
        int size = std::any_cast<int>(this->Get("font", "size"));
        weight = std::any_cast<std::string>(this->Get("font", "weight"));
        style = std::any_cast<std::string>(this->Get("font", "style"));

        if (size <= 0)
            size = 10;
    }

    catch (std::exception) {
        family = "";
        size = 10;
        weight = "system";
        style = "system";
    }
    
    return std::tuple<int, std::string, std::string, std::string>{size, style, weight, family};
}