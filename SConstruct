import os

env = SConscript("godot-cpp/SConstruct")

library_name = "rapidxml_gdextension"

sources = Glob("#src/*.cpp")
env.Prepend(CPPPATH=["#src/rapidxml"]) 
env.Append(CXXFLAGS=['/std:c++17', '/EHsc'])

if env.get("target") in ["template_debug", "template_release"]:
    print("--- Editor build detected, generating documentation... ---")
    
    doc_data = env.GodotCPPDocData(
        "#src/generated/doc_data.gen.cpp", 
        source=Glob("#doc_classes/*.xml")
    )
    
    env.Depends(sources, doc_data)
    sources.append(doc_data)


base_filename = f"{library_name}{env['suffix']}".replace(".","_")


target_path = f"addons/{library_name}/build/{base_filename}"

print(f"--- Building GDExtension target: {target_path}{env['SHLIBSUFFIX']} ---")

library = env.SharedLibrary(target=target_path, source=sources)

env.NoCache(library)
Default(library)