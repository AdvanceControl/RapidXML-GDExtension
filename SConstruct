import os

env = SConscript("godot-cpp/SConstruct")

env.Append(CXXFLAGS=['/EHsc'])

library_name = "rapidxml_gdextension"
sources = Glob("#src/*.cpp")
env.Prepend(CPPPATH=["#src/rapidxml"]) 

if env.get("target") in ["template_debug", "template_release"]:
    print("--- Editor build detected, generating documentation... ---")
    doc_data = env.GodotCPPDocData(
        "#src/generated/doc_data.gen.cpp", 
        source=Glob("#docs/**/*.xml")
    )
    env.Depends(sources, doc_data)
    sources.append(doc_data)

if env["platform"] == "ios":

    static_library_path = f"addons/{library_name}/build/{library_name}{env['suffix']}"
    static_library = env.StaticLibrary(target=static_library_path, source=sources)

    xcframework_target_path = f"addons/{library_name}/build/{library_name}{env['suffix']}"
    xcframework = env.XCFramework(target=xcframework_target_path, source=[static_library])
    
    Default(xcframework)
    library_target = xcframework

else:
    suffix = env['suffix']
    safe_suffix = suffix.replace('.', '_')
    base_filename = f"{library_name}{safe_suffix}"
    target_path = f"addons/{library_name}/build/{base_filename}"

    print(f"--- Building GDExtension target: {target_path}{env['SHLIBSUFFIX']} ---")

    library = env.SharedLibrary(target=target_path, source=sources)
    Default(library)
    library_target = library

env.NoCache(library_target)

# --- 分发任务 (保持不变) ---
gdextension_file = env.Install(f"addons/{library_name}", "#my_extension.gdextension")
release_package = Alias("release_package", [library_target, gdextension_file])
Default(release_package)