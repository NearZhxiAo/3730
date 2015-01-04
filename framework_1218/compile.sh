#!/bin/bash
make -C lib;
make -C test/ clean
make -C test/
make -C test/ install
