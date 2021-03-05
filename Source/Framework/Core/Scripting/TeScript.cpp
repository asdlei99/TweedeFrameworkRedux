#include "TeScript.h"

#include "TeScriptManager.h"
#include "TeNativeScript.h"
#include "Scene/TeSceneManager.h"

namespace te
{
    Script::Script()
        : _nativeScript(nullptr)
        , _state(ScriptState::Enabled)
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

    void Script::Initialize()
    {
        CoreObject::Initialize();
    }

    void Script::FrameSync()
    { }

    void Script::SetNativeScript(const String& name, const HSceneObject& sceneObject, const String& path)
    {
        SetNativeScript(ScriptIdentifier(name, path), sceneObject);
    }

    void Script::SetNativeScript(const ScriptIdentifier& identifier, const HSceneObject& sceneObject)
    {
        OnShutdown();

        if(!identifier.Name.empty())
        {
            NativeScript* nativeScript = gScriptManager().CreateNativeScript(identifier);

            if (nativeScript)
            {
                _nativeScript = nativeScript;
                _nativeScript->SetLibraryName(identifier.Name);
                _nativeScript->SetLibraryPath(identifier.AbsolutePath);
                _nativeScript->SetParentSceneObject(sceneObject);
            }
        }
        else if (_nativeScript)
        {
            gScriptManager().DeleteNativeScript(_nativeScript);
            _nativeScript = nullptr;
        }

        OnStartup();
    }

    void Script::OnStartup()
    {
        if (_nativeScript)
            _nativeScript->OnStartup();
    }

    void Script::OnShutdown()
    {
        if (_nativeScript)
            _nativeScript->OnShutdown();
    }

    void Script::OnDisabled()
    {
        _state = ScriptState::Disabled;

        if (_nativeScript)
            _nativeScript->OnDisabled();
    }

    void Script::OnEnabled()
    {
        _state = ScriptState::Enabled;

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
