//TODO : la retraite doit tenir compte des hp de ce qui est devant moi
//TODO : items
//TODO : buissons
//TODO : neutres
//TODO : sorts
//TODO : pas se mongoliser devant la tour adverse

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

#define max(a, b)   ((a) > (b) ? (a) : (b))
#define min(a, b)   ((a) < (b) ? (a) : (b))

#define XMAX    1920
#define XMIN    0

#define ATTACK_RANGE_MELEE 150

/****************************** Item ******************************/
class Item {
private :
    string _itemName; // contains keywords such as BRONZE, SILVER and BLADE, BOOTS connected by "_" to help you sort easier
    int _itemCost; // BRONZE items have lowest cost, the most expensive items are LEGENDARY
    int _damage; // keyword BLADE is present if the most important item stat is damage
    int _health;
    int _maxHealth;
    int _mana;
    int _maxMana;
    int _moveSpeed; // keyword BOOTS is present if the most important item stat is moveSpeed
    int _manaRegen;


    float _ratioDamage;
    float _ratioHealth;

public :
    Item(string itemName, int itemCost, int damage, int health, int maxHealth, int mana, int maxMana, int moveSpeed, int manaRegen) :
            _itemName(itemName), _itemCost(itemCost), _damage(damage), _health(health), _maxHealth(maxHealth), _mana(mana), _maxMana(maxMana), _moveSpeed(moveSpeed), _manaRegen(manaRegen)
    {
        _ratioDamage = _damage ? (float) _itemCost / (float) _damage : 0;
        _ratioHealth = _health ? (float) _itemCost / (float) _health : 0;
    }

    void trace(string type) const {
        size_t found = _itemName.find(type);
        if (found != string::npos) {
            cerr << setw(20) << left << _itemName << " cout(" << setw(4) << _itemCost << ") dmg(" << _damage << ") hp(" << _health << "/" << _maxHealth << ") mana(" << _mana << "/" << _maxMana << ", +" << _manaRegen << ") move(" << _moveSpeed << ")";
            cerr << " ratios(dmg=" << _ratioDamage << " hp=" << _ratioHealth << ")";
            cerr << endl;
        }
    }

    string getItemName() {return _itemName;};
    int getItemCost() {return _itemCost;};
    int getDamage() {return _damage;};
    int getHealth() {return _health;};
    int getMana() {return _mana;};
    int getManaRegen() {return _manaRegen;};
    int getMoveSpeed() {return _moveSpeed;};
    int getRatioDamage() {return _ratioDamage;};
    int getRatioHealth() {return _ratioHealth;};
};

/****************************** Sacoche ******************************/
class Sacoche {
private :
    static const int _maxItems = 4;
    vector <Item> _items;

public :
    int  getNbPlaces() {return max(0, _maxItems - _items.size());}

    void addItem(Item &item) {
        if (_items.size() == _maxItems) return;
        _items.push_back(item);
    }

    void removeItem(string nomItem) {
        for (int cpt = 0; cpt < _items.size(); cpt++) {
            if (_items[cpt].getItemName() == nomItem) {
                _items.erase(_items.begin() + cpt);
                return;
            }
        }
    }
};

/****************************** Point ******************************/
class Point {
protected :
    int _x;
    int _y;

public :
    Point(int x, int y) : _x(x), _y(y) {}
    int getX() {return _x;}
    int getY() {return _y;}

    double dist(Point &p) const {
        return sqrt((_x - p._x) * (_x - p._x) + (_y - p._y) * (_y - p._y));
    }

    bool isInRange(Point &p, int range) {
        return (dist(p) <= range);
    }
};

/****************************** Circle ******************************/
class Circle : public Point {
protected :
    int _radius;

public :
    Circle(int x, int y, int radius) : Point(x, y), _radius(radius) {}
};

/****************************** Bush ******************************/
class Bush : public Circle {
public :
    Bush(int x, int y, int radius) : Circle(x, y, radius) {}
};

/****************************** Unit ******************************/
class Unit : public Point {
protected :
    // Données en entrée
    int _id;
    int _attackRange;
    int _health;
    int _maxHealth;
    int _shield;
    int _attackDamage;
    int _movementSpeed;
    int _stunDuration;
    int _goldValue;

