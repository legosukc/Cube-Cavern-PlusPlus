#include "N3DS_BufferBase.hpp"

Game::Graphics::Classes::BufferBase_N3DS::~BufferBase_N3DS() {
    if (this->BufferData == NULL) {
        return;
    }
    linearFree(this->BufferData);
}

void Game::Graphics::Classes::BufferBase_N3DS::Bind() {
    C3D_SetBufInfo(&this->bufInfo);
}

Game::Graphics::Classes::BufferBase*
    Game::Graphics::BufferBase::WrapperTemplates::Create_N3DS() {
    Game::Graphics::Classes::BufferBase_N3DS* NewBuffer =
        new Game::Graphics::Classes::BufferBase_N3DS;

    BufInfo_Init(&NewBuffer->bufInfo);

    return reinterpret_cast<Game::Graphics::Classes::BufferBase*>(NewBuffer);
}

void Game::Graphics::BufferBase::WrapperTemplates::CreateBulk_N3DS(
    size_t CreateAmount,
    Game::Graphics::Classes::BufferBase* Buffers[]) {
    for (size_t i = 0; i < CreateAmount; ++i) {
        Buffers[i] = reinterpret_cast<Game::Graphics::Classes::BufferBase*>(
            new Game::Graphics::Classes::BufferBase_N3DS());

        BufInfo_Init(&Buffers[i]->bufInfo);
    }
}

void Game::Graphics::BufferBase::WrapperTemplates::Unbind_N3DS() {
    //BufInfo_Init(C3D_GetBufInfo());
    C3D_SetBufInfo(C3D_GetBufInfo());
}