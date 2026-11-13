#include "N3DS_VertexArray.hpp"

#include "Base_VertexArray.hpp"

namespace Game::Graphics::VertexArray::Wrappers {
    Game::Graphics::Classes::VertexArray_N3DS* BoundVAO_N3DS = NULL;
}

Game::Graphics::Classes::VertexArray*
    Game::Graphics::VertexArray::Wrappers::Create_N3DS() {
    Game::Graphics::Classes::VertexArray_N3DS* NewVertexArray =
        new Game::Graphics::Classes::VertexArray_N3DS();

    AttrInfo_Init(&NewVertexArray->attrInfo);

    return reinterpret_cast<Game::Graphics::Classes::VertexArray*>(
        NewVertexArray);
}

void Game::Graphics::VertexArray::Wrappers::CreateBulk_N3DS(
    size_t CreateAmount,
    Game::Graphics::Classes::VertexArray* VertexArrays[]) {
    for (size_t i = 0; i < CreateAmount; ++i) {
        VertexArrays[i] =
            reinterpret_cast<Game::Graphics::Classes::VertexArray*>(
                new Game::Graphics::Classes::VertexArray_N3DS());

        AttrInfo_Init(
            &reinterpret_cast<Game::Graphics::Classes::VertexArray_N3DS*>(
                 VertexArrays)[i]
                 .attrInfo);
    }
}

void Game::Graphics::VertexArray::Wrappers::Unbind_N3DS() {
    C3D_SetAttrInfo(C3D_GetAttrInfo());
    Wrappers::BoundVAO_N3DS = NULL;
}

void Game::Graphics::VertexArray::Wrappers::AddFloatAttribute_N3DS(
    int AttributeIndex,
    size_t Count) {
    AttrInfo_AddLoader(&Wrappers::BoundVAO_N3DS->attrInfo, AttributeIndex, GPU_FLOAT, Count);
}

void Game::Graphics::VertexArray::Wrappers::AddIntAttribute_N3DS(
    int AttributeIndex,
    size_t Count) {}

Game::Graphics::Classes::VertexArray_N3DS::~VertexArray_N3DS() {}

void Game::Graphics::Classes::VertexArray_N3DS::Bind() {
    C3D_SetAttrInfo(&this->attrInfo);
    Game::Graphics::VertexArray::Wrappers::BoundVAO_N3DS = this;
}

bool Game::Graphics::Classes::VertexArray_N3DS::IsBound() const {
    return Game::Graphics::VertexArray::Wrappers::BoundVAO_N3DS == this;
}