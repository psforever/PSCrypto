include ../Makefile.inc

#### Sources
SRCS = pscrypto.cpp
OBJS := $(SRCS:.cpp=.o)

#### Artifacts
OUT_NAME=pscrypto
LIB = $(call lib-name,$(OUT_NAME))
TEST = $(call exe-name,$(OUT_NAME))

#### Flags
CXXFLAGS := $(CFLAGS) -DBUILD_DLL -I../external/psf-cryptopp
LDFLAGS := -L../external/psf-cryptopp

# Handle OS compiler quirks
ifeq "$(OS)" "mingw32"
  LDFLAGS := $(LDFLAGS) -static-libgcc -static-libstdc++
else ifeq "$(OS)" "cygwin"
  LDFLAGS := $(LDFLAGS) -static-libgcc -static-libstdc++
else
  CXXFLAGS := $(CXXFLAGS) -fPIC
endif

ifdef DEBUG
CXXFLAGS += -g -std=c++03
else
CXXFLAGS += -O2 -DNDEBUG -std=c++03
endif

LIB_FLAGS=$(LDFLAGS) -Wl,-soname=$(LIB)
LIBS=-lcryptopp

#### Recipes

all : $(OBJS) $(LIB)

test : all $(TEST)
	./$(TEST)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(LIB) : $(OBJS)
	$(CXX) -shared $< -o $(LIB) $(LIB_FLAGS) $(LIBS)
ifndef DEBUG
	$(STRIP) $(LIB)
endif

$(TEST) : $(OBJS)
	$(CXX) $(OBJS) -o $(TEST) $(LDFLAGS) $(LIBS)
ifndef DEBUG
	$(STRIP) $(TEST)
endif

clean :
	rm -f $(OBJS) $(LIB) $(TEST) *.pyc

.PHONY : all test clean
