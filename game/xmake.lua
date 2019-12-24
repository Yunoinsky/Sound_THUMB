-- add modes: debug and release 
add_rules("mode.debug", "mode.release")
-- add target
target("game")
    -- set kind
    set_kind("binary")
    set_targetdir("$(projectdir)/build")
    -- add frameworks
    add_frameworkdirs("/Library/Frameworks")
    add_frameworks("SDL2", "SDL2_image", "SDL2_ttf")
    -- add files
    add_files("src/*.cpp", "src/Audio/miniaudio/*.cpp")
