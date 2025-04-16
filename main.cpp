#include <bits/stdc++.h>
#include <utility>
#include "random.h"

using namespace std;

class Creature {
protected:

    string m_name;
    char m_symbol;
    int m_health;
    int m_damage;
    int m_gold;
    int m_exp;

public:
    Creature(string name, const char symbol, const int health, const int damage, const int gold, const int exp):
        m_name{std::move(name)},
        m_symbol{symbol},
        m_health{health},
        m_damage{damage},
        m_gold{gold},
        m_exp{exp}
    {}

    [[nodiscard]] const string& getName() const {return m_name;}
    [[nodiscard]] char getSymbol() const {return m_symbol;}
    [[nodiscard]] int getHealth() const {return m_health;}
    [[nodiscard]] int getDamage() const {return m_damage;}
    [[nodiscard]] int getGold() const {return m_gold;}
    [[nodiscard]] int getExp() const {return m_exp;}

    void reduceHealth(const int health) {
        m_health-=health;
    }

    [[nodiscard]] bool isDead() const {
        return m_health<=0;
    }

    void addGold(const int gold) {
        m_gold+=gold;
    }

    void reduceGold(const int gold) {
        m_gold-=gold;
    }

    void addHealth(const int health) {
        m_health+=health;
    }

    void addStrength(const int strength) {
        m_damage+=strength;
    }
};

class Player: public Creature {
    int m_level {1};

public:
    explicit Player(const string& name):Creature{name, '@', 10, 1, 0, 0}
    {}

    [[nodiscard]] int getLevel() const {return m_level;}

    void levelUp() {
        m_level++;
        m_damage++;
        if (m_level!=20) {
            cout<<"You are now at level "<<m_level<<"\nYou have "<<m_gold<<" gold\n";
        }
    }

    void addExp(const int exp) {
        m_exp+=exp;
        if (m_exp>=100) {
            levelUp();
            m_exp=m_exp%100;
        }
    }

    void displayLevel() const {
        cout<<"Level: "<<m_level<<"  ||   exp: "<<m_exp<<"/100\n";
    }

    [[nodiscard]] bool hasWon() const {
        return m_level>=20;
    }
};

class Monster: public Creature {
public:
    enum Type {
        slime,
        zombie,
        spider,
        orc,


        wolf,
        creeper,
        skeleton,

        kraken,
        behemoth,
        dragon,

        max_types,
    };

private:
    inline static Creature monsterData[] {
        Creature{"slime", 's', 1, 1, 10, 15},
        Creature{"zombie", 'z', 2, 1, 20, 20},
        Creature{"spider", 'S', 3, 1, 45, 60},
        Creature{"orc", 'o', 4, 2, 25, 30},

        Creature{"wolf", 'w', 6, 2, 30, 45},
        Creature{"skeleton", 'k', 12, 2, 65, 80},
        Creature{"creeper", 'c', 10, 2, 35, 50},

        Creature{"kraken", 'k', 16, 3, 75, 90},
        Creature{"behemoth", 'b', 18, 4, 80, 100},
        Creature{"dragon", 'D', 20, 4, 100, 100}
    };

public:
    explicit Monster(const Type type):Creature{monsterData[type]}{}

    static Monster getRandomMonster(const Player& player) {
        if (player.getLevel()>=0 && player.getLevel()<=5) {
            return Monster {static_cast<Type>(Random::get(0, 3))};
        }
        if (player.getLevel()>5 && player.getLevel()<=15) {
            return Monster {static_cast<Type>(Random::get(2, 6))};
        }
        if (player.getLevel()>15 && player.getLevel()<20) {
            return Monster {static_cast<Type>(Random::get(5, 9))};
        }
    }
};

int escape() {
    return Random::get(0, 1);
}

void monsterAttack(const Monster& monster, Player& player) {
    player.reduceHealth(monster.getDamage());
    cout<<"The "<<monster.getName()<<" hit you for "<<monster.getDamage()<<" damage\n";
}

void playerAttack(Monster& monster, const Player& player) {
    monster.reduceHealth(player.getDamage());
    cout<<"You hit the "<<monster.getName()<<" for "<<player.getDamage()<<" damage\n";
}

class Potion {
public:
    enum Type {
        health,
        strength,
        poison,
        exp,
        null,
    };

    enum Effect {
        small,
        medium,
        large,
    };

    Type m_type;
    Effect m_effect;

    Potion(const Type type=null, const Effect effect=small):
        m_type{type},
        m_effect{effect}
    {}

    [[nodiscard]] Type getType() const { return m_type; }
    [[nodiscard]] Effect getEffect() const { return m_effect; }

    [[nodiscard]] string getName() const {
        static string type_name[] {"health", "strength", "poison", "exp"};
        static string effect_name[] {"small", "medium", "large"};

        string res {effect_name[static_cast<int>(m_effect)] + " potion of " + type_name[static_cast<int>(m_type)]};
        return res;
    }

    static Potion getRandomPotion() {
        return {static_cast<Type>(Random::get(0, 2)), static_cast<Effect>(Random::get(0, 2))};
    }
};

