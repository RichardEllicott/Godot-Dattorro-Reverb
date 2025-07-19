# Godot Reverb

## What?

a simple wrapper for the excellent:
https://github.com/el-visio/dattorro-verb/

## Why?

unlike Godot's reverb based on freeverb, the diffusion of this reverb doesn't have the same resonant sound characteristic created by freeverb. El Visio's awesome repo in pure C is linked as a submodule... it seems to be surprisingly small for such great sound quality

## Notes

tested on Godot Engine v4.4.1.stable and working, i'm still figuring out SCONS a bit, please let me know if you find anything bad or unusual (i think one issue i'm still working on is the template i forked creats Obj files everywhere)

As it stands this repo would need to be compiled scons, this is simple just get Python, get scons, run it
