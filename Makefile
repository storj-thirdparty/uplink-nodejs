# Go parameters
GOBUILD=go build
# Uplink-c
GIT_REPO=https://github.com/storj/uplink-c
UPLINKC=uplink-c
UPLINKC_VERSION=v1.7.0
#Library Name
LIBRARY_NAME_BASE=libuplinkc$(UPLINKC_VERSION)
LIBRARY_HEADER=$(LIBRARY_NAME_BASE).h
#JSFOLDER
JSFOLDER=dist
#Color
GREEN_COLOR=\033[32m
RESET_COLOR=\033[0m
# Install
DESTDIR=/usr/local
# OS specific variables
ifeq ($(OS),Windows_NT)
	LIBRARY_NAME = $(LIBRARY_NAME_BASE).dll
	RM := del
	CP := copy
else
	ifeq ($(shell uname -s),Darwin)
		LIBRARY_NAME = $(LIBRARY_NAME_BASE).dylib
	else
		LIBRARY_NAME = $(LIBRARY_NAME_BASE).so
	endif
	RM := rm -rf
	CP := cp
endif

uplink-c:
	git clone --depth 1 -b $(UPLINKC_VERSION) $(GIT_REPO)

.PHONY: build
build: uplink-c
	cd $(UPLINKC);$(GOBUILD) -o ../$(LIBRARY_NAME) -buildmode=c-shared;cd ../;
	$(CP) $(wildcard $(UPLINKC)/*.h) .
	$(CP) ./$(LIBRARY_NAME_BASE).* ./$(JSFOLDER)/;
	@echo ' $(GREEN_COLOR) \n Successfully build $(RESET_COLOR)';

.PHONY: clean
clean:
	$(RM) $(notdir $(wildcard $(UPLINKC)/*.h));
	$(RM) $(UPLINKC);
	$(RM) ./$(LIBRARY_HEADER);
	$(RM) ./$(LIBRARY_NAME);
	@echo ' $(GREEN_COLOR) \n Successfully cleaned $(RESET_COLOR)';

# Note that as of 10/2022, uplink-c's Makefile doesn't run well on Darwin.
# I'd be nice to $(MAKE) -C $(UPLINKC) -f Makefile install.
.PHONY: install
install: build
	install $(LIBRARY_NAME) ${DESTDIR}/lib