//
// Created by sigsegv on 10/12/24.
//

#include "VideoFileDetailsDialog.h"
#include <wx/listctrl.h>
#include <sstream>
#include "FFVideo.h"

VideoFileDetailsDialog::VideoFileDetailsDialog(wxWindow *parent, const std::string &rootDirectory, const std::string &relativeFilename) : wxDialog(parent, wxID_ANY, wxT("Details")) {
    auto sizer = new wxBoxSizer(wxVERTICAL);
    listView = new wxListView(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 400));
    listView->AppendColumn(wxT(""));
    listView->AppendColumn(wxT(""));
    listView->SetColumnWidth(0, 100);
    listView->SetColumnWidth(1, 400);
    int rowCount = 0;
    {
        auto row = rowCount++;
        listView->InsertItem(row, wxT("Root dir: "));
        listView->SetItem(row, 1, wxString::FromUTF8(rootDirectory));
    }
    {
        auto row = rowCount++;
        listView->InsertItem(row, wxT("Filename: "));
        listView->SetItem(row, 1, wxString::FromUTF8(relativeFilename));
    }
    try {
        FFVideo ffv{rootDirectory, relativeFilename};
        {
            auto row = rowCount++;
            listView->InsertItem(row, wxT("Format: "));
            listView->SetItem(row, 1, wxString::FromUTF8(ffv.GetFormatLongName()));
        }
        {
            auto row = rowCount++;
            listView->InsertItem(row, wxT("Duration: "));
            std::stringstream sstr{};
            sstr << ffv.GetDuration();
            listView->SetItem(row, 1, wxString::FromUTF8(sstr.str()));
        }
        auto streams = ffv.GetStreams();
        {
            auto row = rowCount++;
            listView->InsertItem(row, wxT("Streams: "));
            std::stringstream sstr{};
            sstr << streams.size();
            listView->SetItem(row, 1, wxString::FromUTF8(sstr.str()));
        }
        for (const auto &stream : streams) {
            auto row = rowCount++;
            std::stringstream info{};
            switch (stream.GetMediaType()) {
                case FFVMediaType::AUDIO:
                    listView->InsertItem(row, wxT("Audio: "));
                    {
                        auto ad = stream.GetAudioData();
                        info << (int)ad.channels << " channels, rate " << ad.sampleRate;
                    }
                    break;
                case FFVMediaType::VIDEO:
                    listView->InsertItem(row, wxT("Video: "));
                    {
                        auto vd = stream.GetVideoData();
                        info << vd.width << "x" << vd.height;
                    }
                    break;
                default:
                    listView->InsertItem(row, wxT("Stream: "));
                    info << "Unknown";
            }
            listView->SetItem(row, 1, wxString::FromUTF8(info.str()));
        }
    } catch (std::exception &e) {
        auto row = rowCount++;
        listView->InsertItem(row, wxT("Error: "));
        listView->SetItem(row, 1, wxString::FromUTF8(e.what()));
    }
    sizer->Add(listView, 0, wxALL | wxEXPAND, 5);
    SetSizerAndFit(sizer);
}