# get-it
Simple CLI program that opens webpages with shitty authentication in your default browser. Number of tabs that are opened is fixed (lazy in the begining of the project :) This limit should be removed in the future ...)

Prerequisites:
  1) gcc
  2) make
  3) libcurl4-openssl-dev
  4) and a Linux machine ... ;)

Build:
  make

Debug build:
  make debug

Run:
  1) make run
  2) ./get_it (or just get-it if it is placed in your PATH)

Install (by default it is installed in ~/.local/bin/):
  make install

Or you can install it on some other location (you will need sudo privileges for some directories):
  make INSTALL_DIR=<directory> install

Usage:
  Just run it without any arguments.
