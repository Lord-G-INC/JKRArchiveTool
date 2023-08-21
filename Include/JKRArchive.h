#pragma once
#include "BinaryReaderAndWriter.h"
#include "JKRCompression.h"
#include <vector>
#include <memory>

// Heavily based off https://github.com/SunakazeKun/pygapa/blob/main/jsystem/jkrarchive.py

enum JKRFileAttr {
    JKRFileAttr_FILE = 0x1,
    JKRFileAttr_FOLDER = 0x2,
    JKRFileAttr_COMPRESSED = 0x4,
    JKRFileAttr_LOAD_TO_MRAM = 0x10,
    JKRFileAttr_LOAD_TO_ARAM = 0x20,
    JKRFileAttr_LOAD_FROM_DVD = 0x40,
    JKRFileAttr_USE_SZS = 0x80,

    JKRFileAttr_FILE_AND_COMPRESSION = 0x85,
    JKRFileAttr_FILE_AND_PRELOAD = 0x71,
};

enum JKRPreloadType {
    JKRPreloadType_NONE = -1,
    JKRPreloadType_MRAM = 0,
    JKRPreloadType_ARAM = 1,
    JKRPreloadType_DVD = 2,
};

struct JKRArchiveHeader {
    u32 mDVDFileSize;
    u32 mARAMSize;
    u32 mMRAMSize;
    u32 mFileDataSize;
    u32 mFileDataOffset;
    u32 mHeaderSize;
    u32 mFileSize;
};

struct JKRArchiveDataHeader {
    u32 mStringTableOffset;
    u32 mStringTableSize;
    u32 mFileNodeOffset;
    u32 mFileNodeCount;
    u32 mDirNodeOffset;
    u32 mDirNodeCount;
};

class JKRArchive;
class JKRDirectory;

class JKRFolderNode {
public:
    JKRFolderNode() {}

    struct Node {
        u32 mFirstFileOffs;
        u16 mFileCount;
        u16 mHash;
        u32 mNameOffs;
        u8 mShortName[4];
    };

    void unpack(const std::string &);
    std::string getShortName();

    Node mNode;
    bool mIsRoot = false; 
    std::string mName;
    std::shared_ptr<JKRDirectory> mDirectory;
    std::vector<std::shared_ptr<JKRDirectory>> mChildDirs;
};

class JKRDirectory {
public:
    JKRDirectory();

    struct Node {
        u32 mDataSize;
        u32 mData; 
        u32 mAttrAndNameOffs;
        u16 mHash;
        u16 mNodeIdx;
    };

    JKRCompressionType getCompressionType();
    bool isDirectory() { return mAttr & JKRFileAttr_FOLDER; } 
    bool isFile() { return mAttr & JKRFileAttr_FILE; }
    bool isShortcut() { 
        if (!mName.compare("..") || !mName.compare("."))
            return mAttr & JKRFileAttr_FOLDER; 
        return false;
    }
    JKRPreloadType getPreloadType();

    JKRFileAttr mAttr;
    Node mNode;
    std::shared_ptr<JKRFolderNode> mFolderNode;
    std::shared_ptr<JKRFolderNode> mParentNode;
    std::string mName;
    u16 mNameOffs;
    std::shared_ptr<u8[]> mData;
};

class JKRArchive {
public:
    JKRArchive() {}
    JKRArchive(const std::string &);
    JKRArchive(u8*, u32);

    void unpack(const std::string &);
    void save(const std::string &, bool);
    void importFromFolder(const std::string &, JKRFileAttr);
    std::shared_ptr<JKRDirectory> createDir(const std::string &, JKRFileAttr, std::shared_ptr<JKRFolderNode>, std::shared_ptr<JKRFolderNode>);
    std::shared_ptr<JKRDirectory> createFile(const std::string &, std::shared_ptr<JKRFolderNode>, JKRFileAttr);
    std::shared_ptr<JKRFolderNode> createFolder(const std::string &, std::shared_ptr<JKRFolderNode>);

    std::vector<std::shared_ptr<JKRFolderNode>> mFolderNodes;
    std::vector<std::shared_ptr<JKRDirectory>> mDirectories;
    std::shared_ptr<JKRFolderNode> mRoot = nullptr;

    void read(BinaryReader &);
    void write(BinaryWriter &, bool);
private:
    void writeFileData(BinaryWriter &, std::vector<std::shared_ptr<JKRDirectory>>, u32 *);

    void sortNodesAndDirs();
    void sortNodeAndDirs(std::shared_ptr<JKRFolderNode>);
    bool validateName(std::shared_ptr<JKRFolderNode>, const std::string &);

    void collectStrings(std::shared_ptr<JKRFolderNode>, StringPool*, bool);

    s32 align32(s32 val) {
        return (val + 0x1F) & ~0x1F;
    }

    u16 nameHash(const std::string &);

    void importNode(const std::string &, std::shared_ptr<JKRFolderNode>, JKRFileAttr);

    JKRArchiveHeader mHeader;
    JKRArchiveDataHeader mDataHeader;

    std::vector<std::shared_ptr<JKRDirectory>> mMRAMFiles;
    std::vector<std::shared_ptr<JKRDirectory>> mARAMFiles;
    std::vector<std::shared_ptr<JKRDirectory>> mDVDFiles;
    bool mSyncFileIds = true;
    u16 mNextFileIdx = 0;
};
