#include <gtest/gtest.h>

#include <supnp.h>
#include <cmediaserverdelegate.h>

class TestCMediaServerDelegate : public ::testing::Test, public CMediaServerDelegate
{
public:
   TestCMediaServerDelegate()
      : CMediaServerDelegate("aa54689e-627e-483c-839e-cbc3af80384f",
                             "Test Media Server",
                             "saso.vrbinc@gmail.com",
                             "https://github.com/vrbincs")
   {
   }
};


TEST_F(TestCMediaServerDelegate, TestRegisterService)
{
   auto serviceList = getServiceList();
   
   ASSERT_EQ(serviceList.size(), 1);
}
