set_project("stdlibx")
set_version("0.0.1")

set_config("buildir", "build")

add_rules("mode.release", "mode.debug")
-- C++26: stdx uses C++26 library features (e.g. the C++26 mdspan additions
-- std::dims / std::layout_left_padded / std::full_extent in core/mdspan.inl), so
-- C++23 fails to compile them. The CMake build auto-detects and uses C++26 too.
set_languages("c++26")

-- Compiler toolchain selection (the XMake analogue of CMAKE_CXX_COMPILER and the
-- CMake build's GCC/Clang detection). Override with, e.g.:
--     xmake f --toolset=gcc
option("toolset")
    set_default("llvm")
    set_showmenu(true)
    set_values("llvm", "clang", "gcc")
    set_description("Compiler toolchain to use",
                    "    - llvm    clang/clang++ via the LLVM toolchain (default)",
                    "    - clang   alias for llvm",
                    "    - gcc     gcc/g++")
option_end()

-- C++ standard library selection (mirrors the CMake `STDLIB` option). Override
-- with, e.g.:
--     xmake f --stdlib=libc++
option("stdlib")
    set_default("default")
    set_showmenu(true)
    set_values("default", "libstdc++", "libc++")
    set_description("C++ standard library to use",
                    "    - default      compiler default",
                    "    - libstdc++    GNU libstdc++ (-stdlib=libstdc++)",
                    "    - libc++       LLVM libc++ (-stdlib=libc++)")
option_end()

-- Apply the chosen toolchain. `clang` is treated as an alias for the `llvm`
-- toolchain, which is what drives clang/clang++.
local toolset = get_config("toolset") or "llvm"
set_toolchains(toolset == "clang" and "llvm" or toolset)

-- Apply the chosen standard library. `-stdlib=` is a Clang-only flag; GCC always
-- uses libstdc++, so a non-matching request is warned about and ignored, the same
-- way the CMake build handles it.
local stdlib = get_config("stdlib") or "default"
if stdlib ~= "default" then
    if toolset == "gcc" then
        if stdlib ~= "libstdc++" then
            print("warning: stdlib='" .. stdlib .. "' is not applicable with gcc (always libstdc++); ignoring")
        end
    else
        add_cxxflags("-stdlib=" .. stdlib, {force = true})
        add_ldflags("-stdlib=" .. stdlib, {force = true})
    end
end

set_warnings("all")
-- set_warnings("all", "error")

if is_mode("debug") then
    set_symbols("debug")
    set_optimize("none")
else
    set_symbols("hidden")
    set_optimize("fastest")
    set_strip("all")
end

set_policy("build.sanitizer.address", true)
set_policy("build.c++.modules", true)

target("stdlibx")
set_kind("shared")
-- set_kind("static")
add_includedirs("include")
add_files(
    -- Standard library directories
    "src/**/*.cppm"
)
