#include "stdafx.h"
#include "AssetFiles.h"




bool AssetFiles::Add(eAssetId key, const std::wstring& assetFile)
{
	auto it = m_AssetFiles.find(key);
	if (it == m_AssetFiles.end() && key != eAssetId::Empty)
	{
		m_AssetFiles[key] = assetFile;
		return true;
	}
	return false;
}
bool AssetFiles::Get(eAssetId key, std::wstring& assetFile) const
{
	if (key == eAssetId::Empty)
	{
		assetFile = L"";
		return true;
	}
	auto it = m_AssetFiles.find(key);
	if (it != m_AssetFiles.end())
	{
		assetFile = m_BasePath + (*it).second;
		return true;
	}
	Logger::LogWarning(L"AssetFiles::Get >> Cannot find Asset on ID: " + std::to_wstring(int(key)));
	assetFile = m_BasePath + m_Default;
	return true;
}
