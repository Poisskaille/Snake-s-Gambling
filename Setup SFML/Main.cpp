#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<SFML/Audio.hpp>
#include<iostream>
#include<thread>
#include<cstdlib>
#include<cmath>
#include<vector>
#include<thread>
#include<conio.h>
#include<chrono>

using namespace std;
using namespace sf;

int WindowX = 1920;
int WindowY = 1080;

float scaleX = WindowX / 1920.0f;
float scaleY = WindowY / 1080.0f;

Text €, Menu3, Menu1, Menu2, BlackJackText, PlinkoTexT, SurvieText, DiceText, TextNON, Information1, Information2;
Text ButtonPlay, PlayPlinko, PlayButtonText, MiseJoueur, BetButtonText1, BetButtonText2, BetButtonText3, PlayDice, DiceScore;
Text DiceBet1, DiceBet2, MiseTotalText, VotreMise, ResultatJoueurText, ResultatAdversaireText;
RectangleShape ReturnButton, Button, MainMenu1, MainMenu2, MainMenu3, MainMenu4, LeaveButton, PlayButtonDice, PlayButtonSurvie;
RectangleShape ButtonPlayPlinko, Score1, Score2, Score3, Separation, Barre1, FondBlackjack;
RectangleShape BetButton1, BetButton2, BetButton3, PlayButtonBJ, FondMiseJoueur, Tirer, Rester, ResultatJoueurBack, ResultatAdversaireBack;
CircleShape Balls, SurvieBall1, ScorePlus;

enum State { Menu, Plinko, BlackJack, Survie, Dice };
State currentState;

int Mise[5]; // Stocker les mises dans le blackjack et du dé

// Création d'un vector pour mettre tous les boutons dedans (utile pour éviter qu'une condition fasse 3km de long)
vector<RectangleShape*> buttons;

vector<RectangleShape*> hoveredButtons;

void HideAllButtons(vector<RectangleShape*>& buttons) {
    for (auto button : buttons) {
        button->setPosition(4000.f, 4000.f);
    }
}

#pragma region Initialisation des variables

bool isPlayingBj = false;
int MiseTotale;
string MiseString = to_string(MiseTotale);

int Resultat;
int ResultatAdversaire;
bool AdversairePlaying = false;

float Score = 100.f;
string scoreString = to_string(Score);

bool showMainMenu = false;
bool MainButtonPressed = false;
bool IsAnimation = false;

bool HelpList = false;

bool isPlinkoPlaying = false;
bool isBallFalling = false;

int posX;

float deltaTime = 0.016f;

float PlinkoPosX = 1400.0f;
float PlinkoPosY = 50.0f;

bool isPlayingSurvie = false;

int nbDiceBet1 = 0;
int nbDiceBet2 = 51;
int multiplicateur;

float speed = 2.0f;

int nDiceScore;

float GreenX, GreenY;

#pragma endregion Initialisation des variables

float y = (rand() % 20) + 1;
void newY() {
     y = (rand() % 20) + 1;
}

void NewPlusPos() {
    GreenX = (rand() % 1000) + 100;
    GreenY = (rand() % 800) + 100;
}

void newPos() {
    PlinkoPosX = (rand() % 200) + 1300.0f;
    PlinkoPosY = (rand() % 50) + 20.0f;
}

void drawFirstCard() {
    int Carte1 = (rand() % 9) + 1;
        int Carte2 = (rand() % 9) + 1;
    if (Carte2 == 1) {
        Carte2 = 11;
    }
    Resultat = Carte1 + Carte2;

    if (Resultat == 21) {
        isPlayingBj = false;
        Score += MiseTotale * 2;
    }
}

void drawCard() {
    int newCarte = (rand() % 9) + 1;
    Resultat += newCarte;
}

void newDiceScore() {
    nDiceScore = (rand() % 100);
}

