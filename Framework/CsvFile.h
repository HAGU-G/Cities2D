#pragma once
#include "rapidcsv.h"
class CsvFile
{
private:
	rapidcsv::Document doc;
public:
	CsvFile();
	explicit CsvFile(const std::string& filePath);
	virtual ~CsvFile();
	CsvFile(const CsvFile&) = delete;
	CsvFile(CsvFile&&) = delete;
	CsvFile& operator=(const CsvFile&) = delete;
	CsvFile& operator=(CsvFile&&) = delete;

	bool loadFromFile(const std::string& filePath);
	bool IsUnknown() const;
	inline const rapidcsv::Document& GetDocument() const { return doc; }
};

