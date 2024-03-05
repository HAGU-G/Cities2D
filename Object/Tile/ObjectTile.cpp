#include "pch.h"
#include "ObjectTile.h"

ObjectTile::ObjectTile(std::weak_ptr<Scene> scene, GAME_OBJECT_TYPE objectType, const sf::Vector2i& gridCoord)
	:GameObject(scene, objectType), gridCoord(gridCoord)
{

}

ObjectTile::~ObjectTile()
{
	Release();
}

bool ObjectTile::AddAdjacnet(const std::string& key, ADDIREC ad, std::weak_ptr<ObjectTile> ptr)
{
	if (adjacent.find(ad) != adjacent.end())
	{
		adjacent[ad] = std::make_pair(key, ptr);
		ptr.lock()->AddAdjacnet(GetKey(), ADDIREC(~ad), std::dynamic_pointer_cast<ObjectTile, GameObject>(This()));
		return true;
	}
	else
	{
		return false;
	}
}

void ObjectTile::RemoveAdjacent(ADDIREC ad)
{
	auto it = adjacent.find(ad);
	if (it != adjacent.end())
	{
		adjacent.erase(it);
	}
}

void ObjectTile::Init()
{
	sceneTest = std::dynamic_pointer_cast<SceneTest, Scene>(scene.lock());
	SetPosition(sf::Vector2f(gridCoord) * sceneTest.lock()->GetGridSize().x);
	gridCenterPos = position + sceneTest.lock()->GetGridSize() * 0.5f;

	SFGM_TEXTURE.Load("resource/building/House01.png");

	Reset();
}

void ObjectTile::Update(float timeDelta, float timeScale)
{
	tempSprite.setRotation(scene.lock()->GetView().getRotation());
}

void ObjectTile::Draw(sf::RenderWindow& window)
{
	window.draw(tempSprite);
}

void ObjectTile::Reset()
{
	tempSprite.setTexture(SFGM_TEXTURE.Get("resource/building/House01.png"));
	tempSprite.setOrigin(tempSprite.getGlobalBounds().getSize() * 0.5f);
}

void ObjectTile::Release()
{
	GameObject::Release();
	if (!adjacent.empty())
	{
		for (auto& pair : adjacent)
		{
			pair.second.second.lock()->RemoveAdjacent(ADDIREC(~pair.first));
		}
		adjacent.clear();
	}
}

std::shared_ptr<ObjectTile> ObjectTile::Create(std::weak_ptr<Scene> scene, GAME_OBJECT_TYPE objectType, const sf::Vector2i& gridCoord)
{
	std::shared_ptr<ObjectTile> objectTile = std::make_shared<ObjectTile>(scene, objectType, gridCoord);
	objectTile->Init();
	scene.lock()->AddObject(objectTile);

	return objectTile;
}

void ObjectTile::SetPosition(const sf::Vector2f& position)
{
	GameObject::SetPosition(position);
	tempSprite.setPosition(position + sceneTest.lock()->GetGridSize() * 0.5f);
}
