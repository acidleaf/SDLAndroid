#!/usr/local/bin/zsh
(cd android && ndk-build && ant debug && ant debug install)