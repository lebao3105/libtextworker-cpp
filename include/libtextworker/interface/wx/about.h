#include <libtextworker/pch.h>
#include <libtextworker/interface/wx.h>
#include <wx/aboutdlg.h>

namespace libtextworker::wx
{
    /* Generic wx(adv)AboutDialog.
       All infomations are stored in infos attribute.
       Set functions here are aliases of corresponding ones in infos. */
    class AboutDialog
    {
    public:
        wxAboutDialogInfo infos;
        wxWindow* parent;

        void SetArtists(const wxArrayString& artists) { return infos.SetArtists(artists); };
        void SetCopyRight(const wxString& copyright) { return infos.SetCopyright(copyright); };
        void SetDescription(const wxString& description) { return infos.SetDescription(description); };
        void SetDevelopers(const wxArrayString& devs) { return infos.SetDevelopers(devs); };
        void SetDocWriters(const wxArrayString& writers) { return infos.SetDocWriters(writers); };
        void SetIcon(const wxIcon &icon) { return infos.SetIcon(icon); };
        void SetName(const wxString& name) { return infos.SetName(name); };
        void SetTranslators(const wxArrayString& translators) { return infos.SetTranslators(translators); };
        void SetVersion(const wxString& version) { return infos.SetVersion(version); };
        void SetWebSite(const wxString& URL, const wxString& description) { return infos.SetWebSite(URL, description); };
        void SetLicense(const wxString& license) { return infos.SetLicense(license); };
        void SetLicence(const wxString& license) { return SetLicence(license); };

        /* Shows an About dialog with infomations collected. */
        void ShowBox() { return wxAboutBox(infos, parent); };

        template <typename TEvent>
        /* Just like ShowBox but with wx*Event parameter (unused) */
        void ShowBox(TEvent* _) { return wxAboutBox(infos, parent); };
    };
}