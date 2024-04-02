#include "gameObject.h"


lve::GameObject_lv::GameObject_lv(const unsigned int& id_t): id(id_t)
{
}

lve::GameObject_lv lve::GameObject_lv::createGameObject()
{
	static unsigned int c_id = 0;
	return GameObject_lv(c_id++);
}

unsigned int lve::GameObject_lv::getId() const
{
	return id;
}