    // Autres
    bool _alive;
    float _baseAttackTime;

public :
    Unit(int id, int x, int y, int attackRange, int health, int maxHealth, int shield, int attackDamage, int movementSpeed, int stunDuration, int goldValue) :
            _id(id), Point(x, y), _attackRange(attackRange), _health(health), _maxHealth(maxHealth), _shield(shield), _attackDamage(attackDamage), _movementSpeed(movementSpeed), _stunDuration(stunDuration), _goldValue(goldValue),
            _alive(true), _baseAttackTime(0.2)
    {}

    int getId() {return _id;}
    bool getAlive() {return _alive;}
    void setAlive(bool alive) {_alive = alive;}
    int getHealth() {return _health;}
    int getMaxHealth() {return _maxHealth;}
    int getDamage() { return _attackDamage;}
    int getMovementSpeed() {return _movementSpeed;}

    void trace() const {
        cerr << "id(" << _id << ") (" << _x << ", " << _y << ") dmg(" << _attackDamage << ") range(" << _attackRange << ") hp(" << _health << "/" << _maxHealth << ") value(" << _goldValue << ")";
        cerr << endl;
    }
    void maj(int id, int x, int y, int attackRange, int health, int maxHealth, int shield, int attackDamage, int movementSpeed, int stunDuration, int goldValue)
    {
        _id            = id;
        _x             = x;
        _y             = y;
        _attackRange   = attackRange;
        _health        = health;
        _maxHealth     = maxHealth;
        _shield        = shield;
        _attackDamage  = attackDamage;
        _movementSpeed = movementSpeed;
        _stunDuration  = stunDuration;
        _goldValue     = goldValue;

        _alive = true;
    }

    bool isInRange(Point &p) {return Point::isInRange(p, _attackRange);}
    float calculeAttackTime(Point cible) {
        if (_attackRange < ATTACK_RANGE_MELEE) return _baseAttackTime;
        return calculeAttackTime(Point(_x, _y), cible);
    }
    float calculeAttackTime(Point p1, Point p2) {
        if (_attackRange < ATTACK_RANGE_MELEE) return _baseAttackTime;
        return (_baseAttackTime * (1.0 + p1.dist(p2) / _attackRange));
    }
};

/****************************** Hero ******************************/
class Hero : public Unit {
protected :
    int _countDown1;
    int _countDown2;
    int _countDown3;
    int _mana;
    int _maxMana;
    int _manaRegen;
    string _heroType;
    int _isVisible;
    int _itemOwned;

    // Autres
    int _etat;
    string _order;
    Sacoche _sacoche;


public :
    enum heroEtat {
        DEFAULT = 0,
        OTHER
    };

    Hero(int id, int x, int y, int attackRange, int health, int maxHealth, int shield, int attackDamage, int movementSpeed, int stunDuration, int goldValue,
         int countDown1, int countDown2, int countDown3, int mana, int maxMana, int manaRegen, string heroType, int isVisible, int itemOwned);
    Sacoche& getSacoche();
    string getOrder();
    void setOrder(string order);
    void trace() const;
    void maj(int id, int x, int y, int attackRange, int health, int maxHealth, int shield, int attackDamage, int movementSpeed, int stunDuration, int goldValue,
             int countDown1, int countDown2, int countDown3, int mana, int maxMana, int manaRegen, string heroType, int isVisible, int itemOwned);
    void playSolo();

    void retreatOrNot();
    void moveToFriendlyUnitNearestFromEnnemyTower();
    int findIdOfNearestEnnemyFromPos(Point pos);
    int findIdToAttackWhenMovingToPoint(Point pos);
    void buyPotion();
    void buyItemDegats();
};

/****************************** Deadpool ******************************/
class Deadpool : public Hero {
public :
    Deadpool(int id, int x, int y);
};

/****************************** Hulk ******************************/
class Hulk : public Hero {
public :
    Hulk(int id, int x, int y);
};

/****************************** Valkyrie ******************************/
class Valkyrie : public Hero {
public :
    Valkyrie(int id, int x, int y);
};

/****************************** Ironman ******************************/
class Ironman : public Hero {
public :
    Ironman(int id, int x, int y);
};

/****************************** Doctor Strange ******************************/
class DoctorStrange : public Hero {
public :
    DoctorStrange(int id, int x, int y);
};

/****************************** Team ******************************/
class Team {
private :
    vector <Unit> _tabTowers;
    vector <Unit> _tabUnits;
    vector <Hero> _tabHeros;
    int _num;

