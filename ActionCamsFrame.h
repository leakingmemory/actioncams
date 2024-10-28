//
// Created by sigsegv on 9/7/24.
//

#ifndef ACTIONCAMS_ACTIONCAMSFRAME_H
#define ACTIONCAMS_ACTIONCAMSFRAME_H

#include <memory>
#include <map>
#include <wx/wx.h>
#include <wx/treectrl.h>

class wxTreeCtrl;

struct VideoDirectoryItem {
    std::weak_ptr<VideoDirectoryItem> parent;
    std::vector<std::shared_ptr<VideoDirectoryItem>> children;
    wxTreeItemId itemId;
    VideoDirectoryItem(std::shared_ptr<VideoDirectoryItem> directory, wxTreeItemId itemId) : parent(directory), children(), itemId(itemId) {}
};

struct VideoFileItem : VideoDirectoryItem {
    std::string relativeFileName;
    VideoFileItem(std::shared_ptr<VideoDirectoryItem> directory, wxTreeItemId itemId, const std::string &relativeFilename) : VideoDirectoryItem(directory, itemId), relativeFileName(relativeFilename) {}
};

class ActionCamsFrame : public wxFrame {
private:
    std::string directoryRoot;
    std::shared_ptr<VideoDirectoryItem> root;
    std::map<wxTreeItemId,std::weak_ptr<VideoFileItem>> files;
    std::shared_ptr<VideoFileItem> itemForCtxMenu{};
    wxTreeCtrl *videoFileTree;
public:
    ActionCamsFrame();
    void OpenDirectoryPath(const std::string &);
    void OpenDirectoryCommand(wxCommandEvent &e);
    void Exit(wxCommandEvent &e);
    void OpenListItemContextMenu(wxContextMenuEvent &e);
    void OnVideoFileMove(wxCommandEvent &e);
    void OnVideoFileDetails(wxCommandEvent &e);
};


#endif //ACTIONCAMS_ACTIONCAMSFRAME_H
