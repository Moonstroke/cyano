#!/bin/bash

function install_dep() {
	git clone $2 $1
	( cd $1 && make install )
}

function clean_dep() {
	rm -rfv "$1"
}

install_dep 'log' 'https://github.com/Moonstroke/C-log.git'
clean_dep 'log'