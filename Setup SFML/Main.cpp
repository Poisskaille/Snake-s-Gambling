#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<SFML/Audio.hpp>
#include<iostream>
#include<thread>
#include<cstdlib>
#include<cmath>
#include<vector>

using namespace std;
using namespace sf;

int WindowX = 1920;
int WindowY = 1080;

float scaleX = WindowX / 1920.0f;
float scaleY = WindowY / 1080.0f;
float speedSlide = 1.0f;
float targetPosition = 4000.0f * scaleX;

Text Argent, �, Menu3, Menu1, Menu2, BlackJackText, PlinkoTexT, SurvieText;
Text ButtonPlay, PlayPlinko, PlayButtonText, ScoreAdversaire, MiseJoueur, MiseAdversaire, BetButtonText1, BetButtonText2, BetButtonText3;
RectangleShape FondArgent, ReturnButton, Button, MainMenu1, MainMenu2, MainMenu3, LeaveButton;
RectangleShape ButtonPlayPlinko, Score1, Score2, Score3, Separation, Barre1, FondBlackjack, ScoreAdversaireFond, MiseMenuFond;
RectangleShape BetButton1, BetButton2, BetButton3, PlayButtonBJ, FondMiseJoueur, FondMiseAdversaire, Tirer, Rester, VD;
CircleShape Balls;

enum State { Menu, Plinko, BlackJack, Survie };
State currentState;

int Mise[5]; // Stocker les mises dans le blackjack

// Cr�ation d'un vector pour mettre tous les boutons dedans (utile pour �viter qu'une condition fasse 3km de long)
vector<RectangleShape*> buttons;

void HideAllButtons(vector<RectangleShape*>& buttons) {
    for (auto button : buttons) {
        button->setPosition(4000.f, 4000.f);
    }
}

bool isPlayingBj = false;
int MiseTotale;
string MiseString = to_string(MiseTotale);

int Resultat;
int ResultatAdversaire;
bool AdversairePlaying = false;

int Score = 100.f;
string scoreString = to_string(Score);

bool showMainMenu = false;
bool MainButtonPressed = false;
bool IsAnimation = false;

bool isPlinkoPlaying = false;
bool isBallFalling = false;

int posX;

float PlinkoPosX = 1400.0f;
float PlinkoPosY = 50.0f;

void newPos() {
    PlinkoPosX = (rand() % 200) + 1300.0f;
    PlinkoPosY = (rand() % 50) + 20.0f;
}

void drawFirstCard() {
    int Carte1 = (rand() % 9) + 1;
    cerr << "Premiere carte :" << Carte1 << endl;
    int Carte2 = (rand() % 9) + 1;
    cerr << "Deuxieme carte :" << Carte2 << endl;

    Resultat = Carte1 + Carte2;
    cerr << Resultat << endl;
}

void drawCard() {
    int newCarte = (rand() % 9) + 1;
    Resultat += newCarte;
    cerr << "Nouveau score : " << Resultat << endl;
}

