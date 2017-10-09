#include <fstream>

#include <gtest/gtest.h>
#include <rapidxml.hpp>

#include <logger.h>

#include <cupnpactiondesc.h>
#include <cupnpdevicedesc.h>


#define XSTR(x) #x
#define STR(x) XSTR(x)

#define RESOURCE_DEVICE_DESC_XML STR(RESOURCE_PATH)"/resources/mediaServerSCPD.xml"

class TestCUPnPDeviceDesc : public ::testing::Test
{
public:
   std::string openFile(const std::string &xmlFilePath, bool *ok = NULL)
   {
      bool ok_t = false;
      std::string fileContent;
      
      std::ifstream istream(xmlFilePath);
      if(istream.is_open())
      {
         istream.seekg(0, std::ios::end);
         size_t strSize = (1 + istream.tellg());
         fileContent.reserve(strSize);
         istream.seekg(0, std::ios::beg);
         
         fileContent.assign(std::istreambuf_iterator<char>(istream),
                            std::istreambuf_iterator<char>());
         fileContent[strSize-1] = '\0';
         
         ok_t = true;
      }
      
      if(ok)
      {
         *ok = ok_t;
      }
      
      return fileContent;
   }
};


TEST_F(TestCUPnPDeviceDesc, TestDeserialization)
{
   bool ok;
   std::string mediaServerSCPD = openFile(RESOURCE_DEVICE_DESC_XML, &ok);
   ASSERT_TRUE(ok) << "Unable to open XML file. fileName=" << RESOURCE_DEVICE_DESC_XML;
   
   rapidxml::xml_document<> deviceDescXmlDoc;
   rapidxml::xml_node<> *root_node = NULL;
   
   try
   {
      deviceDescXmlDoc.parse<0>(&mediaServerSCPD[0]);
      root_node = deviceDescXmlDoc.first_node("scpd");
   }
   catch(const rapidxml::parse_error &err)
   {
      std::cout << "XML parse error. what='" << err.what() << "'" << std::endl;
      ASSERT_TRUE(false);
   }
   
   CUPnPDeviceDesc upnpDesc("urn:schemas-upnp-org:device:MediaServer:1");
   EXPECT_TRUE(upnpDesc.deserialize(root_node));
   
   EXPECT_EQ(upnpDesc.getActionList().size(), 14);
   
   const CUPnPActionDesc *action0 = upnpDesc.findAction("GetSearchCapabilities");
   ASSERT_TRUE(action0 != NULL);
   const CUPnPActionDesc *action1 = upnpDesc.findAction("GetSortCapabilities");
   ASSERT_TRUE(action1 != NULL);
   const CUPnPActionDesc *action2 = upnpDesc.findAction("Browse");
   ASSERT_TRUE(action2 != NULL);
   const CUPnPActionDesc *action3 = upnpDesc.findAction("GetTransferProgress");
   ASSERT_TRUE(action3 != NULL);
   
   ASSERT_TRUE(action2->findArg("ObjectID") != NULL);
   EXPECT_FALSE(action2->findArg("ObjectID")->isOut());
   EXPECT_TRUE(action2->findArg("Result")->isOut());
}
