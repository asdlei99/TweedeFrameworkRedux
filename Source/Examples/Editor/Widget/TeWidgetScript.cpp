#include "TeWidgetScript.h"

#include "../TeEditor.h"
#include "../ImGuiExt/TeImGuiTextEditor.h"
#include "TeCoreApplication.h"
#include "Gui/TeGuiAPI.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "Components/TeCScript.h"
#include "Scripting/TeScriptManager.h"
#include "Scripting/TeNativeScript.h"
#include "Utility/TeFileStream.h"

namespace te
{
    WidgetScript::WidgetScript()
        : Widget(WidgetType::Script)
        , _selections(gEditor().GetSelectionData())
        , _editor(nullptr)
        , _needsUpdateContent(false)
        , _needsSave(false)
    {
        _title = SCRIPT_TITLE;
    }

    WidgetScript::~WidgetScript()
    { }

    void WidgetScript::Initialize()
    {
        _editor = te_shared_ptr_new<ImGuiTextEditor>();
    }

    void WidgetScript::Update()
    {
        ShowToolbar();
        ShowEditor();

        if (_editor->IsTextChanged())
        {
            _editorContent = _editor->GetText();
            _needsSave = true;
        }
    }

    void WidgetScript::UpdateBackground()
    {
        Save();
    }

    void WidgetScript::PutFocus()
    {
        Widget::PutFocus();

        if (_selections.ClickedComponent && _selections.ClickedComponent->GetCoreType() == TID_CScript)
        {
            auto script = std::static_pointer_cast<CScript>(_selections.ClickedComponent);
            if (script)
            {
                String name = script->GetNativeScriptName();
                String path = script->GetNativeScriptPath();

                if (name.empty())
                    return;

                if (name != _currentScriptName) // Update current script name
                {
                    _currentScriptName = name;
                    _currentScriptPath = path;

                    UpdateEditorContent(); // If current script name different from previous, we need to load its content
                }
            }
        }
    }

    void WidgetScript::Save(bool force)
    {
        if (!_currentScriptName.empty() && (_needsSave || force))
        {
            String path = _currentScriptPath + _currentScriptName + ".cpp";
            FileStream file(path, FileStream::AccessMode::WRITE);
            file.Write((void*)(_editorContent.c_str()), _editorContent.size());
            file.Close();

            _needsSave = false;
        }
    }

    void WidgetScript::Build()
    {
        if (!_currentScriptName.empty())
        {
            if (_needsSave)
            {
                Save(false);
            }
            else
            {
                String path = _currentScriptPath + _currentScriptName + ".cpp";
                gScriptManager().OnMonitorFileModified(path);
            }
        }
    }

    void WidgetScript::UpdateEditorContent()
    {
        if (_currentScriptName != "")
        {
            FileStream file(_currentScriptPath + _currentScriptName + ".cpp");
            _editorContent = file.GetAsString();
            _editor->SetText(_editorContent);
            _needsSave = false;
            file.Close();
        }
        else
        {
            _editor->SetText("");
            _needsSave = false;
        }
    }

    void WidgetScript::ShowEditor()
    {
        ImGui::BeginChild("ScriptEditor", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetWindowHeight() - 90.0f), false);
        _editor->Render("Title", ImVec2(0.0f, ImGui::GetContentRegionMax().y)); // shrink y to bring the compile button into view
        ImGui::EndChild();
    }

    void WidgetScript::ShowToolbar()
    {
        auto ShowButton = [this](const char* title, bool enabled, const std::function<void()>& action)
        {
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Button, (enabled) ? ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] : ImGui::GetStyle().Colors[ImGuiCol_Button]);
            if (ImGui::Button(title, ImVec2(32.0f, 26.0f)))
            {
                if(enabled)
                    action();
            }
            ImGui::PopStyleColor();
        };

        ImGui::BeginChild("ScriptActionList", ImVec2(ImGui::GetContentRegionAvail().x, 42.0f), true);

        // Build current script
        ShowButton(ICON_FA_COGS, (_currentScriptName != ""), [this]() {
            Build();
        });

        // Save current script
        ShowButton(ICON_FA_SAVE, (_currentScriptName != ""), [this]() {
            Save(true);
        });

        ImGui::SameLine();

        Vector<Script*> listedScripts;
        ImGuiExt::ComboOptions<Script*> scriptsOptions;
        const auto& scripts = gScriptManager().GetScripts();
        Script* curScript = nullptr;

        for (const auto& script : scripts)
        {
            if (script->GetNativeScriptName() == _currentScriptName)
                curScript = script;

            if (std::find(listedScripts.begin(), listedScripts.end(), script) == listedScripts.end())
            {
                listedScripts.push_back(script);
                if(script != nullptr)
                    scriptsOptions.AddOption(script, script->GetNativeScriptName());
            }
        }

        scriptsOptions.AddOption(nullptr, ICON_FA_TIMES_CIRCLE " No script");

        if (!curScript)
        {
            _currentScriptName = "";
            _currentScriptPath = "";

            UpdateEditorContent();
        }
        
        if (ImGuiExt::RenderOptionCombo<Script*>(&curScript, "##scripts_editor_options", "Current script", scriptsOptions, 200.0f))
        {
            if (curScript != nullptr)
            {
                _currentScriptName = curScript->GetNativeScriptName();
                _currentScriptPath = curScript->GetNativeScriptPath();

                UpdateEditorContent();
            }
        }

        ImGui::EndChild();
    }
}