void initObjects(Font& font) {
    // Fond Argent
    FondArgent.setSize(Vector2f(600 * scaleX, 100 * scaleY));
    FondArgent.setFillColor(Color::Green);
    FondArgent.setPosition(0, 50 * scaleY);

    Argent.setFont(font);
    Argent.setString("Argent actuel :");
    Argent.setCharacterSize(80 * scaleY);
    Argent.setPosition(70 * scaleX, 50 * scaleY);

    � = Text(scoreString, font, 80 * scaleY);
    �.setPosition(500 * scaleX, 50 * scaleY);

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

    // Menu de s�lection (dans le menu principale)
    MainMenu1.setSize(Vector2f(600 * scaleX, 100 * scaleY));
    MainMenu1.setFillColor(Color(190,237,202));
    buttons.push_back(&MainMenu1);

    MainMenu2.setSize(Vector2f(600 * scaleX, 100 * scaleY));
    MainMenu2.setFillColor(Color(190, 237, 202));
    buttons.push_back(&MainMenu2);

    MainMenu3.setSize(Vector2f(600 * scaleX, 100 * scaleY));
    MainMenu3.setFillColor(Color(190, 237, 202));
    buttons.push_back(&MainMenu3);

    BlackJackText.setFont(font);
    BlackJackText.setString("- BlackJack");
    BlackJackText.setCharacterSize(60 * scaleY);
    BlackJackText.setPosition(1500 * scaleX, 310 * scaleY);
    BlackJackText.setFillColor(Color::Black);

    PlinkoTexT.setFont(font);
    PlinkoTexT.setString("- Plinko");
    PlinkoTexT.setCharacterSize(60 * scaleY);
    PlinkoTexT.setPosition(1500 * scaleX, 465 * scaleY);
    PlinkoTexT.setFillColor(Color::Black);

    SurvieText.setFont(font);
    SurvieText.setString("- Survie");
    SurvieText.setCharacterSize(60 * scaleY);
    SurvieText.setPosition(1500 * scaleX, 620 * scaleY);
    SurvieText.setFillColor(Color::Black);

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
    FondBlackjack.setSize(Vector2f(1920 * scaleX, 1080 * scaleY));
    FondBlackjack.setFillColor(Color(0, 102, 0));

    ScoreAdversaireFond.setSize(Vector2f(1920 * scaleX, 400 * scaleY));
    ScoreAdversaireFond.setFillColor(Color(0, 80, 0));

    ScoreAdversaire.setFont(font);
    ScoreAdversaire.setString("Score adversaire : 0");
    ScoreAdversaire.setCharacterSize(60 * scaleY);
    ScoreAdversaire.setPosition(750 * scaleX, 150 * scaleY);
    ScoreAdversaire.setFillColor(Color::White);

    MiseMenuFond.setSize(Vector2f(1920 * scaleX, 680 * scaleY));
    MiseMenuFond.setFillColor(Color(2, 82, 8));

    // Boutons de mise
    BetButton1.setSize(Vector2f(300 * scaleX, 100 * scaleY));
    BetButton1.setFillColor(Color::Blue);
    buttons.push_back(&BetButton1);

    BetButtonText1.setFont(font);
    BetButtonText1.setString("10 �");
    BetButtonText1.setCharacterSize(60 * scaleY);
    BetButtonText1.setPosition(450, 860);
    BetButtonText1.setFillColor(Color::White);

    BetButton2.setSize(Vector2f(300 * scaleX, 100 * scaleY));
    BetButton2.setFillColor(Color::Blue);
    buttons.push_back(&BetButton2);

    BetButtonText2.setFont(font);
    BetButtonText2.setString("20 �");
    BetButtonText2.setCharacterSize(60 * scaleY);
    BetButtonText2.setPosition(850 * scaleX, 860 * scaleY);
    BetButtonText2.setFillColor(Color::White);

    BetButton3.setSize(Vector2f(300 * scaleX, 100 * scaleY));
    BetButton3.setFillColor(Color::Blue);
    buttons.push_back(&BetButton3);

    BetButtonText3.setFont(font);
    BetButtonText3.setString("50 �");
    BetButtonText3.setCharacterSize(60 * scaleY);
    BetButtonText3.setPosition(1250 * scaleX, 860 * scaleY);
    BetButtonText3.setFillColor(Color::White);

    PlayButtonBJ.setSize(Vector2f(500 * scaleX, 100 * scaleY));
    PlayButtonBJ.setFillColor(Color::Green);
    buttons.push_back(&PlayButtonBJ);

    PlayButtonText.setFont(font);
    PlayButtonText.setString("Play");
    PlayButtonText.setCharacterSize(60 * scaleY);
    PlayButtonText.setPosition(770 * scaleX, 710 * scaleY);
    PlayButtonText.setFillColor(Color::White);

    FondMiseJoueur.setSize(Vector2f(500 * scaleX, 100 * scaleY));
    FondMiseJoueur.setFillColor(Color::White);
    FondMiseJoueur.setPosition(715 * scaleX, 510 * scaleY);

    MiseJoueur.setFont(font);
    MiseJoueur.setString(MiseString);
    MiseJoueur.setCharacterSize(50 * scaleY);
    MiseJoueur.setFillColor(Color::Red);
    MiseJoueur.setPosition(720 * scaleX, 515 * scaleY);

    FondMiseAdversaire.setSize(Vector2f(500 * scaleX, 100 * scaleY));
    FondMiseAdversaire.setFillColor(Color::White);
    FondMiseAdversaire.setPosition(715 * scaleX, 110 * scaleY);

    // Boutons "Tirer" et "Rester"
    Tirer.setSize(Vector2f(200 * scaleX, 100 * scaleY));
    Tirer.setFillColor(Color(22, 184, 65));
    buttons.push_back(&Tirer);

    Rester.setSize(Vector2f(200 * scaleX, 100 * scaleY));
    Rester.setFillColor(Color(184, 22, 22));
    buttons.push_back(&Rester);

    VD.setSize(Vector2f(300.f * scaleX, 200.f * scaleY));
    VD.setFillColor(Color::Blue);
    VD.setPosition((WindowX - VD.getSize().x) / 2.0f, (WindowY - VD.getSize().y) / 2.0f);
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

    // Initialisation de tous les sons n�cessaires pour le menu
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

    // Fond du menu principale et de s�l�ction
    Sprite Background1(FondSnake);
    Background1.setScale(
        window.getSize().x / Background1.getGlobalBounds().width,
        window.getSize().y / Background1.getGlobalBounds().height
    );

    Vector2f velocity(0, 0);
    const float gravity = 100.0f * scaleY;

    float deltaTime = 0.016f;

    initObjects(font);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            Vector2i mousePos = Mouse::getPosition(window);
            
            bool SoundPlaying = false;
            bool hovering = false;
            for (auto button:buttons) {
                if (button->getGlobalBounds().contains(static_cast<Vector2f>(mousePos))) {
                    if (!SoundPlaying && !hovering) {
                        HovButton.play();
                        SoundPlaying = true;
                        hovering = true;
                    }
                    
                }

            }

            if (Keyboard::isKeyPressed(Keyboard::P)) {
                Score += 50;
                string scoreString = to_string(Score);
                �.setString(scoreString);
            }

            // G�rer les clics de la souris
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2i mousePos = Mouse::getPosition(window);

                // Bouton du menu
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
                        MainButtonPressed = true;
                        showMainMenu = true;
                    }
                }
                if (ReturnButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    ExButtonSound.play();
                    BackgroundMainSound.play();
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
                    currentState = Survie;
                }
                if (LeaveButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    window.close();
                }
                if (currentState != BlackJack) {
                    if (MainMenu1.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        BackgroundMainSound.stop();
                        ButtonMenuSound.play();
                        currentState = BlackJack;
                        for (int i = 0; i < 5; i++) {
                            Mise[i] = 0;
                        }
                    }
                }

                if (currentState == BlackJack) {
                    if (!isPlayingBj) {
                        if (PlayButtonBJ.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            if (MiseTotale != 0) {
                                isPlayingBj = true;
                                drawFirstCard();
                            }                          
                        }
                        if (Score >= 10) {
                            if (BetButton1.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                                bool miseAppliquee = false;
                                for (int i = 0; i < 5; i++) {
                                    if (Mise[i] == 0) {
                                        Mise[i] = 10;
                                        Score -= 10;
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
                                        Score -= 20;
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
                                        Score -= 50;
                                        MiseTotale += 50;
                                        miseAppliquee = true;
                                        break;
                                    }
                                }
                            }
                        }
                        
                        string MiseString = to_string(MiseTotale);
                        MiseJoueur.setString(MiseString);
                        string scoreString = to_string(Score);
                        �.setString(scoreString);
                    }
                    else {
                        if (Tirer.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            if (!AdversairePlaying) {
                                drawCard();
                                if (Resultat > 21) {
                                    isPlayingBj = false;
                                    MiseTotale = 0;
                                    string MiseString = to_string(MiseTotale);
                                    MiseJoueur.setString(MiseString);
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
                                cerr << "Resultat de l'adversaire : " << ResultatAdversaire << endl;
                            }
                            AdversairePlaying = false;

                            if (Resultat <= 21 && (Resultat > ResultatAdversaire || ResultatAdversaire > 21)) {
                                cerr << "Gagn� !" << endl;
                                Score += MiseTotale * 1.5;
                            }
                            if (Resultat == ResultatAdversaire) {
                                cerr << "Egalit�" << endl;
                                Score += MiseTotale;
                            }
                            if (Resultat < ResultatAdversaire && ResultatAdversaire <= 21) {
                                cerr << "Perdu !" << endl;
                            }
                            MiseTotale = 0;
                            string MiseString = to_string(MiseTotale);
                            MiseJoueur.setString(MiseString);
                            string scoreString = to_string(Score);
                            �.setString(scoreString);
                            isPlayingBj = false;
                            for (int i = 0; i < 5; i++) {
                                Mise[i] = 0;
                            }

                        }
                    }
                }

                if (currentState == Plinko) {
                    if (!isPlinkoPlaying && ButtonPlayPlinko.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        if (!isBallFalling) {
                            Score -= 10;
                            string scoreString = to_string(Score);
                            �.setString(scoreString);
                            isBallFalling = true;
                            newPos();
                            Balls.setPosition(PlinkoPosX, PlinkoPosY);
                        }
                    }
                }

            }
            if (Balls.getGlobalBounds().intersects(Score1.getGlobalBounds()) || Balls.getGlobalBounds().intersects(Score2.getGlobalBounds()) || Balls.getGlobalBounds().intersects(Score3.getGlobalBounds())) {
                if (Balls.getGlobalBounds().intersects(Score1.getGlobalBounds()) || Balls.getGlobalBounds().intersects(Score3.getGlobalBounds())) {
                    Score *= 1.5;
                    string scoreString = to_string(Score);
                    �.setString(scoreString);
                }
                isBallFalling = false;
                velocity.y = 0;
            }
        }

        window.clear(Color::Black);
        HideAllButtons(buttons);

        switch (currentState) {
        case Menu:
            if (showMainMenu) {
                MainMenu1.setPosition(1320 * scaleX, 295 * scaleY);
                MainMenu2.setPosition(1320 * scaleX, 450 * scaleY);
                MainMenu3.setPosition(1320 * scaleX, 605 * scaleY);
                

                window.draw(Background1);
                window.draw(MainMenu1);
                window.draw(MainMenu2);
                window.draw(MainMenu3);
                window.draw(Menu3);
                window.draw(Menu2);
                window.draw(Menu1);

                window.draw(BlackJackText);
                window.draw(PlinkoTexT);
                window.draw(SurvieText);
            }
            else {
                Button.setPosition(1115 * scaleX, 620 * scaleY);
                window.draw(Background1);
                window.draw(Menu3);
                window.draw(Menu2);
                window.draw(Menu1);

                window.draw(Button);
                window.draw(ButtonPlay);
            }
            break;

        case Plinko:
            ReturnButton.setPosition(0, 980 * scaleY);
            window.draw(FondArgent);
            window.draw(Argent);
            window.draw(�);

            window.draw(ButtonPlayPlinko);
            window.draw(PlayPlinko);
            window.draw(Separation);

            window.draw(Score1);
            window.draw(Score2);
            window.draw(Score3);

            window.draw(Barre1);
            window.draw(Balls);
            window.draw(ReturnButton);
            break;

        case BlackJack:
            if (!isPlayingBj) {
                ReturnButton.setPosition(0, 980 * scaleY);
                BetButton1.setPosition(400 * scaleX, 850 * scaleY);
                BetButton2.setPosition(800 * scaleX, 850 * scaleY);
                BetButton3.setPosition(1200 * scaleX, 850 * scaleY);
                PlayButtonBJ.setPosition(710 * scaleX, 700 * scaleY);
                window.draw(ScoreAdversaireFond);
                window.draw(ScoreAdversaire);
                window.draw(MiseMenuFond);

                window.draw(BetButton1);
                window.draw(BetButtonText1);
                window.draw(BetButton2);
                window.draw(BetButtonText2);
                window.draw(BetButton3);
                window.draw(BetButtonText3);
                window.draw(PlayButtonBJ);
                window.draw(PlayButtonText);
                window.draw(FondMiseJoueur);
                window.draw(FondMiseAdversaire);
                window.draw(ReturnButton);
                window.draw(MiseJoueur);
                window.draw(VD);

                window.draw(FondArgent);
                window.draw(Argent);
                window.draw(�);
            }
            else 
            {
                Tirer.setPosition(1300 * scaleX, 700 * scaleY);
                Rester.setPosition(1600 * scaleX, 700 * scaleY);

                window.draw(ScoreAdversaireFond);
                window.draw(ScoreAdversaire);
                window.draw(MiseMenuFond);
                window.draw(FondMiseAdversaire);
                window.draw(FondMiseJoueur);

                window.draw(Tirer);
                window.draw(Rester);

                window.draw(FondArgent);
                window.draw(Argent);
                window.draw(�);
            }
            break;
            case Survie:
                ReturnButton.setPosition(0, 980 * scaleY);
                window.draw(ReturnButton);
                break;
        }

        if (showMainMenu) {
            LeaveButton.setPosition(1800 * scaleX, 980 * scaleY);
            window.draw(LeaveButton);
        }
        window.display();
    }
}