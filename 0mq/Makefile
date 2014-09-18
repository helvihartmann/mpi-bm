LDFLAGS=-L/opt/fairsoft/jul14p3_build/lib -lzmq -Wl,-rpath=/opt/fairsoft/jul14p3_build/lib/
CXXFLAGS=-I/opt/fairsoft/jul14p3_build/include -std=c++11

all: server client

%: %.cpp
	$(CXX) $^ $(LDFLAGS) $(CXXFLAGS) -o $@

clean:
	$(RM) server client
