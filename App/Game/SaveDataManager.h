#pragma once
#include "Graph.h"

// セーブデータ.
class SaveData
{
public:
	SaveData() = default;
	SaveData(const String& saveDataSubDirName, const GraphCamera& camera, const Graph& graph);
	~SaveData() = default;

	// 代入演算子.
	SaveData& operator=(const SaveData& other);

	// シリアライズに対応させるためのメンバ関数.
	template <class Archive>
	void SIV3D_SERIALIZE(Archive& archive) { archive(saveDataSubDirName, camera, graph); }

	String saveDataSubDirName;	// セーブデータのディレクトリ名.
	GraphCamera camera;			// カメラ.
	Graph graph;				// グラフ.
};


// 書き込むクラス.
class SaveDataWriter
{
public:
	constexpr SaveDataWriter() = default;
	constexpr ~SaveDataWriter() = default;
	void Store(const SaveData& sd, const FilePath& subDirPath) const;

private:
	String SaveDataFileName() const;
};


// 読み込むクラス.
class SaveDataLoader
{
public:
	constexpr SaveDataLoader() = default;
	constexpr ~SaveDataLoader() = default;
	void Load(SaveData& sd, const FilePath& subDirPath) const;

private:
	FilePath FindSaveDataFilePath(const FilePath& subDirPath) const;
	static bool IsBinFile(const FilePath& path);
};


// セーブデータを管理するクラス.
class GameData;
class SaveDataManager : public SaveDataWriter, public SaveDataLoader
{
public:
	constexpr SaveDataManager() = default;
	constexpr ~SaveDataManager() = default;
	void Load(GameData& gd) const;
	void Store(const GameData& gd) const;
	String RootDirName() const { return _rootDirName; }
	std::vector<String> SubDirNames() const { return _subDirNames; }

private:
	const String _rootDirName = U"UserData";
	const std::vector<String> _subDirNames = { U"File1", U"File2", U"File3" };
};
