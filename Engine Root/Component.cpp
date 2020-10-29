#include "Component.h"
#pragma once

Component::Component(ComponentType type, GameObject* owner) : type(type), owner(owner),active(true) {}

Component::~Component() {}

void Component::Enable()
{
	active = true;
}

void Component::Update()
{
}

void Component::Disable()
{
	active = false;
}

void Component::OnEditor()
{
}