    void addOrMajTower(int unitId, int x, int y, int attackRange, int health, int maxHealth, int shield, int attackDamage, int movementSpeed, int stunDuration, int goldValue);
    void addOrMajUnit(int unitId, int x, int y, int attackRange, int health, int maxHealth, int shield, int attackDamage, int movementSpeed, int stunDuration, int goldValue);
    void addOrMajHero(int unitId, int x, int y, int attackRange, int health, int maxHealth, int shield, int attackDamage, int movementSpeed, int stunDuration, int goldValue,
                      int countDown1, int countDown2, int countDown3, int mana, int maxMana, int manaRegeneration, string heroType, int isVisible, int itemsOwned);

public :
    Team(int num) : _num(num) {}

    int getNum() {return _num;}
    vector <Unit>& getTabTowers() {return _tabTowers;}
    vector <Unit>& getTabUnits() {return _tabUnits;}
    vector <Hero>& getTabHeros() {return _tabHeros;}

    void trace() const;
    void clean();
    void addOrMajEntity(int unitId, string unitType, int x, int y, int attackRange, int health, int maxHealth, int shield, int attackDamage, int movementSpeed, int stunDuration, int goldValue,
                        int countDown1, int countDown2, int countDown3, int mana, int maxMana, int manaRegeneration, string heroType, int isVisible, int itemsOwned);
};

/****************************** Monde ******************************/
class Monde {
private :
    static Monde _m;
    Monde() {
        _tabTeam.push_back(Team(0));
        _tabTeam.push_back(Team(1));
    }
    ~Monde() {}

    vector <Bush> _tabBush;
    vector <Point> _tabSpawnPoints;
    vector <Item> _tabPotions;
    vector <Item> _tabStuff;
    vector <Unit> _tabGroots;
    vector <Team> _tabTeam;

    int _myTeam;
    int _otherTeam;
    int _gold;
    int _enemyGold;
    int _roundType;

public :
    static Monde& get() {
        return _m;
    }
    int getRoundType() {return _roundType;}
    int getGold() {return _gold;}
    int getMyTeam() {return _myTeam;}
    int getOtherTeam() {return _otherTeam;}
    vector <Item>& getTabPotions() {return _tabPotions;}
    vector <Item>& getTabItems() {return _tabStuff;}
    Team* getTeam(int team) {
        if (team >=0 && team <= _tabTeam.size()) {
            return &_tabTeam[team];
        }
        cerr << "TEAM invalide : " << team << endl;
    }

    void init();
    void maj();
};
Monde Monde::_m = Monde();

/****************************** Action ******************************/
class Action {
private :
    int _hero = 0;
public :
    void printResult();
};


/************************************************************/
/************************************************************/
/************************************************************/
/************************************************************/
/************************************************************/
/************************************************************/
/************************************************************/
/************************************************************/
/************************************************************/
/************************************************************/


/****************************** Hero ******************************/
Hero::Hero(int id, int x, int y, int attackRange, int health, int maxHealth, int shield, int attackDamage, int movementSpeed, int stunDuration, int goldValue,
           int countDown1, int countDown2, int countDown3, int mana, int maxMana, int manaRegen, string heroType, int isVisible, int itemOwned) :
        Unit(id, x, y, attackRange, health, maxHealth, shield, attackDamage, movementSpeed, stunDuration, goldValue),
        _countDown1(countDown1), _countDown2(countDown2), _countDown3(countDown3), _mana(mana), _maxMana(maxMana), _manaRegen(manaRegen), _heroType(heroType), _isVisible(isVisible), _itemOwned(itemOwned),
        _etat(DEFAULT), _order("WAIT")
{
    _baseAttackTime = 0.1;
}

Deadpool::Deadpool(int id, int x, int y) :
        Hero(id, x, y, 110, 1380, 1380, 0, 80, 200, 0, 300, 0, 0, 0, 100, 100, 1, "DEADPOOL", 1, 0)
{}

Hulk::Hulk(int id, int x, int y) :
        Hero(id, x, y, 95, 1450, 1450, 0, 80, 200, 0, 300, 0, 0, 0, 90, 90, 1, "HULK", 1, 0)
{}

Valkyrie::Valkyrie(int id, int x, int y) :
        Hero(id, x, y, 130, 1400, 1400, 0, 65, 200, 0, 300, 0, 0, 0, 155, 155, 2, "VALKYRIE", 1, 0)
{}

Ironman::Ironman(int id, int x, int y) :
        Hero(id, x, y, 270, 820, 820, 0, 60, 200, 0, 300, 0, 0, 0, 200, 200, 2, "IRONMAN", 1, 0)
{}

