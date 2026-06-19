#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>

class Vector4d {
 public:
  explicit Vector4d(float _x = 0, float _y = 0, float _z = 0, float _w = 0)
      : x{_x}, y{_y}, z{_z}, w{_w} {}

  float x;
  float y;
  float z;
  float w;
};

class Player {
 public:
  explicit Player() {};

  const Vector4d& position() const { return position_; }

  void move(float x, float y) {
    position_.x += x;
    position_.y += y;
  }

 private:
  Vector4d position_{};
};

void render(SDL_Renderer* renderer, const Player& player) {
  SDL_FRect rect{player.position().x - 16.f, player.position().y - 16.f, 32.f,
                 32.f};
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, &rect);
}

int main(int argc, char* argv[]) {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window* window = SDL_CreateWindow("SDL3 Test", 640, 480, 0);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

  Player player{};
  const float speed = 200.f;

  Uint64 prev = SDL_GetTicks();
  bool running = true;
  SDL_Event event;

  while (running) {
    Uint64 now = SDL_GetTicks();
    float delta = (now - prev) / 1000.f;
    prev = now;

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) running = false;
    }

    const bool* keys = SDL_GetKeyboardState(nullptr);

    if (keys[SDL_SCANCODE_W]) player.move(0, -speed * delta);
    if (keys[SDL_SCANCODE_S]) player.move(0, speed * delta);
    if (keys[SDL_SCANCODE_A]) player.move(-speed * delta, 0);
    if (keys[SDL_SCANCODE_D]) player.move(speed * delta, 0);

    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);

    render(renderer, player);

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
