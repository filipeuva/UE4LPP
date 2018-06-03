#include "LivePPModule.h"
#include "ModuleManager.h"
#include "IPluginManager.h"
#include "CoreDelegates.h"

#include "LivePP_PCH.h"
#include "../API/LPP_API.h"
#include "AllowWindowsPlatformTypes.h"
#include "HideWindowsPlatformTypes.h"


#define LOCTEXT_NAMESPACE "FLivePPModule"

IMPLEMENT_MODULE(FLivePPModule, LivePP)
 
void FLivePPModule::StartupModule()
{
    // Get the base directory of this plugin
    FString baseDir    = IPluginManager::Get().FindPlugin("LivePP").IsValid() ? IPluginManager::Get().FindPlugin("LivePP")->GetBaseDir() : "";
    FString lppDllPath = FPaths::Combine(*baseDir, 
#ifdef PLATFORM_64BITS
        TEXT("x64/LPP_x64.dll")
#else
        TEXT("x86/LPP_x86.dll")
#endif
    );

    //Load DLL
    void* lppHModule = !lppDllPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*lppDllPath) : nullptr;
    if (!lppHModule)
    {
        return;
    }

    if (!lpp::lppCheckVersion(static_cast<HMODULE>(lppHModule)))
    {
        // version mismatch detected
        FPlatformProcess::FreeDllHandle(lppHModule);
        lppHModule = nullptr;
        return;
    }

    lpp::lppRegisterProcessGroup(static_cast<HMODULE>(lppHModule), "QuickStart");

    //Register all game modules
    {
        TArray<FModuleStatus> ModuleStatuses;
        FModuleManager::Get().QueryModules(ModuleStatuses);
        const FString gameBinaryDir = FPaths::ConvertRelativePathToFull(FModuleManager::Get().GetGameBinariesDirectory());

        for (const FModuleStatus& ModuleStatus : ModuleStatuses)
        {
            if (!ModuleStatus.bIsLoaded)
            {
                continue; 
            }

            const bool bShouldHook =
                   EnumHasAllFlags(moduelFilter, ELPPHookFilter::All)
                || EnumHasAllFlags(moduelFilter, ELPPHookFilter::Game)        && (ModuleStatus.bIsGameModule                                               )
                || EnumHasAllFlags(moduelFilter, ELPPHookFilter::GameProject) && (FPaths::IsSamePath(gameBinaryDir, FPaths::GetPath(ModuleStatus.FilePath)))
                || EnumHasAllFlags(moduelFilter, ELPPHookFilter::CoreEngine)  && (CoreEngineModuleNames.Contains(ModuleStatus.Name)                        )
                || EnumHasAllFlags(moduelFilter, ELPPHookFilter::CoreEditor)  && (CoreEditorModuleNames.Contains(ModuleStatus.Name)                        )
                || EnumHasAllFlags(moduelFilter, ELPPHookFilter::CustomList)  && (CustomModuleNames.Contains(ModuleStatus.Name)                            );

            if (bShouldHook)
            {
                if (bUseAsyncModuleLoad)
                {
                        if (bHookImports) { lppAsyncLoadTokens.Add(lpp::lppEnableAllModulesAsync(static_cast<HMODULE>(lppHModule), *ModuleStatus.FilePath)); }
                    else                  { lppAsyncLoadTokens.Add(lpp::lppEnableModuleAsync(static_cast<HMODULE>(lppHModule), *ModuleStatus.FilePath)); }
                }
                else
                {
                        if (bHookImports) { lpp::lppEnableAllModulesSync(static_cast<HMODULE>(lppHModule), *ModuleStatus.FilePath); }
                    else                  { lpp::lppEnableModuleSync(static_cast<HMODULE>(lppHModule), *ModuleStatus.FilePath); }
                }
            }
        }
    }
}
 
void FLivePPModule::ShutdownModule()
{
}
 