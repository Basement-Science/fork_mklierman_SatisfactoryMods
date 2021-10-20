// ILikeBanas

#include "HologramLocation_BPLib.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <regex>

FString UHologramLocation_BPLib::RegexReplace(FString inputString, FString regexCode, FString replacementText = "")
{
    std::string originalString = std::string(TCHAR_TO_UTF8(*inputString));
    std::string replacementString = std::string(TCHAR_TO_UTF8(*replacementText));
    std::regex regexp(std::string(TCHAR_TO_UTF8(*regexCode)));
    return std::regex_replace(originalString, regexp, replacementString).c_str();

}

FString UHologramLocation_BPLib::AddCommasToInt(int inputInt)
{
    int number = inputInt;
    auto src = std::to_string(number);
    auto dest = std::string();

    auto count = 3;
    for (auto i = src.crbegin(); i != src.crend(); ++i) {
        if (count == 0)
        {
            dest.push_back(',');
            count = 3;
        }
        if (count--) {
            dest.push_back(*i);
        }
    }
    std::reverse(dest.begin(), dest.end());

    return dest.c_str();
}

FVector UHologramLocation_BPLib::GetConnectionComponentLocation(UFGFactoryConnectionComponent* connection)
{
    return connection->GetConnectorLocation();
}

FVector UHologramLocation_BPLib::GetConnectionComponentRotation(UFGFactoryConnectionComponent* connection)
{
    return connection->GetConnectorNormal();
}

FTransform UHologramLocation_BPLib::GetLiftHologramTransform(AFGConveyorLiftHologram* clHologram)
{
    return clHologram->GetTopTransform();
}

FString UHologramLocation_BPLib::FormatIntAsNumberString(int32 number, bool usePeriod)
{
    FString formattedString;
    formattedString = formattedString.FormatAsNumber(number);
    if (usePeriod)
    {
        formattedString = formattedString.Replace(TEXT(","), TEXT("."),ESearchCase::IgnoreCase);
    }
    return formattedString;
}
