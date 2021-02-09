
#include "SciterView.h"

bool CanRender = false;

SciterView::SciterView(const SpawnParams& params)
    : Script(params)
{
    // Enable ticking OnUpdate function
    _tickUpdate = true;

   
}

void SciterView::OnEnable()
{
    // Here you can add code that needs to be called when script is enabled (eg. register for events)

    /*Render2D::DrawTe*/
}

void SciterView::OnDisable()
{
    // Here you can add code that needs to be called when script is disabled (eg. unregister from events)
}

void SciterView::OnUpdate()
{
   

}


API_FUNCTION() void SciterView::InitializeSCITER()
{
    texture = GPUTexture::New();

    auto textureDescription = GPUTextureDescription::New2D(Screen::GetSize().X, Screen::GetSize().Y, PixelFormat::B8G8R8A8_Typeless);

    if (texture->Init(textureDescription)) {
        LOG(Info, "<SciterAPI>Texture is created");
    }

    auto D3D11Texture = (ID3D11Texture3D*)texture->GetNativePtr();
 
    if (SUCCEEDED(D3D11Texture->QueryInterface(__uuidof(IDXGISurface), (void**)&g_pSurface))) {
        LOG(Info, "<SciterAPI>Got the DXGISurface");
    }

    SciterSetOption(NULL, SCITER_SET_SCRIPT_RUNTIME_FEATURES,
        ALLOW_FILE_IO |
        ALLOW_SOCKET_IO |
        ALLOW_EVAL |
        ALLOW_SYSINFO);

    SciterSetOption(NULL, SCITER_SET_DEBUG_MODE, true);

    bool ok;

    ok = SciterProcX(SciterInstanceId, SCITER_X_MSG_CREATE(GFX_LAYER_D2D, FALSE));

    if (ok) {
        LOG(Info, "<SciterAPI>Engine created");
    }

    SciterSetCallback(SciterInstanceId, SciterCallback, NULL);

    SciterProcX(SciterInstanceId, SCITER_X_MSG_RESOLUTION(UINT(100)));

    ok = SciterProcX(SciterInstanceId, SCITER_X_MSG_SIZE(Screen::GetSize().X, Screen::GetSize().Y));

    if (!ok) {
        LOG(Info, "<SciterAPI::ERROR> Couldn't SCITER_X_MSG_SIZE ");
    }

    ok = SciterLoadFile(SciterInstanceId, L"C:\\dev\\sciter-sdk\\samples\\menu\\menu-bar.htm");

    if (!ok) {
        LOG(Info, "<SciterAPI::ERROR> Failed to load file ");
    }

    OleInitialize(NULL);

    SciterProcX(SciterInstanceId, SCITER_X_MSG_FOCUS(true));

    IsInitialized = true;
}

void SciterView::RenderSCITER()
{
    if (IsInitialized) {
         SciterProcX(SciterInstanceId, SCITER_X_MSG_HEARTBIT(Time::GetCurrentSafe()->DeltaTime.GetTotalSeconds()));

   
        if (CanRender) {
            CanRender = false;
            SCITER_X_MSG_PAINT msg_paint_back(NULL, TRUE);
            msg_paint_back.targetType = SPT_SURFACE;
            msg_paint_back.target.pSurface = g_pSurface;

            BOOL rpaint = SciterProcX(SciterInstanceId, msg_paint_back);

            if (!rpaint)
                LOG(Error, "<SciterAPI::ERROR> Failed to render");

            auto rect = Rectangle::Rectangle(0, 0, Screen::GetSize().X, Screen::GetSize().Y);

            Render2D::DrawTexture(texture, rect);
        }
    }


}

void SciterView::DestroySCITER()
{
    if (IsInitialized) {
        SciterProcX(SciterInstanceId, SCITER_X_MSG_DESTROY());
    }

}

UINT SC_CALLBACK SciterView::SciterCallback(LPSCITER_CALLBACK_NOTIFICATION pns, LPVOID callbackParam)
{
    switch (pns->code)
    {
    case SC_LOAD_DATA:          return DoLoadData((LPSCN_LOAD_DATA)pns);
    case SC_ATTACH_BEHAVIOR:    return DoAttachBehavior((LPSCN_ATTACH_BEHAVIOR)pns);
    case SC_INVALIDATE_RECT: return on_invalidate_rect((LPSCN_INVALIDATE_RECT)pns);
    }
    return 0;
}

UINT SciterView::DoLoadData(LPSCN_LOAD_DATA pnmld)
{
    LPCBYTE pb = 0; UINT cb = 0;
    aux::wchars wu = aux::chars_of(pnmld->uri);
    if (wu.like(WSTR("res:*")))
    {
        // then by calling possibly overloaded load_resource_data method
        if (sciter::load_resource_data(GetModuleHandle(NULL), wu.start + 4, pb, cb))
            ::SciterDataReady(pnmld->hwnd, pnmld->uri, pb, cb);
    }
    else if (wu.like(WSTR("this://app/*"))) {
        // try to get them from archive (if any, you need to call sciter::archive::open() first)
        aux::bytes adata = sciter::archive::instance().get(wu.start + 11);
        if (adata.length)
            ::SciterDataReady(pnmld->hwnd, pnmld->uri, adata.start, adata.length);
    }
    return LOAD_OK;
}

UINT SciterView::DoAttachBehavior(LPSCN_ATTACH_BEHAVIOR lpab)
{
    sciter::event_handler* pb = sciter::behavior_factory::create(lpab->behaviorName, lpab->element);
    if (pb)
    {
        lpab->elementTag = pb;
        lpab->elementProc = sciter::event_handler::element_proc;
        return TRUE;
    }
    return FALSE;
}

UINT SciterView::on_invalidate_rect(LPSCN_INVALIDATE_RECT pnm)
{
    CanRender = true;

    return 0;
}
