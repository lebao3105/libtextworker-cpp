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

void wxColorManager::Configure(wxControl* widget, std::optional<std::string> color, bool recursivelly)
{
    // std::string fg;
    // std::string bg;

    // std::tie(bg, fg) = this->GetColors(color);
    // Needs a converter to RGB

    wxFont font = GetFont();
    widget->SetFont(font);
    widget->Refresh();
}