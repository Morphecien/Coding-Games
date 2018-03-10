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
    Item(string itemName, int itemCost, int damage, int health, int maxHealth, int mana, int maxMana, int moveSpeed, int manaRegen);
    void trace(string type) const;
    string getItemName();
    int getItemCost();
    int getDamage();
    int getHealth();
    int getMana();
    int getManaRegen();
    int getMoveSpeed();
    int getRatioDamage();
    int getRatioHealth();
};

/****************************** Sacoche ******************************/
class Sacoche {
private :
    static const int _maxItems = 4;
    vector <Item> _items;

public :
    int getNbPlaces();
    void addItem(Item &item);
    void removeItem(string nomItem);
};

/****************************** Point ******************************/
class Point {
protected :
    int _x;
    int _y;

public :
    Point(int x, int y);
    int getX();
    int getY();
    double dist(Point &p) const;
    bool isInRange(Point &p, int range);
};

/****************************** Circle ******************************/
class Circle : public Point {
protected :
    int _radius;

public :
    Circle(int x, int y, int radius);
};

/****************************** Bush ******************************/
class Bush : public Circle {
public :
    Bush(int x, int y, int radius);
};

/****************************** Unit ******************************/
class Unit : public Point {
protected :
    // Donn�es en entr�e
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
    Unit(int id, int x, int y, int attackRange, int health, int maxHealth, int shield, int attackDamage, int movementSpeed, int stunDuration, int goldValue);
    int getId();
    bool getAlive();
    void setAlive(bool alive);
    int getHealth();
    int getMaxHealth();
    int getDamage();
    int getMovementSpeed();
    void trace() const;
    void maj(int id, int x, int y, int attackRange, int health, int maxHealth, int shield, int attackDamage, int movementSpeed, int stunDuration, int goldValue);
    bool isInRange(Point &p);
    float calculeAttackTime(Point cible);
    float calculeAttackTime(Point p1, Point p2);
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
    Team(int num);
    int getNum();
    vector <Unit>& getTabTowers();
    vector <Unit>& getTabUnits();
    vector <Hero>& getTabHeros();
    void trace() const;
    void clean();
    void addOrMajEntity(int unitId, string unitType, int x, int y, int attackRange, int health, int maxHealth, int shield, int attackDamage, int movementSpeed, int stunDuration, int goldValue,
                        int countDown1, int countDown2, int countDown3, int mana, int maxMana, int manaRegeneration, string heroType, int isVisible, int itemsOwned);
};

/****************************** Monde ******************************/
class Monde {
private :
    static Monde _m;
    Monde();
    ~Monde();

    vector <Bush> _tabBush;
    vector <Point> _tabSpawnPoints;
    vector <Item> _tabPotions;
    vector <Item> _tabStuff;
    vector <Unit> _tabGroots;
    Team _team0 = Team(0);
    Team _team1 = Team(1);

    int _gold;
    int _enemyGold;
    int _roundType;

public :
    static Monde& get();
    int getRoundType();
    int getGold();
    vector <Item>& getTabPotions();
    vector <Item>& getTabItems();
    Team* getTeam0();
    Team* getTeam1();

    void init();
    void maj();
};
Monde Monde::_m = Monde();

/****************************** Action ******************************/
class Action {
private :
    int _myTeam;
    Team *_moi;
    Team *_lui;

