# ArbiterPlus standalone server build
CXX      ?= g++
CXXFLAGS ?= -std=c++11 -O2 -g -Wall -Wno-unused-result
LDFLAGS  ?= -pthread

COMMON = common/log.cpp common/config.cpp common/arch.cpp common/message.cpp \
         common/inetwork.cpp common/command.cpp common/fserver.cpp common/commoncmd.cpp
SRV    = server/server.cpp arbiter-server.cpp

OBJS   = $(patsubst %.cpp,%.o,$(COMMON) $(SRV))
BIN    = arbiter-server

all: $(BIN)

$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -Icommon -Iserver -MMD -MP -c -o $@ $<

clean:
	rm -f $(OBJS) $(BIN) $(OBJS:.o=.d)

-include $(OBJS:.o=.d)

.PHONY: all clean