// Y'a trop d'objet faut mieux fermer la classe, il n'y a rien d'interessant dedans
void initObjects(Font& font) {

    VotreMise.setFont(font);
    VotreMise.setString("Votre mise :");
    VotreMise.setCharacterSize(70 * scaleY);
    VotreMise.setFillColor(Color(45, 49, 43));
    VotreMise.setPosition(160,420);

    € = Text(scoreString, font, 40 * scaleY);
    €.setFillColor(Color(64, 64, 56));
    €.setPosition(200 * scaleX, 80 * scaleY);

    // Bouton de retour
    ReturnButton.setSize(Vector2f(100 * scaleX, 100 * scaleY));
    ReturnButton.setFillColor(Color::Red);
    buttons.push_back(&ReturnButton);

    LeaveButton.setSize(Vector2f(200 * scaleX, 100 * scaleY));
    LeaveButton.setFillColor(Color(249, 89, 89));
    buttons.push_back(&LeaveButton);

    // Objets du menu principale
    Menu3.setFont(font);
    Menu3.setString("Metal Gear Solid");
    Menu3.setCharacterSize(120 * scaleY);
    Menu3.setFillColor(Color::Black);
    Menu3.setPosition(1050 * scaleX, 330 * scaleY);

    Menu2.setFont(font);
    Menu2.setString("T a c t i c a l   G a m b l i n g   O p e r a t i o n");
    Menu2.setCharacterSize(35 * scaleY);
    Menu2.setFillColor(Color(79, 168, 103));
    Menu2.setPosition(1090 * scaleX, 305 * scaleY);

    Menu1.setFont(font);
    Menu1.setString("Snake's Gambling");
    Menu1.setCharacterSize(90 * scaleY);
    Menu1.setFillColor(Color::Black);
    Menu1.setPosition(1115 * scaleX, 450 * scaleY);

    Button.setSize(Vector2f(500 * scaleX, 110 * scaleY));
    Button.setFillColor(Color(5, 82, 16));
    buttons.push_back(&Button);

    ButtonPlay.setFont(font);
    ButtonPlay.setString("Play");
    ButtonPlay.setCharacterSize(90 * scaleY);
    ButtonPlay.setPosition(1290 * scaleX, 625 * scaleY);

    // Menu de sélection (dans le menu principale)
    MainMenu1.setSize(Vector2f(600 * scaleX, 100 * scaleY));
    MainMenu1.setFillColor(Color(190,237,202));
    buttons.push_back(&MainMenu1);

    MainMenu2.setSize(Vector2f(600 * scaleX, 100 * scaleY));
    MainMenu2.setFillColor(Color(190, 237, 202));
    buttons.push_back(&MainMenu2);

    MainMenu3.setSize(Vector2f(600 * scaleX, 100 * scaleY));
    MainMenu3.setFillColor(Color(190, 237, 202));
    buttons.push_back(&MainMenu3);
    
    MainMenu4.setSize(Vector2f(600 * scaleX, 100 * scaleY));
    MainMenu4.setFillColor(Color(190, 237, 202));
    buttons.push_back(&MainMenu4);

    BlackJackText.setFont(font);
    BlackJackText.setString("- Ocelot's BlackJack");
    BlackJackText.setCharacterSize(60 * scaleY);
    BlackJackText.setPosition(1400 * scaleX, 310 * scaleY);
    BlackJackText.setFillColor(Color::Black);

    PlinkoTexT.setFont(font);
    PlinkoTexT.setString("- The Boss's Plinko");
    PlinkoTexT.setCharacterSize(60 * scaleY);
    PlinkoTexT.setPosition(1400 * scaleX, 465 * scaleY);
    PlinkoTexT.setFillColor(Color::Black);

    SurvieText.setFont(font);
    SurvieText.setString("- Furry's Dice");
    SurvieText.setCharacterSize(60 * scaleY);
    SurvieText.setPosition(1400 * scaleX, 620 * scaleY);
    SurvieText.setFillColor(Color::Black);

    DiceText.setFont(font);
    DiceText.setString("- Volgin's Challenge");
    DiceText.setCharacterSize(60 * scaleY);
    DiceText.setPosition(1400 * scaleX, 775 * scaleY);
    DiceText.setFillColor(Color::Black);

    // Objets pour le Plinko
    ButtonPlayPlinko.setSize(Vector2f(400 * scaleX, 110 * scaleY));
    ButtonPlayPlinko.setFillColor(Color::Blue);
    buttons.push_back(&ButtonPlayPlinko);

    PlayPlinko.setFont(font);
    PlayPlinko.setString("Play");
    PlayPlinko.setCharacterSize(80 * scaleY);
    PlayPlinko.setPosition(340 * scaleX, 760 * scaleY);

    Balls.setRadius(15.0f * scaleY);
    Balls.setFillColor(Color::White);
    Balls.setPosition(1400 * scaleX, 50 * scaleY);

    Vector2f velocity(0, 0);
    const float gravity = 100.0f * scaleY;

    Separation.setSize(Vector2f(50 * scaleX, 1080 * scaleY));
    Separation.setPosition(800 * scaleX, 0);

    /*Barre1.setSize(25.0f * scaleY);
    Barre1.setPosition(1400 * scaleX, 150 * scaleY);*/

    // Scores
    Score1.setSize(Vector2f(366 * scaleX, 100 * scaleY));
    Score1.setFillColor(Color::Green);
    Score1.setPosition(850 * scaleX, 1000 * scaleY);

    Score2.setSize(Vector2f(366 * scaleX, 100 * scaleY));
    Score2.setFillColor(Color::Red);
    Score2.setPosition(1216 * scaleX, 1000 * scaleY);

    Score3.setSize(Vector2f(366 * scaleX, 100 * scaleY));
    Score3.setFillColor(Color::Blue);
    Score3.setPosition(1582 * scaleX, 1000 * scaleY);

    // Objet pour le blackjack

    // Boutons de mise
    BetButton1.setSize(Vector2f(200 * scaleX, 100 * scaleY));
    BetButton1.setFillColor(Color(123, 125, 107));
    buttons.push_back(&BetButton1);

    BetButtonText1.setFont(font);
    BetButtonText1.setString("10 €");
    BetButtonText1.setCharacterSize(60 * scaleY);
    BetButtonText1.setPosition(530, 960);
    BetButtonText1.setFillColor(Color::Black);

    BetButton2.setSize(Vector2f(200 * scaleX, 100 * scaleY));
    BetButton2.setFillColor(Color(123, 125, 107));
    buttons.push_back(&BetButton2);

    BetButtonText2.setFont(font);
    BetButtonText2.setString("20 €");
    BetButtonText2.setCharacterSize(60 * scaleY);
    BetButtonText2.setPosition(920 * scaleX, 960 * scaleY);
    BetButtonText2.setFillColor(Color::Black);

    BetButton3.setSize(Vector2f(200 * scaleX, 100 * scaleY));
    BetButton3.setFillColor(Color(123, 125, 107));
    buttons.push_back(&BetButton3);

    BetButtonText3.setFont(font);
    BetButtonText3.setString("50 €");
    BetButtonText3.setCharacterSize(60 * scaleY);
    BetButtonText3.setPosition(1320 * scaleX, 960 * scaleY);
    BetButtonText3.setFillColor(Color::Black);

    PlayButtonBJ.setSize(Vector2f(400 * scaleX, 100 * scaleY));
    PlayButtonBJ.setFillColor(Color(45,49,43));
    buttons.push_back(&PlayButtonBJ);

    PlayButtonText.setFont(font);
    PlayButtonText.setString("Play");
    PlayButtonText.setCharacterSize(90 * scaleY);
    PlayButtonText.setPosition(280 * scaleX, 700 * scaleY);
    PlayButtonText.setFillColor(Color(94,96,80));

    FondMiseJoueur.setSize(Vector2f(400 * scaleX, 100 * scaleY));
    FondMiseJoueur.setFillColor(Color(45,49,43));
    FondMiseJoueur.setPosition(160 * scaleX, 510 * scaleY);

    MiseJoueur.setFont(font);
    MiseJoueur.setString(MiseString);
    MiseJoueur.setCharacterSize(80 * scaleY);
    MiseJoueur.setFillColor(Color(94,96,80));
    MiseJoueur.setPosition(200 * scaleX, 515 * scaleY);

    ResultatJoueurBack.setSize(Vector2f(400 * scaleX, 100 * scaleY));
    ResultatJoueurBack.setFillColor(Color(45, 49, 43));
    ResultatJoueurBack.setPosition(1400 * scaleX, 700 * scaleY);

    ResultatAdversaireBack.setSize(Vector2f(400 * scaleX, 100 * scaleY));
    ResultatAdversaireBack.setFillColor(Color(45, 49, 43));
    ResultatAdversaireBack.setPosition(1400 * scaleX, 400 * scaleY);

    ResultatAdversaireText.setFont(font);
    ResultatAdversaireText.setCharacterSize(80 * scaleY);
    ResultatAdversaireText.setFillColor(Color(94, 96, 80));
    ResultatAdversaireText.setPosition(1590 * scaleX,405 * scaleY);

    ResultatJoueurText.setFont(font);
    ResultatJoueurText.setCharacterSize(80 * scaleY);
    ResultatJoueurText.setFillColor(Color(94, 96, 80));
    ResultatJoueurText.setPosition(1590 * scaleX, 705 * scaleY);

    // Boutons "Tirer" et "Rester"
    Tirer.setSize(Vector2f(200 * scaleX, 100 * scaleY));
    Tirer.setFillColor(Color(22, 184, 65));
    buttons.push_back(&Tirer);

    Rester.setSize(Vector2f(200 * scaleX, 100 * scaleY));
    Rester.setFillColor(Color(184, 22, 22));
    buttons.push_back(&Rester);

    //Objet pour le dé 

    PlayButtonDice.setSize(Vector2f(256 * scaleX, 100 * scaleY));
    PlayButtonDice.setFillColor(Color(94,96,80));
    buttons.push_back(&PlayButtonDice);

    PlayDice.setFont(font);
    PlayDice.setString("Roll");
    PlayDice.setCharacterSize(100 * scaleY);
    PlayDice.setPosition(898 * scaleX,764 * scaleY);
    PlayDice.setFillColor(Color(82, 82, 69));

    DiceScore.setFont(font);
    DiceScore.setCharacterSize(130 * scaleY);
    DiceScore.setPosition(915 * scaleX, 170 * scaleY);
    DiceScore.setFillColor(Color::Black);

    DiceBet1.setFont(font);
    DiceBet1.setCharacterSize(90 * scaleY);
    DiceBet1.setFillColor(Color::Black);
    DiceBet1.setPosition(350 * scaleX, 450 * scaleY);

    DiceBet2.setFont(font);
    DiceBet2.setCharacterSize(90 * scaleY);
    DiceBet2.setFillColor(Color::Black);
    DiceBet2.setPosition(1550 * scaleX, 450 * scaleY);

    MiseTotalText.setFont(font);
    MiseTotalText.setCharacterSize(90 * scaleY);
    MiseTotalText.setFillColor(Color::Black);
    MiseTotalText.setPosition(930 * scaleX, 450 * scaleY);

    //Survie

    SurvieBall1.setRadius(15 * scaleY);
    SurvieBall1.setFillColor(Color::Red);
    SurvieBall1.setPosition(700, 900);

    TextNON.setFont(font);
    TextNON.setString("Indisponible - Argent different de 0");
    TextNON.setCharacterSize(30 * scaleY);
    TextNON.setFillColor(Color(255,54,74));
    TextNON.setPosition(900 * scaleX, 785 * scaleY);

    Information1.setFont(font);
    Information1.setCharacterSize(50 * scaleX);
    Information1.setFillColor(Color::Black);
    Information1.setPosition(800 * scaleX, 50 * scaleY);

    Information2.setFont(font);
    Information2.setCharacterSize(50 * scaleX);
    Information2.setFillColor(Color::Black);
    Information2.setPosition(800 * scaleX, 100 * scaleY);
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    RenderWindow window(VideoMode(WindowX, WindowY), "Metal Gear Gambling", Style::Fullscreen);
    window.setFramerateLimit(60);
    currentState = Menu;

    Clock clock;

    Font font;
    if (!font.loadFromFile("../font/MGS2.ttf")) {
        return -1;
    }

#pragma region Sons et Textures
    // Initialisation de tous les sons nécessaires pour le menu
    SoundBuffer MainMenuSound;
    if (!MainMenuSound.loadFromFile("../son/MenuSelect.wav")) {
        return -1;
    }
    Sound ButtonMainSound;
    ButtonMainSound.setBuffer(MainMenuSound);

    SoundBuffer MenuSound;
    if (!MenuSound.loadFromFile("../son/ButtonSelected.wav")) {
        return -1;
    }
    Sound ButtonMenuSound;
    ButtonMenuSound.setBuffer(MenuSound);

    SoundBuffer ExitButtonSound;
    if (!ExitButtonSound.loadFromFile("../son/ExitButton.wav")) {
        return -1;
    }
    Sound ExButtonSound;
    ExButtonSound.setBuffer(ExitButtonSound);

    SoundBuffer HoveredButton;
    if (!HoveredButton.loadFromFile("../son/ButtonHovered.wav")) {
        return -1;
    }
    Sound HovButton;
    HovButton.setBuffer(HoveredButton);

    SoundBuffer BackGroundSound;
    if (!BackGroundSound.loadFromFile("../son/Metal Gear Solid 3 - CQC title screen (ISOLATED).wav")) {
        return -1;
    }
    Sound BackgroundMainSound;
    BackgroundMainSound.setBuffer(BackGroundSound);

    Texture FondSnake;
    if (!FondSnake.loadFromFile("../image/capsule_616x353.png")) {
        return -1;
    }

    // Fond du menu principale et de séléction
    Sprite Background1(FondSnake);
    Background1.setScale(
        window.getSize().x / Background1.getGlobalBounds().width,
        window.getSize().y / Background1.getGlobalBounds().height
    );

    Texture OrangeBox;
    if (!OrangeBox.loadFromFile("../image/e9dnyx0pcqv41-removebg-preview.png")) {
        return -1;
    }

    Sprite OrangeBoxSprite(OrangeBox);
    OrangeBoxSprite.setPosition(900, 500);
    OrangeBoxSprite.setScale(Vector2f(0.25 * scaleX, 0.25 * scaleY));

    Texture FondDice;
    if (!FondDice.loadFromFile("../image/FondFOXDice.png")) {
        return -1;
    }

    Sprite FondDiceSprite(FondDice);
    FondDiceSprite.setScale(
        window.getSize().x / FondDiceSprite.getGlobalBounds().width,
        window.getSize().y / FondDiceSprite.getGlobalBounds().height
    );

    Texture FondFox;
    if (!FondFox.loadFromFile("../image/FondFOX.png")) {
        return -1;
    }

    Sprite FondSurvie(FondFox);
    FondSurvie.setScale(
        window.getSize().x / FondSurvie.getGlobalBounds().width,
        window.getSize().y / FondSurvie.getGlobalBounds().height
    );

    Texture SupplyBoxSprite;
    if (!SupplyBoxSprite.loadFromFile("../image/SupplyBox.png")) {
        return -1;
    }

    Sprite SupplyBox(SupplyBoxSprite);
    SupplyBox.setPosition(100, 100);
    SupplyBox.setScale(Vector2f(0.35 * scaleX, 0.35 * scaleY));

    Texture FondBjSprite;
    if (!FondBjSprite.loadFromFile("../image/FondFOXBJ.png")) {
        return -1;
    }

    Sprite FondFoxBJ(FondBjSprite);
    FondFoxBJ.setScale(
        window.getSize().x / FondFoxBJ.getGlobalBounds().width,
        window.getSize().y / FondFoxBJ.getGlobalBounds().height
    );

    Texture OcelotArtSprite;
    if (!OcelotArtSprite.loadFromFile("../image/Mgs3-ocelot (1).png")) {
        return -1;
    }

    Sprite RevolverOcelotArt(OcelotArtSprite);
    RevolverOcelotArt.setPosition(1300, 200);
    RevolverOcelotArt.setScale(0.25f * scaleX, 0.25f * scaleY);

#pragma endregion Sons et Textures

    initObjects(font);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            Vector2i mousePos = Mouse::getPosition(window);

#pragma region Boutons & Keyboard
            
            for (auto button : buttons) {
                if (button->getGlobalBounds().contains(static_cast<Vector2f>(mousePos))) {
                    if (find(hoveredButtons.begin(), hoveredButtons.end(), button) == hoveredButtons.end()) {
                        HovButton.play();
                        hoveredButtons.push_back(button);
                    }
                }
                else {
                    hoveredButtons.erase(remove(hoveredButtons.begin(), hoveredButtons.end(), button), hoveredButtons.end());
                }
            }
            if (MainMenu1.getGlobalBounds().contains(static_cast<Vector2f>(mousePos))) {
                Information1.setString("Revolver Ocelot vous met au defi");
                Information2.setString("Affrontez le dans un combat de BlackJack");
            }
            if (MainMenu3.getGlobalBounds().contains(static_cast<Vector2f>(mousePos))) {
                Information1.setString("The Fury vous met au defi");
                Information2.setString("Pensez vous etre chanceux ?");
            }
            if (MainMenu4.getGlobalBounds().contains(static_cast<Vector2f>(mousePos))) {
                Information1.setString("Volgin vous met au defi");
                Information2.setString("A court d'argent ? Relevez ce defi");
            }

            if (Keyboard::isKeyPressed(Keyboard::P)) {
                Score += 50;
            }
            if (Keyboard::isKeyPressed(Keyboard::O)) {
                Score = 0;
            }

            if (Keyboard::isKeyPressed(Keyboard::Space)) {
                if (!isPlayingSurvie) {
                    isPlayingSurvie = true;
                }
            }
            if (Keyboard::isKeyPressed(Keyboard::R)) {
                MiseTotale = 0;
                for (int i = 0; i < 5; i++) {
                    Mise[i] = 0;
                }
            }

            if (isPlayingSurvie || currentState == Dice) {
                if (Keyboard::isKeyPressed(Keyboard::Z) || Keyboard::isKeyPressed(Keyboard::Up)) {
                    switch (currentState) {
                    case Survie:
                        OrangeBoxSprite.move(0, -40);
                        break;
                    case Dice:
                        if (nbDiceBet1 != nbDiceBet2 - 1) {
                            nbDiceBet1 += 1;
                        }                      
                        break;
                    }
                }
                if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)) {
                    switch (currentState) {
                    case Survie:
                        OrangeBoxSprite.move(0, 40);
                        break;
                    case Dice:
                        if (nbDiceBet1 != 0) {
                            nbDiceBet1 -= 1;
                        }
                        break;
                    }
                }
                if (Keyboard::isKeyPressed(Keyboard::Q) || Keyboard::isKeyPressed(Keyboard::Left)) {
                    switch (currentState) {
                    case Survie:
                        OrangeBoxSprite.move(-40, 0);
                        break;
                    case Dice:
                        if (nbDiceBet2 != nbDiceBet1 + 1) {
                            nbDiceBet2 -= 1;
                        }
                        break;
                    }
                }
                if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) {
                    switch (currentState) {
                    case Survie:
                        OrangeBoxSprite.move(40, 0);
                        break;
                    case Dice:
                        if (nbDiceBet2 != 100) {
                            nbDiceBet2 += 1;
                        }
                        break;
                    }
                }
            }
            
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2i mousePos = Mouse::getPosition(window);

                if (Button.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    if (!MainButtonPressed) {

                        Menu3.setRotation(90);
                        Menu2.setRotation(90);
                        Menu1.setRotation(90);

                        Menu3.setCharacterSize(196 * scaleY);
                        Menu2.setCharacterSize(46 * scaleY);
                        Menu1.setCharacterSize(110 * scaleY);

                        Menu3.setPosition(300 * scaleX,0);
                        Menu2.setPosition(346 * scaleX, 180 * scaleY);
                        Menu1.setPosition(110 * scaleX,270 * scaleY);
                        ButtonMainSound.play();
                        BackgroundMainSound.play();
                        BackgroundMainSound.setLoop(true);
                        MainButtonPressed = true;
                        showMainMenu = true;
                    }
                }
                if (ReturnButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    ExButtonSound.play();
                    BackgroundMainSound.play();
                    BackgroundMainSound.setLoop(true);
                    currentState = Menu;
                    isPlayingBj = false;
                    isPlinkoPlaying = false;
                }
                if (MainMenu2.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    ButtonMenuSound.play();
                    BackgroundMainSound.stop();
                    currentState = Plinko;
                }
                if (MainMenu3.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        ButtonMenuSound.play();
                        BackgroundMainSound.stop();
                        currentState = Dice;
                    }
                if (Score == 0) {
                    if (MainMenu4.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            ButtonMenuSound.play();
                            BackgroundMainSound.stop();
                            currentState = Survie;
                        }
                    }
                if (LeaveButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    window.close();
                }
                if (MainMenu1.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        BackgroundMainSound.stop();
                        ButtonMenuSound.play();
                        currentState = BlackJack;
                        for (int i = 0; i < 5; i++) {
                            Mise[i] = 0;
                        }
                    }
                if (!isPlayingBj || currentState == Dice) {
                        if (PlayButtonBJ.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            if (MiseTotale <= Score) {
                                if (MiseTotale != 0) {
                                    isPlayingBj = true;
                                    Score -= MiseTotale;
                                    drawFirstCard();
                                }
                            }                                                 
                        }
                        if (Score >= 10) {
                            if (BetButton1.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                                bool miseAppliquee = false;
                                for (int i = 0; i < 5; i++) {
                                    if (Mise[i] == 0) {
                                        Mise[i] = 10;
                                        MiseTotale += 10;
                                        miseAppliquee = true;
                                        break;
                                    }
                                }
                            }
                        }

                        if (Score >= 20) {
                            if (BetButton2.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                                bool miseAppliquee = false;
                                for (int i = 0; i < 5; i++) {
                                    if (Mise[i] == 0) {
                                        Mise[i] = 20;
                                        MiseTotale += 20;
                                        miseAppliquee = true;
                                        break;
                                    }
                                }
                            }
                        }
                        
                        if (Score >= 50) {
                            if (BetButton3.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                                bool miseAppliquee = false;

                                for (int i = 0; i < 5; i++) {
                                    if (Mise[i] == 0) {
                                        Mise[i] = 50;
                                        MiseTotale += 50;
                                        miseAppliquee = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    else {
                        if (Tirer.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            if (!AdversairePlaying) {
                                drawCard();
                                if (Resultat > 21) {
                                    isPlayingBj = false;
                                    MiseTotale = 0;
                                    for (int i = 0; i < 5; i++) {
                                        Mise[i] = 0;
                                    }
                                }
                                else if (Resultat <= 21) {
                                    // Continuer la partie
                                }
                            }
                        }
                        if (Rester.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            AdversairePlaying = true;
                            ResultatAdversaire = 0;
                            while (ResultatAdversaire < 17) {
                                int CarteAdv = (rand() % 9) + 1;
                                ResultatAdversaire += CarteAdv;
                            }
                            AdversairePlaying = false;

                            if (Resultat <= 21 && (Resultat > ResultatAdversaire || ResultatAdversaire > 21)) {
                                //cerr << "Gagné !" << endl;
                                Score += MiseTotale * 1.5;
                            }
                            if (Resultat == ResultatAdversaire) {
                                //cerr << "Egalité" << endl;
                                Score += MiseTotale;
                            }
                            if (Resultat < ResultatAdversaire && ResultatAdversaire <= 21) {
                                //cerr << "Perdu !" << endl;
                            }
                            MiseTotale = 0;
                            isPlayingBj = false;
                            for (int i = 0; i < 5; i++) {
                                Mise[i] = 0;
                            }

                        }
                    }               

                if (currentState == Plinko) {
                    if (!isPlinkoPlaying && ButtonPlayPlinko.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        if (!isBallFalling) {
                            Score -= 10;
                            isBallFalling = true;
                            newPos();
                            Balls.setPosition(PlinkoPosX, PlinkoPosY);
                        }
                    }
                }

                if (PlayButtonDice.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    if (Score - MiseTotale < 0 || nbDiceBet2 - nbDiceBet1 > 80) {
                        //cerr << "Impossible de lancer la partie" << endl;
                    }
                    else {
                        newDiceScore();
                        Score -= MiseTotale;

                        int const K = 100;
                        multiplicateur = K / (nbDiceBet2 - nbDiceBet1) + 1;
                        if (nDiceScore >= nbDiceBet1 && nDiceScore <= nbDiceBet2) {
                            Score += MiseTotale * multiplicateur;
                        }
                    }
                }                   
            }
            if (Balls.getGlobalBounds().intersects(Score1.getGlobalBounds()) || Balls.getGlobalBounds().intersects(Score2.getGlobalBounds()) || Balls.getGlobalBounds().intersects(Score3.getGlobalBounds())) {
                if (Balls.getGlobalBounds().intersects(Score1.getGlobalBounds()) || Balls.getGlobalBounds().intersects(Score3.getGlobalBounds())) {
                    Score *= 1.5;
                    
                }
                isBallFalling = false;
            }
        }

#pragma endregion Boutons & Keyboard


        Vector2f playerPos = OrangeBoxSprite.getPosition();
        Vector2f enemyPos = SurvieBall1.getPosition();
        Vector2f direction = playerPos - enemyPos;

        float length = sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length != 0) {
            direction /= length; 
        }
        if (SurvieBall1.getGlobalBounds().intersects(OrangeBoxSprite.getGlobalBounds())) {
            isPlayingSurvie = false;
            OrangeBoxSprite.setPosition(1000, 600);
            SurvieBall1.setPosition(700, 900);
            currentState = Menu;
            speed = 2.f;
        }

        if (SupplyBox.getGlobalBounds().intersects(OrangeBoxSprite.getGlobalBounds())) {
            Score += 10;
            NewPlusPos();
            speed *= 1.5f;
            SupplyBox.setPosition(GreenX, GreenY);
        }

        window.clear(Color::Black);
        HideAllButtons(buttons);

        string scoreString = to_string(Score);
        €.setString(scoreString);

        string MiseString = to_string(MiseTotale);
        MiseJoueur.setString(MiseString);

        string DiceString = to_string(nDiceScore);
        DiceScore.setString(DiceString);

        string DiceBet1String = to_string(nbDiceBet1);
        DiceBet1.setString(DiceBet1String);

        string DiceBet2String = to_string(nbDiceBet2);
        DiceBet2.setString(DiceBet2String);

        string MiseTotalString = to_string(MiseTotale);
        MiseTotalText.setString(MiseTotalString);

        string ResuJoueurString = to_string(Resultat);
        ResultatJoueurText.setString(ResuJoueurString);

        string ResuAdvString = to_string(ResultatAdversaire);
        ResultatAdversaireText.setString(ResuAdvString);

        switch (currentState) {

        case Menu:
            window.draw(Background1);
            if (HelpList) {

                break;
            }
            if (showMainMenu) {
                MainMenu1.setPosition(1320 * scaleX, 295 * scaleY);
                MainMenu2.setPosition(1320 * scaleX, 450 * scaleY);
                MainMenu3.setPosition(1320 * scaleX, 605 * scaleY);
                MainMenu4.setPosition(1320 * scaleX, 760 * scaleY);
                
                window.draw(MainMenu1);
                window.draw(MainMenu2);
                window.draw(MainMenu3);
                window.draw(MainMenu4);
                window.draw(Menu3);
                window.draw(Menu2);
                window.draw(Menu1);
                window.draw(Information1);
                window.draw(Information2);

                window.draw(BlackJackText);
                window.draw(PlinkoTexT);
                window.draw(SurvieText);
                window.draw(DiceText);
                if (Score != 0) {
                    window.draw(TextNON);
                }
            }
            else {
                Button.setPosition(1115 * scaleX, 620 * scaleY);
                window.draw(Menu3);
                window.draw(Menu2);
                window.draw(Menu1);

                window.draw(Button);
                window.draw(ButtonPlay);
            }
            break;

        case Plinko:
            window.draw(€);

            window.draw(ButtonPlayPlinko);
            window.draw(PlayPlinko);
            window.draw(Separation);

            window.draw(Score1);
            window.draw(Score2);
            window.draw(Score3);

            window.draw(Barre1);
            window.draw(Balls);
            break;

        case BlackJack:
            window.draw(FondFoxBJ);
            window.draw(RevolverOcelotArt);
            window.draw(FondMiseJoueur);
            window.draw(MiseJoueur);
            window.draw(€);
            window.draw(VotreMise);
            window.draw(ResultatJoueurBack);
            window.draw(ResultatAdversaireBack);
            window.draw(ResultatJoueurText);
            window.draw(ResultatAdversaireText);
            if (!isPlayingBj) {
                BetButton1.setPosition(465 * scaleX, 950 * scaleY);
                BetButton2.setPosition(865 * scaleX, 950 * scaleY);
                BetButton3.setPosition(1265 * scaleX, 950 * scaleY);
                PlayButtonBJ.setPosition(160 * scaleX, 700 * scaleY);

                window.draw(BetButton1);
                window.draw(BetButtonText1);
                window.draw(BetButton2);
                window.draw(BetButtonText2);
                window.draw(BetButton3);
                window.draw(BetButtonText3);
                window.draw(PlayButtonBJ);
                window.draw(PlayButtonText);
            }
            else 
            {
                Tirer.setPosition(1300 * scaleX, 900 * scaleY);
                Rester.setPosition(1600 * scaleX, 900 * scaleY);

                window.draw(Tirer);
                window.draw(Rester);

            }
            break;

            case Survie:
                newY();
                window.draw(FondFoxBJ);
                window.draw(OrangeBoxSprite);
                window.draw(SurvieBall1);
                window.draw(€);
                window.draw(SupplyBox);
                if (isPlayingSurvie) {
                    SurvieBall1.move(direction * speed);
                }                             
                break;
            case Dice:
                window.draw(FondDiceSprite);
                BetButton1.setPosition(465 * scaleX, 950 * scaleY);
                BetButton2.setPosition(865 * scaleX, 950 * scaleY);
                BetButton3.setPosition(1265 * scaleX, 950 * scaleY);

                PlayButtonDice.setPosition(831 * scaleX, 750 * scaleY);

                window.draw(BetButton1);
                window.draw(BetButton2);
                window.draw(BetButton3);
                window.draw(BetButtonText1);
                window.draw(BetButtonText2);
                window.draw(BetButtonText3);
                window.draw(MiseTotalText);
                window.draw(€);
                window.draw(PlayButtonDice);
                window.draw(PlayDice);
                window.draw(DiceScore);
                window.draw(DiceBet1);
                window.draw(DiceBet2);
                
                break;
        }

        if (showMainMenu) {
            LeaveButton.setPosition(1800 * scaleX, 980 * scaleY);
            window.draw(LeaveButton);
        }

        if (currentState != Menu && !isPlayingBj) {
            ReturnButton.setPosition(0, 980 * scaleY);
            window.draw(ReturnButton);
        }
        window.display();
    }
}