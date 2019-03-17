/** @file
  Copyright (C) 2018, vit9696. All rights reserved.

  All rights reserved.

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/

#include <Library/OcTemplateLib.h>
#include <Library/OcSerializeLib.h>
#include <Library/OcMiscLib.h>
#include <Library/OcAppleKernelLib.h>

#include <sys/time.h>

/*
 clang -g -fsanitize=undefined,address -I../Include -I../../Include -I../../../MdePkg/Include/ -I../../../EfiPkg/Include/ -include ../Include/Base.h Prelinked.c ../../Library/OcXmlLib/OcXmlLib.c ../../Library/OcTemplateLib/OcTemplateLib.c ../../Library/OcSerializeLib/OcSerializeLib.c ../../Library/OcMiscLib/Base64Decode.c ../../Library/OcStringLib/OcAsciiLib.c ../../Library/OcMachoLib/CxxSymbols.c ../../Library/OcMachoLib/Header.c ../../Library/OcMachoLib/Relocations.c ../../Library/OcMachoLib/Symbols.c ../../Library/OcAppleKernelLib/Prelinked.c ../../Library/OcAppleKernelLib/Patcher.c ../../Library/OcMiscLib/DataPatcher.c -o Prelinked

 for fuzzing:
 clang-mp-7.0 -Dmain=__main -g -fsanitize=undefined,address,fuzzer -I../Include -I../../Include -I../../../MdePkg/Include/ -I../../../EfiPkg/Include/ -include ../Include/Base.h Prelinked.c ../../Library/OcXmlLib/OcXmlLib.c ../../Library/OcTemplateLib/OcTemplateLib.c ../../Library/OcSerializeLib/OcSerializeLib.c ../../Library/OcMiscLib/Base64Decode.c ../../Library/OcStringLib/OcAsciiLib.c ../../Library/OcMachoLib/CxxSymbols.c ../../Library/OcMachoLib/Header.c ../../Library/OcMachoLib/Relocations.c ../../Library/OcMachoLib/Symbols.c ../../Library/OcAppleKernelLib/Prelinked.c ../../Library/OcAppleKernelLib/Patcher.c ../../Library/OcMiscLib/DataPatcher.c -o Prelinked
 rm -rf DICT fuzz*.log ; mkdir DICT ; cp Prelinked.plist DICT ; ./Prelinked -jobs=4 DICT

 rm -rf Prelinked.dSYM DICT fuzz*.log Prelinked
*/

