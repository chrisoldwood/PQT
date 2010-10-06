////////////////////////////////////////////////////////////////////////////////
//! \file   ParameterParser.cpp
//! \brief  .
//! \author Chris Oldwood

#include "Common.hpp"
#include "ParameterParser.hpp"
#include <WCL/StrArray.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Find all parameters embedded in the query text.

bool FindParameters(const CString& strQuery, CStrArray& astrParams, CString& strError)
{
	size_t nParamStart = 0;

	// For all parameters.
	while ((nParamStart = strQuery.Find(TXT("${"), nParamStart)) != Core::npos)
	{
		// Find parameter end marker.
		size_t nParamEnd = strQuery.Find(TXT('}'), nParamStart+1);

		// Parameter end maker not found?
		if (nParamEnd == Core::npos)
		{
			strError = TXT("Parameter terminator '}' missing.");
			return false;
		}

		// Extract parameter name.
		CString strParam = strQuery.Mid(nParamStart+2, nParamEnd-nParamStart-2);

		if (strParam.Empty())
		{
			strError = TXT("Nameless parameter '${}' found."); 
			return false;
		}

		// Add to list, if a new parameter.
		if (astrParams.Find(strParam) == Core::npos)
			astrParams.Add(strParam);

		// Skip passed parameter.
		nParamStart = nParamEnd+1;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Replace all parameters with their values.

CString ReplaceParameters(const CString& strQuery, CStrArray astrParams, CStrArray astrValues)
{
	ASSERT(astrParams.Size() == astrValues.Size());

	CString strResult;

	size_t nParamStart = 0;
	size_t nParamEnd = nParamStart - 1;

	// For all parameters.
	while ((nParamStart = strQuery.Find(TXT("${"), nParamStart)) != Core::npos)
	{
		// Copy query up to parameter start marker.
		strResult += strQuery.Mid(nParamEnd+1, nParamStart-nParamEnd-1);

		// Find parameter end marker.
		nParamEnd = strQuery.Find(TXT('}'), nParamStart+1);

		ASSERT(nParamEnd != Core::npos);

		// Extract parameter name.
		CString strParam = strQuery.Mid(nParamStart+2, nParamEnd-nParamStart-2);

		// Find parameter index.
		size_t nParam = astrParams.Find(strParam);

		ASSERT(nParam != Core::npos);

		// Substitute value.
		strResult += astrValues[nParam];

		// Skip passed parameter.
		nParamStart = nParamEnd+1;
	}

	// Copy tail end of query.
	strResult += strQuery.Mid(nParamEnd+1, strQuery.Length()-nParamEnd-1);

	return strResult;
}
