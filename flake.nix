{
  description = "CS 4522 Project 1: Matrix";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs?ref=nixos-unstable";

  outputs =
    { self, nixpkgs }:
    let
      systems = [ "x86_64-linux" ];
      forSystems = func: nixpkgs.lib.genAttrs systems (system: func (import nixpkgs { inherit system; }));
    in
    {
      formatter = forSystems (pkgs: pkgs.nixfmt-tree.override { settings.formatter.nixfmt.options = [ "-w 120" ]; });

      devShells = forSystems (pkgs: {
        default = pkgs.mkShell {
          buildInputs = with pkgs; [
            clang-tools
            llvmPackages_latest.clang
          ];
        };
      });
    };
}
