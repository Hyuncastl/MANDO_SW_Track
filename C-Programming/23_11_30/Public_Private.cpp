#include<iostream>

using namespace std;

class Character{
    private:
        int hp;
        int speed;
        void speedUp();
    public:
        string name;
        void move();
        void jump();
        void hit();


        void obtainItem();
        void setHp(int _hp);
        int getHp();
};
void Character::speedUp(){
    speed++;
}
void Character::obtainItem(){
    speedUp();
}
void Character::setHp(int _hp){
    hp = _hp;
}
int Character::getHp(){
    return hp;
}


int main(){

    Character mario;
    
    mario.name = "슈퍼마리오";
    mario.obtainItem();
    //mario.speed += 10000;
    mario.setHp(100);
    cout << mario.getHp() <<endl;

    return 0;
}
