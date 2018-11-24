#ifndef HEARTHSTONE_MINION_H
#define HEARTHSTONE_MINION_H


#include "Card.h"
#include <vector>
class Player;
class Enchantment;

class Minion : public Card {

private:
    int att, defense, actionValue, recordActionValue, magic;   //magic is the cost of ability
    bool haveAbility, haveTrigger;
    std::vector<Enchantment*> recordEnchantment;

    bool canAttack();
    bool isDead();
public:
    Minion(int cost, std::string name, std::string description, int att, int def, int actionValue,
           int recordActionValue, int magic, bool haveAbility, bool haveTrigger);

    virtual ~Minion();
    int getActionValue();
    int getMagic();
    int getDef();
    int getAtt();

    bool hasEnchant();
    bool hasAbility();
    bool hasTrigger();

    void setActionValue();
    void setRecordActionValue(int);
    void setDef(int i);
    void setMagic(int);

    Enchantment &getEnchant(int i);
    int numOfEnchant();
    void pushEnchantment(Enchantment*);
    void mutateDef(int);   // i is the damage source. When i is positive, it is receiving damage.
    void mutateAtt(int);   // i is the buff effect. When i is positive, it is buffing attack

    void attack(Player &); // attacks the opposing player
    void attack(Minion &otherMinion, Player &player, Player &otherPlayer); // attacks the ith minion of the opposing player
    bool play(Player &player, Player &otherPlayer) override; //put the ith minion on the minion slot and return true if succeed.
    bool play(Player &, Card &) override;  //always false
    virtual void trigger(Trigger t, Player &) {};          // did not implement these two
    virtual void trigger(Trigger t, Minion &myMinion, Minion &otherMinion, Player &player, Player &otherPlayer) {};
    virtual bool ability(Player &);
    virtual bool ability(Player &, Card &);
};


class AirElemental final : public Minion {
public:
    AirElemental();
    void trigger(Trigger t,Player&) override;
    void trigger(Trigger t, Minion &myMinion, Minion &otherMinion, Player &player, Player &otherPlayer) override;
    bool ability(Player &) override;
    bool ability(Player &, Card &) override;
};


class EarthElemental final : public Minion {
public:
    EarthElemental();
    void trigger(Trigger t, Player&) override;
    void trigger(Trigger t, Minion &myMinion, Minion &otherMinion, Player &player, Player &otherPlayer) override;
    bool ability(Player &) override;
    bool ability(Player &, Card &) override;
};


class BoneGolem final : public Minion {
public:
    BoneGolem();
    void trigger(Trigger t, Player&) override;
    void trigger(Trigger t, Minion &myMinion, Minion &otherMinion, Player &player, Player &otherPlayer) override;
    bool ability(Player &) override;
    bool ability(Player &, Card &) override;
};

class FireElemental final : public Minion {
public:
    FireElemental();
    void trigger(Trigger t, Player&) override;
    void trigger(Trigger t, Minion &myMinion, Minion &otherMinion, Player &player, Player &otherPlayer) override;
    bool ability(Player &) override;
    bool ability(Player &, Card &) override;
};


class PotionSeller final: public Minion {
public:
    PotionSeller();
    void trigger(Trigger t,Player&) override;
    void trigger(Trigger t, Minion &myMinion, Minion &otherMinion, Player &player, Player &otherPlayer) override;
    bool ability(Player &) override;
    bool ability(Player &, Card &) override;
};


class NovicePyromancer : public Minion {
public:
    NovicePyromancer();
    void trigger(Trigger t,Player&) override;
    void trigger(Trigger t, Minion &myMinion, Minion &otherMinion, Player &player, Player &otherPlayer) override;
    bool ability(Player &) override;
    bool ability(Player &, Card &) override;
};


class ApprenticeSummoner : public Minion {
public:
    ApprenticeSummoner();
    void trigger(Trigger t,Player&) override;
    void trigger(Trigger t, Minion &myMinion, Minion &otherMinion, Player &player, Player &otherPlayer) override;
    bool ability(Player &) override;
    bool ability(Player &, Card &) override;
};


class MasterSummoner : public Minion {
public:
    MasterSummoner();
    void trigger(Trigger t,Player&) override;
    void trigger(Trigger t, Minion &myMinion, Minion &otherMinion, Player &player, Player &otherPlayer) override;
    bool ability(Player &) override;
    bool ability(Player &, Card &) override;
};

#endif
