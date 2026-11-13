#pragma once

#include "Base_VertexArray.hpp"

#include <citro3d.h>

namespace Game::Graphics::Classes {

    class VertexArray_N3DS : public VertexArray {
       public:
        virtual ~VertexArray_N3DS() override;

        C3D_AttrInfo attrInfo;

        virtual void Bind() override;
        virtual bool IsBound() const override;
    };
}

namespace Game::Graphics::VertexArray::Wrappers {
    Game::Graphics::Classes::VertexArray* Create_N3DS();

    extern Game::Graphics::Classes::VertexArray_N3DS* BoundVAO_N3DS;

    void CreateBulk_N3DS(size_t CreateAmount,
                         Game::Graphics::Classes::VertexArray* VertexArrays[]);

    void Unbind_N3DS();

    void AddFloatAttribute_N3DS(int AttributeIndex, size_t Count);
    void AddIntAttribute_N3DS(int AttributeIndex, size_t Count);
}