    void retreatOrNot(Hero &hero);
    void simpleAttackAttempt(Hero &hero);
    void moveToFriendlyUnitNearestFromEnnemyTower(Hero &hero);
    int findIdOfNearestEnnemyFromPos(Point pos);
    int findIdToAttackWhenMovingToPoint(Hero &hero, Point pos);
    void buyPotion(Hero &hero);
    void buyItemDegats(Hero &hero);

public :
    Action(int myTeam);
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


/****************************** Item ******************************/
Item::Item(string itemName, int itemCost, int damage, int health, int maxHealth, int mana, int maxMana, int moveSpeed, int manaRegen) :
        _itemName(itemName), _itemCost(itemCost), _damage(damage), _health(health), _maxHealth(maxHealth), _mana(mana), _maxMana(maxMana), _moveSpeed(moveSpeed), _manaRegen(manaRegen)
{
    _ratioDamage = _damage ? (float) _itemCost / (float) _damage : 0;
    _ratioHealth = _health ? (float) _itemCost / (float) _health : 0;
}

void Item::trace(string type) const {
    size_t found = _itemName.find(type);
    if (found != string::npos) {
        cerr << setw(20) << left << _itemName << " cout(" << setw(4) << _itemCost << ") dmg(" << _damage << ") hp(" << _health << "/" << _maxHealth << ") mana(" << _mana << "/" << _maxMana << ", +" << _manaRegen << ") move(" << _moveSpeed << ")";
        cerr << " ratios(dmg=" << _ratioDamage << " hp=" << _ratioHealth << ")";
        cerr << endl;
    }
}

string Item::getItemName() {return _itemName;};
int Item::getItemCost() {return _itemCost;};
int Item::getDamage() {return _damage;};
int Item::getHealth() {return _health;};
int Item::getMana() {return _mana;};
int Item::getManaRegen() {return _manaRegen;};
int Item::getMoveSpeed() {return _moveSpeed;};
int Item::getRatioDamage() {return _ratioDamage;};
int Item::getRatioHealth() {return _ratioHealth;};


/****************************** Sacoche ******************************/
int  Sacoche::getNbPlaces() {return max(0, _maxItems - _items.size());}

void Sacoche::addItem(Item &item) {
    if (_items.size() == _maxItems) return;
    _items.push_back(item);
}

void Sacoche::removeItem(string nomItem) {
    for (int cpt = 0; cpt < _items.size(); cpt++) {
        if (_items[cpt].getItemName() == nomItem) {
            _items.erase(_items.begin() + cpt);
            return;
        }
    }
}


/****************************** Point ******************************/
Point::Point(int x, int y) : _x(x), _y(y) {}
int Point::getX() {return _x;}
int Point::getY() {return _y;}

double Point::dist(Point &p) const {
    return sqrt((_x - p._x) * (_x - p._x) + (_y - p._y) * (_y - p._y));
}

bool Point::isInRange(Point &p, int range) {
    return (dist(p) <= range);
}

/****************************** Circle ******************************/
Circle::Circle(int x, int y, int radius) : Point(x, y), _radius(radius) {}

/****************************** Bush ******************************/
Bush::Bush(int x, int y, int radius) : Circle(x, y, radius) {}

/****************************** Unit ******************************/
Unit::Unit(int id, int x, int y, int attackRange, int health, int maxHealth, int shield, int attackDamage, int movementSpeed, int stunDuration, int goldValue) :
        _id(id), Point(x, y), _attackRange(attackRange), _health(health), _maxHealth(maxHealth), _shield(shield), _attackDamage(attackDamage), _movementSpeed(movementSpeed), _stunDuration(stunDuration), _goldValue(goldValue),
        _alive(true), _baseAttackTime(0.2)
{}

int Unit::getId() {return _id;}
bool Unit::getAlive() {return _alive;}
void Unit::setAlive(bool alive) {_alive = alive;}
int Unit::getHealth() {return _health;}
int Unit::getMaxHealth() {return _maxHealth;}
int Unit::getDamage() { return _attackDamage;}
int Unit::getMovementSpeed() {return _movementSpeed;}

void Unit::trace() const {
    cerr << "id(" << _id << ") (" << _x << ", " << _y << ") dmg(" << _attackDamage << ") range(" << _attackRange << ") hp(" << _health << "/" << _maxHealth << ") value(" << _goldValue << ")";
    cerr << endl;
}

void Unit::maj(int id, int x, int y, int attackRange, int health, int maxHealth, int shield, int attackDamage, int movementSpeed, int stunDuration, int goldValue)
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

bool Unit::isInRange(Point &p) {
    return Point::isInRange(p, _attackRange);
}

float Unit::calculeAttackTime(Point cible) {
    if (_attackRange < ATTACK_RANGE_MELEE) return _baseAttackTime;
    return calculeAttackTime(Point(_x, _y), cible);
}

float Unit::calculeAttackTime(Point p1, Point p2) {
    if (_attackRange < ATTACK_RANGE_MELEE) return _baseAttackTime;
    return (_baseAttackTime * (1.0 + p1.dist(p2) / _attackRange));
}

/****************************** Hero ******************************/
Hero::Hero(int id, int x, int y, int attackRange, int health, int maxHealth, int shield, int attackDamage, int movementSpeed, int stunDuration, int goldValue,
           int countDown1, int countDown2, int countDown3, int mana, int maxMana, int manaRegen, string heroType, int isVisible, int itemOwned) :
        Unit(id, x, y, attackRange, health, maxHealth, shield, attackDamage, movementSpeed, stunDuration, goldValue),
        _countDown1(countDown1), _countDown2(countDown2), _countDown3(countDown3), _mana(mana), _maxMana(maxMana), _manaRegen(manaRegen), _heroType(heroType), _isVisible(isVisible), _itemOwned(itemOwned),
        _etat(DEFAULT), _order("WAIT")
{
    _baseAttackTime = 0.1;
}

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

void playSolo()
{
    //int tmp = Monde::get().getGold();
}

/****************************** Team ******************************/
void Team::addOrMajTower(int unitId, int x, int y, int attackRange, int health, int maxHealth, int shield, int attackDamage, int movementSpeed, int stunDuration, int goldValue) {
    for (int cpt = 0; cpt < _tabTowers.size(); cpt++) {
        if (_tabTowers[cpt].getId() == unitId) {
            _tabTowers[cpt].maj(unitId, x, y, attackRange, health, maxHealth, shield, attackDamage, movementSpeed, stunDuration, goldValue);
            return;
        }
    }
    // Si on est l�, c'est qu'on n'a pas trouv� l'unit�
    _tabTowers.push_back(Unit(unitId, x, y, attackRange, health, maxHealth, shield, attackDamage, movementSpeed, stunDuration, goldValue));
}

void Team::addOrMajUnit(int unitId, int x, int y, int attackRange, int health, int maxHealth, int shield, int attackDamage, int movementSpeed, int stunDuration, int goldValue) {
    for (int cpt = 0; cpt < _tabUnits.size(); cpt++) {
        if (_tabUnits[cpt].getId() == unitId) {
            _tabUnits[cpt].maj(unitId, x, y, attackRange, health, maxHealth, shield, attackDamage, movementSpeed, stunDuration, goldValue);
            return;
        }
    }
    // Si on est l�, c'est qu'on n'a pas trouv� l'unit�
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
    // Si on est l�, c'est qu'on n'a pas trouv� l'unit�
    _tabHeros.push_back(Hero(unitId, x, y, attackRange, health, maxHealth, shield, attackDamage, movementSpeed, stunDuration, goldValue, countDown1, countDown2, countDown3, mana, maxMana, manaRegeneration, heroType, isVisible, itemsOwned));
}

Team::Team(int num) : _num(num) {}

int Team::getNum() {return _num;}
vector <Unit>& Team::getTabTowers() {return _tabTowers;}
vector <Unit>& Team::getTabUnits() {return _tabUnits;}
vector <Hero>& Team::getTabHeros() {return _tabHeros;}

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
Monde::Monde() {}
Monde::~Monde() {}

Monde& Monde::get() {
    return _m;
}

int Monde::getRoundType() {return _roundType;}
int Monde::getGold() {return _gold;}
vector <Item>& Monde::getTabPotions() {return _tabPotions;}
vector <Item>& Monde::getTabItems() {return _tabStuff;}
Team* Monde::getTeam0() {return &_team0;}
Team* Monde::getTeam1() {return &_team1;}

void Monde::init() {
    // On r�cup�re Bush et spawnPoints
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
            cerr << "ERREUR init Monde" << endl;
        }
    }

    // On r�cup�re les items
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

        if (team == 0) {
            _team0.addOrMajEntity(unitId, unitType, x, y, attackRange, health, maxHealth, shield, attackDamage, movementSpeed, stunDuration, goldValue,
                                  countDown1, countDown2, countDown3, mana, maxMana, manaRegeneration, heroType, isVisible, itemsOwned);
        } else if (team == 1) {
            _team1.addOrMajEntity(unitId, unitType, x, y, attackRange, health, maxHealth, shield, attackDamage, movementSpeed, stunDuration, goldValue,
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
                // Si pas trouv�, on le cr�e
                _tabGroots.push_back(Hero(unitId, x, y, attackRange, health, maxHealth, shield, attackDamage, movementSpeed, stunDuration, goldValue, countDown1, countDown2, countDown3, mana, maxMana, manaRegeneration, heroType, isVisible, itemsOwned));
            }
        } else {
            cerr << "ERREUR Monde.maj (team = " << team << ") unitType=" << unitType << ")" << endl;
        }
    }
    _team0.clean();
    _team1.clean();
    _team0.trace();
    _team1.trace();
    cerr << "NEUTRES :" << endl;
    for (int cpt = 0; cpt < _tabGroots.size(); cpt++) {
        _tabGroots[cpt].trace();
    }
}

