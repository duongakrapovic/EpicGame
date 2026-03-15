#pragma once

// Kích thước tile gốc của tileset (pixel)
static constexpr int ORIGINAL_TILE_SIZE = 16;

// Scale toàn game
static constexpr int SCALE = 3;

// Tile size trong game (pixel thật)
static constexpr int TILE_SIZE = ORIGINAL_TILE_SIZE * SCALE;
