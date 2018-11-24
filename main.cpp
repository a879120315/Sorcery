#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <memory>
#include <typeindex>

#include "Board.h"
#include "Player.h"
#include "Card/Spell.h"
#include "Card/Ritual.h"
#include "Card/Enchantment.h"
#include "Display.h"

using namespace std;


void endOfGame(bool quit, Player &p1, Player &p2){
    if(quit) {
        cout << "the player has quitted the game" << endl;
    }
    else if(p1.isDead()){
        //player 1 is dead and palyer 2 wins
        cout << "player 2 has win!" << endl;
    }
    else if(p2.isDead()){
        //player 2 is dead and palyer 1 wins
        cout << "player 1 has win!" << endl;
    }

}
//Make a deck from the file
void makeDeck(istream &in, Player &p) {
    cout << "make a deck" << endl;

    vector<unique_ptr<Card>> deck;
    map<string, unique_ptr<Card>> cards;

    //Spells
    cards["Banish"] = make_unique<Banish>();
    cards["Unsummon"] = make_unique<Unsummon>();
    cards["Recharge"] = make_unique<Recharge>();
    cards["Disenchant"] = make_unique<Disenchant>();
    cards["Raise Dead"] = make_unique<RaiseDead>();
    cards["Blizzard"] = make_unique<Blizzard>();

    //Minions
    cards["Air Elemental"] = make_unique<AirElemental>();
    cards["Earth Elemental"] = make_unique<EarthElemental>();
    cards["Boom Golem"] = make_unique<BoneGolem>();
    cards["Fire Elemental"] = make_unique<FireElemental>();
    cards["Potion Seller"] = make_unique<PotionSeller>();
    cards["Novice Pyromancer"] = make_unique<NovicePyromancer>();
    cards["Apprentice Summoner"] = make_unique<ApprenticeSummoner>();
    cards["Master Summoner"] = make_unique<MasterSummoner>();

    //Enchantments
    cards["Giant Strength"] = make_unique<GiantStrength>();
    cards["Enrarge"] = make_unique<Enrage>();
    cards["Haste"] = make_unique<Haste>();
    cards["Magic Fatigue"] = make_unique<MagicFatigue>();
    cards["Silence"] = make_unique<Silence>();

    //Ritual
    cards["Dark Ritual"] = make_unique<DarkRitual>();
    cards["Aura of Power"] = make_unique<AuraOfPower>();
    cards["Standstill"] = make_unique<Standstill>();

    //loop to make a deck
    string name;
    int count = 0 ;
    while (getline(in, name)) {
        cout << count << name << endl;
             deck.push_back(cards[name]);
             p.addToDeck(cards[name]);
             count++;
    }
}

void displayFile(string path) {
    ifstream fin(path);
    string s;
    while(getline(fin, s)){
        cout << s << endl;
    }
}

