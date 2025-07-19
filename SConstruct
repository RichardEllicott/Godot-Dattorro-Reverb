#!/usr/bin/env python
import os
import sys

from methods import print_error

# optional, this is not required but helps vs.code pylance to understand the file
from SCons.Script import *

# libname = "EXTENSION-NAME"
libname = "GDExtensionAudioS2" # new name, note that /demo/bin/example.gdextension needed to be changed
projectdir = "demo"

source_folder = "src/" # top level folder
folder_exclusions = ["gen"] # exclude named folders





## automaticly find folders excluding folder_exclusions and folders starting with a '.'
source_folders = []
for root, dirs, files in os.walk(source_folder):

    for dir in list(dirs):
        if dir.startswith('.'): # exclude folders starting with .
            dirs.remove(dir)
            continue

        if dir in folder_exclusions: # exclude exclusions
            dirs.remove(dir)

    root = root.replace("\\", "/")

    if not root.endswith('/'): # ensure we end with a / (needed for next glob)
        root = root + '/'

    source_folders.append(root)

for folder in source_folders:
    print("📂 Added folder:", folder)



localEnv = Environment(tools=["default"], PLATFORM="")


# Build profiles can be used to decrease compile times.
# You can either specify "disabled_classes", OR
# explicitly specify "enabled_classes" which disables all other classes.
# Modify the example file as needed and uncomment the line below or
# manually specify the build_profile parameter when running SCons.

# localEnv["build_profile"] = "build_profile.json"

customs = ["custom.py"]
customs = [os.path.abspath(path) for path in customs]

opts = Variables(customs, ARGUMENTS)
opts.Update(localEnv)

Help(opts.GenerateHelpText(localEnv))

env = localEnv.Clone()



if not (os.path.isdir("godot-cpp") and os.listdir("godot-cpp")):
    print_error("""godot-cpp is not available within this folder, as Git submodules haven't been initialized.
Run the following command to download godot-cpp:

    git submodule update --init --recursive""")
    sys.exit(1)

env = SConscript("godot-cpp/SConstruct", {"env": env, "customs": customs})

# based on chat, should put obj files in a build dir
# https://claude.ai/chat/74c5b476-7129-4c49-b622-134e4d3eb3e6
build_dir = "build/"
env.VariantDir(build_dir, source_folder, duplicate=0)


## modified to use all source_folders (note an issue with this from claude)
## https://claude.ai/chat/d359adbc-b191-42fc-b43d-8ced74c70d9f
sources = []
# Add src/maps/ to header search path if your headers are there
env.Append(CPPPATH=source_folders)
for folder in source_folders:

    file_paths = env.Glob(folder + "*.cpp") # orginal just added cpp
    file_paths += env.Glob(folder + "*.c") # added by me, also include c

    for path in file_paths:
        print("🍅 Added source file:", path) # added by me, makes it easy to see what files added

    sources.extend(file_paths) # ORGINAL


# # Instead of manual file walking, use:
# # howver claude gave up on me at this point, it's likely obj files shouldn't be where they are but i will need to leave it like this for the moment
# sources = Glob("src/*.cpp")
# sources += Glob("src/**/*.cpp")  # Recursive
# sources += Glob("src/**/*.c")    # Add C files




# ## I ADDED THIS TO TRY AND DETECT MEMORY LEAKS
# ## SO FAR I CANT FIND ANY EVEN WHEN I DELIBERATLY TRY AND CREATE THEM
# ## https://claude.ai/chat/d359adbc-b191-42fc-b43d-8ced74c70d9f
# ## learning to find memory leaks as i've added raw C
# if env["target"] == "template_debug":
#     env.Append(CPPDEFINES=["_CRTDBG_MAP_ALLOC"])
#     env.Append(LINKFLAGS=["/DEBUG"])



if env["target"] in ["editor", "template_debug"]:
    try:
        doc_data = env.GodotCPPDocData(
            "src/gen/doc_data.gen.cpp", source=Glob("doc_classes/*.xml"))
        sources.append(doc_data)
    except AttributeError:
        print("Not including class reference as we're targeting a pre-4.3 baseline.")

# .dev doesn't inhibit compatibility, so we don't need to key it.
# .universal just means "compatible with all relevant arches" so we don't need to key it.
suffix = env['suffix'].replace(".dev", "").replace(".universal", "")

lib_filename = "{}{}{}{}".format(
    env.subst('$SHLIBPREFIX'), libname, suffix, env.subst('$SHLIBSUFFIX'))

library = env.SharedLibrary(
    "bin/{}/{}".format(env['platform'], lib_filename),
    source=sources,
)

copy = env.Install("{}/bin/{}/".format(projectdir, env["platform"]), library)

default_args = [library, copy]
Default(*default_args)