/****************************** Action ******************************/
void Action::retreatOrNot(Hero &hero) {
    // Si on est au dela de notre unit� la plus avanc�e, d�clenche un rapprochement vers cette unit�
    int minX = XMAX;
    int maxX = XMIN;
    for (int cpt = 0; cpt < _moi->getTabUnits().size(); cpt++) {
        minX = min(minX, _moi->getTabUnits()[cpt].getX());
        maxX = max(maxX, _moi->getTabUnits()[cpt].getX());
    }
    if ((_myTeam == 0 && hero.getX() > maxX) || (_myTeam == 1 && hero.getX() < minX)) {
        moveToFriendlyUnitNearestFromEnnemyTower(hero);
        return;
    }
}

void Action::simpleAttackAttempt(Hero &hero) {
    for (int cpt = 0; cpt < _lui->getTabTowers().size(); cpt++) {
        if (hero.isInRange(_lui->getTabTowers()[cpt])) {
            hero.setOrder("ATTACK_NEAREST TOWER");
            return;
        }
    }
    for (int cpt = 0; cpt < _lui->getTabUnits().size(); cpt++) {
        if (hero.isInRange(_lui->getTabUnits()[cpt])) {
            hero.setOrder("ATTACK_NEAREST UNIT");
            return;
        }
    }
    for (int cpt = 0; cpt < _lui->getTabHeros().size(); cpt++) {
        if (hero.isInRange(_lui->getTabHeros()[cpt])) {
            hero.setOrder("ATTACK_NEAREST HERO");
            return;
        }
    }
}

