add_requires("glm >= 1.0.1", "glfw >= 3.4", "vulkansdk")

target("vulkanLearn")
    set_kind("binary")
    add_files("src/*.cpp")
    -- add_files("src/*.hpp")
    add_packages("glm", "glfw", "vulkansdk")
