using System;
using System.Collections.Generic;
using FlaxEngine;
using FlaxEngine.GUI;
using FlaxEditor;

namespace Game
{
    public class SciterControl : Control
    {
        bool IsInitialized = false;

        SciterView sciterView;
    
        public override void Draw()
        {
            base.Draw();

            if (!IsInitialized)
            {
                sciterView = new SciterView();

                sciterView.InitializeSCITER();

                IsInitialized = true;
            }

            if (IsInitialized)
            {
                sciterView.RenderSCITER();
            }

        }

        public override void OnDestroy()
        {
            if (IsInitialized)
            {
                sciterView.DestroySCITER();
            }

            base.OnDestroy();
        }
    }
}
