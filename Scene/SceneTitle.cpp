#include "pch.h"
#include "SceneTitle.h"

SceneTitle::SceneTitle(const std::string& name)
	:Scene(name)
{
}
void SceneTitle::Init()
{
	Scene::Init();
	background00.setTexture(SFGM_TEXTURE.Load("resource/ui/Background00.png"));
	background01.setTexture(SFGM_TEXTURE.Load("resource/ui/Background01.png"));
	background02.setTexture(SFGM_TEXTURE.Load("resource/ui/Background02.png"));
	sf::Vector2f tempScale(1.f,1.f);
	if (view.getSize().x > view.getSize().y)
	{
		tempScale *= 9.f / GameManager::GetDefaultSize().x * view.getSize().x;
	}
	else
	{
		tempScale *= 9.f / GameManager::GetDefaultSize().y * view.getSize().y;
	}
	background00.setScale(tempScale);
	background01.setScale(tempScale);
	background01.setColor({ 255,255,255,180 });
	background02.setScale(tempScale);
	tool::SetOrigin(background00, ORIGIN::CC);
	tool::SetOrigin(background01, ORIGIN::CC);
	tool::SetOrigin(background02, ORIGIN::CC);
	tool::SetOrigin(background02, ORIGIN::CC);


	viewCenter = view.getCenter();
	background00.setPosition(viewCenter);
	background01.setPosition(viewCenter);
	background02.setPosition(viewCenter);
}

void SceneTitle::Update(float timeDelta, float timeScale)
{
	Scene::Update(timeDelta, timeScale);
	if (firstLoad)
		PostInit();
	loadingIcon.rotate(360 * timeDelta * timeScale);

	if (textTitle.getPosition() != titlePos)
	{
		sf::Vector2f titleDir = tool::GetNormalize(titlePos - textTitle.getPosition());
		textTitle.setPosition( textTitle.getPosition() + titleDir * view.getSize().y * 0.2f * timeDelta * timeScale);
		if (tool::Distance(titlePos, textTitle.getPosition()) <= view.getSize().y * 0.2f * timeDelta * timeScale)
			textTitle.setPosition(titlePos);
		textTitleShadow.setPosition(textTitle.getPosition() + sf::Vector2f(10.f, 10.f));
	}

	if (textTitle2.getPosition() != title2Pos)
	{
		sf::Vector2f title2Dir = tool::GetNormalize(title2Pos - textTitle2.getPosition());
		textTitle2.setPosition(textTitle2.getPosition() + title2Dir * view.getSize().y * 0.2f * timeDelta * timeScale);
		if (tool::Distance(title2Pos, textTitle2.getPosition()) <= view.getSize().y * 0.2f * timeDelta * timeScale)
			textTitle2.setPosition(title2Pos);
	}
	if (IOManager::IsMouseInWindow())
	{
		background00.setPosition(background00.getPosition() + ((mousePosWorld - viewCenter) * -0.01f + viewCenter - background00.getPosition()) * timeDelta * timeScale);
		background01.setPosition(background01.getPosition() + ((mousePosWorld - viewCenter) * -0.03f + viewCenter - background01.getPosition()) * timeDelta * timeScale);
		background02.setPosition(background02.getPosition() + ((mousePosWorld - viewCenter) * -0.05f + viewCenter - background02.getPosition()) * timeDelta * timeScale);
	}
	else
	{
		background00.setPosition(background00.getPosition() + (viewCenter - background00.getPosition()) * timeDelta * timeScale);
		background01.setPosition(background01.getPosition() + (viewCenter - background01.getPosition()) * timeDelta * timeScale);
		background02.setPosition(background02.getPosition() + (viewCenter - background02.getPosition()) * timeDelta * timeScale);
	}

	if (isLoaded)
	{
		isLoaded = false;
		loadingIcon.setColor(sf::Color::Transparent);
		SceneManager::Use("SceneMenu");
	}
}

void SceneTitle::PostInit()
{
	firstLoad = false;

	textTitle.setFillColor(sf::Color::White);
	textTitle.setFont(SFGM_FONT.Load("resource/font/ROKAF Sans Bold.ttf"));
	textTitle.setCharacterSize(180);
	textTitle.setString("CITIES");
	tool::SetOrigin(textTitle, ORIGIN::TC);
	textTitle.setPosition(viewCenter.x - 10.f, view.getSize().y);
	titlePos = viewCenter + sf::Vector2f(-10.f, -view.getSize().y * 0.33);

	textTitle2.setFillColor({ 20,20,20,230 });
	textTitle2.setFont(SFGM_FONT.Load("resource/font/DOSIyagiBoldface.ttf"));
	textTitle2.setCharacterSize(340);
	textTitle2.setString("2D");
	tool::SetOrigin(textTitle2, ORIGIN::TC);
	textTitle2.setPosition(viewCenter.x-5.f, view.getSize().y);
	title2Pos = titlePos + sf::Vector2f(-5.f,-textTitle2.getGlobalBounds().getSize().y*1.05f);

	textTitleShadow.setFillColor({ 0,0,50,140 });
	textTitleShadow.setFont(SFGM_FONT.Load("resource/font/ROKAF Sans Bold.ttf"));
	textTitleShadow.setCharacterSize(180);
	textTitleShadow.setString(textTitle.getString());
	tool::SetOrigin(textTitleShadow, ORIGIN::TC);

	musicGameStart.setBuffer(SFGM_SOUNDBUFFER.Load("resource/music/Title.wav"));
	musicGameStart.play();

	loadingIcon.setTexture(SFGM_TEXTURE.Load("resource/ui/loading.png"));
	tool::SetOrigin(loadingIcon, ORIGIN::CC);
	loadingIcon.setScale(0.5f, 0.5f);
	loadingIcon.setPosition(view.getSize().x-loadingIcon.getGlobalBounds().width*0.5f, loadingIcon.getGlobalBounds().height * 0.5f);




	std::thread t1(&SceneTitle::LoadingGame, this);
	t1.detach();
}

void SceneTitle::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);

	if (firstLoad)
	{
		window.draw(background00);
		window.draw(background01);
		window.draw(background02);
	}
	else
	{
		window.draw(background00);
		window.draw(textTitle2);
		window.draw(background01);
		window.draw(textTitleShadow);
		window.draw(textTitle);
		window.draw(background02);
		window.draw(loadingIcon);
	}
}

void SceneTitle::LoadingGame()
{
	//난수 생성기
	GameManager::rd = new std::random_device;
	GameManager::rg = new std::mt19937((*GameManager::rd)());

	//리소스 준비
	SceneManager::canChange = false;
	SceneManager::Wait("SceneGame");
	SceneManager::Wait("SceneGameUI");
	SceneManager::Wait("SceneMenu",true);
	SceneManager::canChange = true;
	isLoaded = true;
}
