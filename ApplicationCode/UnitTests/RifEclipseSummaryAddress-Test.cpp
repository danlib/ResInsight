#include "gtest/gtest.h"

#include "RifEclipseSummaryAddress.h"

#include <string>

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
TEST(RifEclipseSummaryAddressTest, TestEclipseAddressParsing_Field)
{
    std::string addrString = "FOPT";

    RifEclipseSummaryAddress addr = RifEclipseSummaryAddress::fromEclipseTextAddress(addrString);

    EXPECT_TRUE(addr.isValid());
    EXPECT_EQ(RifEclipseSummaryAddress::SUMMARY_FIELD, addr.category());
    EXPECT_EQ("FOPT", addr.quantityName());
    EXPECT_FALSE(addr.isErrorResult());
}

TEST(RifEclipseSummaryAddressTest, TestEclipseAddressParsing_Aquifer)
{
    std::string addrString = "AAQU:456";

    RifEclipseSummaryAddress addr = RifEclipseSummaryAddress::fromEclipseTextAddress(addrString);

    EXPECT_TRUE(addr.isValid());
    EXPECT_EQ(RifEclipseSummaryAddress::SUMMARY_AQUIFER, addr.category());
    EXPECT_EQ("AAQU", addr.quantityName());
    EXPECT_EQ(456, addr.aquiferNumber());
    EXPECT_FALSE(addr.isErrorResult());
}

TEST(RifEclipseSummaryAddressTest, TestEclipseAddressParsing_Network)
{
    std::string addrString = "NETW";

    RifEclipseSummaryAddress addr = RifEclipseSummaryAddress::fromEclipseTextAddress(addrString);

    EXPECT_TRUE(addr.isValid());
    EXPECT_EQ(RifEclipseSummaryAddress::SUMMARY_NETWORK, addr.category());
    EXPECT_EQ("NETW", addr.quantityName());
    EXPECT_FALSE(addr.isErrorResult());
}

TEST(RifEclipseSummaryAddressTest, DISABLED_TestEclipseAddressParsing_Misc)
{
    std::string addrString = "MISC";

    RifEclipseSummaryAddress addr = RifEclipseSummaryAddress::fromEclipseTextAddress(addrString);

    EXPECT_TRUE(addr.isValid());
    EXPECT_EQ(RifEclipseSummaryAddress::SUMMARY_MISC, addr.category());
    EXPECT_EQ("NETW", addr.quantityName());
    EXPECT_FALSE(addr.isErrorResult());
}

TEST(RifEclipseSummaryAddressTest, TestEclipseAddressParsing_Region)
{
    std::string addrString = "REGI:7081";

    RifEclipseSummaryAddress addr = RifEclipseSummaryAddress::fromEclipseTextAddress(addrString);

    EXPECT_TRUE(addr.isValid());
    EXPECT_EQ(RifEclipseSummaryAddress::SUMMARY_REGION, addr.category());
    EXPECT_EQ("REGI", addr.quantityName());
    EXPECT_EQ(7081, addr.regionNumber());
    EXPECT_FALSE(addr.isErrorResult());
}

TEST(RifEclipseSummaryAddressTest, TestEclipseAddressParsing_RegionToRegion)
{
    std::string addrString = "REFR:7081-8001";

    RifEclipseSummaryAddress addr = RifEclipseSummaryAddress::fromEclipseTextAddress(addrString);

    EXPECT_TRUE(addr.isValid());
    EXPECT_EQ(RifEclipseSummaryAddress::SUMMARY_REGION_2_REGION, addr.category());
    EXPECT_EQ("REFR", addr.quantityName());
    EXPECT_EQ(7081, addr.regionNumber());
    EXPECT_EQ(8001, addr.regionNumber2());
    EXPECT_FALSE(addr.isErrorResult());
}

TEST(RifEclipseSummaryAddressTest, TestEclipseAddressParsing_WellGroup)
{
    std::string addrString = "GGRP:WELLS1";

    RifEclipseSummaryAddress addr = RifEclipseSummaryAddress::fromEclipseTextAddress(addrString);

    EXPECT_TRUE(addr.isValid());
    EXPECT_EQ(RifEclipseSummaryAddress::SUMMARY_WELL_GROUP, addr.category());
    EXPECT_EQ("GGRP", addr.quantityName());
    EXPECT_EQ("WELLS1", addr.wellGroupName());
    EXPECT_FALSE(addr.isErrorResult());
}

