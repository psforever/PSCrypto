# Common makefile helper file

# Note - := means expand all and save result, = means expand all each time
AR := $(PREFIX)ar
CC := $(PREFIX)gcc
CXX := $(PREFIX)g++
LD := $(PREFIX)ld
STRIP := $(PREFIX)strip

# Get a good guess as to our compile target
gcc_machine := $(subst -, ,$(shell $(CC) -dumpmachine))

GCC_ARCH := $(word 1,$(gcc_machine))
OS := $(word 3,$(gcc_machine))

ifeq "$(ARCH)" ""
  ifeq "$(GCC_ARCH)" ""
  $(error "Unable to determine architecture")
  endif
endif

ifeq "$(OS)" ""
$(error "Unable to determine operating system")
endif

# Consolidate the GCC architecture value.
# If it was user defined, and it disagrees with the GCC value, we are probably
# trying a multilib build
ifdef ARCH
  # user arch and GCC arch did not match
  ifneq "$(ARCH)" "$(GCC_ARCH)"
    ifeq ($(findstring $(ARCH), i686 x86_64),)
    $(error "Unsupported architecture $(ARCH)")
    endif

    ifeq "$(ARCH)" "i686"
      CFLAGS := $(CFLAGS) -m32
      CXXFLAGS := $(CXXFLAGS) -m32
      LDFLAGS := $(LDFLAGS) -m32
    else ifeq "$(ARCH)" "x86_64")
      CFLAGS := $(CFLAGS) -m64
      CXXFLAGS := $(CXXFLAGS) -m64
      LDFLAGS := $(LDFLAGS) -m64
    endif
  endif
else
  ifeq ($(findstring $(GCC_ARCH), i686 x86_64),)
  $(error "Unsupported architecture $(ARCH)")
  endif

  ARCH := $(GCC_ARCH)
endif

ifeq ($(findstring $(OS), linux cygwin mingw32),)
$(error "Unsupported operating system $(OS)")
endif

# Output artifact functions
ifeq "$(OS)" "mingw32"
  lib-name = $(1).dll
  exe-name = $(1).exe
else ifeq "$(OS)" "cygwin"
  lib-name = $(1).dll
  exe-name = $(1).exe
else
  lib-name = lib$(1).so
  exe-name = $(1)
endif
