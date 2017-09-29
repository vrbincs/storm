#ifndef CUPNPWWW_H
   #define CUPNPWWW_H

#include <string>
#include <map>

class CUPnPWWW
{
public:   
   virtual ~CUPnPWWW();
   
   bool addDir(const std::string &dir);
   bool addFile(const std::string &dir, 
                const std::string &filename,
                const char *fileBuf,
                size_t fileBufLen);
                
   bool addFileWithPath(const std::string &filePath,
                        const char *fileBuf,
                        size_t fileBufLen);

   static CUPnPWWW *instance();
   static bool extractPath(const std::string &filePath,
                           std::string &dir,
                           std::string &file);
private:
   CUPnPWWW();
   friend class CUPnPWWWPriv;
};

#endif // CUPNPWWW_H