Potion shop(Player& player) {
    static pair<Potion, pair<int, int>> stock[] {
        {{Potion::health, Potion::small}, {3, 50}},
        {{Potion::health, Potion::medium}, {3, 75}},
        {{Potion::health, Potion::large}, {3, 100}},
        {{Potion::strength, Potion::small}, {3, 50}},
        {{Potion::strength, Potion::medium}, {3, 75}},
        {{Potion::strength, Potion::large}, {3, 100}},
        {{Potion::exp, Potion::small}, {3, 50}},
        {{Potion::exp, Potion::medium}, {3, 75}},
        {{Potion::exp, Potion::large}, {3, 100}}
    };

    cout<<"You have "<<player.getGold()<<" gold, 'q' to exit\n";

    char i='a';
    for (const auto &v: stock) {
        cout<<i++<<". "<<v.first.getName()<<" x"<<v.second.first<<", cost: "<<v.second.second<<"\n";
    }

    char inp;
    do {
        cin>>inp;
        if (inp=='q') {
            return {Potion::null, Potion::small};
        }
    } while (!(inp>='a' && inp<='i'));

    if (stock[inp-'a'].second.first>0 && player.getGold()>=stock[inp-'a'].second.second) {
        player.reduceGold(stock[inp-'a'].second.second);
        return stock[inp-'a'].first;
    }
    if (stock[inp-'a'].second.first<0) {
        cout<<"Not available\n";
        return {Potion::null, Potion::small};
    }
    if (stock[inp-'a'].second.second>player.getGold()) {
        cout<<"Insufficient funds\n";
        return {Potion::null, Potion::small};
    }
}

void consumePotion(Player& player, const Potion& potion) {
    switch (potion.getType()) {
        case Potion::health:
            switch (potion.getEffect()) {
                case Potion::small:
                    player.addHealth(2); break;
                case Potion::medium:
                    player.addHealth(3); break;
                case Potion::large:
                    player.addHealth(5); break;
            }
        break;
        case Potion::strength:
            switch (potion.getEffect()) {
                case Potion::small:
                    player.addStrength(1); break;
                case Potion::medium:
                    player.addStrength(2); break;
                case Potion::large:
                    player.addStrength(3); break;
            }
        break;
        case Potion::poison:
            switch (potion.getEffect()) {
                case Potion::small:
                    player.reduceHealth(1); break;
                case Potion::medium:
                    player.reduceHealth(2); break;
                case Potion::large:
                    player.reduceHealth(3); break;
            }
        break;
        case Potion::exp:
            switch (potion.getEffect()) {
                case Potion::small:
                    player.addExp(25); break;
                case Potion::medium:
                    player.addExp(50); break;
                case Potion::large:
                    player.addExp(75); break;
            }
        break;
        case Potion::null:
            switch (potion.getEffect()) {
                case Potion::small:
                case Potion::medium:
                case Potion::large:
                    break;
            }
        break;
    }

    if (potion.getType()!=Potion::null) {
        cout<<"You consumed "<<potion.getName()<<'\n';
    }
}

void fightMonster(Player& player) {
    Monster monster {Monster::getRandomMonster(player)};

    cout<<"You encountered a "<<monster.getName()<<" ("<<monster.getSymbol()<<")\n";
    while (!player.isDead() && !monster.isDead()) {

        char input;
        do {
            cout<<"(R)un || (F)ight || (H)ealth || (S)hop || (L)evel: ";
            cin>>input;

        } while (input!='r' && input!='f' && input!='h' && input!='s' && input!='l');

        if (input=='h' || input=='H') {
            cout<<"You have "<<player.getHealth()<<" health\n";
            continue;
        }

        if (input=='r' || input=='R') {
            if (escape()) {
                cout<<"You successfully fled\n";
                return;
            }
            else {
                cout<<"You failed to flee\n";
                monsterAttack(monster, player);
            }
        }

        if (input=='f' || input=='F') {
            playerAttack(monster, player);
            if (!monster.isDead()) {
                monsterAttack(monster, player);
            }
        }

        if (input=='s') {
            const Potion potion {shop(player)};
            consumePotion(player, potion);
        }

        if (input=='l') {
            player.displayLevel();
        }
    }

    if (monster.isDead()) {

        cout<<"You killed the "<<monster.getName()<<"\n";

        player.addGold(monster.getGold());
        player.addExp(monster.getExp());

        if (Random::get(1, 3)==1) {
            cout<<"You found a mythical potion! Do you want to drink it? [y/n]: ";
            char c;
            cin>>c;
            if (c=='y') {
                const Potion potion {Potion::getRandomPotion()};
                consumePotion(player, potion);
            }
        }
    }
}

int main() {
    cout<<"Enter your name: ";
    string name;
    cin>>name;
    Player player {name};
    cout<<"Welcome, "<<name<<'\n';

    while (!player.isDead() && !player.hasWon()) {
        fightMonster(player);
    }

    if (player.isDead()) {
        cout<<"You died at level "<<player.getLevel()<<" and had "<<player.getGold()<<" gold\n";
    }
    else if (player.hasWon()) {
        cout<<"You won with "<<player.getGold()<<" gold\n";
    }

    return 0;

}
