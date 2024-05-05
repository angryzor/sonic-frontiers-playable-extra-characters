# Sonic Frontiers Playable Extra Characters

This mod allows you to play with Amy, Knuckles and Tails in other gamemodes than "Another Story"!

## Setting up the development environment

You will need to have the following prerequisites installed:

* Visual Studio 2022
* CMake 3.28 or higher

Check out the project and make sure to also check out its submodules:

```sh
git clone --recurse-submodules https://github.com/angryzor/sonic-frontiers-playable-extra-characters.git
```

Now let CMake do its thing:

```sh
cmake -B build
```

If you have Sonic Frontiers installed in a non-standard location, you can specify that location
with the `GAME_FOLDER` variable:

```sh
cmake -B build -DGAME_FOLDER="C:\ShadowFrontiers"
```

Once CMake is finished, navigate to the build directory and open `playable-extra-characters.sln` with VS2022.
You should have a fully working environment available.

Building the INSTALL project will install the mod into HedgeModManager's `Mods` directory.
