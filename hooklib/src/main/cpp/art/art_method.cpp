//
// Created by swift on 2019/2/3.
//
#include <cstdint>
#include "../includes/art_method.h"
#include "../includes/cast_art_method.h"
#include "../includes/hide_api.h"
#include "../includes/utils.h"

extern int SDK_INT;

using namespace art::mirror;
using namespace SandHook;

void ArtMethod::tryDisableInline() {
    if (SDK_INT < ANDROID_O)
        return;
    uint32_t accessFlag = getAccessFlags();
    accessFlag &= ~ 0x08000000;
    setAccessFlags(accessFlag);
}

void ArtMethod::disableInterpreterForO() {
    uint32_t accessFlag = getAccessFlags();
    accessFlag |= 0x0100;
    setAccessFlags(accessFlag);
}

void ArtMethod::disableCompilable() {
    uint32_t accessFlag = getAccessFlags();
    if (SDK_INT >= ANDROID_O2) {
        accessFlag |= 0x02000000;
        accessFlag |= 0x00800000;
    } else {
        accessFlag |= 0x01000000;
    }
    setAccessFlags(accessFlag);
}

bool ArtMethod::isAbstract() {
    uint32_t accessFlags = getAccessFlags();
    return ((accessFlags & 0x0400) != 0);
}

bool ArtMethod::isNative() {
    uint32_t accessFlags = getAccessFlags();
    return ((accessFlags & 0x0100) != 0);
}

bool ArtMethod::isStatic() {
    uint32_t accessFlags = getAccessFlags();
    return ((accessFlags & 0x0008) != 0);
}

bool ArtMethod::isCompiled() {
    return getQuickCodeEntry() != SandHook::CastArtMethod::quickToInterpreterBridge;
}

bool ArtMethod::isThumbCode() {
    #if defined(__arm__)
    return (reinterpret_cast<Size>(getQuickCodeEntry()) & 0x1) == 0x1;
    #else
    return false;
    #endif
}

void ArtMethod::setAccessFlags(uint32_t flags) {
    CastArtMethod::accessFlag->set(this, flags);
}

void ArtMethod::setPrivate() {
    uint32_t accessFlag = getAccessFlags();
    accessFlag &= ~ 0x0001;
    accessFlag &= ~ 0x0004;
    accessFlag |= 0x0002;
    setAccessFlags(accessFlag);
}

void ArtMethod::setStatic() {
    uint32_t accessFlag = getAccessFlags();
    accessFlag |= 0x0008;
    setAccessFlags(accessFlag);
};

uint32_t ArtMethod::getAccessFlags() {
    return CastArtMethod::accessFlag->get(this);
}

uint32_t ArtMethod::getDexMethodIndex() {
    return CastArtMethod::dexMethodIndex->get(this);
}

void* ArtMethod::getQuickCodeEntry() {
    return CastArtMethod::entryPointQuickCompiled->get(this);
}

void* ArtMethod::getInterpreterCodeEntry() {
    return CastArtMethod::entryPointFormInterpreter->get(this);
}

void* ArtMethod::getDeclaringClassPtr() {
    return CastArtMethod::declaringClass->get(this);
}

void ArtMethod::setQuickCodeEntry(void *entry) {
    CastArtMethod::entryPointQuickCompiled->set(this, entry);
}

void ArtMethod::setJniCodeEntry(void *entry) {
}

void ArtMethod::setInterpreterCodeEntry(void *entry) {
    CastArtMethod::entryPointFormInterpreter->set(this, entry);
}

void ArtMethod::setDexCacheResolveList(void *list) {
    CastArtMethod::dexCacheResolvedMethods->set(this, list);
}

void ArtMethod::setDexCacheResolveItem(uint32_t index, void* item) {
    CastArtMethod::dexCacheResolvedMethods->setElement(this, index, item);
}

void ArtMethod::setDeclaringClassPtr(void *classPtr) {
    CastArtMethod::declaringClass->set(this, classPtr);
}

bool ArtMethod::compile(JNIEnv* env) {
    if (isCompiled())
        return true;
    Size threadId = getAddressFromJavaByCallMethod(env, "com/swift/sandhook/SandHook", "getThreadId");
    if (threadId == 0)
        return false;
    return compileMethod(this, reinterpret_cast<void *>(threadId)) && isCompiled();
}

void ArtMethod::flushCache() {
    flushCacheExt(reinterpret_cast<Size>(this), size());
}

void ArtMethod::backup(ArtMethod *backup) {
    memcpy(backup, this, size());
}

Size ArtMethod::size() {
    return CastArtMethod::size;
}