void playGame(istream &in, Player *p1, Player *p2, bool testMode, bool graphicMode) {
    string input; // only use with cin for input purpose
    int i, j, p; // only use with cin for input purpose
    bool quit = false; // if the player choose to quit the game so no winner
    vector<unique_ptr<Display>> view;//vector for different displays
    if(graphicMode){
        //Display graphic = new Graphic();
       unique_ptr<Display> graphic = make_unique<Graphic>();
       view.emplace_back(graphic);
    }

    //display text
    unique_ptr<Display> text = make_unique<Text>();
    view.emplace_back(text);


    cout << "Welcome to sorcery" << endl;
    cout << "Please indicate both players' name" << endl;

    //get the names of the players
    in >> input;
    p1->setName(input);
    in >> input;
    p2->setName(input);

    //shuffle the deck from the players
    p1->shuffle();
    p2->shuffle();

    int round = 1; //count the # of rounds the game has occur, use to track which player is active player
    while ((!p1->isDead() && !p2->isDead()) && !quit) {

        //Game Loop
        cout << "round" << round << endl;
        Player *player = round % 2 ? p1 : p2; // This is the active player in current round
        Player *other = (player == p1) ? p2 : p1;

        player->getMyBoard()->notifyAll(Card::Trigger::START_OF_TURN, *player);

        while(true) {
            //loop for a round of one player
            string cmd;
            if (getline(in, input)) {}
            else if(&in != &cin && getline(cin, input)) {}
            else { break; }
            istringstream iss(input);
            iss>>cmd;

            if (cmd == "help") {
                 displayFile("help.txt");
            } else if (cmd == "end") {
                cout << "end is called" << endl;
                player->getMyBoard()->notifyAll(Card::Trigger::END_OF_TURN, *player);
                break;
            } else if (cmd == "quit") {
                cout << "quit is called" << endl;
                quit = true;
            } else if (cmd == "draw" && testMode) {
                cout << "draw is called and it is in testMode" << endl;
                player->drawCard();// 5.1.4
            } else if (cmd == "discard" && testMode) {
                cout << "discard is called and it is in testMode" << endl;
                cin >> i;  // 5.1.5
                player->discardCard(i);
            } else if (cmd == "attack") {
                iss >> i;
                cout << "attack is called" << endl;
                if(player->getMyBoard()->getMinion(i).getCost() >= player->getMagic()){
                    //the player does not have enough magic to attack
                    cout << "do not have enough magic to attack" << endl;
                    continue;
                }

                if(iss >> j){
                    Minion &myMinion = player->getMyBoard()->getMinion(i);
                    Minion &otherMinion = other->getMyBoard()->getMinion(j);
                    myMinion.attack(otherMinion, *player, *other);
                    if (myMinion.isDead()){
                        //Trigger t, Minion &myMinion, Minion &otherMinion, Player &player, Player &other
                        player->getMyBoard()->notifyAll(Card::Trigger::MY_MINION_LEAVE, myMinion, otherMinion, *player, *other);
                        other->getMyBoard()->notifyAll(Card::Trigger::OTHER_MINION_LEAVE, otherMinion, myMinion, *other, *player);
                    }
                    if(otherMinion.isDead()) {
                        player->getMyBoard()->notifyAll(Card::Trigger::OTHER_MINION_LEAVE, myMinion, otherMinion, *player, *other);
                        other->getMyBoard()->notifyAll(Card::Trigger::MY_MINION_LEAVE, otherMinion, myMinion, *player, *other);
                    }
                }else{
                    player->getMyBoard()->getMinion(i).attack(*other);
                }

            } else if (cmd == "play") {
                cout << "play is called" << endl;
                cin >> i;
                if(player->getCard(i).getCost() >= player->getMagic()){
                    cout << "Do not have enough magic to play this card" << endl;
                     continue;
                }

                if(iss >> p >> j){ // play i p t
                    Player  *pl = (p == 1) ? p1 : p2;
                    iss >> j;
                    bool success = player->getCard(i).play(*player, pl->getCard(i));
                    if(success) player->moveCardToBoard(i);
                }
                else{ //play i
                    bool success = player->getCard(i).play(*player);
                    if(success) player->moveCardToBoard(i);
                }

            } else if (input == "use") {
                //TODO: 5.1.8 if statement for if player has enough magic to attack
            } else if (input == "inspect") {
                cin >> i;
               //loop through to output the interface
                for(vector<unique_ptr<Display>>::iterator it = view.begin() ; it != view.end(); ++it){
                    (*it)->displayMinion(player->getMyBoard()->getMinion(i));
                }
            } else if (input == "hand") {
                //displays the hand of an active player
                for(auto &it: view){
                    it->displayHand(player);
                }
            } else if (input == "board") {
                for(auto &it: view){
                    it->displayBoard(player->getMyBoard());
                }
            }
        }
    }
    endOfGame(quit, *p1, *p2);
}

int main(int argc, char *argv[]) {
    Player *p1 = new Player{};
    Player *p2 = new Player{};

    //5.2 all the command line arguments
    int numArgs = argc - 1;
    string initPath;
    bool readFile = false;
    string deckPath1 = "default.deck";
    string deckPath2 = "default.deck";
    bool testMode = false;
    bool graphicMode = false;


    //for loop to do the arguments
    for (int i = 0; i < numArgs; ++i) {
        if (argv[i] == "-deck1") { //player1 uses another deck
            cout << "player1 gets deck from another file" << endl;
            deckPath1 = argv[++i];
        } else if (argv[i] == "-deck2") { //player2's deck < filename
            cout << "player2 gets deck from another file" << endl;
            deckPath2 = argv[++i];
        } else if (argv[i] == "-init") {//want to read commands first from a file
            cout << "init mode" << endl;
            readFile = true;
            initPath = argv[++i];
        } else if (argv[i] == "-testing") {
            cout << "testing mode" << endl;
            testMode = true;
        } else if (argv[i] == "-graphics") {
            graphicMode = true;
            cout << "graphic mode" << endl;
        }
    }

    ifstream f1{deckPath1};
    makeDeck(f1, *p1);
    ifstream f2{deckPath2};
    makeDeck(f2, *p2);

    ifstream f{initPath};
    if (readFile) {
        playGame(f, p1, p2, testMode, graphicMode);
    } else {
        playGame(cin, p1, p2, testMode, graphicMode);
    }
}