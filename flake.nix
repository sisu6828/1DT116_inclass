{
  description = "dev shell for pedsim (converted from shell.nix)";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = {
    self,
    nixpkgs,
    flake-utils,
    ...
  }:
    flake-utils.lib.eachDefaultSystem (system: let
      pkgs = import nixpkgs {
        inherit system;
        config.allowUnfree = true;
      };
    in {
      devShells.default = pkgs.mkShell rec {
        buildInputs = with pkgs; [
          libgcc
          zlib
          gcc
          cmake
          pkg-config
          libxkbcommon
          libGL
          wayland
          xorg.libXcursor
          xorg.libXrandr
          xorg.libXi
          xorg.libX11
          llvmPackages.openmp
          tinyxml-2
          python313Packages.matplotlib
          python313Packages.numpy
          bear
          just
          gdb

          # CUDA support
          cudaPackages.cuda_cudart
          cudatoolkit
          cudaPackages.cudnn
        ];

        LD_LIBRARY_PATH = "${pkgs.stdenv.cc.cc.lib}/lib/:${
          pkgs.lib.makeLibraryPath buildInputs
        }:$LD_LIBRARY_PATH";

        # NOTE: One probably don't need the if statments since nix garantees that we have the
        # packages in buildInputs, but keeping it for (now) as a "sanity" check.
        shellHook = ''

          if command -v nvcc 2>&1 /dev/null; then
              echo "Setting up CUDA environment variables"
              export CUDA_HOME=${pkgs.cudatoolkit}
              export PATH=${pkgs.cudatoolkit}/bin:$PATH
              export LD_LIBRARY_PATH=${pkgs.cudatoolkit}/lib64:$LD_LIBRARY_PATH
              echo "CUDA_AVAILABLE=1" > ./demo/config.mk
              ln -f ./libpedsim/Makefile.cuda ./libpedsim/Makefile;
          else
              echo "CUDA not found, disabling CUDA support"
              echo "CUDA_AVAILABLE=" > ./demo/config.mk
              ln -f ./libpedsim/Makefile.nocuda ./libpedsim/Makefile
          fi

              echo "Qt5 not detected"
              ln -f ./demo/Makefile.noqt ./demo/Makefile;
        '';
      };
    });
}
