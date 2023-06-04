# GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=debug_x64
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug_x64)
  raylib_config = debug_x64
  CubeAsm_config = debug_x64
  rayImGui_config = debug_x64
endif
ifeq ($(config),debug_x86)
  raylib_config = debug_x86
  CubeAsm_config = debug_x86
  rayImGui_config = debug_x86
endif
ifeq ($(config),release_x64)
  raylib_config = release_x64
  CubeAsm_config = release_x64
  rayImGui_config = release_x64
endif
ifeq ($(config),release_x86)
  raylib_config = release_x86
  CubeAsm_config = release_x86
  rayImGui_config = release_x86
endif

PROJECTS := raylib CubeAsm rayImGui

.PHONY: all clean help $(PROJECTS) 

all: $(PROJECTS)

raylib:
ifneq (,$(raylib_config))
	@echo "==== Building raylib ($(raylib_config)) ===="
	@${MAKE} --no-print-directory -C _build -f raylib.make config=$(raylib_config)
endif

CubeAsm: rayImGui raylib
ifneq (,$(CubeAsm_config))
	@echo "==== Building CubeAsm ($(CubeAsm_config)) ===="
	@${MAKE} --no-print-directory -C _build -f CubeAsm.make config=$(CubeAsm_config)
endif

rayImGui:
ifneq (,$(rayImGui_config))
	@echo "==== Building rayImGui ($(rayImGui_config)) ===="
	@${MAKE} --no-print-directory -C _build -f rayImGui.make config=$(rayImGui_config)
endif

clean:
	@${MAKE} --no-print-directory -C _build -f raylib.make clean
	@${MAKE} --no-print-directory -C _build -f CubeAsm.make clean
	@${MAKE} --no-print-directory -C _build -f rayImGui.make clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug_x64"
	@echo "  debug_x86"
	@echo "  release_x64"
	@echo "  release_x86"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   raylib"
	@echo "   CubeAsm"
	@echo "   rayImGui"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"