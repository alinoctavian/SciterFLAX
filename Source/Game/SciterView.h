
#pragma once

#include "Engine/Scripting/Script.h"
#include "Engine/Render2D/Render2D.h"
#include "Engine/Core/Log.h"

#include "Engine/Engine/Screen.h"
#include "Engine/Graphics/Textures/GPUTexture.h"
#include <Engine/Core/Math/Rectangle.h>
#include "Engine/Engine/Time.h"

#include <d3d11.h>

#include "Sciter/include/sciter-x.h"
#include "Sciter/include/sciter-x-host-callback.h"
#include "Sciter/include/sciter-x-request.hpp"
#include "Sciter/include/sciter-x-dom.hpp"
#include "Sciter/include/sciter-x-behavior.h"

#define SciterInstanceId ((void *)1)

API_CLASS() class GAME_API SciterView : public Script
{
DECLARE_SCRIPTING_TYPE(SciterView);
    
    GPUTexture* texture;
    bool IsInitialized = false;
    IDXGISurface* g_pSurface = NULL; // for SCITER

    // [Script]
    void OnEnable() override;
    void OnDisable() override;
    void OnUpdate() override;

    API_FUNCTION() void InitializeSCITER();
    API_FUNCTION() void RenderSCITER();
    API_FUNCTION() void DestroySCITER();
    
    static  UINT SC_CALLBACK SciterCallback(LPSCITER_CALLBACK_NOTIFICATION pns, LPVOID callbackParam);
    static UINT DoLoadData(LPSCN_LOAD_DATA pnmld);
    static   UINT DoAttachBehavior(LPSCN_ATTACH_BEHAVIOR lpab);
    static UINT on_invalidate_rect(LPSCN_INVALIDATE_RECT pnm);
};