TEST(RifEclipseSummaryAddressTest, TestEclipseAddressParsing_Well)
{
    std::string addrString = "WGAS:B-2H";

    RifEclipseSummaryAddress addr = RifEclipseSummaryAddress::fromEclipseTextAddress(addrString);

    EXPECT_TRUE(addr.isValid());
    EXPECT_EQ(RifEclipseSummaryAddress::SUMMARY_WELL, addr.category());
    EXPECT_EQ("WGAS", addr.quantityName());
    EXPECT_EQ("B-2H", addr.wellName());
    EXPECT_FALSE(addr.isErrorResult());
}

TEST(RifEclipseSummaryAddressTest, TestEclipseAddressParsing_WellCompletion)
{
    std::string addrString = "CDATA:B-1H:15,13,14";

    RifEclipseSummaryAddress addr = RifEclipseSummaryAddress::fromEclipseTextAddress(addrString);

    EXPECT_TRUE(addr.isValid());
    EXPECT_EQ(RifEclipseSummaryAddress::SUMMARY_WELL_COMPLETION, addr.category());
    EXPECT_EQ("CDATA", addr.quantityName());
    EXPECT_EQ("B-1H", addr.wellName());
    EXPECT_EQ(15, addr.cellI());
    EXPECT_EQ(13, addr.cellJ());
    EXPECT_EQ(14, addr.cellK());
    EXPECT_FALSE(addr.isErrorResult());
}

TEST(RifEclipseSummaryAddressTest, TestEclipseAddressParsing_WellLgr)
{
    std::string addrString = "LWABC:LGRNA:B-10H";

    RifEclipseSummaryAddress addr = RifEclipseSummaryAddress::fromEclipseTextAddress(addrString);

    EXPECT_TRUE(addr.isValid());
    EXPECT_EQ(RifEclipseSummaryAddress::SUMMARY_WELL_LGR, addr.category());
    EXPECT_EQ("LWABC", addr.quantityName());
    EXPECT_EQ("LGRNA", addr.lgrName());
    EXPECT_EQ("B-10H", addr.wellName());
    EXPECT_FALSE(addr.isErrorResult());
}

TEST(RifEclipseSummaryAddressTest, TestEclipseAddressParsing_WellCompletionLgr)
{
    std::string addrString = "LCGAS:LGR1:B-1H:11,12,13";

    RifEclipseSummaryAddress addr = RifEclipseSummaryAddress::fromEclipseTextAddress(addrString);

    EXPECT_TRUE(addr.isValid());
    EXPECT_EQ(RifEclipseSummaryAddress::SUMMARY_WELL_COMPLETION_LGR, addr.category());
    EXPECT_EQ("LCGAS", addr.quantityName());
    EXPECT_EQ("LGR1", addr.lgrName());
    EXPECT_EQ("B-1H", addr.wellName());
    EXPECT_EQ(11, addr.cellI());
    EXPECT_EQ(12, addr.cellJ());
    EXPECT_EQ(13, addr.cellK());
    EXPECT_FALSE(addr.isErrorResult());
}

TEST(RifEclipseSummaryAddressTest, TestEclipseAddressParsing_WellSegment)
{
    std::string addrString = "SGMT:B-5H:32";

    RifEclipseSummaryAddress addr = RifEclipseSummaryAddress::fromEclipseTextAddress(addrString);

    EXPECT_TRUE(addr.isValid());
    EXPECT_EQ(RifEclipseSummaryAddress::SUMMARY_WELL_SEGMENT, addr.category());
    EXPECT_EQ("SGMT", addr.quantityName());
    EXPECT_EQ("B-5H", addr.wellName());
    EXPECT_EQ(32, addr.wellSegmentNumber());
    EXPECT_FALSE(addr.isErrorResult());
}

