#pragma once

class SceneGame;

class DataManager final
{
private:
public:
	static bool LoadGame(const std::shared_ptr<SceneGame>& sceneGame);
	static bool SaveGame(const std::shared_ptr<SceneGame>& sceneGame);
};

