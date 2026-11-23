#include "Listener.hpp"

#include "../../../MathClasses/Vector3.hpp"

namespace Game::Sound::Classes {

    void ListenerStruct::SetPosition(const Math::Vector3& Position) {}
    void ListenerStruct::SetVelocity(const Math::Vector3& Velocity) {}
    void ListenerStruct::SetOrientation(const Math::Vector3& Look,
                                        const Math::Vector3& Up) {}

}

namespace Game::Sound {
    Game::Sound::Classes::ListenerStruct Listener;
}