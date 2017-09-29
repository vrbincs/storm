#include <upnp/upnp.h>
#include <cstring>
#include <set>

#include "logger.h"
#include "cupnpwww.h"
#include "cupnphelper.h"

class CUPnPWWWPriv;
CUPnPWWWPriv *l_upnpWWWPriv = NULL;

#define CSTRING(STR_DEST, STR_SRC) size_t __str_len = (strlen(STR_SRC) + 1); STR_DEST = (char *)malloc(__str_len); memcpy(STR_DEST, STR_SRC, __str_len)

struct FileData
{
   const char *buffer;
   size_t bufferLength;
   uint32_t lastModified;
   
   FileData()
      : buffer(NULL),
        bufferLength(0),
        lastModified(CUPnPHelper::getCurrentLocalTime())
   {
   }
   
   virtual ~FileData()
   {
      LOGGER_TRACE("File buffer");
      delete buffer;
   }
   
   static FileData *create() 
   {
      return new FileData();
   }
};

struct FileHandle
{
   FileHandle(const FileData *fileData_t)
      : pos(0),
        fileData(fileData_t)
   {
   }
      
   size_t pos;
   const FileData *fileData;
};

class CUPnPWWWPriv
{
public:
   CUPnPWWWPriv()
      : m_cupnpWwwInst(new CUPnPWWW())
   {
      int err;
      UpnpVirtualDirCallbacks virtualDirCallback = {getInfo, open, read, write, seek, close};
      
      if((err = UpnpSetVirtualDirCallbacks(&virtualDirCallback)) != UPNP_E_SUCCESS)
      {
         LOGGER_ERROR("Unable to set handler for mamanging virtual dir callbacks. err=" << err);
      }
   }
   
   virtual ~CUPnPWWWPriv()
   {
      auto it0 = m_fileSystem.begin();
      for(; it0 != m_fileSystem.end(); it0++)
      {
         auto it1 = it0->second.begin();
         for(; it1 != it0->second.end(); it1++)
         {
            delete it1->second;
         }
      }
   }
   
   FileData *findFile(const std::string &filePath)
   {
      std::string dir;
      std::string file;
      
      if(!CUPnPWWW::extractPath(filePath, dir, file))
      {
         LOGGER_TRACE("Bad file path. filePath=" << filePath);
         return NULL;
      }
      
      auto dir_it = l_upnpWWWPriv->m_fileSystem.find(dir);
      if(dir_it != l_upnpWWWPriv->m_fileSystem.end())
      {
         auto file_it = dir_it->second.find(file);
         if(file_it != dir_it->second.end())
         {
            return file_it->second;
         }
         else
         {
            LOGGER_WARN("file does not exist. dir=" << dir << ", filePath=" << filePath);
         }
      }
      else
      {
         LOGGER_WARN("Directory does not exist. dir=" << dir);
      }
      
      return NULL;
   }
   
   FileHandle *fileOpen(const std::string &filePath)
   {
      FileData *fileInfo = findFile(filePath);
      
      if(fileInfo)
      {
         FileHandle *fileHnd = new FileHandle(fileInfo);
         m_openFileHnd.insert(fileHnd);
         
         return fileHnd;
      }
      
      return NULL;
   }
   
   bool fileClose(FileHandle *fileHandle)
   {
      auto hnd_it = m_openFileHnd.find(fileHandle);
      
      if(hnd_it != m_openFileHnd.end())
      {
         delete *hnd_it;
         m_openFileHnd.erase(hnd_it);
         
         LOGGER_TRACE("File closed. m_openFileHnd.size() =" << m_openFileHnd.size());
         return true;
      }
      else
      {
         LOGGER_WARN("File not opened. fileHandle=" << fileHandle);
      }
      
      return false;
   }
   

   static int getInfo(const char *fileName, struct File_Info* info)
   {
      LOGGER_TRACE("getInfo. fileName=" << fileName);
      
      FileData *fileInfo = l_upnpWWWPriv->findFile(fileName);
      if(fileInfo)
      {
         char *contentType;
         CSTRING(contentType, "text/xml");
         
         info->file_length   = fileInfo->bufferLength;
         info->last_modified = fileInfo->lastModified;
         info->is_directory  = false;
         info->is_readable   = true;
         info->content_type  = contentType;
         
         LOGGER_TRACE("File access. fileName=" << fileName << ", content_type=" << info->content_type << ", last_modified=" << info->last_modified << ", file_length=" << info->file_length);
         
         return 0;
      }
      else
      {
         LOGGER_ERROR("Unable to find file. fileName=" << fileName);
      }
   
      return -1;
   }

   static UpnpWebFileHandle open(const char *fileName, enum UpnpOpenFileMode Mode)
   {
      LOGGER_TRACE("open::" << fileName);
      return l_upnpWWWPriv->fileOpen(fileName);
   }
   
   static int close(UpnpWebFileHandle fileHnd)
   {
      LOGGER_TRACE("close::" << fileHnd);
      FileHandle *fileHandle = reinterpret_cast<FileHandle *>(fileHnd);
      
      if(l_upnpWWWPriv->fileClose(fileHandle))
      {
         return 0;
      }
      
      return 1;
   }

