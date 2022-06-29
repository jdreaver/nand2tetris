{
  inputs = {
    nixpkgs-unstable.url = "github:nixos/nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs-unstable }:
    let
      pkgs = import nixpkgs-unstable { system = "x86_64-linux"; config = { allowUnfree = true; }; };
    in {
      devShell.x86_64-linux = pkgs.mkShell {
        buildInputs = with pkgs; [
          # Java, to run nand2tetris tools
          jre

          # Rust
          cargo
          rustc
          clippy
          rustfmt
          rust-analyzer

          # C
          gcc
          gcc.man
          gdb
          ddd # Nice GUI debugger
          valgrind

          # ASM
          nasm

          # Python
          python3

          # Misc
          unzip
        ];
      };
    };
}