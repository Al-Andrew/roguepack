add_rules("mode.debug", "mode.release")
add_requires("raylib", {configs = {shared = false}})
add_requires("catch2")
set_languages("c++20")
set_warnings("all")

target('roguepack')
    set_kind('static')
    add_files('src/*.cpp')
    add_includedirs('include', {public = true})
    add_packages('raylib', {public = true})
    
    if is_mode("debug") then
        add_defines("ROGUEPACK_DEBUG")
    end

target('roguepack-test')
    set_kind('binary')
    add_files('test/*.cpp')
    add_deps('roguepack')

target('sandbox')
    set_kind('binary')
    add_files('sandbox/*.cpp')
    add_deps('roguepack')