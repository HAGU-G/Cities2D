#include "pch.h"
#include "SceneTitle.h"

SceneTitle::SceneTitle(const std::string& name)
	:Scene(name)
{
}

void SceneTitle::Resource()
{
	SFGM_SOUNDBUFFER.Load("resource/music/LittleClose.wav");
}

void SceneTitle::Init()
{
	musicGameStart.openFromFile("resource/music/Title.wav");
	musicGameStart.play();
	background00.setTexture(SFGM_TEXTURE.Load("resource/ui/Background00.png"));
	background01.setTexture(SFGM_TEXTURE.Load("resource/ui/Background01.png"));
	background02.setTexture(SFGM_TEXTURE.Load("resource/ui/Background02.png"));
	background00.setScale(9.f, 9.f);
	background01.setScale(9.f, 9.f);
	background01.setColor({ 255,255,255,180 });
	background02.setScale(9.f, 9.f);
	tool::SetOrigin(background00, ORIGIN::CC);
	tool::SetOrigin(background01, ORIGIN::CC);
	tool::SetOrigin(background02, ORIGIN::CC);
	tool::SetOrigin(background02, ORIGIN::CC);

	Resource();
	Scene::Init();

	textTitle.setFillColor(sf::Color::White);
	textTitle.setFont(SFGM_FONT.Load("resource/font/ROKAF Sans Bold.ttf"));
	textTitle.setCharacterSize(180);
	textTitle.setString("CITIES");
	textTitle.setPosition(view.getSize().x * 0.5f - 5.f, view.getSize().y);
	tool::SetOrigin(textTitle, ORIGIN::TC);

	textTitle2.setFillColor({ 20,20,20,230 });
	textTitle2.setFont(SFGM_FONT.Load("resource/font/ROKAF Sans Bold.ttf"));
	textTitle2.setCharacterSize(340);
	textTitle2.setString("2D");
	tool::SetOrigin(textTitle2, ORIGIN::TC);

	textTitleShadow.setFillColor({ 0,0,50,140 });
	textTitleShadow.setFont(SFGM_FONT.Load("resource/font/ROKAF Sans Bold.ttf"));
	textTitleShadow.setCharacterSize(180);
	textTitleShadow.setString(textTitle.getString());
	tool::SetOrigin(textTitleShadow, ORIGIN::TC);

	viewCenter = view.getCenter();
	background00.setPosition(viewCenter);
	background01.setPosition(viewCenter);
	background02.setPosition(viewCenter);
}

void SceneTitle::Update(float timeDelta, float timeScale)
{
	Scene::Update(timeDelta, timeScale);


	if (moveTimer <= 3.8f)
	{
		moveTimer += timeDelta * timeScale;
		textTitle.setPosition(textTitle.getPosition() + sf::Vector2f(0.f, -220.f) * timeDelta * timeScale);
		textTitle2.setPosition(textTitle.getPosition());
		textTitleShadow.setPosition(textTitle.getPosition() + sf::Vector2f(10.f, 10.f));
	}
	else if (moveTimer < 6.0f)
	{
		moveTimer += timeDelta * timeScale;
		textTitle2.setPosition(textTitle2.getPosition() + sf::Vector2f(0.f, -150.f) * timeDelta * timeScale);
	}

	if (IOManager::IsMouseInWindow())
	{
		background00.setPosition(background00.getPosition() +( (GetMousePosWorld() - viewCenter) *0.01f+ viewCenter- background00.getPosition()) * timeDelta * timeScale);
		background01.setPosition(background01.getPosition() + ((GetMousePosWorld() - viewCenter) *0.03f+viewCenter- background01.getPosition()) * timeDelta * timeScale);
		background02.setPosition(background02.getPosition() + ((GetMousePosWorld() - viewCenter) *0.05f+ viewCenter- background02.getPosition()) * timeDelta * timeScale);
	}
	else
	{
		background00.setPosition(background00.getPosition() + (viewCenter - background00.getPosition()) * timeDelta * timeScale);
		background01.setPosition(background01.getPosition() + (viewCenter - background01.getPosition()) * timeDelta * timeScale);
		background02.setPosition(background02.getPosition() + (viewCenter - background02.getPosition()) * timeDelta * timeScale);
	}
}

void SceneTitle::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
	window.draw(background00);
	window.draw(textTitle2);
	window.draw(background01);
	window.draw(textTitleShadow);
	window.draw(textTitle);
	window.draw(background02);
}