// Rapprochement vers l'unit� la plus avanc�e
void Action::moveToFriendlyUnitNearestFromEnnemyTower(Hero &hero) {
    int minX = XMAX, minY = hero.getY();
    int maxX = XMIN, maxY = hero.getY();
    for (int cpt = 0; cpt < _moi->getTabUnits().size(); cpt++) {
        if (_moi->getTabUnits()[cpt].getX() < minX) {
            minX = _moi->getTabUnits()[cpt].getX();
            minY = _moi->getTabUnits()[cpt].getY();
        }
        if (_moi->getTabUnits()[cpt].getX() > maxX) {
            maxX = _moi->getTabUnits()[cpt].getX();
            maxY = _moi->getTabUnits()[cpt].getY();
        }
    }
    if (_myTeam == 0) {
        //_order = "MOVE " + to_string(maxX - 10) + " " + to_string(maxY);
        hero.setOrder("MOVE_ATTACK " + to_string(maxX - 10) + " " + to_string(maxY) + " " + to_string(findIdToAttackWhenMovingToPoint(hero, Point(maxX - 10, maxY))));
    } else {
        //_order = "MOVE " + to_string(minX + 10) + " " + to_string(minY);
        hero.setOrder("MOVE_ATTACK " + to_string(minX + 10) + " " + to_string(minY) + " " + to_string(findIdToAttackWhenMovingToPoint(hero, Point(minX + 10, minY))));
    }
}

int Action::findIdOfNearestEnnemyFromPos(Point pos) {
    double minDist = XMAX;
    int bestId = 0;
    for (int cpt = 0; cpt < _lui->getTabTowers().size(); cpt++) {
        double tmp = _lui->getTabTowers()[cpt].dist(pos);
        if (tmp < minDist) {
            minDist = tmp;
            bestId  = _lui->getTabTowers()[cpt].getId();
        }
    }
    for (int cpt = 0; cpt < _lui->getTabUnits().size(); cpt++) {
        double tmp = _lui->getTabUnits()[cpt].dist(pos);
        if (tmp < minDist) {
            minDist = tmp;
            bestId  = _lui->getTabUnits()[cpt].getId();
        }
    }
    for (int cpt = 0; cpt < _lui->getTabHeros().size(); cpt++) {
        double tmp = _lui->getTabHeros()[cpt].dist(pos);
        if (tmp < minDist) {
            minDist = tmp;
            bestId  = _lui->getTabHeros()[cpt].getId();
        }
    }
    return bestId;
}

