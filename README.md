# Firefly Fantasies

A captivating puzzle game set on a hexagonal board where players aim to connect fireflies of the same color to light up the night. As you match these luminous creatures, the tiles beneath them illuminate. Light up the entire board to summon a special firefly randomly at the top of the board, guiding it to the bottom in minimal moves for bonus points. With each level, the board size changes, not necessarily following a rule.

## Demo Animation

![Firefly Fantasies Animation](https://github.com/danielw98/FireflyFantasies/blob/main/FireflyFantasies.gif?raw=true)

## Getting Started

Clone the repository to get started:

```bash
git clone https://github.com/danielw98/FireflyFantasies.git
cd FireflyFantasies
```

## How to Play

- **Make Connections**: Aim to form the largest possible groups of same-colored fireflies.
- **Light Up Tiles**: Matching fireflies illuminate their tiles. Light them all to progress.
- **Rainbow Firefly**: Allows connections between fireflies of different types
- **Bomb Firefly**: Explodes an entire row and column of the board corresponding to its position, ligthing up the tiles beneath them.
- **Special Firefly**: Upon lighting all tiles, a unique firefly appears. Lead it to its destination efficiently for bonuses.
- **Level Up**: Complete the objective to advance, facing greater challenges.

## Build the Game

Utilize the provided Makefile for easy compilation and then run the executable:

```bash
make
./FireflyFantasies
```

Ensure you have the necessary SDL2 libraries installed on your system.

## Dive Into the Code

The codebase which includes the core gameplay logic is spread across the `src/` and `include/` directories, for readability and scalability of the Firefly Fantasies game:

- **Core Mechanics:** `Game.cpp` orchestrates gameplay flow, integrating all game components.
- **Rendering and UI:** `GUIManager.cpp` and `FontRenderer.cpp` manage graphical user interface elements, leveraging SDL2 for rendering.
- **Game Entities:** `Firefly.cpp` and `Board.cpp` define the game's primary entities, their behaviors, and interactions.
- **Utility and Management:** `TextureManager.cpp` handles the loading and management of textures, while `GlobalFunctions.cpp` provides utility functions.

Headers in `include/`, with categorizations such as `enums/` for enumerations like `FireflyType.h` and `structs/` for data structures like `GameContext.h`, organize the game's definitions and interfaces.

Explore these files to understand the game's implementation, from rendering techniques to the logic of the hexagonal board and firefly interactions. Assets for fireflies and other game elements are found under `assets/`.

## Contribute

Contributions are welcome! Whether it's new features, bug fixes, or optimizations, feel free to fork this repository, make your changes, and submit a pull request.

## License
This project is released under the MIT License. See the LICENSE file for more details.