DoctorStrange::DoctorStrange(int id, int x, int y) :
        Hero(id, x, y, 245, 955, 955, 0, 50, 200, 0, 300, 0, 0, 0, 300, 300, 2, "DOCTOR_STRANGE", 1, 0)
{}

Sacoche& Hero::getSacoche() {return _sacoche;}
string Hero::getOrder() {return _order;}
void Hero::setOrder(string order) {_order = order;}

void Hero::trace() const {
    cerr << "id(" << _id << ") (" << _x << ", " << _y << ") dmg(" << _attackDamage << ") range(" << _attackRange << ") hp(" << _health << "/" << _maxHealth << ")";
    cerr << " mana(" << _mana << "/" << _maxMana << ", +" << _manaRegen << ") item(" << _itemOwned << ")";
    cerr << endl;
}

void Hero::maj(int id, int x, int y, int attackRange, int health, int maxHealth, int shield, int attackDamage, int movementSpeed, int stunDuration, int goldValue,
               int countDown1, int countDown2, int countDown3, int mana, int maxMana, int manaRegen, string heroType, int isVisible, int itemOwned)
{
    Unit::maj(id, x, y, attackRange, health, maxHealth, shield, attackDamage, movementSpeed, stunDuration, goldValue);
    _countDown1    = countDown1;
    _countDown2    = countDown2;
    _countDown3    = countDown3;
    _mana          = mana;
    _maxMana       = maxMana;
    _manaRegen     = manaRegen;
    _heroType      = heroType;
    _isVisible     = isVisible;
    _itemOwned     = itemOwned;

    _alive = true;
    _order = "WAIT";
}

void Hero::playSolo()
{
    //int tmp = Monde::get().getGold();
}

void Hero::retreatOrNot() {
    // Si on est au dela de notre unité la plus avancée, déclenche un rapprochement vers cette unité
    int minX = XMAX;
    int maxX = XMIN;

    Team *myTeam = Monde::get().getTeam(Monde::get().getMyTeam());
    for (int cpt = 0; cpt < myTeam->getTabUnits().size(); cpt++) {
        minX = min(minX, myTeam->getTabUnits()[cpt].getX());
        maxX = max(maxX, myTeam->getTabUnits()[cpt].getX());
    }
    if ((myTeam->getNum() == 0 && _x > maxX) || (myTeam->getNum() == 1 && _x < minX)) {
        moveToFriendlyUnitNearestFromEnnemyTower();
        return;
    }
}

// Rapprochement vers l'unité la plus avancée
void Hero::moveToFriendlyUnitNearestFromEnnemyTower() {
    int minX = XMAX, minY = _y;
    int maxX = XMIN, maxY = _y;
    Team *myTeam = Monde::get().getTeam(Monde::get().getMyTeam());
    for (int cpt = 0; cpt < myTeam->getTabUnits().size(); cpt++) {
        if (myTeam->getTabUnits()[cpt].getX() < minX) {
            minX = myTeam->getTabUnits()[cpt].getX();
            minY = myTeam->getTabUnits()[cpt].getY();
        }
        if (myTeam->getTabUnits()[cpt].getX() > maxX) {
            maxX = myTeam->getTabUnits()[cpt].getX();
            maxY = myTeam->getTabUnits()[cpt].getY();
        }
    }
    if (myTeam->getNum() == 0) {
        //_order = "MOVE " + to_string(maxX - 10) + " " + to_string(maxY);
        _order = "MOVE_ATTACK " + to_string(maxX - 10) + " " + to_string(maxY) + " " + to_string(findIdToAttackWhenMovingToPoint(Point(maxX - 10, maxY)));
    } else {
        //_order = "MOVE " + to_string(minX + 10) + " " + to_string(minY);
        _order = "MOVE_ATTACK " + to_string(minX + 10) + " " + to_string(minY) + " " + to_string(findIdToAttackWhenMovingToPoint(Point(minX + 10, minY)));
    }
}

