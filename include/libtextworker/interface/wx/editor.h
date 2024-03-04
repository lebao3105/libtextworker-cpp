#include <wx/stc/stc.h>

#include <libtextworker/UI.h>
#include <libtextworker/interface/wx/miscs.h>
#include <libtextworker/libtextworker.h>
#include <libtextworker/general.h>
#include <libtextworker/get_config.h>

using namespace libtextworker;
using namespace libtextworker::general;
using namespace libtextworker::get_config;
using namespace libtextworker::UI;

namespace libtextworker::wx
{
    class StyledTextControl: public wxStyledTextCtrl
    {
    public:
        GetConfig cfg;

        void EditorInit(char* configPath = "")
        {
            if (configPath != "")
                configPath = CraftItems({EDITOR_DIR, "default.ini"});
            
            cfg = GetConfig(stock_editor_configs, configPath);

            LineNumbers();
            DNDSupport();
            IndentationSet();

            if (std::find(cfg.true_values.begin(), cfg.true_values.end(), cfg.Get("menu", "enabled")) != cfg.true_values.end())
                Bind(wxEVT_RIGHT_DOWN, &MenuPopup);
        };

        bool DNDSupport() {
            if (std::find(cfg.true_values.begin(), cfg.true_values.end(), cfg.Get("editor", "dnd_enabled")) != cfg.true_values.end())
            {
                SetDropTarget(DragNDropTarget(this));
                return true;
            }
            return false;
        };

        void IndentationSet() {
            int size = std::any_cast<int>(cfg.Get("indentaion", "size"));
            char* tp = std::any_cast<char*>(cfg.Get("indentation", "type"));
            char* show_guide = std::any_cast<char*>(cfg.Get("indentation", "show_guide"));
            char* bk_unindent = std::any_cast<char*>(cfg.Get("indentation", "backspace_unindents"));
            char* view_ws = std::any_cast<char*>(cfg.Get("editor", "view_whitespaces"));

            SetUseTabs(tp == "tabs");
            SetBackSpaceUnIndents(std::find(cfg.true_values.begin(), cfg.true_values.end(), bk_unindent) != cfg.true_values.end());
            SetViewWhiteSpace(std::find(cfg.true_values.begin(), cfg.true_values.end(), view_ws) != cfg.true_values.end());
            SetIndent(size);
            SetIndentationGuides(std::find(cfg.true_values.begin(), cfg.true_values.end(), show_guide) != cfg.true_values.end());
        };

        bool LineNumbers() {
            char* state = std::any_cast<char*>(cfg.Get("editor", "line_count"));
            if (std::find(cfg.true_values.begin(), cfg.true_values.end(), state) != cfg.true_values.end())
            {
                SetMarginWidth(0, 0);
                return false;
            }

            SetMarginWidth(0, 20);
            SetMarginType(0, wxSTC_MARGIN_NUMBER);
            SetMarginMask(0, 0);
            Bind(wxEVT_STC_UPDATEUI, &OnUIUpdate);
        };

        void OnUIUpdate(wxCommandEvent& event)
        {
            int line_count = GetLineCount();
            int margin_width;
            int last_line_width;

            if (line_count <= 1000)
            {
                margin_width = 40;
            }
            else
            {
                last_line_width = TextWidth(wxSTC_STYLE_LINENUMBER, std::any_cast<char*>(line_count));
                margin_width = last_line_width + 4;
            }
            
            SetMarginWidth(0, margin_width);
            event.Skip();
        }

        void MenuPopup(wxMouseEvent& event) {
            wxPoint pt = event.GetPosition();
            wxMenu* menu = CreateMenu(this, {{wxID_CUT, NULL, NULL, [=](wxCommandEvent&){ Cut(); }, NULL},
                                             {wxID_COPY, NULL, NULL, [=](wxCommandEvent&){ Copy(); }, NULL},
                                             {wxID_PASTE, NULL, NULL, [=](wxCommandEvent&){ Paste(); }, NULL},
                                             {wxID_SEPARATOR, NULL, NULL, NULL, NULL},
                                             {wxID_UNDO, NULL, NULL, [=](wxCommandEvent&){ Undo(); }, NULL}});
            PopupMenu(menu, pt);
        };

    };

    class DragNDropTarget: public wxFileDropTarget, wxTextDropTarget
    {
    public:
        wxStyledTextCtrl* Target;
        DragNDropTarget(wxStyledTextCtrl* target) { Target = target; };
        bool OnDropText(wxCoord x, wxCoord y, const wxString& text) { Target->WriteText(text); };
        bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames) {
            Target->LoadFile(filenames[0]);
        };
    };
}