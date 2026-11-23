#pragma once

#include "Base_BufferBase.hpp"

#include <citro3d.h>

namespace Game::Graphics::Classes {
    
    class BufferBase_N3DS : public BufferBase {
       public:
        virtual ~BufferBase_N3DS() override;

        virtual void Bind() override;

        C3D_BufInfo bufInfo;
        void* BufferData = NULL;
    };
}

namespace Game::Graphics::BufferBase::WrapperTemplates {

    Game::Graphics::Classes::BufferBase* Create_N3DS();

    void CreateBulk_N3DS(size_t CreateAmount,
                         Game::Graphics::Classes::BufferBase* Buffers[]);

    void Unbind_N3DS();
}