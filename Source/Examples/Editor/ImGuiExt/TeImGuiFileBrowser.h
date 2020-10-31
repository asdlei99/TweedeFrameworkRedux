#pragma once

/*
MIT License

Copyright (c) 2019 gallickgunner

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "TeCorePrerequisites.h"

#include "ImGui/imgui.h"
#include <string>
#include <vector>

namespace te
{
    class ImGuiFileBrowser
    {
        public:
            ImGuiFileBrowser();
            ~ImGuiFileBrowser();

            enum class DialogMode
            {
                SELECT, //Select Directory Mode
                OPEN,   //Open File mode
                SAVE    //Save File mode.
            };

            /* Use this to show an open file dialog. The function takes label for the window,
             * the size, a DialogMode enum value defining in which mode the dialog should operate and optionally the extensions that are valid for opening.
             * Note that the select directory mode doesn't need any extensions.
             */
            bool ShowFileDialog(const String& label, const DialogMode mode, const ImVec2& sz_xy = ImVec2(0,0), const String& valid_types = "*.*");

            /* Store the opened/saved file name or dir name (incase of selectDirectoryDialog) and the absolute path to the selection
             * Should only be accessed when above functions return true else may contain garbage.
             */
            String SelectedFileName;
            String SelectedPath;
            String ext;    // Store the saved file extension
            bool   IsCancelled;

        private:
            struct Info
            {
                Info(String name, bool is_hidden) : name(name), is_hidden(is_hidden)
                {
                }
                String name;
                bool is_hidden;
            };

            //Enum used as bit flags.
            enum FilterMode
            {
                FilterMode_Files = 0x01,
                FilterMode_Dirs = 0x02
            };

            //Helper Functions
            static bool AlphaSortComparator(const Info& a, const Info& b);
            ImVec2 GetButtonSize(String button_text);

            /* Helper Functions that render secondary modals
             * and help in validating file extensions and for filtering, parsing top navigation bar.
             */
            void SetValidExtTypes(const String& valid_types_string);
            bool ValidateFile();
            void ShowErrorModal();
            void ShowInvalidFileModal();
            bool ShowReplaceFileModal();
            void ShowHelpMarker(String desc);
            void ParsePathTabs(String str);
            void FilterFiles(int filter_mode);

            /* Core Functions that render the 4 different regions making up
             * a simple file dialog
             */
            bool RenderNavAndSearchBarRegion();
            bool RenderFileListRegion();
            bool RenderInputTextAndExtRegion();
            bool RenderButtonsAndCheckboxRegion();
            bool RenderInputComboBox();
            void RenderExtBox();

            /* Core Functions that handle navigation and
             * reading directories/files
             */
            bool ReadDIR(String path);
            bool OnNavigationButtonClick(int idx);
            bool OnDirClick(int idx);

            // Functions that reset state and/or clear file list when reading new directory
            void ClearFileList();
            void CloseDialog();

#if TE_PLATFORM == TE_PLATFORM_WIN32
            bool LoadWindowsDrives(); // Helper Function for Windows to load Drive Letters.
#endif

#if TE_PLATFORM == TE_PLATFORM_LINUX
            void InitCurrentPath();   // Helper function for UNIX based system to load Absolute path using realpath
#endif

            ImVec2 min_size, max_size, input_combobox_pos, input_combobox_sz;
            DialogMode dialog_mode;
            int filter_mode, col_items_limit, selected_idx, selected_ext_idx;
            float col_width, ext_box_width;
            bool show_hidden, show_inputbar_combobox, is_dir, is_appearing, filter_dirty, validate_file;
            char input_fn[256];

            Vector<String> valid_exts;
            Vector<String> current_dirlist;
            Vector<Info> subdirs;
            Vector<Info> subfiles;
            String current_path, error_msg, error_title, invfile_modal_id, repfile_modal_id;

            ImGuiTextFilter filter;
            String valid_types;
            Vector<const Info*> filtered_dirs; // Note: We don't need to call delete. It's just for storing filtered items from subdirs and subfiles so we don't use PassFilter every frame.
            Vector<const Info*> filtered_files;
            Vector<std::reference_wrapper<String>> inputcb_filter_files;
    };
}
