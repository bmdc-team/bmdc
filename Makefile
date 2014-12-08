all:
	mkdir -p ${HOME}/BMDC/
	scons PREFIX=${HOME}/BMDC/ release=1
	scons install
Release:
	mkdir -p ${HOME}/BMDC/
	scons PREFIX=${HOME}/BMDC/ release=1
	scons install
Debug:
	mkdir -p ${HOME}/BMDCTEST/
	scons PREFIX=${HOME}/BMDCTEST/ debug=1 profile=0
	scons install
cleanrelease:
	scons -c
cleandebug:
	scons debug=1 -c
