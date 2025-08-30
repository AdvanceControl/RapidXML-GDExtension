# SConstruct (专业最终版)

import os

# --- 1. 初始化 godot-cpp 环境并克隆 ---
# 这是最稳定、最专业的做法，能确保所有工具和配置被正确加载。
env = SConscript("godot-cpp/SConstruct").Clone()

# --- 2. 项目专属配置 ---
library_name = "rapidxml_gdextension"
output_dir = f"addons/{library_name}"
# build_dir 用于存放所有 .obj 等中间文件，保持源码目录干净
build_dir = f"build/{env['suffix'][1:]}" # env['suffix'] 类似于 .windows.template_debug.x86_64

# --- 3. 设置干净的构建目录 ---
VariantDir(build_dir, "src", duplicate=False)

# --- 4. 平台特定的编译器标志 ---
# 确保所有 Windows MSVC 构建都启用异常处理
if env["platform"] == "windows" and not env.get("use_mingw", False):
    env.Append(CXXFLAGS=['/EHsc'])

# --- 5. 定义我们自己的源文件 ---
sources = []
# a. 手动编写的源文件
sources.extend(Glob(f"#{build_dir}/*.cpp"))
# b. 头文件路径
env.Prepend(CPPPATH=["#src/rapidxml"]) 

# --- 6. 文档生成逻辑 (已加固) ---
if env.get("target") in ["template_debug", "template_release"]:
    doc_files = Glob("#docs/**/*.xml")
    if doc_files:
        print("--- Editor build detected, generating documentation into source tree... ---")
        # a. 将生成的 C++ 文件输出到源码树 (src/generated)
        doc_data = env.GodotCPPDocData(
            "#src/generated/doc_data.gen.cpp",
            source=doc_files
        )
        # b. 明确依赖关系，确保先生成文档再编译
        env.Depends(sources, doc_data)
        # c. 将生成的源文件也添加到要编译的列表中
        sources.append(doc_data)
    else:
        print("--- Editor build detected, but no documentation files found. Skipping. ---")

# --- 7. 构建最终的库文件 ---
base_filename = f"{library_name}{env['suffix']}"
output_build_dir = f"{output_dir}/build" 

if env["platform"] == "ios":
    # iOS 需要构建静态库并打包成 XCFramework
    static_library_path = f"{output_build_dir}/{base_filename}"
    library = env.StaticLibrary(target=static_library_path, source=sources)
    xcframework_path = f"{output_build_dir}/{library_name}{env['suffix'].split('.')[0]}.{env['suffix'].split('.')[1]}.universal"
    final_target = env.XCFramework(target=xcframework_path, source=[library])
else:
    # 其他所有平台都构建共享库 (.dll, .so, .dylib)
    # 手动构建完整的、带后缀的目标文件名，避免 SCons 混淆
    final_filename = f"{base_filename}{env['SHLIBSUFFIX']}"
    target_path = f"{output_build_dir}/{final_filename}"
    library = env.SharedLibrary(target=target_path, source=sources)
    final_target = library

Default(final_target)
print(f"--- Building GDExtension target: {final_target[0]} ---")


# --- 8. 定义用于分发打包的任务 ---
# a. 复制 .gdextension 配置文件
gdextension_file = env.Install(output_dir, f"#{library_name}.gdextension")

# b. 复制 LICENSE 和 README
license_file = env.Install(output_dir, "#LICENSE")
readme_file = env.Install(output_dir, "#README.md")

# c. 复制编辑器插件脚本 (假设在 plugin/ 目录下)
plugin_scripts = env.Install(output_dir, Glob("#plugin/*"))

# d. 复制整个 translations 文件夹
translations_dir = env.Install(output_dir, "#translations")

# e. 创建一个名为 "package" 的别名，用于一键构建所有需要分发的文件
package_target = Alias("package", [
    final_target, 
    gdextension_file, 
    license_file, 
    readme_file,
    plugin_scripts,
    translations_dir
])
Default(package_target)