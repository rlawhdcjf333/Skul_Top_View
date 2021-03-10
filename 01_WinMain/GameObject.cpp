#include "pch.h"
#include "GameObject.h"

GameObject::GameObject()
	:mIsActive(true), mIsDestroy(false) {}

GameObject::GameObject(const string & name)
	: mName(name), mIsActive(true), mIsDestroy(false) {}
