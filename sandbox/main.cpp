#include "roguepack/gui/gui.hpp"
#include "raymath.h"


void DrawGrid(Vector2 stride, f32 thickness, Color color) {
    f32 window_width = static_cast<f32>(GetScreenWidth());
    f32 window_height = static_cast<f32>(GetScreenHeight());
    for (i32 x = 0; x < window_width; x += static_cast<i32>(stride.x)) {
        f32 x_float = static_cast<f32>(x);
        DrawLineEx(Vector2{ x_float, 0 }, Vector2{ x_float, window_width }, thickness, color);
    }
    for (i32 y = 0; y < window_height; y += static_cast<i32>(stride.y)) {
        f32 y_float = static_cast<f32>(y);
        DrawLineEx(Vector2{ 0, y_float }, Vector2{ window_height, y_float }, thickness, color);
    }
}

static constexpr i32 WINDOW_WIDTH = 900;
static constexpr i32 WINDOW_HEIGHT = 900;
static constexpr Vector2 GRID_CELL_SIZE{90.f, 90.f};
static constexpr f32 ROUND_TIME_MAX = 3.0f;

class CanTakeTurn {
public:
    template<typename T>
    CanTakeTurn(const T& canTakeTurn)
        : object(&canTakeTurn),
          speed_impl([](const void* obj){ return static_cast<const T*>(obj)->speed; }) {}

    f32 speed() const {
        return speed_impl(object);
    }
private:
    const void* object;
    f32 (*speed_impl)(const void*);
};

class Player {
public:
    Vector2 position = { 0, 0 };
    f32 speed = 5;

    static Player& the() {
        return instance;
    }

    private:
        static Player instance;
        Player() {}
};

Player Player::instance{};

enum class PlayerAction {
    WAIT,
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
};

PlayerAction GetPlayerAction() {
    if (IsKeyPressed(KEY_UP)) {
        return PlayerAction::MOVE_UP;
    } else if (IsKeyPressed(KEY_DOWN)) {
        return PlayerAction::MOVE_DOWN;
    } else if (IsKeyPressed(KEY_LEFT)) {
        return PlayerAction::MOVE_LEFT;
    } else if (IsKeyPressed(KEY_RIGHT)) {
        return PlayerAction::MOVE_RIGHT;
    } else if (IsKeyPressed(KEY_SPACE)) {
        return PlayerAction::WAIT;
    }
}

void DrawPlayer() {
    static constexpr Vector2 palyer_padding = { 10, 10 };
    static constexpr Color player_color = GREEN;
    auto player_position = Player::the().position;
    DrawRectangle(static_cast<i32>(player_position.x + palyer_padding.x), static_cast<i32>(player_position.y + palyer_padding.y),
                  static_cast<i32>(GRID_CELL_SIZE.x - 2*palyer_padding.x), static_cast<i32>(GRID_CELL_SIZE.y - 2*palyer_padding.y), player_color);
}

class Animation {
public:
    virtual bool Update(f32 delta_time) = 0;
};

class MoveAnimation : public Animation {
    Vector2& position;
    Vector2 target_position;
    f32 duration_seconds;
    f32 elapsed_seconds;
public:
    MoveAnimation(Vector2& position, Vector2 target_position, f32 duration_seconds) 
        : position(position),
          target_position(target_position),
          duration_seconds(duration_seconds),
          elapsed_seconds(0) {}
    
    bool Update(f32 delta_time) override {
        elapsed_seconds += delta_time;
        if (elapsed_seconds >= duration_seconds) {
            position = target_position;
            return true;
        } else {
            f32 t = elapsed_seconds / duration_seconds;
            position = Vector2Lerp(position, target_position, t);
            return false;
        }
    }
};

Animation* animation = nullptr;

void Update(f32 delta_time, PlayerAction action) {
    
    if (animation != nullptr) {
        if (animation->Update(delta_time)) {
            delete animation;
            animation = nullptr;
        }
        return;
    }

    static constexpr f32 move_animation_duration = 0.3f;
    auto& player_position = Player::the().position;
    switch (action) {
        case PlayerAction::WAIT:
            break;
        case PlayerAction::MOVE_UP:
            animation = new MoveAnimation(player_position, Vector2Add(player_position, Vector2{ 0, -GRID_CELL_SIZE.y }), move_animation_duration);
            break;
        case PlayerAction::MOVE_DOWN:
            animation = new MoveAnimation(player_position, Vector2Add(player_position, Vector2{ 0, GRID_CELL_SIZE.y }), move_animation_duration);
            break;
        case PlayerAction::MOVE_LEFT:
            animation = new MoveAnimation(player_position, Vector2Add(player_position, Vector2{ -GRID_CELL_SIZE.x, 0 }), move_animation_duration);
            break;
        case PlayerAction::MOVE_RIGHT:
            animation = new MoveAnimation(player_position, Vector2Add(player_position, Vector2{ GRID_CELL_SIZE.x, 0 }), move_animation_duration);
            break;
        default:
            break;
    }
}

int main(int argc, char *argv[]) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "roguepack");
    SetTargetFPS(60);
    TraceLog(LOG_INFO, "Player speed: %f", CanTakeTurn(Player::the()).speed());
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawGrid(GRID_CELL_SIZE, 1, GRAY);
        
        PlayerAction action = GetPlayerAction();
        f32 delta_time = GetFrameTime();
        Update(delta_time, action);
        DrawPlayer();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}