int Hero::findIdOfNearestEnnemyFromPos(Point pos) {
    double minDist = XMAX;
    int bestId = 0;
    Team *otherTeam = Monde::get().getTeam(Monde::get().getOtherTeam());
    for (int cpt = 0; cpt < otherTeam->getTabTowers().size(); cpt++) {
        double tmp = otherTeam->getTabTowers()[cpt].dist(pos);
        if (tmp < minDist) {
            minDist = tmp;
            bestId  = otherTeam->getTabTowers()[cpt].getId();
        }
    }
    for (int cpt = 0; cpt < otherTeam->getTabUnits().size(); cpt++) {
        double tmp = otherTeam->getTabUnits()[cpt].dist(pos);
        if (tmp < minDist) {
            minDist = tmp;
            bestId  = otherTeam->getTabUnits()[cpt].getId();
        }
    }
    for (int cpt = 0; cpt < otherTeam->getTabHeros().size(); cpt++) {
        double tmp = otherTeam->getTabHeros()[cpt].dist(pos);
        if (tmp < minDist) {
            minDist = tmp;
            bestId  = otherTeam->getTabHeros()[cpt].getId();
        }
    }
    return bestId;
}

// Cherche une cible de faéon é faire de l'or ou l'empécher d'en faire
int Hero::findIdToAttackWhenMovingToPoint(Point pos) {
    double tmpDist = dist(pos);
    double timerRestant = 1 - (tmpDist / getMovementSpeed());

    // Possibilité de finir la tour à _lui ? une unité ? un héro ?
    Team *otherTeam = Monde::get().getTeam(Monde::get().getOtherTeam());
    for (int cpt = 0; cpt < otherTeam->getTabTowers().size(); cpt++) {
        if (_attackDamage < otherTeam->getTabTowers()[cpt].getHealth()) continue;
        if (calculeAttackTime(pos, otherTeam->getTabTowers()[cpt]) < timerRestant)
            return otherTeam->getTabTowers()[cpt].getId();
    }
    for (int cpt = 0; cpt < otherTeam->getTabHeros().size(); cpt++) {
        if (_attackDamage < otherTeam->getTabHeros()[cpt].getHealth()) continue;
        if (calculeAttackTime(pos, otherTeam->getTabHeros()[cpt]) < timerRestant) return otherTeam->getTabHeros()[cpt].getId();
    }
    for (int cpt = 0; cpt < otherTeam->getTabUnits().size(); cpt++) {
        if (_attackDamage < otherTeam->getTabUnits()[cpt].getHealth()) continue;
        if (calculeAttackTime(pos, otherTeam->getTabUnits()[cpt]) < timerRestant) return otherTeam->getTabUnits()[cpt].getId();
    }

    // Possibilité de l'empécher de me finir une unité ? un héo ?
    Team *myTeam = Monde::get().getTeam(Monde::get().getMyTeam());
    for (int cpt = 0; cpt < myTeam->getTabUnits().size(); cpt++) {
        if (_attackDamage < myTeam->getTabUnits()[cpt].getHealth()) continue;
        if (calculeAttackTime(pos, myTeam->getTabUnits()[cpt]) < timerRestant) return myTeam->getTabUnits()[cpt].getId();
    }
    for (int cpt = 0; cpt < myTeam->getTabHeros().size(); cpt++) {
        if (_id == myTeam->getTabHeros()[cpt].getId()) continue;
        if (_attackDamage < myTeam->getTabHeros()[cpt].getHealth()) continue;
        if (calculeAttackTime(pos, myTeam->getTabHeros()[cpt]) < timerRestant) return myTeam->getTabHeros()[cpt].getId();
    }

    return findIdOfNearestEnnemyFromPos(pos);
}

// Tente un achat de popo
void Hero::buyPotion() {
    int blessure = _maxHealth - _health;
    if (blessure < _health) return; // Encore + de 50% de vie
    if (blessure < 500) return;
    if (_sacoche.getNbPlaces() == 0) {
        // TODO vendre qqch ?
        return;
    }

    // Blessé é hauteur de 50% ou plus
    int bestHealth = 0;
    int bestIndice = -1;
    for (int cpt = 0; cpt < Monde::get().getTabPotions().size(); cpt++) {
        if (Monde::get().getTabPotions()[cpt].getHealth() > bestHealth && Monde::get().getGold() >= Monde::get().getTabPotions()[cpt].getItemCost()) {
            bestHealth = Monde::get().getTabPotions()[cpt].getHealth();
            bestIndice = cpt;
        }
    }
    if (bestIndice != -1) {
        _order = "BUY " + Monde::get().getTabPotions()[bestIndice].getItemName();
    }
}

