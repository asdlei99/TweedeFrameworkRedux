#include "TeScript.h"

#include "TeScriptManager.h"
#include "TeNativeScript.h"
#include "Scene/TeSceneManager.h"

using namespace std::placeholders;

namespace te
{
    Script::Script()
        : _nativeScript(nullptr)
        , _state(0)
    { 
        gScriptManager().RegisterScript(this);
    }

    Script::~Script()
    {
        SetNativeScript(String(), HSceneObject());
        gScriptManager().UnregisterScript(this);
    }

    SPtr<Script> Script::CreateEmpty()
    {
        Script* script = new (te_allocate<Script>()) Script();
        SPtr<Script> scriptPtr = te_core_ptr<Script>(script);
        scriptPtr->SetThisPtr(scriptPtr);

        return scriptPtr;
    }

    SPtr<Script> Script::Create(const String& name, const HSceneObject& sceneObject)
    {
        SPtr<Script> scriptPtr = CreateEmpty();
        scriptPtr->Initialize();
        scriptPtr->SetNativeScript(name, sceneObject);

        return scriptPtr;
    }

    void Script::SetNativeScript(const String& name, const HSceneObject& sceneObject, const String& path)
    {
        SetNativeScript(ScriptIdentifier(name, path), sceneObject);
    }

    void Script::SetNativeScript(const ScriptIdentifier& identifier, const HSceneObject& sceneObject)
    {
        auto SetScriptTask = [this, identifier, sceneObject]() {
            // Lock lock(this->_mutex);

            OnShutdown();

            if(!identifier.Name.empty())
            {
                NativeScript* nativeScript = gScriptManager().CreateNativeScript(identifier);

                if (nativeScript)
                {
                    this->_nativeScript = nativeScript;
                    this->_nativeScript->SetLibraryName(identifier.Name);
                    this->_nativeScript->SetLibraryPath(identifier.AbsolutePath);
                    this->_nativeScript->SetParentSceneObject(sceneObject);
                }
            }
            else if (this->_nativeScript)
            {
                gScriptManager().DeleteNativeScript(this->_nativeScript);
                this->_nativeScript = nullptr;
            }
        };

        auto SetScriptCallback = [this]() {
            this->OnStartup();
        };


        if (identifier.Name != "")
        {
            SetScriptTask();
            SetScriptCallback();

            //SPtr<Task> task = Task::Create("SetNativeScript", SetScriptTask, SetScriptCallback);
            //gTaskScheduler().AddTask(task); TODO rewrite ScriptManager
        }
        else
        {
            SetScriptTask();
            SetScriptCallback();
        }
    }

    void Script::OnStartup()
    {
        if (_state &= (UINT32)ScriptState::Started)
            return;

        _state |= (UINT32)ScriptState::Started;

        if (_nativeScript)
            _nativeScript->OnStartup();
    }

    void Script::OnShutdown()
    {
        if (!(_state &= (UINT32)ScriptState::Started))
            return;

        _state &= ~(UINT32)ScriptState::Started;

        if (_nativeScript)
            _nativeScript->OnShutdown();
    }

    void Script::OnDisabled()
    {
        _state &= ~(UINT32)ScriptState::Enabled;

        if (_nativeScript)
            _nativeScript->OnDisabled();
    }

    void Script::OnEnabled()
    {
        _state |= (UINT32)ScriptState::Enabled;

        if (_nativeScript)
            _nativeScript->OnEnabled();
    }

    void Script::PreUpdate()
    {
        if (_nativeScript)
            _nativeScript->PreUpdate();
    }

    void Script::Update()
    {
        if (_nativeScript)
            _nativeScript->Update();
    }

    void Script::PostUpdate()
    {
        if (_nativeScript)
            _nativeScript->PostUpdate();
    }

    void Script::PostRender()
    {
        if (_nativeScript)
            _nativeScript->PostRender();
    }
}
