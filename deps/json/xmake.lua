package("json")
    set_urls("https://github.com/nlohmann/json.git")

    add_deps("cmake")

    add_versions("origin/master", "9cca280a4d0ccf0c08f47a99aa71d1b0e52f8d03")

    on_install(function (package)
        local configs = {}
        table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. (package:debug() and "Debug" or "Release"))
        import("package.tools.cmake").install(package, configs)
        os.cp("include", package:installdir())
    end)
package_end()