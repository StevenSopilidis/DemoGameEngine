#include "test_object.h"

#include "engine.h"
#include "material.h"
#include "mesh_component.h"

#include <iostream>

TestObject::TestObject() {}

void TestObject::Update(float deltaTime) { engine::GameObject::Update(deltaTime); }