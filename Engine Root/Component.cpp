#include "Component.h"
#pragma once

Component::Component(): type(ComponentType::None),active(true),owner(nullptr) {}

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
