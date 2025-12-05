# AOV-DumpSkinID

> [!WARNING] 
> This tool is intended for educational and authorized penetration testing purposes only.
> By using this tool, you agree to use it responsibly and ethically, and to comply with all applicable laws and regulations.
> The developer assumes no responsibility for any misuse of this tool.

## How to use

Download pre-compiled libs [HERE](https://github.com/leletuananh222/AOV-DumpSkinID/releases) and follow steps below

Note: Non-root methods involves modifying APK file, you may need to bypass APK integrity or signature check if it's present. I won't get into the details of bypassing anything because it is simply out of the scope

## Method 1: Fake lib

This is a trick to load our own libunity.so or libmain.so and load game's renamed original lib libTuanMeta.so .Can't decide which lib? Try libmain.so first as it sometimes work better than libunity.so

### Root

- Make sure you know the architecture of the game and your device before proceed
- Use any file manager app that can access root. Go to /data/data/(package name)/lib or /data/app/(obfuscated name)/(package name-obfuscated name)/lib
- Rename original libunity.so or libmain.so to libTuanMeta.so
- Put our lib file libmain.so or you can rename it to libunity.so (if you choose libunity.so) to the lib folder

### Non-root

Some games may have APK integrity and signature check. You may need to bypass it before adding AOV-DumpSkinID

- Open APK file as ZIP using Winrar. 7zip or other zip utilities
- Rename game's libunity.so or libmain.so to libTuanMeta.so
- Put our fake lib file libunity.so or libmain.so to the lib folder
- Zipalign and sign the APK file
- Install the APK

### Method 2: Lib call

- First rename our lib to the name you desire. ex: libZRT.so
- Then open all game's classex.dex, go to com/unity3d/player/UnityPlayerActivity
- Find OnCreate

```smali
.method protected onCreate(Landroid/os/Bundle;)V
    .locals 2

    const-string v0, "ZRT"
 
    invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V

    whatever code below
```

### Dumping

- Run the game
- Wait a few seconds. Let the game load into main screen
- Once the dump is complete, it will save the HeroSkinData.txt in /storage/emulated/0/Android/data/(Package name)/files/

## Credits

- MJx0 [KittyMemory](https://github.com/Perfare/Zygisk-Il2CppDumper)
- jmpews [Dobby](https://github.com/jmpews/Dobby)
- AndnixSH [Auto-Il2cppDumper](https://github.com/AndnixSH/Auto-Il2cppDumper/tree/master)
- adamyaxley [Obfuscate](https://github.com/adamyaxley/Obfuscate)
- HexHacking [xDL](https://github.com/hexhacking/xDL)