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
        # Resolve a glib version conflict that only shows up on Linux:
        #   gstreamer/1.22.3 (pulled in by gtk when mediactrl=True) pins glib/2.78.3
        #   gtk / gdk-pixbuf / pango / ... accept glib/[^2.78] -> conan resolves 2.86.5
        # Both constraints are satisfied by 2.78.3, so pin the whole graph there.
        if self.settings.os == "Linux":
            self.requires("glib/2.78.3", override=True)

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