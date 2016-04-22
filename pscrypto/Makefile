SRCS = pscrypto.cpp
OBJS := $(SRCS:.cpp=.o)

# TODO: this needs to be fixed for linux
LIB = pscrypto.dll
TEST = pscrypto

AR = $(PREFIX)ar
CXX = $(PREFIX)g++
LD = $(PREFIX)g++
STRIP = $(PREFIX)strip

CXXFLAGS := $(CFLAGS) -DBUILD_DLL -I../external/psf-cryptopp -fPIC
LDFLAGS := -L../external/psf-cryptopp -static-libgcc -static-libstdc++

ifdef DEBUG
CXXFLAGS += -g
else
CXXFLAGS += -O2 -DNDEBUG
endif

LIB_FLAGS=$(LDFLAGS) -Wl,-soname=$(LIB)
LIBS=-lcryptopp

all : $(OBJS) $(LIB) $(TEST)

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
	rm -f $(OBJS) $(LIB) *.pyc
