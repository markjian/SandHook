//
// Created by 甘尧 on 2019/1/12.
//

#ifndef SANDHOOK_CAST_ART_METHOD_H
#define SANDHOOK_CAST_ART_METHOD_H

#include "cast.h"
#include "trampoline_manager.h"

namespace SandHook {

    class CastArtMethod {
    public:
        static Size size;
        static IMember<art::mirror::ArtMethod, void*>* entryPointQuickCompiled;
        static IMember<art::mirror::ArtMethod, void*>* entryPointFormInterpreter;
        static ArrayMember<art::mirror::ArtMethod,void*>* dexCacheResolvedMethods;
        static IMember<art::mirror::ArtMethod, uint32_t>* dexMethodIndex;
        static IMember<art::mirror::ArtMethod, uint32_t>* accessFlag;
        static IMember<art::mirror::ArtMethod, void*>* declaringClass;
        static void* quickToInterpreterBridge;
        static void* genericJniStub;
        static void* staticResolveStub;

        static void init(JNIEnv *env);
        static void copy(art::mirror::ArtMethod* from, art::mirror::ArtMethod* to);

    };

}

#endif //SANDHOOK_CAST_ART_METHOD_H


