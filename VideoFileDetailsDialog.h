//
// Created by sigsegv on 10/12/24.
//

#ifndef ACTIONCAMS_VIDEOFILEDETAILSDIALOG_H
#define ACTIONCAMS_VIDEOFILEDETAILSDIALOG_H

#include <wx/wx.h>

class wxListView;

class VideoFileDetailsDialog : public wxDialog {
private:
    wxListView *listView;
public:
    VideoFileDetailsDialog(wxWindow *parent, const std::string &rootDirectory, const std::string &relativeFilename);
};


#endif //ACTIONCAMS_VIDEOFILEDETAILSDIALOG_H
