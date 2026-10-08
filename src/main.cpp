#include <SDL3/SDL.h>
#include <iostream>
#include <string>
#include <stdexcept>
#include <random>
#include <vector>
#include <array>

void render(SDL_Renderer* my_ren, SDL_FRect shape){
    SDL_SetRenderDrawColor(my_ren, 0, 255, 0, 255);
    SDL_RenderFillRect(my_ren, &shape);
}

void apple(SDL_Renderer* my_ren, SDL_FRect shape){
    SDL_SetRenderDrawColor(my_ren, 255, 0, 0, 255);
    SDL_RenderFillRect(my_ren, &shape);
}

int main() {

    std::random_device rd;  
    std::mt19937 gen(rd());  
    std::uniform_int_distribution<int> dist(0, 39);
    std::random_device rd2;  
    std::mt19937 gen2(rd2());
    std::uniform_int_distribution<int> dist2(0, 23);

    int value1 = dist(gen);
    int value2 = dist2(gen2);

    float frametime = 1000.0f / 6.0f;  

    int width{1000};
    int height{600};

    // Initialize SDL3
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cout << "SDL failed to initialize: " << SDL_GetError() << std::endl;
    }

    SDL_Window * window{nullptr};
    SDL_Renderer * renderer{nullptr};

    window = SDL_CreateWindow("My SDL3 Window", width, height, 0);
    renderer = SDL_CreateRenderer(window, nullptr);

    if (window == nullptr){
        std::cout << "Window has Failed to init. ERROR: " << SDL_GetError() << std::endl;
    }

    //init snake
    int x_snake{0};
    int y_snake{200};
    int w_snake{25};
    int h_snake{25};

    int velx{25};
    int vely{0};

    std::vector<std::array<int,2>> positions{{x_snake, y_snake}};

    //init apple
    int x_app{25*value1};
    int y_app{25*value2};

    bool game_run{true};

    SDL_Event event;

    Uint32 last = SDL_GetTicks();

    while (game_run)
    {
        Uint32 frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                game_run = false;
            }
            
        }

        const bool * key_states = SDL_GetKeyboardState(NULL);

        if (key_states[SDL_SCANCODE_DOWN] && vely != -25) {    
            vely = 25;
            velx = 0;
        }
        else if (key_states[SDL_SCANCODE_UP] && vely != 25)
        {
            vely = -25;
            velx = 0;
        }
        else if (key_states[SDL_SCANCODE_LEFT] && velx != 25)
        {
            vely = 0;
            velx = -25;
        }
        else if (key_states[SDL_SCANCODE_RIGHT] && velx != -25)
        {
            vely = 0;
            velx = 25;
        }

        //Snake loop off screen
        if(x_snake > width){
            x_snake = 0;
        }
        if(x_snake < 0){
            x_snake = width;
        }
        if(y_snake > height){
            y_snake = 0;
        }
        if(y_snake < 0){
            y_snake = height;
        }

        //Move apple
        if(x_snake == x_app && y_snake == y_app){
            x_app = 25*dist(gen);
            y_app = 25*dist2(gen2);
        }

        Uint32 now = SDL_GetTicks();
        //float dt = (now-last) / 1000.0f;
        if(now - last >= frametime){
            x_snake += velx;
            y_snake += vely;

            last = now;

            positions.push_back({x_snake, y_snake});
            
            if(!(x_snake==x_app && y_snake==y_app))
            {
                positions.erase(positions.begin());
            }

            //check collision 
            if (positions.size() > 3){
                for (long long unsigned int i = 0; i < positions.size()-1; i++){
                    if (positions[i][0] == positions[positions.size()-1][0] &&
                        positions[i][1] == positions[positions.size()-1][1]){
                            game_run = false;
                        }
                }
            }
            
        }

        SDL_SetRenderDrawColor(renderer, 2, 2, 29, 255);
        SDL_RenderClear(renderer);
        SDL_FRect red_apple{x_app, y_app, 25, 25};
        apple(renderer, red_apple);
        for (long long unsigned int i = 0; i < positions.size(); i++){
            SDL_FRect square{positions[i][0], positions[i][1], w_snake, h_snake};
            render(renderer, square);
        }

        SDL_RenderPresent(renderer);
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();
}