STATIC CHAR8 KextInfoPlistData[] = {
  0x3C, 0x3F, 0x78, 0x6D, 0x6C, 0x20, 0x76, 0x65,
  0x72, 0x73, 0x69, 0x6F, 0x6E, 0x3D, 0x22, 0x31,
  0x2E, 0x30, 0x22, 0x20, 0x65, 0x6E, 0x63, 0x6F,
  0x64, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x55, 0x54,
  0x46, 0x2D, 0x38, 0x22, 0x3F, 0x3E, 0x0D, 0x3C,
  0x21, 0x44, 0x4F, 0x43, 0x54, 0x59, 0x50, 0x45,
  0x20, 0x70, 0x6C, 0x69, 0x73, 0x74, 0x20, 0x50,
  0x55, 0x42, 0x4C, 0x49, 0x43, 0x20, 0x22, 0x2D,
  0x2F, 0x2F, 0x41, 0x70, 0x70, 0x6C, 0x65, 0x2F,
  0x2F, 0x44, 0x54, 0x44, 0x20, 0x50, 0x4C, 0x49,
  0x53, 0x54, 0x20, 0x31, 0x2E, 0x30, 0x2F, 0x2F,
  0x45, 0x4E, 0x22, 0x20, 0x22, 0x68, 0x74, 0x74,
  0x70, 0x3A, 0x2F, 0x2F, 0x77, 0x77, 0x77, 0x2E,
  0x61, 0x70, 0x70, 0x6C, 0x65, 0x2E, 0x63, 0x6F,
  0x6D, 0x2F, 0x44, 0x54, 0x44, 0x73, 0x2F, 0x50,
  0x72, 0x6F, 0x70, 0x65, 0x72, 0x74, 0x79, 0x4C,
  0x69, 0x73, 0x74, 0x2D, 0x31, 0x2E, 0x30, 0x2E,
  0x64, 0x74, 0x64, 0x22, 0x3E, 0x0D, 0x3C, 0x70,
  0x6C, 0x69, 0x73, 0x74, 0x20, 0x76, 0x65, 0x72,
  0x73, 0x69, 0x6F, 0x6E, 0x3D, 0x22, 0x31, 0x2E,
  0x30, 0x22, 0x3E, 0x0D, 0x3C, 0x64, 0x69, 0x63,
  0x74, 0x3E, 0x0D, 0x09, 0x3C, 0x6B, 0x65, 0x79,
  0x3E, 0x43, 0x46, 0x42, 0x75, 0x6E, 0x64, 0x6C,
  0x65, 0x49, 0x64, 0x65, 0x6E, 0x74, 0x69, 0x66,
  0x69, 0x65, 0x72, 0x3C, 0x2F, 0x6B, 0x65, 0x79,
  0x3E, 0x0D, 0x09, 0x3C, 0x73, 0x74, 0x72, 0x69,
  0x6E, 0x67, 0x3E, 0x61, 0x73, 0x2E, 0x76, 0x69,
  0x74, 0x39, 0x36, 0x39, 0x36, 0x2E, 0x54, 0x65,
  0x73, 0x74, 0x44, 0x72, 0x69, 0x76, 0x65, 0x72,
  0x3C, 0x2F, 0x73, 0x74, 0x72, 0x69, 0x6E, 0x67,
  0x3E, 0x0D, 0x09, 0x3C, 0x6B, 0x65, 0x79, 0x3E,
  0x43, 0x46, 0x42, 0x75, 0x6E, 0x64, 0x6C, 0x65,
  0x49, 0x6E, 0x66, 0x6F, 0x44, 0x69, 0x63, 0x74,
  0x69, 0x6F, 0x6E, 0x61, 0x72, 0x79, 0x56, 0x65,
  0x72, 0x73, 0x69, 0x6F, 0x6E, 0x3C, 0x2F, 0x6B,
  0x65, 0x79, 0x3E, 0x0D, 0x09, 0x3C, 0x73, 0x74,
  0x72, 0x69, 0x6E, 0x67, 0x3E, 0x36, 0x2E, 0x30,
  0x3C, 0x2F, 0x73, 0x74, 0x72, 0x69, 0x6E, 0x67,
  0x3E, 0x0D, 0x09, 0x3C, 0x6B, 0x65, 0x79, 0x3E,
  0x43, 0x46, 0x42, 0x75, 0x6E, 0x64, 0x6C, 0x65,
  0x4E, 0x61, 0x6D, 0x65, 0x3C, 0x2F, 0x6B, 0x65,
  0x79, 0x3E, 0x0D, 0x09, 0x3C, 0x73, 0x74, 0x72,
  0x69, 0x6E, 0x67, 0x3E, 0x43, 0x50, 0x55, 0x46,
  0x72, 0x69, 0x65, 0x6E, 0x64, 0x44, 0x61, 0x74,
  0x61, 0x50, 0x72, 0x6F, 0x76, 0x69, 0x64, 0x65,
  0x72, 0x3C, 0x2F, 0x73, 0x74, 0x72, 0x69, 0x6E,
  0x67, 0x3E, 0x0D, 0x09, 0x3C, 0x6B, 0x65, 0x79,
  0x3E, 0x43, 0x46, 0x42, 0x75, 0x6E, 0x64, 0x6C,
  0x65, 0x50, 0x61, 0x63, 0x6B, 0x61, 0x67, 0x65,
  0x54, 0x79, 0x70, 0x65, 0x3C, 0x2F, 0x6B, 0x65,
  0x79, 0x3E, 0x0D, 0x09, 0x3C, 0x73, 0x74, 0x72,
  0x69, 0x6E, 0x67, 0x3E, 0x4B, 0x45, 0x58, 0x54,
  0x3C, 0x2F, 0x73, 0x74, 0x72, 0x69, 0x6E, 0x67,
  0x3E, 0x0D, 0x09, 0x3C, 0x6B, 0x65, 0x79, 0x3E,
  0x43, 0x46, 0x42, 0x75, 0x6E, 0x64, 0x6C, 0x65,
  0x53, 0x68, 0x6F, 0x72, 0x74, 0x56, 0x65, 0x72,
  0x73, 0x69, 0x6F, 0x6E, 0x53, 0x74, 0x72, 0x69,
  0x6E, 0x67, 0x3C, 0x2F, 0x6B, 0x65, 0x79, 0x3E,
  0x0D, 0x09, 0x3C, 0x73, 0x74, 0x72, 0x69, 0x6E,
  0x67, 0x3E, 0x31, 0x2E, 0x30, 0x2E, 0x30, 0x3C,
  0x2F, 0x73, 0x74, 0x72, 0x69, 0x6E, 0x67, 0x3E,
  0x0D, 0x09, 0x3C, 0x6B, 0x65, 0x79, 0x3E, 0x43,
  0x46, 0x42, 0x75, 0x6E, 0x64, 0x6C, 0x65, 0x56,
  0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x3C, 0x2F,
  0x6B, 0x65, 0x79, 0x3E, 0x0D, 0x09, 0x3C, 0x73,
  0x74, 0x72, 0x69, 0x6E, 0x67, 0x3E, 0x31, 0x2E,
  0x30, 0x2E, 0x30, 0x3C, 0x2F, 0x73, 0x74, 0x72,
  0x69, 0x6E, 0x67, 0x3E, 0x0D, 0x09, 0x3C, 0x6B,
  0x65, 0x79, 0x3E, 0x49, 0x4F, 0x4B, 0x69, 0x74,
  0x50, 0x65, 0x72, 0x73, 0x6F, 0x6E, 0x61, 0x6C,
  0x69, 0x74, 0x69, 0x65, 0x73, 0x3C, 0x2F, 0x6B,
  0x65, 0x79, 0x3E, 0x0D, 0x09, 0x3C, 0x64, 0x69,
  0x63, 0x74, 0x3E, 0x0D, 0x09, 0x09, 0x3C, 0x6B,
  0x65, 0x79, 0x3E, 0x54, 0x65, 0x73, 0x74, 0x44,
  0x61, 0x74, 0x61, 0x50, 0x72, 0x6F, 0x76, 0x69,
  0x64, 0x65, 0x72, 0x3C, 0x2F, 0x6B, 0x65, 0x79,
  0x3E, 0x0D, 0x09, 0x09, 0x3C, 0x64, 0x69, 0x63,
  0x74, 0x3E, 0x0D, 0x09, 0x09, 0x09, 0x3C, 0x6B,
  0x65, 0x79, 0x3E, 0x43, 0x46, 0x42, 0x75, 0x6E,
  0x64, 0x6C, 0x65, 0x49, 0x64, 0x65, 0x6E, 0x74,
  0x69, 0x66, 0x69, 0x65, 0x72, 0x3C, 0x2F, 0x6B,
  0x65, 0x79, 0x3E, 0x0D, 0x09, 0x09, 0x09, 0x3C,
  0x73, 0x74, 0x72, 0x69, 0x6E, 0x67, 0x3E, 0x63,
  0x6F, 0x6D, 0x2E, 0x61, 0x70, 0x70, 0x6C, 0x65,
  0x2E, 0x64, 0x72, 0x69, 0x76, 0x65, 0x72, 0x2E,
  0x41, 0x70, 0x70, 0x6C, 0x65, 0x41, 0x43, 0x50,
  0x49, 0x50, 0x6C, 0x61, 0x74, 0x66, 0x6F, 0x72,
  0x6D, 0x3C, 0x2F, 0x73, 0x74, 0x72, 0x69, 0x6E,
  0x67, 0x3E, 0x0D, 0x09, 0x09, 0x09, 0x3C, 0x6B,
  0x65, 0x79, 0x3E, 0x49, 0x4F, 0x43, 0x6C, 0x61,
  0x73, 0x73, 0x3C, 0x2F, 0x6B, 0x65, 0x79, 0x3E,
  0x0D, 0x09, 0x09, 0x09, 0x3C, 0x73, 0x74, 0x72,
  0x69, 0x6E, 0x67, 0x3E, 0x41, 0x70, 0x70, 0x6C,
  0x65, 0x41, 0x43, 0x50, 0x49, 0x43, 0x50, 0x55,
  0x3C, 0x2F, 0x73, 0x74, 0x72, 0x69, 0x6E, 0x67,
  0x3E, 0x0D, 0x09, 0x09, 0x09, 0x3C, 0x6B, 0x65,
  0x79, 0x3E, 0x49, 0x4F, 0x4E, 0x61, 0x6D, 0x65,
  0x4D, 0x61, 0x74, 0x63, 0x68, 0x3C, 0x2F, 0x6B,
  0x65, 0x79, 0x3E, 0x0D, 0x09, 0x09, 0x09, 0x3C,
  0x73, 0x74, 0x72, 0x69, 0x6E, 0x67, 0x3E, 0x70,
  0x72, 0x6F, 0x63, 0x65, 0x73, 0x73, 0x6F, 0x72,
  0x3C, 0x2F, 0x73, 0x74, 0x72, 0x69, 0x6E, 0x67,
  0x3E, 0x0D, 0x09, 0x09, 0x09, 0x3C, 0x6B, 0x65,
  0x79, 0x3E, 0x49, 0x4F, 0x50, 0x72, 0x6F, 0x62,
  0x65, 0x53, 0x63, 0x6F, 0x72, 0x65, 0x3C, 0x2F,
  0x6B, 0x65, 0x79, 0x3E, 0x0D, 0x09, 0x09, 0x09,
  0x3C, 0x69, 0x6E, 0x74, 0x65, 0x67, 0x65, 0x72,
  0x3E, 0x31, 0x31, 0x30, 0x30, 0x3C, 0x2F, 0x69,
  0x6E, 0x74, 0x65, 0x67, 0x65, 0x72, 0x3E, 0x0D,
  0x09, 0x09, 0x09, 0x3C, 0x6B, 0x65, 0x79, 0x3E,
  0x49, 0x4F, 0x50, 0x72, 0x6F, 0x76, 0x69, 0x64,
  0x65, 0x72, 0x43, 0x6C, 0x61, 0x73, 0x73, 0x3C,
  0x2F, 0x6B, 0x65, 0x79, 0x3E, 0x0D, 0x09, 0x09,
  0x09, 0x3C, 0x73, 0x74, 0x72, 0x69, 0x6E, 0x67,
  0x3E, 0x49, 0x4F, 0x41, 0x43, 0x50, 0x49, 0x50,
  0x6C, 0x61, 0x74, 0x66, 0x6F, 0x72, 0x6D, 0x44,
  0x65, 0x76, 0x69, 0x63, 0x65, 0x3C, 0x2F, 0x73,
  0x74, 0x72, 0x69, 0x6E, 0x67, 0x3E, 0x0D, 0x09,
  0x09, 0x09, 0x3C, 0x6B, 0x65, 0x79, 0x3E, 0x4F,
  0x70, 0x65, 0x6E, 0x43, 0x6F, 0x72, 0x65, 0x3C,
  0x2F, 0x6B, 0x65, 0x79, 0x3E, 0x0D, 0x09, 0x09,
  0x09, 0x3C, 0x73, 0x74, 0x72, 0x69, 0x6E, 0x67,
  0x3E, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57,
  0x6F, 0x72, 0x6C, 0x64, 0x21, 0x3C, 0x2F, 0x73,
  0x74, 0x72, 0x69, 0x6E, 0x67, 0x3E, 0x0D, 0x09,
  0x09, 0x3C, 0x2F, 0x64, 0x69, 0x63, 0x74, 0x3E,
  0x0D, 0x09, 0x3C, 0x2F, 0x64, 0x69, 0x63, 0x74,
  0x3E, 0x0D, 0x09, 0x3C, 0x6B, 0x65, 0x79, 0x3E,
  0x4E, 0x53, 0x48, 0x75, 0x6D, 0x61, 0x6E, 0x52,
  0x65, 0x61, 0x64, 0x61, 0x62, 0x6C, 0x65, 0x43,
  0x6F, 0x70, 0x79, 0x72, 0x69, 0x67, 0x68, 0x74,
  0x3C, 0x2F, 0x6B, 0x65, 0x79, 0x3E, 0x0D, 0x09,
  0x3C, 0x73, 0x74, 0x72, 0x69, 0x6E, 0x67, 0x3E,
  0x43, 0x6F, 0x70, 0x79, 0x72, 0x69, 0x67, 0x68,
  0x74, 0x20, 0xC2, 0xA9, 0x20, 0x32, 0x30, 0x31,
  0x39, 0x20, 0x76, 0x69, 0x74, 0x39, 0x36, 0x39,
  0x36, 0x2E, 0x20, 0x41, 0x6C, 0x6C, 0x20, 0x72,
  0x69, 0x67, 0x68, 0x74, 0x73, 0x20, 0x72, 0x65,
  0x73, 0x65, 0x72, 0x76, 0x65, 0x64, 0x2E, 0x3C,
  0x2F, 0x73, 0x74, 0x72, 0x69, 0x6E, 0x67, 0x3E,
  0x0D, 0x09, 0x3C, 0x6B, 0x65, 0x79, 0x3E, 0x4F,
  0x53, 0x42, 0x75, 0x6E, 0x64, 0x6C, 0x65, 0x52,
  0x65, 0x71, 0x75, 0x69, 0x72, 0x65, 0x64, 0x3C,
  0x2F, 0x6B, 0x65, 0x79, 0x3E, 0x0D, 0x09, 0x3C,
  0x73, 0x74, 0x72, 0x69, 0x6E, 0x67, 0x3E, 0x52,
  0x6F, 0x6F, 0x74, 0x3C, 0x2F, 0x73, 0x74, 0x72,
  0x69, 0x6E, 0x67, 0x3E, 0x0D, 0x3C, 0x2F, 0x64,
  0x69, 0x63, 0x74, 0x3E, 0x0D, 0x3C, 0x2F, 0x70,
  0x6C, 0x69, 0x73, 0x74, 0x3E
};

