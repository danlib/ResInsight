/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2017  Statoil ASA
// 
//  ResInsight is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
// 
//  ResInsight is distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.
// 
//  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html> 
//  for more details.
//
/////////////////////////////////////////////////////////////////////////////////

#include "RifReaderEclipseRft.h"

#include "ert/ecl/ecl_rft_file.h"

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RifReaderEclipseRft::RifReaderEclipseRft(const std::string& fileName):
    m_fileName(fileName), m_ecl_rft_file(nullptr)
{
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RifReaderEclipseRft::~RifReaderEclipseRft()
{
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RifReaderEclipseRft::open()
{
    if (m_fileName.empty()) return;

    m_ecl_rft_file = ecl_rft_file_alloc_case(m_fileName.data());

    if (m_ecl_rft_file == NULL) return;

    int fileSize = ecl_rft_file_get_size(m_ecl_rft_file);

    for (int i = 0; i < fileSize; i++)
    {
        ecl_rft_node_type* node = ecl_rft_file_iget_node(m_ecl_rft_file, i);
        
        std::string wellNameStdString = ecl_rft_node_get_well_name(node);
        QString wellName(wellNameStdString.c_str());
        m_wellNames.insert(wellName);

        time_t timeStepTime_t = ecl_rft_node_get_date(node);
        QDateTime timeStep;
        timeStep.setTimeSpec(Qt::TimeSpec::UTC);
        timeStep.setTime_t(timeStepTime_t);

        RifEclipseRftAddress addressPressure(wellName, timeStep, RifEclipseRftAddress::PRESSURE);
        m_eclipseRftAddresses.push_back(addressPressure);
        m_rftAddressToLibeclNodeIdx[addressPressure] = i;

        RifEclipseRftAddress addressDepth(wellName, timeStep, RifEclipseRftAddress::DEPTH);
        m_eclipseRftAddresses.push_back(addressDepth);
        m_rftAddressToLibeclNodeIdx[addressDepth] = i;

        if (ecl_rft_node_is_RFT(node))
        {
            RifEclipseRftAddress addressSwat(wellName, timeStep, RifEclipseRftAddress::SWAT);
            m_eclipseRftAddresses.push_back(addressSwat);
            m_rftAddressToLibeclNodeIdx[addressSwat] = i;

            RifEclipseRftAddress addressSoil(wellName, timeStep, RifEclipseRftAddress::SOIL);
            m_eclipseRftAddresses.push_back(addressSoil);
            m_rftAddressToLibeclNodeIdx[addressSoil] = i;

            RifEclipseRftAddress addressSgas(wellName, timeStep, RifEclipseRftAddress::SGAS);
            m_eclipseRftAddresses.push_back(addressSgas);
            m_rftAddressToLibeclNodeIdx[addressSgas] = i;
        }
        else if (ecl_rft_node_is_PLT(node))
        {
            RifEclipseRftAddress addressWrat(wellName, timeStep, RifEclipseRftAddress::WRAT);
            m_eclipseRftAddresses.push_back(addressWrat);
            m_rftAddressToLibeclNodeIdx[addressWrat] = i;

            RifEclipseRftAddress addressOrat(wellName, timeStep, RifEclipseRftAddress::ORAT);
            m_eclipseRftAddresses.push_back(addressOrat);
            m_rftAddressToLibeclNodeIdx[addressOrat] = i;

            RifEclipseRftAddress addressGrat(wellName, timeStep, RifEclipseRftAddress::GRAT);
            m_eclipseRftAddresses.push_back(addressGrat);
            m_rftAddressToLibeclNodeIdx[addressGrat] = i;
        }
    }

    return;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
const std::vector<RifEclipseRftAddress>& RifReaderEclipseRft::eclipseRftAddresses()
{
    if (!m_ecl_rft_file)
    {
        open();
    }

    return m_eclipseRftAddresses;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RifReaderEclipseRft::values(const RifEclipseRftAddress& rftAddress, std::vector<double>* values)
{
    CVF_ASSERT(values);

    if (!m_ecl_rft_file)
    {
        open();
    }
    
    values->clear();

    int index = indexFromAddress(rftAddress);
    if (index < 0) return;

    ecl_rft_node_type* node = ecl_rft_file_iget_node(m_ecl_rft_file, index);
    
    RifEclipseRftAddress::RftWellLogChannelName wellLogChannelName = rftAddress.wellLogChannelName();

    switch (wellLogChannelName)
    {
    case RifEclipseRftAddress::DEPTH:
    {
        for (int i = 0; i < ecl_rft_node_get_size(node); i++)
        {
            values->push_back(ecl_rft_node_iget_depth(node, i));
        }
        break;
    }
    case RifEclipseRftAddress::PRESSURE:
    {
        for (int i = 0; i < ecl_rft_node_get_size(node); i++)
        {
            values->push_back(ecl_rft_node_iget_pressure(node, i));
        }
        break;
    }
    case RifEclipseRftAddress::SWAT:
    {
        for (int i = 0; i < ecl_rft_node_get_size(node); i++)
        {
            values->push_back(ecl_rft_node_iget_swat(node, i));
        }
        break;
    }
    case RifEclipseRftAddress::SOIL:
    {
        for (int i = 0; i < ecl_rft_node_get_size(node); i++)
        {
            values->push_back(ecl_rft_node_iget_soil(node, i));
        }
        break;
    }
    case RifEclipseRftAddress::SGAS:
    {
        for (int i = 0; i < ecl_rft_node_get_size(node); i++)
        {
            values->push_back(ecl_rft_node_iget_sgas(node, i));
        }
        break;
    }
    case RifEclipseRftAddress::WRAT:
    {
        for (int i = 0; i < ecl_rft_node_get_size(node); i++)
        {
            values->push_back(ecl_rft_node_iget_wrat(node, i));
        }
        break;
    }
    case RifEclipseRftAddress::ORAT:
    {
        for (int i = 0; i < ecl_rft_node_get_size(node); i++)
        {
            values->push_back(ecl_rft_node_iget_orat(node, i));
        }
        break;
    }
    case RifEclipseRftAddress::GRAT:
    {
        for (int i = 0; i < ecl_rft_node_get_size(node); i++)
        {
            values->push_back(ecl_rft_node_iget_grat(node, i));
        }
        break;
    }
    default:
        break;
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
std::vector<QDateTime> RifReaderEclipseRft::availableTimeSteps(const QString& wellName, const RifEclipseRftAddress::RftWellLogChannelName& wellLogChannelName)
{
    if (!m_ecl_rft_file)
    {
        open();
    }

    std::vector<QDateTime> timeSteps;

    if (wellName == "") return timeSteps;

    for (const RifEclipseRftAddress& address : m_eclipseRftAddresses)
    {
        if (address.wellName() == wellName && address.wellLogChannelName() == wellLogChannelName)
        {
            timeSteps.push_back(address.timeStep());
        }
    }
    return timeSteps;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
std::vector<RifEclipseRftAddress::RftWellLogChannelName> RifReaderEclipseRft::availableWellLogChannels(const QString& wellName)
{
    if (!m_ecl_rft_file)
    {
        open();
    }

    std::vector<RifEclipseRftAddress::RftWellLogChannelName> wellLogChannelNames;

    if (wellName == "") return wellLogChannelNames;

    for (const RifEclipseRftAddress& address : m_eclipseRftAddresses)
    {
        if (address.wellName() == wellName)
        {
            RifEclipseRftAddress::RftWellLogChannelName name = address.wellLogChannelName();
            if (name != RifEclipseRftAddress::RftWellLogChannelName::DEPTH)
            {
                wellLogChannelNames.push_back(address.wellLogChannelName());
            }
        }
    }
    return wellLogChannelNames;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
const std::set<QString>& RifReaderEclipseRft::wellNames()
{
    if (!m_ecl_rft_file)
    {
        open();
    }

    return m_wellNames;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
int RifReaderEclipseRft::indexFromAddress(const RifEclipseRftAddress& rftAddress) const
{
    auto it = m_rftAddressToLibeclNodeIdx.find(rftAddress);

    if (it != m_rftAddressToLibeclNodeIdx.end())
    {
        return it->second;
    }

    return -1;
}
