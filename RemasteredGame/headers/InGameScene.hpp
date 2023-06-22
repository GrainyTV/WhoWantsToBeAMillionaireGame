#ifndef INGAMESCENE_HPP
#define INGAMESCENE_HPP

#include <initializer_list>
#include <stdexcept>
#include <format>
#include "DrawableScene.hpp"
#include "Primitive.hpp"
#include "Hexa.hpp"

using std::initializer_list;
using std::runtime_error;
using std::format;

class InGameScene : public DrawableScene
{
private:
	const Hexa question;

	const Hexa answer_a;
	
	const Hexa answer_b;
	
	const Hexa answer_c;
	
	const Hexa answer_d;

public:
	InGameScene();

	void Draw() const override;
};

#endif