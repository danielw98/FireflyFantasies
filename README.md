# Firefly Fantasies

A captivating puzzle game set on a hexagonal board where players aim to connect fireflies of the same color to light up the night. As you match these luminous creatures, the tiles beneath them illuminate. Light up the entire board to summon a special firefly, guiding it to its goal in minimal moves for bonus points. With each level, the challenge intensifies, offering endless entertainment.

## Demo animation

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

Explore `game.cpp` for the game's core logic, accompanied by SDL2 for rendering and interaction handling. Assets for fireflies and other game elements are found under assets/.

## Contribute

Contributions are welcome! Whether it's new features, bug fixes, or optimizations, feel free to fork this repository, make your changes, and submit a pull request.

## License
This project is released under the MIT License. See the LICENSE file for more details.