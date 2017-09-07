#include <gtest/gtest.h>

#include <cupnpctrl.h>

class TestCUPnPCtrl : public ::testing::Test, public CUPnPCtrl
{
public:

};


TEST_F(TestCUPnPCtrl, TestRegisterDeviceService)
{
   std::string mediaServerSCPD = openFile("mediaServerSCPD.xml");
   
   ASSERT_TRUE(!mediaServerSCPD.empty());
   ASSERT_TRUE(registerDeviceService("urn:schemas-upnp-org:device:MediaServer:1",
                                     mediaServerSCPD));

   
}
