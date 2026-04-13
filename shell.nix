{
  pkgs ? import <nixpkgs> { },
}:

pkgs.mkShell {
  buildInputs = with pkgs; [
    SDL2
    SDL2_image
    SDL2_ttf
    SDL2_mixer
    gcc
    cmake
    pkg-config
    valgrind
  ];

  shellHook = ''
    echo "Chip-8 dev environment ready"
    echo "SDL2:    g++ main.cpp -o app \$(sdl2-config --cflags --libs)"
  '';
}
