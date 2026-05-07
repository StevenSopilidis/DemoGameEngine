#include "component.h"

#include "game_object.h"

namespace engine
{
GameObject* Component::Owner() { return owner_; }
} // namespace engine