#pragma once

namespace Game::Graphics::_misc {

    struct _Enums_BindTargets {
        using IntType = unsigned int;

        constexpr _Enums_BindTargets();
        constexpr _Enums_BindTargets(IntType _VertexBuffer,
                                     IntType _ElementBuffer,
                                     IntType _UniformBuffer);

        IntType VertexBuffer{}, ElementBuffer{}, UniformBuffer{};
    };

    struct _Enums_AllocationTypes {
        using IntType = unsigned int;

        constexpr _Enums_AllocationTypes();
        constexpr _Enums_AllocationTypes(IntType _Streaming,
                                         IntType _Drawing,
                                         IntType _Reading);

        IntType Streaming{}, Drawing{}, Reading{};
    };

    struct _Namespace_Buffer {
        virtual void CopyToBuffer(_Enums_BindTargets::IntType BindTarget,
                                  size_t Size,
                                  const void* Data,
                                  _Enums_AllocationTypes::IntType AllocateFor);

        virtual void CopyToBufferPointer(_Enums_BindTargets::IntType BindTarget,
                                         int Offset,
                                         size_t Size,
                                         const void* Data);
        virtual void AllocateBuffer(
            _Enums_BindTargets::IntType BindTarget,
            size_t Size,
            _Enums_AllocationTypes::IntType AllocateFor);
    };
}

constexpr Game::Graphics::_misc::_Enums_BindTargets::_Enums_BindTargets() =
    default;
constexpr Game::Graphics::_misc::_Enums_BindTargets::_Enums_BindTargets(
    IntType _VertexBuffer,
    IntType _ElementBuffer,
    IntType _UniformBuffer)
    : VertexBuffer(_VertexBuffer),
      ElementBuffer(_ElementBuffer),
      UniformBuffer(_UniformBuffer) {}

constexpr Game::Graphics::_misc::_Enums_AllocationTypes::
    _Enums_AllocationTypes() = default;
constexpr Game::Graphics::_misc::_Enums_AllocationTypes::_Enums_AllocationTypes(
    IntType _Streaming,
    IntType _Drawing,
    IntType _Reading)
    : Streaming(_Streaming), Drawing(_Drawing), Reading(_Reading) {}

namespace Game::Graphics {
    extern Game::Graphics::_misc::_Enums_BindTargets BindTargets;
    extern Game::Graphics::_misc::_Enums_AllocationTypes AllocationTypes;
}