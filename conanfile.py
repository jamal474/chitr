from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout

class Chitr(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    default_options = {
        "wxwidgets/*:mediactrl": True
    }

    def requirements(self):
        self.requires("iconfontcppheaders/cci.20240620")
        # wxWidgets on Linux: use the system package (apt) instead of Conan.
        # The Conan Center wxwidgets recipe hard-pins gst-plugins-base/1.19.2 when
        # mediactrl=True, and that recipe is Conan-1-only (cannot be resolved by
        # Conan 2 at all). System wxWidgets on Linux uses the distro's own
        # gstreamer libs and just works. Windows/macOS still build from Conan.
        if self.settings.os != "Linux":
            self.requires("wxwidgets/3.2.8")

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