#!bin/bash

kill -9 $(pgrep -x test)
sudo killall -9 jackd

