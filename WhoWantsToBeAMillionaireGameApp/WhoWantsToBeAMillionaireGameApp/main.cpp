#include <locale>
#include "megjelenites.h"

int main(int argc, char* args[])
{
    srand((unsigned int)(time(NULL)));
    setlocale(LC_ALL, "hu_HU.utf8");
    
    Adatok a1 = Adatok();
    Jatek j1 = Jatek(a1);
    Megjelenites m1 = Megjelenites();

    while(j1.isrunning())
    {
        while(SDL_PollEvent(m1.addressOfEvent()))
        {
            switch(m1.typeOfEvent())
            {
                case SDL_QUIT:
                    j1.terminateRunning();
                    m1.freeResources(j1);
                    break;
                case SDL_MOUSEMOTION:
                    m1.mousepos(j1);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    m1.mouseclick(j1, a1, m1);
                    break;
            }
        }

        m1.screen_renderer(j1, a1);
        SDL_RenderPresent(m1.addressOfRenderer());
    }

    return 0;
}