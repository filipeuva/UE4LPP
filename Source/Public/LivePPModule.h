#pragma once
 
#include "ModuleManager.h"
#include "AllowWindowsPlatformTypes.h"
#include "HideWindowsPlatformTypes.h"
 
enum class ELPPHookFilter : uint8
{
    None        = 0,
    Game        = 1,
    GameProject = 2,
    CoreEngine  = 4,
    CoreEditor  = 8,

    CustomList = 128,
    All        = 255,
};
ENUM_CLASS_FLAGS(ELPPHookFilter);

class FLivePPModule : public IModuleInterface
{
public:
     FLivePPModule()
    {
        CoreEngineModuleNames = {
            TEXT("Core"),
            TEXT("CoreUObject"),
            TEXT("Engine"),
            TEXT("RenderCore"),
            TEXT("ShaderCore"),
            TEXT("RHI"),
            TEXT("InputCore"),
        };

        CoreEditorModuleNames = {
            TEXT("UnrealEd"),
            TEXT("Sequencer"),
        };

        CustomModuleNames = {
            TEXT("Slate"),
            TEXT("SlateCore"),
            TEXT("ApplicationCore")
        };
    }


	/** IModuleInterface implementation */
	void StartupModule();
	void ShutdownModule();


private:
    void*	              lppHModule = nullptr;
    TArray<void*>         lppAsyncLoadTokens;
    ELPPHookFilter        moduelFilter          = ELPPHookFilter::Game | ELPPHookFilter::GameProject /*| ELPPHookFilter::CoreEngine*/;
    bool                  bHookImports          = true;
    bool                  bUseAsyncModuleLoad   = false;

    TArray<FString> CoreEngineModuleNames;
    TArray<FString> CoreEditorModuleNames;
    TArray<FString> CustomModuleNames;
};