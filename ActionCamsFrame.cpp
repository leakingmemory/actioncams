//
// Created by sigsegv on 9/7/24.
//

#include "ActionCamsFrame.h"
#include "ActionCamsIds.h"
#include "VideoDirectory.h"
#include "VideoFile.h"
#include "VideoFileDetailsDialog.h"
#include <wx/treectrl.h>
#include <sstream>

ActionCamsFrame::ActionCamsFrame() : wxFrame(nullptr, wxID_ANY, "ActionCams") {
    auto *fileMenu = new wxMenu();
    fileMenu->Append(ActionCams_OpenDir, wxT("Open directory"));
    fileMenu->Append(ActionCams_Exit, wxT("Exit"));
    auto *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, wxT("File"));
    SetMenuBar(menuBar);

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    videoFileTree = new wxTreeCtrl(this, wxID_ANY);
    sizer->Add(videoFileTree, 1, wxEXPAND);
    SetSizerAndFit(sizer);

    Bind(wxEVT_MENU, &ActionCamsFrame::OpenDirectoryCommand, this, ActionCams_OpenDir);
    Bind(wxEVT_MENU, &ActionCamsFrame::Exit, this, ActionCams_Exit);
    Bind(wxEVT_MENU, &ActionCamsFrame::OnVideoFileDetails, this, ActionCams_FileCtxMenu_Details);
    videoFileTree->Bind(wxEVT_CONTEXT_MENU, &ActionCamsFrame::OpenListItemContextMenu, this, wxID_ANY);
}

void ActionCamsFrame::OpenDirectoryPath(const std::string &dir) {
    directoryRoot = dir;
    VideoDirectory videoDirectory{dir};
    videoFileTree->DeleteAllItems();
    auto treeRoot = videoFileTree->AddRoot(wxString::FromUTF8(dir));
    root = std::make_shared<VideoDirectoryItem>(std::shared_ptr<VideoDirectoryItem>(), treeRoot);
    for (const auto &year : videoDirectory.GetYears()) {
        auto yearRoot = videoFileTree->AppendItem(treeRoot, wxString::FromUTF8(year));
        auto &yearRootItem = root->children.emplace_back(std::make_shared<VideoDirectoryItem>(root, yearRoot));
        for (const auto &week : videoDirectory.GetWeeks(year)) {
            std::stringstream weekStr{};
            weekStr << "Week " << week;
            auto weekRoot = videoFileTree->AppendItem(yearRoot, wxString::FromUTF8(weekStr.str()));
            auto &weekRootItem = yearRootItem->children.emplace_back(std::make_shared<VideoDirectoryItem>(yearRootItem, weekRoot));
            for (const auto &date : videoDirectory.GetDates(year, week)) {
                auto dateRoot = videoFileTree->AppendItem(weekRoot, wxString::FromUTF8(date));
                auto dateRootItem = weekRootItem->children.emplace_back(std::make_shared<VideoDirectoryItem>(weekRootItem, dateRoot));
                for (const auto &vf: videoDirectory.GetVideoFiles(year, week, date)) {
                    auto relativeFilename = vf->GetRelativeFilename();
                    auto videoFileWxItemId = videoFileTree->AppendItem(dateRoot, relativeFilename);
                    auto videoFileItem = std::make_shared<VideoFileItem>(dateRootItem, videoFileWxItemId, relativeFilename);
                    dateRootItem->children.emplace_back(videoFileItem);
                    files.insert_or_assign(videoFileWxItemId, videoFileItem);
                }
            }
        }
    }
}

void ActionCamsFrame::OpenDirectoryCommand(wxCommandEvent &e) {
    wxDirDialog dirDialog{this, wxT("Please select a directory")};
    auto result = dirDialog.ShowModal();
    if (result == wxID_OK) {
        auto path = dirDialog.GetPath();
        OpenDirectoryPath(path.ToStdString());
    }
}

void ActionCamsFrame::Exit(wxCommandEvent &e) {
    Close();
}

void ActionCamsFrame::OpenListItemContextMenu(wxContextMenuEvent &e) {
    auto selectedId = videoFileTree->GetSelection();
    std::shared_ptr<VideoFileItem> videoFileItem{};
    {
        auto iterator = files.find(selectedId);
        if (iterator == files.end()) {
            return;
        }
        videoFileItem = iterator->second.lock();
        if (!videoFileItem) {
            return;
        }
    }
    itemForCtxMenu = videoFileItem;
    wxMenu menu(wxString::FromUTF8(videoFileItem->relativeFileName));
    menu.Append(ActionCams_FileCtxMenu_Move, wxT("Move"));
    menu.Append(ActionCams_FileCtxMenu_Details, wxT("Details"));
    PopupMenu(&menu);
}

void ActionCamsFrame::OnVideoFileMove(wxCommandEvent &e) {

}

void ActionCamsFrame::OnVideoFileDetails(wxCommandEvent &e) {
    VideoFileDetailsDialog detailsDialog{this, directoryRoot, itemForCtxMenu->relativeFileName};
    detailsDialog.ShowModal();
}