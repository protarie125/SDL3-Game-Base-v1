#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>

#include <random>

std::random_device rd{};
std::mt19937 rng{rd()};
std::uniform_real_distribution<float> dist{0.f, 1.f};

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
  explicit Player() = default;

  const Vector4d& position() const { return position_; }

  void move(float x, float y) {
    position_.x += x;
    position_.y += y;
  }

  void clamp(float min_x, float max_x, float min_y, float max_y) {
    if (position_.x < min_x) position_.x = min_x;
    if (position_.x > max_x) position_.x = max_x;
    if (position_.y < min_y) position_.y = min_y;
    if (position_.y > max_y) position_.y = max_y;
  }

 private:
  Vector4d position_{};
};

class Enemy {
 public:
  explicit Enemy() = default;

  const Vector4d& position() const { return position_; }

  void move(float x, float y) {
    position_.x += x;
    position_.y += y;
  }

  void clamp(float min_x, float max_x, float min_y, float max_y) {
    if (position_.x < min_x) position_.x = min_x;
    if (position_.x > max_x) position_.x = max_x;
    if (position_.y < min_y) position_.y = min_y;
    if (position_.y > max_y) position_.y = max_y;
  }

  void update(float delta) {
    constexpr float speed = 200.f;

    direction_change_timer_ += delta;
    if (1 < direction_change_timer_) {
      direction_change_timer_ = 0.f;

      float dir_x = dist(rng) - 0.5f;
      float dir_y = dist(rng) - 0.5f;
      auto arg = std::sqrtf(dir_x * dir_x + dir_y * dir_y);
      if (0 < arg) {
        dir_x /= arg;
        dir_y /= arg;
      }

      velocity_.x = dir_x * speed;
      velocity_.y = dir_y * speed;
    }

    move(velocity_.x * delta, velocity_.y * delta);
  }

 private:
  Vector4d position_{};
  Vector4d velocity_{};
  float direction_change_timer_{};
};

void render(SDL_Renderer* renderer, const Player& player) {
  SDL_FRect rect{player.position().x - 16.f, player.position().y - 16.f, 32.f,
                 32.f};
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, &rect);
}

void render(SDL_Renderer* renderer, const Enemy& enemy) {
  SDL_FRect rect{enemy.position().x - 16.f, enemy.position().y - 16.f, 32.f,
                 32.f};
  SDL_SetRenderDrawColor(renderer, 200, 90, 90, 255);
  SDL_RenderFillRect(renderer, &rect);
}

void warning_enemy(const Player& player, const Enemy& enemy, SDL_Window* wind) {
  auto dx = player.position().x - enemy.position().x;
  auto dy = player.position().y - enemy.position().y;
  auto dist = std::sqrtf(dx * dx + dy * dy);

  if (dist < 150.f) {
    SDL_SetWindowTitle(wind, "ENEMY NEAR!");
  } else {
    SDL_SetWindowTitle(wind, "SDL3 Test");
  }
}

int main(int argc, char* argv[]) {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window* window = SDL_CreateWindow("SDL3 Test", 640, 480, 0);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

  Player player{};
  const float speed = 200.f;

  Enemy enemy{};

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

    player.clamp(16, 640 - 16, 16, 480 - 16);
    enemy.update(delta);
    enemy.clamp(16, 640 - 16, 16, 480 - 16);

    warning_enemy(player, enemy, window);

    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);

    render(renderer, player);
    render(renderer, enemy);

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
