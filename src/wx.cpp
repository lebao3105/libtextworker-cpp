#include <libtextworker/interface/wx.h>
#include <typeinfo>

using namespace libtextworker::UI::wx;

wxFont wxColorManager::GetFont()
{
    int size;
    std::string style;
    std::string weight;
    std::string family;

    std::tie(size, style, weight, family) = ColorManager::GetFont();
    if (weight == "system") weight = "default";
    if (style == "system") style = "default";
    return wxFont(size, wxFONTFAMILY_MODERN, FONTST[style], FONTWT[weight], 0, family);
}

template <typename T>
void wxColorManager::Configure(T* widget, std::optional<std::string> color, bool recursivelly)
{
    if (recursive_configure)
        for (auto child: widget->GetChildren())
            Configure(child, color, recursivelly);

    wxFont font = GetFont();
    widget->SetFont(font);
    widget->Refresh();
}