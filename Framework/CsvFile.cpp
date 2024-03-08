#include "pch.h"
#include "CsvFile.h"
#include <io.h>
bool CsvFile::loadFromFile(const std::string& filePath)
{
	errno_t err = 0;
	if ((err = _access_s(filePath.c_str(), 4)) == 0)
	{
		doc.Load(filePath);
		return true;
	}
	return false;
}

bool CsvFile::IsUnknown() const
{
	return doc.GetColumnName(0) == "CANT";
}

CsvFile::CsvFile()
{
}

CsvFile::CsvFile(const std::string& filePath)
	:doc(filePath)
{

}

CsvFile::~CsvFile()
{
}
