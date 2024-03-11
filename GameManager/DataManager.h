#pragma once

class SceneGame;

class DataManager final
{
private:
public:
	static bool SaveTile(const std::shared_ptr<SceneGame>& sceneGame);
	static bool SaveUnit(const std::shared_ptr<SceneGame>& sceneGame);
	static bool SaveMayor(const std::shared_ptr<SceneGame>& sceneGame);

	static bool LoadTile(const std::shared_ptr<SceneGame>& sceneGame);
	static bool LoadUnit(const std::shared_ptr<SceneGame>& sceneGame);
};