long long current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}

uint8_t *readFile(const char *str, uint32_t *size) {
  FILE *f = fopen(str, "rb");

  if (!f) return NULL;

  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fseek(f, 0, SEEK_SET);

  uint8_t *string = malloc(fsize + 1);
  fread(string, fsize, 1, f);
  fclose(f);

  string[fsize] = 0;
  *size = fsize;

  return string;
}

STATIC
UINT8
IOAHCIBlockStoragePatchFind[] = {
  0x41, 0x50, 0x50, 0x4C, 0x45, 0x20, 0x53, 0x53, 0x44, 0x00
};

STATIC
UINT8
IOAHCIBlockStoragePatchReplace[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

STATIC
PATCHER_GENERIC_PATCH
IOAHCIBlockStoragePatch = {
  .Base    = NULL, // Symbolic patch
  .Find    = IOAHCIBlockStoragePatchFind,
  .Mask    = NULL,
  .Replace = IOAHCIBlockStoragePatchReplace,
  .Size    = sizeof (IOAHCIBlockStoragePatchFind),
  .Count   = 1,
  .Skip    = 0
};

STATIC
UINT8
IOAHCIPortPatchFind[] = {
  0x45, 0x78, 0x74, 0x65, 0x72, 0x6E, 0x61, 0x6C
};

STATIC
UINT8
IOAHCIPortPatchReplace[] = {
  0x49, 0x6E, 0x74, 0x65, 0x72, 0x6E, 0x61, 0x6C
};

STATIC
PATCHER_GENERIC_PATCH
IOAHCIPortPatch = {
  .Base    = NULL, // For symbolic patch
  .Find    = IOAHCIPortPatchFind,
  .Mask    = NULL,
  .Replace = IOAHCIPortPatchReplace,
  .Size    = sizeof (IOAHCIPortPatchFind),
  .Count   = 1,
  .Skip    = 0
};

STATIC
UINT8
DisableAppleHDAPatchReplace[] = {
  0x31, 0xC0, 0xC3 // xor eax, eax ; ret
};

STATIC
PATCHER_GENERIC_PATCH
DisableAppleHDAPatch = {
  .Base    = "__ZN20AppleHDACodecGeneric5probeEP9IOServicePi",
  .Find    = NULL,
  .Mask    = NULL,
  .Replace = DisableAppleHDAPatchReplace,
  .Size    = sizeof (DisableAppleHDAPatchReplace),
  .Count   = 1,
  .Skip    = 0
};

STATIC
VOID
ApplyKextPatches (
  PRELINKED_CONTEXT  *Context
  )
{
  EFI_STATUS       Status;
  PATCHER_CONTEXT  Patcher;

  Status = PatcherInitContextFromPrelinked (
    &Patcher,
    Context,
    "com.apple.iokit.IOAHCIBlockStorage"
    );

  if (!EFI_ERROR (Status)) {
    Status = PatcherApplyGenericPatch (&Patcher, &IOAHCIBlockStoragePatch);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "Failed to apply patch com.apple.iokit.IOAHCIBlockStorage - %r\n", Status));
    } else {
      DEBUG ((DEBUG_WARN, "Patch success com.apple.iokit.IOAHCIBlockStorage\n"));
    }
  } else {
    DEBUG ((DEBUG_WARN, "Failed to find com.apple.iokit.IOAHCIBlockStorage - %r\n", Status));
  }

  Status = PatcherInitContextFromPrelinked (
    &Patcher,
    Context,
    "com.apple.driver.AppleAHCIPort"
    );

  if (!EFI_ERROR (Status)) {
    Status = PatcherApplyGenericPatch (&Patcher, &IOAHCIPortPatch);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "Failed to apply patch com.apple.driver.AppleAHCIPort - %r\n", Status));
    } else {
      DEBUG ((DEBUG_WARN, "Patch success com.apple.driver.AppleAHCIPort\n"));
    }
  } else {
    DEBUG ((DEBUG_WARN, "Failed to find com.apple.driver.AppleAHCIPort - %r\n", Status));
  }

  Status = PatcherInitContextFromPrelinked (
    &Patcher,
    Context,
    "com.apple.driver.AppleHDA"
    );

  if (!EFI_ERROR (Status)) {
    Status = PatcherApplyGenericPatch (&Patcher, &DisableAppleHDAPatch);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "Failed to apply patch com.apple.driver.AppleHDA - %r\n", Status));
    } else {
      DEBUG ((DEBUG_WARN, "Patch success com.apple.driver.AppleHDA\n"));
    }
  } else {
    DEBUG ((DEBUG_WARN, "Failed to find com.apple.driver.AppleHDA - %r\n", Status));
  }
}

