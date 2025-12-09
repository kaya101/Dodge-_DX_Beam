#include "../stdafx.h"
#include "SaveDataManager.h"
#include "../Utils/Common.h"

/*	FileData start		********************************************************************************************************************/

SaveData::SaveData(const String& saveDataSubDirName, const GraphCamera& camera, const Graph& graph)
	: saveDataSubDirName(saveDataSubDirName) {}

SaveData& SaveData::operator=(const SaveData& other)
{
	if (this != &other)
	{
		saveDataSubDirName = other.saveDataSubDirName;
	}
	return *this;
}

/*	FileData end		********************************************************************************************************************/


/*		SaveDataWriter start		*********************************************************************************************************/

void SaveDataWriter::store(const SaveData& sd, const FilePath& subDirPath) const
{
	const FilePath savePath = FileSystem::PathAppend(subDirPath, SaveDataFileName());
	FileSystem::CreateDirectories(FileSystem::ParentPath(savePath));
	Serializer<BinaryWriter> writer{ savePath };
	if (not writer) throw Error{ U"Failed to open `{}`"_fmt(savePath) };
	writer(sd);
}

String SaveDataWriter::SaveDataFileName() const
{
	return String(U"save_" + DateTime::NowUTC().format(U"'UTC'_yyyy-MM-dd_HH_mm_ss_SSS") + U".bin");
}

/*		SaveDataWriter end			*********************************************************************************************************/


/*		SaveDataLoader start		*********************************************************************************************************/

void SaveDataLoader::load(SaveData& sd, const FilePath& subDirPath) const
{
	const FilePath savePath = FindSaveDataFilePath(subDirPath);
	Deserializer<BinaryReader> reader{ savePath };
	if (not reader) throw Error{ U"Failed to open `{}`"_fmt(savePath) };
	SaveData saveData{};
	reader(saveData);
	sd = saveData;
}

FilePath SaveDataLoader::FindSaveDataFilePath(const FilePath& subDirPath) const
{
	if (not FileSystem::Exists(subDirPath))
	{
		Print << U"指定ディレクトリ：'{}'が見つかりません。"_fmt(subDirPath);
		return U"";
	}
	const Array<FilePath> paths = FileSystem::DirectoryContents(subDirPath, Recursive::No);
	Array<FilePath> binFiles = paths.filter(IsBinFile);
	if (binFiles.isEmpty())
	{
		Print << U"セーブファイルが見つかりません。";
		return U"";
	}
	std::sort(binFiles.begin(), binFiles.end(),
		[](const FilePath& a, const FilePath& b)
		{
			return FileSystem::WriteTime(a) > FileSystem::WriteTime(b);
		}
	);
	return binFiles.front();
}

bool SaveDataLoader::IsBinFile(const FilePath& path)
{
	return (FileSystem::Extension(path).lowercase() == U"bin");
}

/*		SaveDataLoader end			*********************************************************************************************************/


/*		SaveDataManager start		*********************************************************************************************************/

void SaveDataManager::load(GameData& gd) const
{
	//SaveDataLoader::load(gd.saveData, FileSystem::PathAppend(m_RootDirName, gd.saveData.saveDataSubDirName));
}

void SaveDataManager::store(const GameData& gd) const
{
	//SaveDataWriter::store(gd.saveData, FileSystem::PathAppend(m_RootDirName, gd.saveData.saveDataSubDirName));
}

/*		SaveDataManager end			*********************************************************************************************************/