// Tente un achat d'item
void Hero::buyItemDegats() {
    if (_sacoche.getNbPlaces() < 2) return;

    // On va prendre le meilleur ratio achetable
    Item *dmgBestRatioAchetable = NULL;
    for (int cpt = 0; cpt < Monde::get().getTabItems().size(); cpt++) {
        if (Monde::get().getTabItems()[cpt].getDamage() &&
            Monde::get().getGold() >= Monde::get().getTabItems()[cpt].getItemCost() &&
            (dmgBestRatioAchetable == NULL || Monde::get().getTabItems()[cpt].getRatioDamage() < dmgBestRatioAchetable->getRatioDamage())) {
            dmgBestRatioAchetable = &Monde::get().getTabItems()[cpt];
        }
    }
    if (dmgBestRatioAchetable != NULL) {
        _order = "BUY " + dmgBestRatioAchetable->getItemName();
    }
}

/****************************** Team ******************************/
void Team::addOrMajTower(int unitId, int x, int y, int attackRange, int health, int maxHealth, int shield, int attackDamage, int movementSpeed, int stunDuration, int goldValue) {
    for (int cpt = 0; cpt < _tabTowers.size(); cpt++) {
        if (_tabTowers[cpt].getId() == unitId) {
            _tabTowers[cpt].maj(unitId, x, y, attackRange, health, maxHealth, shield, attackDamage, movementSpeed, stunDuration, goldValue);
            return;
        }
    }
    // Si on est lé, c'est qu'on n'a pas trouvé l'unité
    _tabTowers.push_back(Unit(unitId, x, y, attackRange, health, maxHealth, shield, attackDamage, movementSpeed, stunDuration, goldValue));
}

void Team::addOrMajUnit(int unitId, int x, int y, int attackRange, int health, int maxHealth, int shield, int attackDamage, int movementSpeed, int stunDuration, int goldValue) {
    for (int cpt = 0; cpt < _tabUnits.size(); cpt++) {
        if (_tabUnits[cpt].getId() == unitId) {
            _tabUnits[cpt].maj(unitId, x, y, attackRange, health, maxHealth, shield, attackDamage, movementSpeed, stunDuration, goldValue);
            return;
        }
    }
    // Si on est lé, c'est qu'on n'a pas trouvé l'unité
    _tabUnits.push_back(Unit(unitId, x, y, attackRange, health, maxHealth, shield, attackDamage, movementSpeed, stunDuration, goldValue));
}

void Team::addOrMajHero(int unitId, int x, int y, int attackRange, int health, int maxHealth, int shield, int attackDamage, int movementSpeed, int stunDuration, int goldValue,
                        int countDown1, int countDown2, int countDown3, int mana, int maxMana, int manaRegeneration, string heroType, int isVisible, int itemsOwned) {
    for (int cpt = 0; cpt < _tabHeros.size(); cpt++) {
        if (_tabHeros[cpt].getId() == unitId) {
            _tabHeros[cpt].maj(unitId, x, y, attackRange, health, maxHealth, shield, attackDamage, movementSpeed, stunDuration, goldValue, countDown1, countDown2, countDown3, mana, maxMana, manaRegeneration, heroType, isVisible, itemsOwned);
            return;
        }
    }
    // Si on est là, c'est qu'on n'a pas trouvé l'unité
    if (heroType == "DEADPOOL") {
        _tabHeros.push_back(Deadpool(unitId, x, y));
    }
    else if (heroType == "HULK") {
        _tabHeros.push_back(Hulk(unitId, x, y));
    }
    else if (heroType == "VALKYRIE") {
        _tabHeros.push_back(Valkyrie(unitId, x, y));
    }
    else if (heroType == "IRONMAN") {
        _tabHeros.push_back(Ironman(unitId, x, y));
    }
    else if (heroType == "DOCTOR_STRANGE") {
        _tabHeros.push_back(DoctorStrange(unitId, x, y));
    }
}

void Team::trace() const {
    cerr << "TEAM " << _num << " :" << endl;
    for (int cpt = 0; cpt < _tabTowers.size(); cpt++) {_tabTowers[cpt].trace();}
    for (int cpt = 0; cpt < _tabUnits.size(); cpt++)  {_tabUnits[cpt].trace();}
    for (int cpt = 0; cpt < _tabHeros.size(); cpt++)  {_tabHeros[cpt].trace();}
}

void Team::clean() {
    for (int cpt = _tabUnits.size() - 1; cpt >= 0; cpt--) {
        if (_tabUnits[cpt].getAlive() == false) {
            _tabUnits.erase(_tabUnits.begin() + cpt);
            continue;
        }
        // maj pour le prochain tour
        _tabUnits[cpt].setAlive(false);
    }
    for (int cpt = _tabHeros.size() - 1; cpt >= 0; cpt--) {
        if (_tabHeros[cpt].getAlive() == false) {
            _tabHeros.erase(_tabHeros.begin() + cpt);
            continue;
        }
        _tabHeros[cpt].setAlive(false);
    }
}

