# SConstruct (专业最终版)

import os

env = SConscript("godot-cpp/SConstruct").Clone()

library_name = "rapidxml_gdextension"
output_dir = f"addons/{library_name}"
build_dir = f"build/{env['suffix'][1:]}"


VariantDir(build_dir, "src", duplicate=False)

if env["platform"] == "windows" and not env.get("use_mingw", False):
    env.Append(CXXFLAGS=['/EHsc'])

sources = []
sources.extend(Glob(f"#{build_dir}/*.cpp"))
env.Prepend(CPPPATH=["#src/rapidxml"]) 

if env.get("target") in ["template_debug", "template_release"]:
    doc_files = Glob("#doc_classes/*.xml")
    if doc_files:
        print("--- Editor build detected, generating documentation into source tree... ---")
        doc_data = env.GodotCPPDocData(
            "#src/generated/doc_data.gen.cpp",
            source=doc_files
        )
        env.Depends(sources, doc_data)
        sources.append(doc_data)
    else:
        print("--- Editor build detected, but no documentation files found. Skipping. ---")

base_filename = f"{library_name}{env['suffix']}"
output_build_dir = f"{output_dir}/build" 

if env["platform"] == "ios":
    static_library_path = f"{output_build_dir}/{base_filename}"
    library = env.StaticLibrary(target=static_library_path, source=sources)
    xcframework_path = f"{output_build_dir}/{library_name}{env['suffix'].split('.')[0]}.{env['suffix'].split('.')[1]}.universal"
    final_target = env.XCFramework(target=xcframework_path, source=[library])
else:
    final_filename = f"{base_filename}{env['SHLIBSUFFIX']}"
    target_path = f"{output_build_dir}/{final_filename}"
    library = env.SharedLibrary(target=target_path, source=sources)
    final_target = library

Default(final_target)
print(f"--- Building GDExtension target: {final_target[0]} ---")



gdextension_file = env.Install(output_dir, f"#{library_name}.gdextension")

license_file = env.Install(output_dir, "#LICENSE")
readme_file = env.Install(output_dir, "#README.md")

plugin_scripts = env.Install(output_dir, Glob("#plugin/*"))

translations_dir = env.Install(output_dir, "#translations")

package_target = Alias("package", [
    final_target, 
    gdextension_file, 
    license_file, 
    readme_file,
    plugin_scripts,
    translations_dir
])
Default(package_target)