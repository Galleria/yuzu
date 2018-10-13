// Copyright 2018 yuzu emulator team
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <vector>
#include "common/common_funcs.h"
#include "common/common_types.h"
#include "common/swap.h"
#include "core/file_sys/vfs_types.h"

namespace Core::Crypto {

enum class Package2Type {
    NormalMain,
    NormalSub,
    SafeModeMain,
    SafeModeSub,
    RepairMain,
    RepairSub,
};

class PartitionDataManager {
public:
    static const u8 MAX_KEYBLOB_SOURCE_HASH;

    explicit PartitionDataManager(FileSys::VirtualDir sysdata_dir);
    ~PartitionDataManager();

    // BOOT0
    bool HasBoot0() const;
    FileSys::VirtualFile GetBoot0Raw() const;
    std::array<u8, 0xB0> GetEncryptedKeyblob(u8 index) const;
    std::array<std::array<u8, 0xB0>, 0x20> GetEncryptedKeyblobs() const;
    std::vector<u8> GetSecureMonitor() const;
    std::array<u8, 0x10> GetPackage2KeySource() const;
    std::array<u8, 0x10> GetAESKekGenerationSource() const;
    std::array<u8, 0x10> GetTitlekekSource() const;
    std::array<std::array<u8, 0x10>, 0x20> GetTZMasterKeys(std::array<u8, 0x10> master_key) const;
    std::array<u8, 0x10> GetRSAKekSeed3() const;
    std::array<u8, 0x10> GetRSAKekMask0() const;
    std::vector<u8> GetPackage1Decrypted() const;
    std::array<u8, 0x10> GetMasterKeySource() const;
    std::array<u8, 0x10> GetKeyblobMACKeySource() const;
    std::array<u8, 0x10> GetKeyblobKeySource(u8 revision) const;

    // Fuses
    bool HasFuses() const;
    FileSys::VirtualFile GetFusesRaw() const;
    std::array<u8, 0x10> GetSecureBootKey() const;

    // K-Fuses
    bool HasKFuses() const;
    FileSys::VirtualFile GetKFusesRaw() const;

    // Package2
    bool HasPackage2(Package2Type type = Package2Type::NormalMain) const;
    FileSys::VirtualFile GetPackage2Raw(Package2Type type = Package2Type::NormalMain) const;
    void DecryptPackage2(std::array<std::array<u8, 16>, 0x20> package2, Package2Type type);
    const std::vector<u8>& GetPackage2FSDecompressed(
        Package2Type type = Package2Type::NormalMain) const;
    std::array<u8, 0x10> GetKeyAreaKeyApplicationSource(
        Package2Type type = Package2Type::NormalMain) const;
    std::array<u8, 0x10> GetKeyAreaKeyOceanSource(
        Package2Type type = Package2Type::NormalMain) const;
    std::array<u8, 0x10> GetKeyAreaKeySystemSource(
        Package2Type type = Package2Type::NormalMain) const;
    std::array<u8, 0x10> GetSDKekSource(Package2Type type = Package2Type::NormalMain) const;
    std::array<u8, 0x20> GetSDSaveKeySource(Package2Type type = Package2Type::NormalMain) const;
    std::array<u8, 0x20> GetSDNCAKeySource(Package2Type type = Package2Type::NormalMain) const;
    std::array<u8, 0x10> GetHeaderKekSource(Package2Type type = Package2Type::NormalMain) const;
    std::array<u8, 0x20> GetHeaderKeySource(Package2Type type = Package2Type::NormalMain) const;
    const std::vector<u8>& GetPackage2SPLDecompressed(
        Package2Type type = Package2Type::NormalMain) const;
    std::array<u8, 0x10> GetAESKeyGenerationSource(
        Package2Type type = Package2Type::NormalMain) const;

    // PRODINFO
    bool HasProdInfo() const;
    FileSys::VirtualFile GetProdInfoRaw() const;
    void DecryptProdInfo(std::array<u8, 0x20> bis_key);
    std::array<u8, 0x240> GetETicketExtendedKek() const;

private:
    FileSys::VirtualFile boot0;
    FileSys::VirtualFile fuses;
    FileSys::VirtualFile kfuses;
    std::array<FileSys::VirtualFile, 6> package2;
    FileSys::VirtualFile prodinfo;
    FileSys::VirtualFile secure_monitor;
    FileSys::VirtualFile package1_decrypted;

    // Processed
    std::array<FileSys::VirtualFile, 6> package2_decrypted;
    FileSys::VirtualFile prodinfo_decrypted;
    std::vector<u8> secure_monitor_bytes;
    std::vector<u8> package1_decrypted_bytes;
    std::array<std::vector<u8>, 6> package2_fs;
    std::array<std::vector<u8>, 6> package2_spl;
};

std::array<u8, 0x10> FindKeyFromHex16(const std::vector<u8>& binary, std::array<u8, 0x20> hash);

} // namespace Core::Crypto