// Cherche une cible de fa�on � faire de l'or ou l'emp�cher d'en faire
int Action::findIdToAttackWhenMovingToPoint(Hero &hero, Point pos) {
    double tmpDist = hero.dist(pos);
    double timerRestant = 1 - (tmpDist / hero.getMovementSpeed());

    // Possibilit� de finir la tour � _lui ? une unit� ? un h�ro ?
    for (int cpt = 0; cpt < _lui->getTabTowers().size(); cpt++) {
        if (hero.getDamage() < _lui->getTabTowers()[cpt].getHealth()) continue;
        if (hero.calculeAttackTime(pos, _lui->getTabTowers()[cpt]) < timerRestant) return _lui->getTabTowers()[cpt].getId();
    }
    for (int cpt = 0; cpt < _lui->getTabUnits().size(); cpt++) {
        if (hero.getDamage() < _lui->getTabUnits()[cpt].getHealth()) continue;
        if (hero.calculeAttackTime(pos, _lui->getTabUnits()[cpt]) < timerRestant) return _lui->getTabUnits()[cpt].getId();
    }
    for (int cpt = 0; cpt < _lui->getTabHeros().size(); cpt++) {
        if (hero.getDamage() < _lui->getTabHeros()[cpt].getHealth()) continue;
        if (hero.calculeAttackTime(pos, _lui->getTabHeros()[cpt]) < timerRestant) return _lui->getTabHeros()[cpt].getId();
    }

    // Possibilit� de l'emp�cher de me finir une unit� ? un h�o ?
    for (int cpt = 0; cpt < _moi->getTabUnits().size(); cpt++) {
        if (hero.getDamage() < _moi->getTabUnits()[cpt].getHealth()) continue;
        if (hero.calculeAttackTime(pos, _moi->getTabUnits()[cpt]) < timerRestant) return _moi->getTabUnits()[cpt].getId();
    }
    for (int cpt = 0; cpt < _moi->getTabHeros().size(); cpt++) {
        if (hero.getId() == _moi->getTabHeros()[cpt].getId()) continue;
        if (hero.getDamage() < _moi->getTabHeros()[cpt].getHealth()) continue;
        if (hero.calculeAttackTime(pos, _moi->getTabHeros()[cpt]) < timerRestant) return _moi->getTabHeros()[cpt].getId();
    }

    return findIdOfNearestEnnemyFromPos(pos);
}

// Tente un achat de popo
void Action::buyPotion(Hero &hero) {
    int blessure = hero.getMaxHealth() - hero.getHealth();
    if (blessure < hero.getHealth()) return; // Encore + de 50% de vie
    if (blessure < 500) return;
    if (hero.getSacoche().getNbPlaces() == 0) {
        // TODO vendre qqch ?
        return;
    }

    // Bless� � hauteur de 50% ou plus
    int bestHealth = 0;
    int bestIndice = -1;
    for (int cpt = 0; cpt < Monde::get().getTabPotions().size(); cpt++) {
        if (Monde::get().getTabPotions()[cpt].getHealth() > bestHealth && Monde::get().getGold() >= Monde::get().getTabPotions()[cpt].getItemCost()) {
            bestHealth = Monde::get().getTabPotions()[cpt].getHealth();
            bestIndice = cpt;
        }
    }
    if (bestIndice != -1) {
        hero.setOrder("BUY " + Monde::get().getTabPotions()[bestIndice].getItemName());
    }
}

// Tente un achat d'item
void Action::buyItemDegats(Hero &hero) {
    if (hero.getSacoche().getNbPlaces() < 2) return;

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
        hero.setOrder("BUY " + dmgBestRatioAchetable->getItemName());
    }
}

Action::Action(int myTeam) : _myTeam(myTeam)
{
    if (_myTeam == 0) {
        _moi = Monde::get().getTeam0();
        _lui = Monde::get().getTeam1();
    } else {
        _moi = Monde::get().getTeam1();
        _lui = Monde::get().getTeam0();
    }
}

void Action::printResult() {
    if (Monde::get().getRoundType() < 0) {
        cout << "WAIT" << endl;
    } else {
        for (int cpt = 0; cpt < _moi->getTabHeros().size(); cpt++) {
            if (_moi->getTabHeros()[cpt].getOrder() == "WAIT") {
                retreatOrNot(_moi->getTabHeros()[cpt]);
            }
            if (_moi->getTabHeros()[cpt].getOrder() == "WAIT") {
                buyPotion(_moi->getTabHeros()[cpt]);
            }
            if (_moi->getTabHeros()[cpt].getOrder() == "WAIT") {
                //buyItemDegats(_moi->getTabHeros()[cpt]);
            }
            if (_moi->getTabHeros()[cpt].getOrder() == "WAIT") {
                //simpleAttackAttempt(_moi->getTabHeros()[cpt]);
            }
            if (_moi->getTabHeros()[cpt].getOrder() == "WAIT") {
                moveToFriendlyUnitNearestFromEnnemyTower(_moi->getTabHeros()[cpt]);
            }
            cout << _moi->getTabHeros()[cpt].getOrder() << endl;
        }
    }
}


/**
 * Made with love by AntiSquid, Illedan and Wildum.
 * You can help children learn to code while you participate by donating to CoderDojo.
 **/
int main()
{
    int myTeam;
    cin >> myTeam; cin.ignore();
    Action action = Action(myTeam);

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

