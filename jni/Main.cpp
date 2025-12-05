#include "Includes/obfuscate.h"
#include "Includes/Logger.h"
#include "Includes/Macros.h"
#include "Includes/Utils.h"
#include "TuanMeta/Call_Me.h"
#include "Hook.h"

ProcMap il2cppMap;
void *Init_Thread() {

	while (!il2cppMap.isValid()) {
		il2cppMap = KittyMemory::getLibraryBaseMap("libil2cpp.so");
		sleep(1);
	}
	
	Attach();

	GetHeroName = (String *(*)(uint32_t)) Method("Project_d.dll", "Assets.Scripts.GameSystem", "CHeroInfo", "GetHeroName", 1);
	GetSkinName = (String *(*)(uint32_t)) Method("Project_d.dll", "Assets.Scripts.GameSystem", "CSkinInfo", "GetSkinName", 1);
	TuanMetaHook("AovTdr.dll", "ResData", "ResHeroSkin", "TransferData", 1, TransferData, _TransferData);
	
	return nullptr;
}

#define RealLibToLoad "libTuanMeta.so"

void *pLibRealUnity = 0;
typedef jint(JNICALL *CallJNI_OnLoad_t)(JavaVM *vm, void *reserved);
typedef void(JNICALL *CallJNI_OnUnload_t)(JavaVM *vm, void *reserved);

CallJNI_OnLoad_t RealJNIOnLoad = 0;
CallJNI_OnUnload_t RealJNIOnUnload = 0;

JNIEXPORT jint JNICALL CallJNIOL(
        JavaVM *vm, void *reserved) {
    if (!pLibRealUnity)
        pLibRealUnity = dlopen(RealLibToLoad, RTLD_NOW);
    if (!RealJNIOnLoad)
        RealJNIOnLoad = reinterpret_cast<CallJNI_OnLoad_t>(dlsym(pLibRealUnity, "JNI_OnLoad"));
    return RealJNIOnLoad(vm, reserved);
}

JNIEXPORT void JNICALL CallJNIUL(
        JavaVM *vm, void *reserved) {
    if (!pLibRealUnity)
        pLibRealUnity = dlopen(RealLibToLoad, RTLD_NOW);
    if (!RealJNIOnUnload)
        RealJNIOnUnload = reinterpret_cast<CallJNI_OnUnload_t>(dlsym(pLibRealUnity,
                                                                     "JNI_OnUnload"));
    RealJNIOnUnload(vm, reserved);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
	JNIEnv*globalEnv;
	vm->GetEnv((void**)&globalEnv, JNI_VERSION_1_6);

	std::thread(Init_Thread).detach();
	
	pLibRealUnity = dlopen(RealLibToLoad, RTLD_NOW);
	if (pLibRealUnity) return CallJNIOL(vm, reserved);
	else return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved) {

    CallJNIUL(vm, reserved);
}