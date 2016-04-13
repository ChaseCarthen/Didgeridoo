BUILDROOT=build
include $(BUILDROOT)/SystemDefinitions

# If this line fails it means you need to copy build/InstallPaths.example
# to build/InstallPaths and modify it to where you want to install.
include $(BUILDROOT)/InstallPaths

#-------------------------------------------------------------------------------

#VERBOSE = 1
#RELEASE = 1

DEFAULTDEBUGLEVEL = 3
DEFAULTOPTLEVEL = 2

LINKFLAGS =
EXTRACINCLUDEFLAGS = -Isrc
EXTRALINKDIRFLAGS =
EXTRALINKLIBFLAGS = -lfreeimage -lpthread -lalut -lopenal
#PACKAGES = PERFTOOLS 
PACKAGES = CAVR

#-------------------------------------------------------------------------------

EXEDIR = bin
LIBDIR = lib

LIBRARY_DIR = src
LIBRARY_FILENAME = libgraphics.a
LIBRARY = $(LIBDIR)/$(LIBRARY_FILENAME)
ALL += $(LIBRARY)

DEMO_DIR = src/demo
DEMO = $(EXEDIR)/demo
ALL += $(DEMO)

ALL += $(TESTS)

#-------------------------------------------------------------------------------

.PHONY: all
all: $(ALL)
	@echo Running tests...

.PHONY: extraclean
extraclean:

.PHONY: extraveryclean
extraveryclean:
	rm -rf docs

.PHONY: docs
docs:
	rm -rf docs
	doxygen doxyfile

# Installation rule.
# Yes, this will copy the cpp files too though they are not needed.
# If you are bored please write the code to not copy the cpp files.
.PHONY: install
install:
	cp $(LIBRARY) $(LIB_INSTALL_DIR)
	cp -r $(LIBRARY_DIR) $(SRC_INSTALL_DIR)

.PHONY: uninstall
uninstall:
	@echo You have entered DANGEROUS waters.
	@echo We are about to delete:
	@echo
	@echo "\t$(LIB_INSTALL_DIR)/$(LIBRARY_FILENAME)"
	@echo "\t$(SRC_INSTALL_DIR)"
	@echo
	@echo If you want to turn around, then hit CTRL-C. NOW.
	@echo
	rm -i $(LIB_INSTALL_DIR)/$(LIBRARY_FILENAME)
	@echo
	rm -ifr $(SRC_INSTALL_DIR)

include $(BUILDROOT)/BasicMakefile

#-------------------------------------------------------------------------------

LIBRARY_SRC = $(shell find -L $(LIBRARY_DIR) -name "*.cpp")
$(LIBRARY): $(LIBRARY_SRC:$(LIBRARY_DIR)/%.cpp=$(OBJDIR)/$(LIBRARY_DIR)/%.o)

DEMO_SRC = $(shell find -L $(DEMO_DIR) -name "*.cpp")
$(DEMO): $(LIBRARY) $(DEMO_SRC:$(DEMO_DIR)/%.cpp=$(OBJDIR)/$(DEMO_DIR)/%.o)
$(DEMO): PACKAGES += GLUT

