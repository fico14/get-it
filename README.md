# get-it
Simple program that opens webpages with shitty authentication in Firefox. It can open up to 5 tabs in the same session.

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
  2) ./get_it

Install (in my case it'll be installed in directory that's in my $PATH and where I keep my local apps, but you can change location in the Makefile if you like):
  make install 

Usage: 
  Just run it without any arguments.
