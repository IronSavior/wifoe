SOURCES := $(wildcard src/*.c*) $(wildcard src/**/*.c*)

BASE_NAME := wifoe

OUTPUT_NAME ?= $(BASE_NAME)
BOOST_LIBS ?= program_options
PKGCONFIG_LIBS ?= libnl-3.0 libnl-genl-3.0

LDLIBS ?= $(addprefix -lboost_, $(BOOST_LIBS)) $(shell pkg-config --libs-only-l $(PKGCONFIG_LIBS)) -lstdc++
LDFLAGS ?= $(shell pkg-config --libs-only-L $(PKGCONFIG_LIBS))

DEPFLAGS ?= -MMD -MP -MF .deps/$<.dep
CPPFLAGS ?= -Isrc/misc $(BOOST_FLAGS) $(shell pkg-config --cflags $(PKGCONFIG_LIBS)) -Wall -O2
CXXFLAGS ?= -std=c++11

OBJECTS := $(addsuffix .o, $(addprefix .build/, $(SOURCES)))
DEPFILES := $(subst .o,.dep, $(subst .build/,.deps/, $(OBJECTS)))

all: $(OUTPUT_NAME)

.build/%.c.o: %.c
	@mkdir -p .deps/$(dir $<) .build/$(dir $<)
	$(COMPILE.c) $(DEPFLAGS) -o $@ $<

.build/%.cpp.o: %.cpp
	@mkdir -p .deps/$(dir $<) .build/$(dir $<)
	$(COMPILE.cpp) $(DEPFLAGS) -o $@ $<

$(OUTPUT_NAME): $(OBJECTS)
	$(LINK.o) $^ $(LDLIBS) -o $@

clean:
	@rm -rf .deps/ .build/ $(OUTPUT_NAME)

-include $(DEPFILES)