int main(int argc, char** argv) {
  UINT32 Size;
  UINT32 AllocSize;
  UINT8  *Prelinked;
  PRELINKED_CONTEXT Context;
  if ((Prelinked = readFile(argc > 1 ? argv[1] : "prelinkedkernel.unpack", &Size)) == NULL) {
    printf("Read fail\n");
    return -1;
  }


  AllocSize = PRELINKED_ALIGN (Size + sizeof (KextInfoPlistData));

  Prelinked = realloc (Prelinked, AllocSize);
  if (Prelinked == NULL) {
    printf("Realloc fail\n");
    return -1;
  }

  EFI_STATUS Status = PrelinkedContextInit (&Context, Prelinked, Size, AllocSize);

  if (!EFI_ERROR (Status)) {
    ApplyKextPatches (&Context);

    Status = PrelinkedInjectPrepare (&Context);
    if (EFI_ERROR (Status)) {
      printf("Prelink inject prepare error %zx\n", Status);
    }

    Status = PrelinkedInjectKext (
      &Context,
      "/Library/Extensions/TestDriver.kext",
      KextInfoPlistData,
      sizeof (KextInfoPlistData),
      NULL,
      NULL,
      0
      );

    if (EFI_ERROR (Status)) {
      printf("Prelink inject error %zx\n", Status);
    }

    Status = PrelinkedInjectComplete (&Context);

    if (EFI_ERROR (Status)) {
      printf("Prelink inject complete error %zx\n", Status);
    }

    FILE *Fh = fopen("out.bin", "wb");

    if (Fh != NULL) {
      fwrite (Prelinked, Context.PrelinkedSize, 1, Fh);
      fclose(Fh);

      if (!EFI_ERROR (Status)) {
        printf("All good\n");
      }
    } else {
      printf("File error\n");
    }

    PrelinkedContextFree (&Context);
  } else {
    printf("Context creation error %zx\n", Status);
  }

  free(Prelinked);

  return 0;
}

INT32 LLVMFuzzerTestOneInput(CONST UINT8 *Data, UINTN Size) {
  VOID *NewData = AllocatePool (Size);
  if (NewData) {
    CopyMem (NewData, Data, Size);
    XML_DOCUMENT *doc = XmlDocumentParse((char *)NewData, Size, TRUE);
    if (doc != NULL) {
      UINT32 s;
      CHAR8 *buf = XmlDocumentExport(doc, &s, 0);
      if (buf != NULL) {
        FreePool (buf);
      }
      XmlDocumentFree (doc);
    }
    FreePool (NewData);
  }
  return 0;
}
