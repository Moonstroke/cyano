#!/bin/bash

function install_git_repo() {
	git clone -v "$2" "$1"
	( cd "$1" && make install )
}

function clean_git_repo() {
	rm -rfv "$1"
}


# Refreshing the APT cache doesn't hurt
apt-get update

# We obviously need the SDL installed
apt-get install -y libasound2-dev libdbus-1-dev libegl1-mesa-dev libgl1-mesa-dev\
                   libgles2-mesa-dev libglu1-mesa-dev libibus-1.0-dev\
                   libmirclient-dev libpulse-dev libsdl2-2.0-0 libsndio-dev\
                   libudev-dev libwayland-dev libx11-dev libxcursor-dev\
                   libxext-dev libxi-dev libxinerama-dev libxkbcommon-dev\
                   libxrandr-dev libxss-dev libxt-dev libxv-dev libxxf86vm-dev\
                   libsdl2-dev

install_git_repo 'log' 'https://github.com/Moonstroke/C-log.git'
clean_git_repo 'log'
