#pragma once

#include "component.h"
#include "glm/ext/quaternion_float.hpp"
#include "glm/ext/vector_float3.hpp"

#include <memory>
#include <string>

namespace engine
{
struct KeyFrameVec3
{
    float     time{};
    glm::vec3 value{};
};

struct KeyFrameQuat
{
    float     time{};
    glm::quat value{1.0f, 0.0f, 0.0f, 0.0f};
};

struct TransformTrack
{
    std::string               target_name;
    std::vector<KeyFrameVec3> positions;
    std::vector<KeyFrameQuat> rotations;
    std::vector<KeyFrameVec3> scales;
};

struct AnimationClip
{
    std::string                 name;
    float                       duration{};
    bool                        looping{true};
    std::vector<TransformTrack> tracks;
};

struct ObjectBinding
{
    GameObject*         object{};
    std::vector<size_t> track_indices;
};

class AnimationComponent : public Component
{
    COMPONENT(AnimationComponent)
  public:
    void Update(float deltaTime) override;
    void SetClip(AnimationClip* clip);
    void RegisterClip(const std::string& name, const std::shared_ptr<AnimationClip>& clip);
    void Play(const std::string& name, bool loop = true);

    bool IsPlaying() const;

  private:
    void      BuildBindings();
    glm::vec3 Interpolate(const std::vector<KeyFrameVec3>& keys, float time);
    glm::quat Interpolate(const std::vector<KeyFrameQuat>& keys, float time);

    AnimationClip*                                                  clip_{};
    float                                                           time_{};
    bool                                                            looping_{true};
    bool                                                            is_playing_{false};
    std::unordered_map<std::string, std::shared_ptr<AnimationClip>> clips_;
    std::unordered_map<GameObject*, std::unique_ptr<ObjectBinding>> bindings_;
};
} // namespace engine