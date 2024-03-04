/* FIXME: Reorder this */
#include <wx/menu.h>
#include <wx/xrc/xmlres.h>
#include <wx/filesys.h>
#include <wx/fs_mem.h>

#include <functional>
#include <libintl.h>
#include <regex>
#include <libtextworker/pch.h>
#include <libtextworker/interface/wx.h>
#include <fstream>

namespace libtextworker::wx
{
    template <typename TWind, typename EventType>
    /* Creates a wxMenu with a list of tuples:
       (id, label, helptext, handler, kind)
       No sub-menu support. Events (wxEVT_MENU) are automatically bond to the window. */
    wxMenu* CreateMenu(TWind* parent, std::vector<std::tuple<wxStandardID, char*, char*, std::function<void(EventType&)>, wxItemKind>> items)
    {
        wxMenu* target;
        wxStandardID id;
        char* label, helptext;
        std::function<void(EventType&)> handler;
        wxItemKind kind;

        for (std::tuple item: items) {
            std::tie(id, label, helptext, handler, kind) = item;
            parent->Bind(wxEVT_MENU, handler, target->Append(id, label, helptext, kind));
        };

        return target;
    };

    template <typename TWind, typename EventType>
    /* Binds wxEVT_MENU events an easier and more convenient way. Ideal for XRC menus. */
    void BindMenuEvents(TWind* window, wxMenu* menu, std::vector<std::tuple<std::function<void(EventType&)>, int>> items)
    {
        std::function<void(EventType)> callback;
        int position;

        for (std::tuple item: items)
        {
            std::tie(callback, position) = item;
            window->Bind(wxEVT_MENU, &callback, menu->FindItemByPosition(position));
        }
    };

    template <typename TWind, typename EventType>
    void BindEvents(TWind* window, std::vector<std::tuple<wxEvent, std::function<void(EventType)>, int>> items)
    {
        wxEvent event;
        std::function<void(EventType)> callback;
        int position;

        for (std::tuple item: items)
        {
            std::tie(event, callback, position) = item;
            window->Bind(event, callback, window->GetChildren()[position]);
        }
    };

    /* Class to read wxXRC UIs. */
    class XMLBuilder
    {
    public:
        wxWindow* Parent;
        wxXmlResource* Res;
        std::function<char*(const char*)> translateFunc;

        template <typename GetType, typename ReturnType>
        XMLBuilder(wxWindow* parent, char* filePath, std::function<GetType(ReturnType)> transFunc = &gettext) {
            Parent = parent;
            translateFunc = transFunc;

            std::fstream fileReadWrite(filePath);
            std::stringstream fileContent;
            fileContent << fileReadWrite.rdbuf();

            std::regex re("_(['\"](.*?)['\"])");
            
            fileContent.str(std::regex_replace(fileContent, re, [transFunc](const std::smatch& m) {
                return transFunc(m.str()); // not tested, the entire project is not tested;-)
            }));

            wxFileSystem::AddHandler(new wxMemoryFSHandler);
            wxMemoryFSHandler::AddFile(filePath, fileContent.str());

            Res = wxXmlResource::Get();
            Res->Load("memory:" + filePath);
        };

        /* Returns a XRC object. */
        wxObject* loadObject(const char* objectname, const char* objecttype) {
            return Res->LoadObjectRecursively(this->Parent, objectname, objecttype);
        };
    };

}