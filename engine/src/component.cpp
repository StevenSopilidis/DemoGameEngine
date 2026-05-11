#include "component.h"

#include "game_object.h"

namespace engine
{
size_t Component::next_id_{0};

GameObject* Component::Owner() const { return owner_; }
} // namespace engine