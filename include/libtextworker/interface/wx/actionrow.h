#include <wx/wx.h>
#include <libtextworker/interface/wx.h>

namespace libtextworker::wx
{
    class ActionRow: public wxBoxSizer
    {
    public:
        wxWindow* parent;
        void SetParent(wxWindow* obj) { parent = obj; };

        template <typename TObj, typename... TArg>
        /* Places an object to a side of the row. 
           Unlike Python where allows unpacking args and kwds (tuple and dictionary)
           for object initial arguments, this turns it off.
           Returns the created object. */
        TObj* PlaceObj(std::function<TObj*(...)> object, int stretch = 1)
        {
            TObj* target = object(parent);
            Add(target, stretch, wxALL | wxALIGN_CENTER_VERTICAL, 5);
            return target;
        };
    };
}