   static int read(UpnpWebFileHandle fileHnd, char *buffer, size_t buflen)
   {
      LOGGER_TRACE("read. buflen=" << buflen);
      FileHandle *fileHandle = reinterpret_cast<FileHandle *>(fileHnd);
      
      if(fileHandle)
      {
         size_t buflen_t = buflen;
         
         if((fileHandle->pos + buflen) > fileHandle->fileData->bufferLength)
         {
            buflen_t = fileHandle->fileData->bufferLength;
         }
         
         memcpy(buffer, fileHandle->fileData->buffer, buflen_t);
      }
      else
      {
         LOGGER_ERROR("read error. Invalid file handle. fileHnd=" << fileHnd);
      }
      
      return -1;
   }

   static int seek(UpnpWebFileHandle fileHnd, long offset, int origin)
   {
      LOGGER_TRACE("seek. fileHnd=" << fileHnd << ", offset=" << offset << ", " << origin);
      FileHandle *fileHandle = reinterpret_cast<FileHandle *>(fileHnd);
      
      if(fileHandle)
      {
         if(origin == SEEK_CUR)
         {
            fileHandle->pos += offset;
         }
         else if(origin == SEEK_END)
         {
            fileHandle->pos = (fileHandle->fileData->bufferLength - offset);
         }
         else if(origin == SEEK_SET)
         {
            fileHandle->pos = offset;
         }
         
         if(fileHandle->pos > fileHandle->fileData->bufferLength)
         {
            fileHandle->pos = fileHandle->fileData->bufferLength;
            LOGGER_WARN("seek requested beyond EOF.");
         }
         else if(fileHandle->pos < 0)
         {
            fileHandle->pos = 0;
            LOGGER_WARN("seek requested beyond BOF.");
         }
         else
         {
            return 0;
         }
      }
      else
      {
         LOGGER_ERROR("seek error. Invalid file handle. fileHnd=" << fileHnd);
      }
      
      return -1;
   }

   static int write(UpnpWebFileHandle fileHnd, char *buf, size_t buflen)
   {
      LOGGER_TRACE("write NOT SUPPORTED!");
      return -1;
   }
   
   CUPnPWWW *instance()
   {
      return m_cupnpWwwInst;
   }
   
   std::map<std::string, std::map<std::string, FileData *> > m_fileSystem;
   CUPnPWWW *m_cupnpWwwInst;
   std::set<FileHandle *> m_openFileHnd;
};

CUPnPWWW::CUPnPWWW()
{
   
}

CUPnPWWW::~CUPnPWWW()
{
}

bool CUPnPWWW::addDir(const std::string &dir)
{
   int err;
   if((err = UpnpAddVirtualDir(dir.c_str())) != UPNP_E_SUCCESS)
   {
      LOGGER_ERROR("Unable to add virtual dir. err=" << err);
      return false;
   }
   
   auto dir_it = l_upnpWWWPriv->m_fileSystem.find(dir);
   if(dir_it == l_upnpWWWPriv->m_fileSystem.end())
   {
      l_upnpWWWPriv->m_fileSystem[dir] = std::map<std::string, FileData *>();
      LOGGER_TRACE("Directory added to the www server; dir=" << dir);
   }
   
   return true;
}

bool CUPnPWWW::addFile(const std::string &dir, 
                       const std::string &filename,
                       const char *fileBuf,
                       size_t fileBufLen)
{
   auto dir_it = l_upnpWWWPriv->m_fileSystem.find(dir);
   
   if(dir_it == l_upnpWWWPriv->m_fileSystem.end())
   {
      LOGGER_WARN("Directory does not exist. dir=" << dir);
      return false;
   }
   
   auto file_it = dir_it->second.find(filename);
   if(file_it != dir_it->second.end())
   {
      LOGGER_WARN("File already exists. file=" << filename);
      return false;
   }
   
   
   FileData *fileData = FileData::create();
   fileData->buffer = fileBuf;
   fileData->bufferLength = fileBufLen;
   
   dir_it->second.insert(std::pair<std::string, FileData *>(filename, fileData));
   
   LOGGER_TRACE("File added to the www server; dir=" << dir << ", filename=" << filename);
   
   return true;
}

bool CUPnPWWW::addFileWithPath(const std::string &filePath,
                               const char *fileBuf,
                               size_t fileBufLen)
{
   std::string dir;
   std::string file;
   
   if(!extractPath(filePath, dir, file))
   {
      LOGGER_WARN("Inavlid path. filePath=" << filePath);
      return false;
   }
   
   if(l_upnpWWWPriv->m_fileSystem.find(dir) == l_upnpWWWPriv->m_fileSystem.end())
   {
      if(!addDir(dir))
      {
         LOGGER_WARN("Unable to add directory. dir=" << dir);
         return false;
      }
   }
   
   addFile(dir, file, fileBuf, fileBufLen);
   
   return false;
}

bool CUPnPWWW::extractPath(const std::string &filePath,
                           std::string &dir,
                           std::string &file)
{
   if(filePath.empty())
   {
      LOGGER_ERROR("Unable to extract path. File path should not be empty.");
      return false;
   }
   
   size_t posBeg = 0;
   size_t posEnd = filePath.find_last_of('/');
   
   if(posEnd == std::string::npos)
   {
      dir = '/';
      posEnd = 0;
   }
   else
   {
      dir = filePath.substr(posBeg, posEnd);
      posEnd++;
   }
   
   file = filePath.substr(posEnd);
   
   return true;
}

CUPnPWWW *CUPnPWWW::instance()
{
   if(!l_upnpWWWPriv)
   {
      l_upnpWWWPriv = new CUPnPWWWPriv();
   }
   
   return l_upnpWWWPriv->instance();
}
