#pragma once

enum class AnimType {
    WalkHammer, UseHammer,
    WalkHatchet, UseHatchet,
    WalkPickaxe, UsePickaxe,
    WalkNone, UseHands,
    UseShovel, WalkShovel
};

enum class Direction { Up, Left, Down, Right };

struct AnimInfo {
    int baseRow;
    int frameCount;
};

struct Animation {
    AnimType type = AnimType::WalkNone;
    Direction dir = Direction::Down;
    int frame = 0;
    int frameCount = 9;
    float timer = 0.f;
};

const std::map<AnimType, AnimInfo> animInfos = {
    {AnimType::WalkHammer,   {0, 9}},
    {AnimType::UseHammer,    {4, 6}},
    {AnimType::WalkHatchet,  {8, 9}},
    {AnimType::UseHatchet,   {12, 6}},
    {AnimType::WalkPickaxe,  {16, 9}},
    {AnimType::UsePickaxe,   {20, 6}},
    {AnimType::WalkNone,     {24, 9}},
    {AnimType::UseHands,     {28, 6}},
    {AnimType::UseShovel,    {32, 8}},
    {AnimType::WalkShovel,   {36, 9}}
};