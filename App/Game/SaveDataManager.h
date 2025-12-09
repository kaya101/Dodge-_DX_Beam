#pragma once
#include "Graph.h"
#include "../Utils/Common.h"

// セーブデータ.
class SaveData
{
public:
	explicit SaveData() = default;
	explicit SaveData(const String& saveDataSubDirName, const GraphCamera& camera, const Graph& graph);
	~SaveData() = default;

	// 代入演算子.
	SaveData& operator=(const SaveData& other);

	// シリアライズに対応させるためのメンバ関数.
	template <class Archive>
	void SIV3D_SERIALIZE(Archive& archive) { archive(saveDataSubDirName); }

	String saveDataSubDirName;	// セーブデータのディレクトリ名.
	//GraphCamera camera;			// カメラ.
	//Graph graph;				// グラフ.
	SampleCount numSamples; // サンプリング数.
	LogicRange logicRange; // 表示している論理座標範囲.
	CoeffMatrix coeffMatrix; // 係数行列.
};


// 書き込むクラス.
class SaveDataWriter
{
public:
	explicit constexpr SaveDataWriter() = default;
	constexpr ~SaveDataWriter() = default;
	void store(const SaveData& sd, const FilePath& subDirPath) const;

private:
	String SaveDataFileName() const;
};


// 読み込むクラス.
class SaveDataLoader
{
public:
	explicit constexpr SaveDataLoader() = default;
	constexpr ~SaveDataLoader() = default;
	void load(SaveData& sd, const FilePath& subDirPath) const;

private:
	FilePath FindSaveDataFilePath(const FilePath& subDirPath) const;
	static bool IsBinFile(const FilePath& path);
};


// セーブデータを管理するクラス.
using FileName = String;
class GameData;
class SaveDataManager : public SaveDataWriter, public SaveDataLoader
{
public:
	explicit constexpr SaveDataManager() = default;
	constexpr ~SaveDataManager() = default;
	void load(GameData& gd) const;
	void store(const GameData& gd) const;
	String RootDirName() const { return m_RootDirName; }
	std::vector<String> SubDirNames() const { return m_SubDirNames; }

private:
	const String m_RootDirName = U"UserData";
	const std::vector<String> m_SubDirNames = { U"File1", U"File2", U"File3" };
	static const std::map<String, SaveData> m_datas;
};
