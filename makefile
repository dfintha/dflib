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
CXXFLAGS=-std=c++1z -Wall -Wextra -Werror -pedantic
INCLUDEPATHS=-Isrc -Isrc/lib -Isrc/test

# Uncomment for Text Formatting
BLD=#`tput bold`
RED=#`tput setaf 1`
GRN=#`tput setaf 2`
YLW=#`tput setaf 3`
BLU=#`tput setaf 5`
TEL=#`tput setaf 6`
WHT=#`tput setaf 7`
NRM=#`tput sgr0`

.PHONY: test, all, debug, release, loc, clean

# Run Tests
test: $(BINARY)
	@printf "%s[ Tests ]%s $(BINARY)\n\n" "$(BLD)$(WHT)" $(NRM)
	@$(BINARY)
	@printf "\n%s[Success] Tests Passed!%s\n" "$(BLD)$(GRN)" $(NRM)

all: release

# Build Debug Executable
debug: CXXFLAGS += -DDEBUG -Og -g3 -ggdb
debug: $(BINARY)
	@printf "%s[Success] Build Succeeded!%s\n" "$(BLD)$(GRN)" $(NRM)
	@printf "%s[ Build ]%s Debug Binary is Built\n" "$(BLD)$(GRN)" $(NRM)

# Build Release Executable
release: CXXFLAGS += -Ofast -g0
release: $(BINARY)
	@printf "%s[ Strip ]%s $(BINARY)\n" "$(BLD)$(TEL)" $(NRM)
	@strip $(BINARY)
	@printf "%s[Success] Build Succeeded!%s\n" "$(BLD)$(GRN)" $(NRM)
	@printf "%s[ Build ]%s Release Binary is Built\n" "$(BLD)$(GRN)" $(NRM)

# Print Lines of Code
loc:
	@printf "%s[ Lines ]%s " "$(BLD)$(YLW)" $(NRM)
	@cat src/lib/* src/test/* | wc -l

# Compile and Link the Test
$(BINARY): $(LIBOBJECTS) $(TESTOBJECTS)
	@mkdir -p bin
	@printf "%s[Linking]%s $@\n" "$(BLD)$(TEL)" $(NRM)
	@$(CXX) $(LIBOBJECTS) $(TESTOBJECTS) -o $@

# Compile Test Sources
obj/test/%.o: src/test/%.cpp $(LIBHEADERS) $(TESTHEADERS)
	@mkdir -p obj/test
	@printf "%s[Compile]%s $<\n" "$(BLD)$(BLU)" $(NRM)
	@$(CXX) $(CXXFLAGS) $(INCLUDEPATHS) -c $< -o $@

# Compile Library Sources
obj/lib/%.o: src/lib/%.cpp $(LIBHEADERS)
	@mkdir -p obj/lib
	@printf "%s[Compile]%s $<\n" "$(BLD)$(BLU)" $(NRM)
	@$(CXX) $(CXXFLAGS) $(INCLUDEPATHS) -c $< -o $@

# Remove Temporary Files
clean:
	@printf "%s[ Clean ]%s bin\n" "$(BLD)$(RED)" $(NRM)
	@rm -rf bin
	@printf "%s[ Clean ]%s obj\n" "$(BLD)$(RED)" $(NRM)
	@rm -rf obj

