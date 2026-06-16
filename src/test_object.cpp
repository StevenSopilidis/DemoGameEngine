#include "test_object.h"

#include "engine.h"
#include "material.h"
#include "mesh_component.h"

#include <iostream>

TestObject::TestObject()
{
    auto mesh     = engine::Mesh::Load("models/Suzanne.gltf");
    auto material = engine::Material::Load("materials/suzanne.mat.json");

    AddComponent(new engine::MeshComponent(material, mesh));
    SetPosition(glm::vec3(0.0f, 0.0f, -5.0f));
}

void TestObject::Update(float deltaTime) { engine::GameObject::Update(deltaTime); }