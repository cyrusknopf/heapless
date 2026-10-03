{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
  nativeBuildInputs = with pkgs; [ gcc gnumake bear ];
  buildInputs = [ pkgs.gmp.dev pkgs.mpfr.dev pkgs.libmpc ];
}
