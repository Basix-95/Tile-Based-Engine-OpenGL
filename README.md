# OpenGL 2D Tile-Based Platformer Engine

This is a 2D platformer engine built from scratch using **C and OpenGL**.  
The project demonstrates how to create a simple tile-based game system without any external game frameworks.

---
---

## How It Works

### Tile-Based System

- The world is represented as a **grid of tiles**, where each tile corresponds to a small square of the game map.
- Tiles are stored in **2D arrays**, with each value representing a tile type (e.g., ground, platform, obstacle).
- Each tile type has an associated **texture** loaded via `stb_image.h`.
- The renderer loops through the grid and draws only the visible tiles using **OpenGL quads**.
- This system allows easy expansion of maps by simply editing the 2D array or map file.

### Game Flow

1. **Initialization**:  
   - Load OpenGL context, shaders, and textures.  
   - Initialize player, tiles, and other game objects.

2. **Game Loop**:  
   - **Input**: Capture player movements.  
   - **Update**: Apply physics, collisions, and tile interactions.  
   - **Render**: Draw the visible part of the map, player, and other objects.

3. **Modular Design**:  
   - `game/` handles game logic.  
   - `player/` handles input and player state.  
   - `renderer/` handles OpenGL drawing.  
   - `tileset/` handles map loading, tile definitions, and collisions.

---

## Building the Project
**Linux / macOS**:
make
./platformer

**Windows (with MinGW or MSYS2)**:
build.bat
platformer.exe

make
./platformer
