#include "animation_component.h"

#include "game_object.h"

#include <iostream>

namespace engine
{

void AnimationComponent::Update(float deltaTime)
{
    if (!clip_)
    {
        return;
    }

    if (!is_playing_)
    {
        return;
    }

    time_ += deltaTime;

    if (time_ > clip_->duration)
    {
        if (looping_)
        {
            time_ = std::fmod(time_, clip_->duration);
        }
        else
        {
            time_       = 0.0f;
            is_playing_ = false;
            return;
        }
    }

    for (auto& binding : bindings_)
    {
        auto& obj          = binding.first;
        auto& trackIndices = binding.second->track_indices;
        for (auto i : trackIndices)
        {
            auto& track = clip_->tracks[i];
            if (!track.positions.empty())
            {
                auto pos = Interpolate(track.positions, time_);
                obj->SetPosition(pos);
            }
            if (!track.rotations.empty())
            {
                auto rot = Interpolate(track.rotations, time_);
                obj->SetRotation(rot);
            }
            if (!track.scales.empty())
            {
                auto scale = Interpolate(track.scales, time_);
                obj->SetScale(scale);
            }
        }
    }
}

glm::vec3 AnimationComponent::Interpolate(const std::vector<KeyFrameVec3>& keys, float time)
{
    if (keys.empty())
    {
        return glm::vec3{0.0f};
    }

    if (keys.size() == 1)
    {
        return keys[0].value;
    }

    if (time <= keys.front().time)
    {
        return keys.front().value;
    }

    if (time >= keys.back().time)
    {
        return keys.back().value;
    }

    size_t i0{};
    size_t i1{};

    for (size_t i{1}; i < keys.size(); i++)
    {
        if (time <= keys[i].time)
        {
            i1 = i;
            break;
        }
    }

    i0 = i1 > 0 ? i1 - 1 : 0;

    if (time >= keys[i0].time && time <= keys[i1].time)
    {
        float deltaTime = keys[i1].time - keys[i0].time;
        float k         = (time - keys[i0].time) / deltaTime;

        return glm::mix(keys[i0].value, keys[i1].value, k);
    }

    return keys.back().value;
}

glm::quat AnimationComponent::Interpolate(const std::vector<KeyFrameQuat>& keys, float time)
{
    if (keys.empty())
    {
        return glm::quat{};
    }

    if (keys.size() == 1)
    {
        return keys[0].value;
    }

    if (time <= keys.front().time)
    {
        return keys.front().value;
    }

    if (time >= keys.back().time)
    {
        return keys.back().value;
    }

    size_t i0{};
    size_t i1{};

    for (size_t i{1}; i < keys.size(); i++)
    {
        if (time <= keys[i].time)
        {
            i1 = i;
            break;
        }
    }

    i0 = i1 > 0 ? i1 - 1 : 0;

    if (time >= keys[i0].time && time <= keys[i1].time)
    {
        float deltaTime = keys[i1].time - keys[i0].time;
        float k         = (time - keys[i0].time) / deltaTime;

        return glm::slerp(keys[i0].value, keys[i1].value, k);
    }

    return keys.back().value;
}

void AnimationComponent::SetClip(AnimationClip* clip)
{
    clip_ = clip;
    BuildBindings();
}

void AnimationComponent::RegisterClip(const std::string&                    name,
                                      const std::shared_ptr<AnimationClip>& clip)
{
    clips_[name] = clip;
}

void AnimationComponent::Play(const std::string& name, bool loop)
{
    if (clip_ && clip_->name == name)
    {
        time_       = 0.0f;
        is_playing_ = true;
        looping_    = loop;
    }
    else
    {
        auto it = clips_.find(name);
        if (it != clips_.end())
        {
            SetClip(it->second.get());
            time_       = 0.0f;
            is_playing_ = true;
            looping_    = loop;
        }
    }
}

void AnimationComponent::BuildBindings()
{
    bindings_.clear();
    if (!clip_)
    {
        return;
    }

    for (size_t i{}; i < clip_->tracks.size(); i++)
    {
        auto& track     = clip_->tracks[i];
        auto  targetObj = owner_->FindChildByName(track.target_name);

        if (targetObj)
        {
            auto it = bindings_.find(targetObj);
            if (it != bindings_.end())
            {
                it->second->track_indices.push_back(i);
            }
            else
            {
                auto binding    = std::make_unique<ObjectBinding>();
                binding->object = targetObj;
                binding->track_indices.push_back(i);
                bindings_.emplace(targetObj, std::move(binding));
            }
        }
    }
}

bool AnimationComponent::IsPlaying() const { return is_playing_; }

} // namespace engine