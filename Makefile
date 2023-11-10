CXX = g++
CXXFLAGS = -std=c++11 -pthread
SRCS = prll_pi_nosync.cpp prll_pi_sync.cpp atomic_pi.cpp false_sharing_pi.cpp local_sum_pi.cpp barrier.cpp prefix_sum.cpp half_area.cpp
EXECS = $(SRCS:.cpp=)

.PHONY: all clean

all: $(EXECS)

%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(EXECS)