void Team::addOrMajEntity(int unitId, string unitType, int x, int y, int attackRange, int health, int maxHealth, int shield, int attackDamage, int movementSpeed, int stunDuration, int goldValue,
                          int countDown1, int countDown2, int countDown3, int mana, int maxMana, int manaRegeneration, string heroType, int isVisible, int itemsOwned) {
    if (unitType == "UNIT") {
        addOrMajUnit(unitId, x, y, attackRange, health, maxHealth, shield, attackDamage, movementSpeed, stunDuration, goldValue);
    } else if (unitType == "HERO") {
        addOrMajHero(unitId, x, y, attackRange, health, maxHealth, shield, attackDamage, movementSpeed, stunDuration, goldValue, countDown1, countDown2, countDown3, mana, maxMana, manaRegeneration, heroType, isVisible, itemsOwned);
    } else if (unitType == "TOWER") {
        addOrMajTower(unitId, x, y, attackRange, health, maxHealth, shield, attackDamage, movementSpeed, stunDuration, goldValue);
    } else {
        cerr << "ERREUR Team.addOrMajEntity (unitType=" << unitType << ")" << endl;
    }
}

/****************************** Monde ******************************/
void Monde::init() {
    int myTeam;
    cin >> _myTeam; cin.ignore();
    _otherTeam = 1 - _myTeam;

    // On récupére Bush et spawnPoints
    int bushAndSpawnPointCount; // usefrul from wood1, represents the number of bushes and the number of places where neutral units can spawn
    cin >> bushAndSpawnPointCount; cin.ignore();
    for (int i = 0; i < bushAndSpawnPointCount; i++) {
        string entityType; // BUSH, from wood1 it can also be SPAWN
        int x;
        int y;
        int radius;
        cin >> entityType >> x >> y >> radius; cin.ignore();
        cerr << entityType << " (" << x << ", " << y << ") radius(" << radius << ")" << endl;
        if (entityType == "BUSH") {
            _tabBush.push_back(Bush(x, y, radius));
        } else if (entityType == "SPAWN") {
            _tabSpawnPoints.push_back(Point(x, y));
        } else {
            cerr << "ERREUR init Monde, entityType=" << entityType << endl;
        }
    }

    // On récupére les items
    int itemCount; // useful from wood2
    cin >> itemCount; cin.ignore();
    for (int i = 0; i < itemCount; i++) {
        string itemName; // contains keywords such as BRONZE, SILVER and BLADE, BOOTS connected by "_" to help you sort easier
        int itemCost; // BRONZE items have lowest cost, the most expensive items are LEGENDARY
        int damage; // keyword BLADE is present if the most important item stat is damage
        int health;
        int maxHealth;
        int mana;
        int maxMana;
        int moveSpeed; // keyword BOOTS is present if the most important item stat is moveSpeed
        int manaRegeneration;
        int isPotion; // 0 if it's not instantly consumed
        cin >> itemName >> itemCost >> damage >> health >> maxHealth >> mana >> maxMana >> moveSpeed >> manaRegeneration >> isPotion; cin.ignore();
        if (isPotion) {
            _tabPotions.push_back(Item(itemName, itemCost, damage, health, maxHealth, mana, maxMana, moveSpeed, manaRegeneration));
        } else {
            _tabStuff.push_back(Item(itemName, itemCost, damage, health, maxHealth, mana, maxMana, moveSpeed, manaRegeneration));
        }
    }
    cerr << itemCount << " items :" << endl;
    for (int cpt = 0; cpt < _tabPotions.size(); cpt++) {_tabPotions[cpt].trace("potion");}
    cerr << endl;
    for (int cpt = 0; cpt < _tabStuff.size(); cpt++) {_tabStuff[cpt].trace("Gadget");}
    cerr << endl;
    for (int cpt = 0; cpt < _tabStuff.size(); cpt++) {_tabStuff[cpt].trace("Boots");}
    cerr << endl;
    for (int cpt = 0; cpt < _tabStuff.size(); cpt++) {_tabStuff[cpt].trace("Blade");}
}

