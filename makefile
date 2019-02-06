# Output Test Binary
BINARY=bin/dflib-test

# Object Files
LIBOBJECTS=

TESTOBJECTS=obj/test/dftest.o 			        \
            obj/test/dflib-test.o

# Header Files
LIBHEADERS=src/lib/circulator.hpp               \
           src/lib/comparable.hpp               \
           src/lib/console.hpp                  \
           src/lib/dependant.hpp                \
		   src/lib/documentation.hpp            \
           src/lib/flagset.hpp                  \
           src/lib/metalist.hpp                 \
           src/lib/number.hpp                   \
           src/lib/resource.hpp                 \
           src/lib/series.hpp                   \
           src/lib/strings.hpp                  \
           src/lib/types.hpp                    \
           src/lib/utilities.hpp
TESTHEADERS=src/test/dftest.hpp

# Compiler Options
CXX=g++
CXXFLAGS=-std=c++17 -Wall -Wextra -Werror -pedantic
INCLUDEPATHS=-Isrc -Isrc/lib -Isrc/test

.PHONY: run, all, clean

all: $(BINARY)

loc:
	@printf "[N] "
	@cat src/lib/* src/test/* | wc -l

run: $(BINARY)
	@$(BINARY)

$(BINARY): $(LIBOBJECTS) $(TESTOBJECTS)
	@mkdir -p bin
	@printf "[L] $@\n"
	@$(CXX) $(LIBOBJECTS) $(TESTOBJECTS) -o $@
	@printf "[+] Build Succeeded!\n"

obj/test/%.o: src/test/%.cpp $(LIBHEADERS) $(TESTHEADERS)
	@mkdir -p obj/test
	@printf "[C] $<\n"
	@$(CXX) $(CXXFLAGS) $(INCLUDEPATHS) -c $< -o $@

obj/lib/%.o: src/lib/%.cpp $(LIBHEADERS)
	@mkdir -p obj/lib
	@printf "[C] $<\n"
	@$(CXX) $(CXXFLAGS) $(INCLUDEPATHS) -c $< -o $@

clean:
	@printf "[R] bin\n"
	@rm -rf bin
	@printf "[R] obj\n"
	@rm -rf obj

