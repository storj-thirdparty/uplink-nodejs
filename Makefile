# Go parameters
GOCMD=go
GOBUILD=$(GOCMD) build
GOCLEAN=$(GOCMD) clean
GOTEST=$(GOCMD) test
GOGET=$(GOCMD) get
# Uplink-c
GIT_REPO=https://github.com/storj/uplink-c
UPLINKC_NAME=uplink-c
UPLINKC_VERSION=v1.0.5
#Library Name
LIBRARY_NAME_LINUX=libuplinkc.so
LIBRARY_NAME_DARWIN=libuplinkc.dylib
LIBRARY_NAME_WIN=libuplinkc.dll
LIBRARY_UPLINK=*.h
DELETE_LIBRARY_HEADER=libuplinkc.h
#JSFOLDER
JSFOLDER=dist
JSFOLDERLIB=libuplinkc.*
#Color
RED_COLOR=\033[31m
GREEN_COLOR=\033[32m
RESET_COLOR=\033[0m
#
build:
ifdef OS
	git clone ${GIT_REPO}
	(cd ${UPLINKC_NAME}) && (${GOBUILD} -o ../${LIBRARY_NAME_WIN} -buildmode=c-shared) && (move ${LIBRARY_UPLINK} ../)
	rmdir /s ${UPLINKC_NAME}
else
	echo "$(shell uname)";\
     if [ ! -d $(UPLINKC_NAME) ]; then\
      git clone $(GIT_REPO);\
     fi;\
     if [ $(shell uname) = Darwin ]; then\
      cd $(UPLINKC_NAME);$(GOBUILD) -o ../$(LIBRARY_NAME_DARWIN) -buildmode=c-shared;mv $(LIBRARY_UPLINK) ../;cd ../;cp ./$(JSFOLDERLIB) ./$(JSFOLDER)/;\
     fi;\
     if [ $(shell uname) = Linux ]; then\
      cd $(UPLINKC_NAME);$(GOBUILD) -o ../$(LIBRARY_NAME_LINUX) -buildmode=c-shared;mv $(LIBRARY_UPLINK) ../;cd ../;cp ./$(JSFOLDERLIB) ./$(JSFOLDER)/;\
     fi;\
  if test -d ./$(UPLINKC_NAME); then rm -rf ./$(UPLINKC_NAME); fi;\
  echo ' $(GREEN_COLOR) \n Successfully build $(RESET_COLOR)';
endif
clean:
ifdef OS
	(IF EXIST ${LIBRARY_UPLINK}; (del "${LIBRARY_UPLINK}")) && (IF EXIST ${DELETE_LIBRARY_HEADER}; (del "${DELETE_LIBRARY_HEADER}")) && (IF EXIST ${LIBRARY_NAME_WIN}; (del "${LIBRARY_NAME_WIN}")) && (IF EXIST ${UPLINKC_NAME}; (rmdir /s "${UPLINKC_NAME}"))
else
	if test -d $(UPLINKC_NAME); then rm -rf $(UPLINKC_NAME); fi
	if test -f ./$(LIBRARY_UPLINK); then rm ./$(LIBRARY_UPLINK); fi;\
  if test -f ./$(DELETE_LIBRARY_HEADER); then rm ./$(DELETE_LIBRARY_HEADER); fi;\
    if [ $(shell uname) = Darwin ]; then\
      if test -f ./$(LIBRARY_NAME_DARWIN); then rm ./$(LIBRARY_NAME_DARWIN); fi;\
     fi;\
     if [ $(shell uname) = Linux ]; then\
      if test -f $(LIBRARY_NAME_LINUX); then rm $(LIBRARY_NAME_LINUX); fi;\
     fi;
endif
	@echo ' $(GREEN_COLOR) \n Successfully cleaned $(RESET_COLOR)';