TEST(RifEclipseSummaryAddressTest, TestEclipseAddressParsing_Block)
{
    std::string addrString = "BLOC:123,122,121";

    RifEclipseSummaryAddress addr = RifEclipseSummaryAddress::fromEclipseTextAddress(addrString);

    EXPECT_TRUE(addr.isValid());
    EXPECT_EQ(RifEclipseSummaryAddress::SUMMARY_BLOCK, addr.category());
    EXPECT_EQ("BLOC", addr.quantityName());
    EXPECT_EQ(123, addr.cellI());
    EXPECT_EQ(122, addr.cellJ());
    EXPECT_EQ(121, addr.cellK());
    EXPECT_FALSE(addr.isErrorResult());
}

TEST(RifEclipseSummaryAddressTest, TestEclipseAddressParsing_BlockLgr)
{
    std::string addrString = "LBABC:LGRN:45,47,49";

    RifEclipseSummaryAddress addr = RifEclipseSummaryAddress::fromEclipseTextAddress(addrString);

    EXPECT_TRUE(addr.isValid());
    EXPECT_EQ(RifEclipseSummaryAddress::SUMMARY_BLOCK_LGR, addr.category());
    EXPECT_EQ("LBABC", addr.quantityName());
    EXPECT_EQ("LGRN", addr.lgrName());
    EXPECT_EQ(45, addr.cellI());
    EXPECT_EQ(47, addr.cellJ());
    EXPECT_EQ(49, addr.cellK());
    EXPECT_FALSE(addr.isErrorResult());
}

TEST(RifEclipseSummaryAddressTest, TestEclipseAddressParsing_Imported)
{
    std::string addrString = "FAULT (Imp)";

    RifEclipseSummaryAddress addr = RifEclipseSummaryAddress::fromEclipseTextAddress(addrString);

    EXPECT_TRUE(addr.isValid());
    EXPECT_EQ(RifEclipseSummaryAddress::SUMMARY_IMPORTED, addr.category());
    EXPECT_EQ("FAULT (Imp)", addr.quantityName());
    EXPECT_FALSE(addr.isErrorResult());
}

TEST(RifEclipseSummaryAddressTest, TestEclipseAddressParsing_ErrorResult1)
{
    std::string addrString = "ER:AAQU:456";

    RifEclipseSummaryAddress addr = RifEclipseSummaryAddress::fromEclipseTextAddress(addrString);

    EXPECT_TRUE(addr.isValid());
    EXPECT_EQ(RifEclipseSummaryAddress::SUMMARY_AQUIFER, addr.category());
    EXPECT_EQ("AAQU", addr.quantityName());
    EXPECT_EQ(456, addr.aquiferNumber());
    EXPECT_TRUE(addr.isErrorResult());
}

TEST(RifEclipseSummaryAddressTest, TestEclipseAddressParsing_ErrorResult2)
{
    std::string addrString = "ERR:LCGAS:LGR1:B-1H:11,12,13";

    RifEclipseSummaryAddress addr = RifEclipseSummaryAddress::fromEclipseTextAddress(addrString);

    EXPECT_TRUE(addr.isValid());
    EXPECT_EQ(RifEclipseSummaryAddress::SUMMARY_WELL_COMPLETION_LGR, addr.category());
    EXPECT_EQ("LCGAS", addr.quantityName());
    EXPECT_EQ("LGR1", addr.lgrName());
    EXPECT_EQ("B-1H", addr.wellName());
    EXPECT_EQ(11, addr.cellI());
    EXPECT_EQ(12, addr.cellJ());
    EXPECT_EQ(13, addr.cellK());
    EXPECT_TRUE(addr.isErrorResult());
}

TEST(RifEclipseSummaryAddressTest, TestEclipseAddressParsing_ErrorResult3)
{
    std::string addrString = "ERROR:FAULT (Imp)";

    RifEclipseSummaryAddress addr = RifEclipseSummaryAddress::fromEclipseTextAddress(addrString);

    EXPECT_TRUE(addr.isValid());
    EXPECT_EQ(RifEclipseSummaryAddress::SUMMARY_IMPORTED, addr.category());
    EXPECT_EQ("FAULT (Imp)", addr.quantityName());
    EXPECT_TRUE(addr.isErrorResult());
}