from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout

class Chitr(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    default_options = {
        "wxwidgets/*:mediactrl": True
    }

    def requirements(self):
        self.requires("wxwidgets/3.2.8")
        self.requires("iconfontcppheaders/cci.20240620")

    def generate(self):
            deps = CMakeDeps(self)
            deps.generate()
            tc = CMakeToolchain(self)
            tc.generate()

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()