#pragma once

struct SceneResolver
{
    template<typename T>
    void operator()(const T& scene) const
    {
        scene.redraw();
    }   
};

class MainMenuScene
{
private:

public:
    void redraw() const;
};

class InGameScene
{
private:

public:
    void redraw() const;
};