void Monde::maj() {
    cin >> _gold; cin.ignore();
    cin >> _enemyGold; cin.ignore();
    cin >> _roundType; cin.ignore();

    int entityCount;
    cin >> entityCount; cin.ignore();
    for (int i = 0; i < entityCount; i++) {
        int unitId;
        int team;
        string unitType; // UNIT, HERO, TOWER, can also be GROOT from wood1
        int x;
        int y;
        int attackRange;
        int health;
        int maxHealth;
        int shield; // useful in bronze
        int attackDamage;
        int movementSpeed;
        int stunDuration; // useful in bronze
        int goldValue;
        int countDown1; // all countDown and mana variables are useful starting in bronze
        int countDown2;
        int countDown3;
        int mana;
        int maxMana;
        int manaRegeneration;
        string heroType; // DEADPOOL, VALKYRIE, DOCTOR_STRANGE, HULK, IRONMAN
        int isVisible; // 0 if it isn't
        int itemsOwned; // useful from wood1
        cin >> unitId >> team >> unitType >> x >> y >> attackRange >> health >> maxHealth >> shield >> attackDamage >> movementSpeed >> stunDuration >> goldValue >> countDown1 >> countDown2 >> countDown3 >> mana >> maxMana >> manaRegeneration >> heroType >> isVisible >> itemsOwned; cin.ignore();

        if (team == 0 || team == 1) {
            _tabTeam[team].addOrMajEntity(unitId, unitType, x, y, attackRange, health, maxHealth, shield, attackDamage, movementSpeed, stunDuration, goldValue,
                                  countDown1, countDown2, countDown3, mana, maxMana, manaRegeneration, heroType, isVisible, itemsOwned);
        } else if (team == -1) {
            bool trouve = false;
            for (int cpt = 0; cpt < _tabGroots.size(); cpt++) {
                if (_tabGroots[cpt].getId() == unitId) {
                    _tabGroots[cpt].maj(unitId, x, y, attackRange, health, maxHealth, shield, attackDamage, movementSpeed, stunDuration, goldValue);
                    trouve = true;
                    break;
                }
            }
            if (!trouve) {
                // Si pas trouvé, on le crée
                _tabGroots.push_back(Hero(unitId, x, y, attackRange, health, maxHealth, shield, attackDamage, movementSpeed, stunDuration, goldValue, countDown1, countDown2, countDown3, mana, maxMana, manaRegeneration, heroType, isVisible, itemsOwned));
            }
        } else {
            cerr << "ERREUR Monde.maj (team = " << team << ") unitType=" << unitType << ")" << endl;
        }
    }
    for (int cpt = 0; cpt < _tabTeam.size(); cpt++) {
        _tabTeam[cpt].clean();
        _tabTeam[cpt].trace();
    }
    cerr << "NEUTRES :" << endl;
    for (int cpt = 0; cpt < _tabGroots.size(); cpt++) {
        _tabGroots[cpt].trace();
    }
}

/****************************** Action ******************************/
void Action::printResult() {
    if (Monde::get().getRoundType() < 0) {
        if (!_hero++) {
            cout << "IRONMAN" << endl;
        } else {
            cout << "DOCTOR_STRANGE" << endl;
        }
    } else {
        Team *myTeam = Monde::get().getTeam(Monde::get().getMyTeam());
        for (int cpt = 0; cpt < myTeam->getTabHeros().size(); cpt++) {
            if (myTeam->getTabHeros()[cpt].getOrder() == "WAIT") {
                myTeam->getTabHeros()[cpt].retreatOrNot();
            }
            if (myTeam->getTabHeros()[cpt].getOrder() == "WAIT") {
                myTeam->getTabHeros()[cpt].buyPotion();
            }
            if (myTeam->getTabHeros()[cpt].getOrder() == "WAIT") {
                //buyItemDegats(_moi->getTabHeros()[cpt]);
            }
            if (myTeam->getTabHeros()[cpt].getOrder() == "WAIT") {
                myTeam->getTabHeros()[cpt].moveToFriendlyUnitNearestFromEnnemyTower();
            }
            cout << myTeam->getTabHeros()[cpt].getOrder() << endl;
        }
    }
}


/**
 * Made with love by AntiSquid, Illedan and Wildum.
 * You can help children learn to code while you participate by donating to CoderDojo.
 **/
int main()
{
    Action action = Action();

    Monde::get().init();

    // game loop
    while (1) {
        Monde::get().maj();

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

        // If roundType has a negative value then you need to output a Hero name, such as "DEADPOOL" or "VALKYRIE".
        // Else you need to output roundType number of any valid action, such as "WAIT" or "ATTACK unitId"
        action.printResult();
    }
}

