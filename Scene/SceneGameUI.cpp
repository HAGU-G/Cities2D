#include "pch.h"
#include "SceneGameUI.h"
#include <ObjectButton.h>

SceneGameUI::SceneGameUI(const std::string& name)
	:Scene(name)
{
}

void SceneGameUI::AddResource()
{
	SFGM_TEXTURE.Add("resource/ui/optionSquare.png");
	SFGM_TEXTURE.Add("resource/ui/optionSquareOn.png");
	SFGM_TEXTURE.Add("resource/ui/optionSquareDown.png");
	SFGM_TEXTURE.Add("resource/ui/r_.png");
	SFGM_TEXTURE.Add("resource/ui/r_On.png");
	SFGM_TEXTURE.Add("resource/ui/r_Down.png");
	SFGM_TEXTURE.Add("resource/ui/c_.png");
	SFGM_TEXTURE.Add("resource/ui/c_On.png");
	SFGM_TEXTURE.Add("resource/ui/c_Down.png");
	SFGM_TEXTURE.Add("resource/ui/i_.png");
	SFGM_TEXTURE.Add("resource/ui/i_On.png");
	SFGM_TEXTURE.Add("resource/ui/i_Down.png");
	SFGM_TEXTURE.Add("resource/ui/destroy.png");
	SFGM_TEXTURE.Add("resource/ui/destroyOn.png");
	SFGM_TEXTURE.Add("resource/ui/destroyDown.png");
	SFGM_TEXTURE.Add("resource/ui/road.png");
	SFGM_TEXTURE.Add("resource/ui/roadOn.png");
	SFGM_TEXTURE.Add("resource/ui/roadDown.png");
	SFGM_TEXTURE.Add("resource/ui/play.png");
	SFGM_TEXTURE.Add("resource/ui/playOn.png");
	SFGM_TEXTURE.Add("resource/ui/playDown.png");
	SFGM_TEXTURE.Add("resource/ui/pause.png");
	SFGM_TEXTURE.Add("resource/ui/pauseOn.png");
	SFGM_TEXTURE.Add("resource/ui/pauseDown.png");
	SFGM_TEXTURE.Add("resource/ui/rci.png");
	SFGM_TEXTURE.Add("resource/ui/citizen.png");
	SFGM_TEXTURE.Add("resource/ui/grid.png");
	SFGM_TEXTURE.Add("resource/ui/money.png");

	SFGM_FONT.Add("resource/font/ROKAF Sans Medium.ttf");

	SFGM_SOUNDBUFFER.Add("resource/sfx/InMouse.wav");
	SFGM_SOUNDBUFFER.Add("resource/sfx/Click.wav");
}

void SceneGameUI::Init()
{
	Scene::Init();

	//상단
	ObjectButton::Create(This(), { view.getSize().x - 5.f, 5.f }, "optionSquare")->SetOrigin(ORIGIN::TR);

	//하단
	underBarBack.setSize({ view.getSize().x, 140.f });
	underBarFront.setSize({ view.getSize().x, 70.f });
	underBarBack.setPosition({ 0.f,view.getSize().y });
	underBarFront.setPosition({ 0.f,view.getSize().y });
	underBarBack.setFillColor({ 40,40,60,180 });
	underBarFront.setFillColor({ 40,40,60,180 });
	tool::SetOrigin(underBarBack, ORIGIN::LB);
	tool::SetOrigin(underBarFront, ORIGIN::LB);

	//2단
	float secondFloarY = view.getSize().y - 105.f;
	ObjectButton::Create(This(), { 5.f, secondFloarY }, "rci")->SetOrigin(ORIGIN::LM);
	buttonRoad = ObjectButton::Create(This(), { view.getCenter().x - 71.5f, secondFloarY }, "road");
	buttonRoad-> SetOrigin(ORIGIN::RM);
	buttonR = ObjectButton::Create(This(), { view.getCenter().x - 2.5f, secondFloarY }, "r_");
	buttonR->SetOrigin(ORIGIN::RM);
	buttonC = ObjectButton::Create(This(), { view.getCenter().x + 2.5f, secondFloarY }, "c_");
	buttonC->SetOrigin(ORIGIN::LM);
	buttonI = ObjectButton::Create(This(), { view.getCenter().x + 71.5f, secondFloarY }, "i_");
	buttonI->SetOrigin(ORIGIN::LM);
	buttonDestroy = ObjectButton::Create(This(), { view.getSize().x - 5.f, secondFloarY }, "destroy");
	buttonDestroy->SetOrigin(ORIGIN::RM);

	//1단
	float firstFloarY = view.getSize().y-35.f;
	buttonPlay = ObjectButton::Create(This(), { 5.f, firstFloarY }, "play");
	buttonPlay->SetOrigin(ORIGIN::LM);
	buttonPause = ObjectButton::Create(This(), { 74.f, firstFloarY }, "pause");
	buttonPause->SetOrigin(ORIGIN::LM);
	ObjectButton::Create(This(), { view.getSize().x - 5.f, firstFloarY }, "citizen")->SetOrigin(ORIGIN::RM);
	ObjectButton::Create(This(), { view.getSize().x - 74.f, firstFloarY }, "grid")->SetOrigin(ORIGIN::RM);


	tempText.setFillColor(sf::Color::White);
	tempText.setCharacterSize(40);
	tempText.setFont(SFGM_FONT.Get(""));


}

void SceneGameUI::Draw(sf::RenderWindow& window)
{
	window.draw(underBarBack);
	window.draw(underBarFront);
	window.draw(tempText);
	Scene::Draw(window);

}

void SceneGameUI::Update(float timeDelta, float timeScale)
{
	Scene::Update(timeDelta, timeScale);
}

void SceneGameUI::UpdateRCIGraph(int r, int c, int i)
{
	tempText.setString(std::to_string(r) + " " + std::to_string(c) + " " + std::to_string(i));
}

void SceneGameUI::SetTempText(const std::string& str)
{
	tempText.setString(str);
}


void SceneGameUI::Play()
{
	buttonPause->UnSelect();
}
void SceneGameUI::Pause()
{
	buttonPlay->UnSelect();
}

void SceneGameUI::Road()
{
	buttonR->UnSelect();
	buttonC->UnSelect();
	buttonI->UnSelect();
	buttonDestroy->UnSelect();

	isBuildMode = true;
	isDestoryMode = false;
}
void SceneGameUI::R()
{
	buttonRoad->UnSelect();
	buttonC->UnSelect();
	buttonI->UnSelect();
	buttonDestroy->UnSelect();

	rci.residence = 1;
	rci.commerce = 0;
	rci.industry = 0;

	isBuildMode = true;
	isDestoryMode = false;
}
void SceneGameUI::C()
{
	buttonRoad->UnSelect();
	buttonR->UnSelect();
	buttonI->UnSelect();
	buttonDestroy->UnSelect();

	rci.residence = 0;
	rci.commerce = 1;
	rci.industry = 0;

	isBuildMode = true;
	isDestoryMode = false;
}
void SceneGameUI::I()
{
	buttonRoad->UnSelect();
	buttonR->UnSelect();
	buttonC->UnSelect();
	buttonDestroy->UnSelect();

	rci.residence = 0;
	rci.commerce = 0;
	rci.industry = 1;

	isBuildMode = true;
	isDestoryMode = false;
}
void SceneGameUI::Destroy()
{
	buttonRoad->UnSelect();
	buttonR->UnSelect();
	buttonC->UnSelect();
	buttonI->UnSelect();


	isBuildMode = false;
	isDestoryMode = true;
}