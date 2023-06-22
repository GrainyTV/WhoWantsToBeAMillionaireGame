#ifndef DRAWABLESCENE_HPP
#define DRAWABLESCENE_HPP

class DrawableScene
{
private:

public:
	virtual void Draw() const = 0;

	virtual ~DrawableScene() = default;
};

#endif