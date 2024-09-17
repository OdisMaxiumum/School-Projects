
/*************************
 * Student Author Name: Oday Suleiman
 
 * ***********************/




#include <string>
#include <iostream>
#include <stdlib.h> 
#include <stdio.h>
#include <iomanip>
#include <vector>

using namespace std;



//#################################################################################################



class Dice{
	private:
		int value;
	
	public:

		//Randomly assigns a value between from 1 to 6 to the dice.
		void roll();
		int reveal() {return value;}
		Dice();
		//The autograder will set the values directly instead of calling roll() to avoid randomness!
		void setValue(int value) {this->value = value;}

};

void Dice:: roll(){
	value = (rand() % 6) + 1;
}
Dice:: Dice(){
	roll();
}
//#################################################################################################


const int HAND_SIZE = 5;

class Hand {
	public:

		//Display the value of the five dice
		void show();	
		void play();
		Dice* getDice(int diceNum);
		
		//selection is the string of dice numbers that the player wants to keep.
		//For example, "125" means that player wants to keep the first, second, and fifth dice, and roll the rest.
		void setSelection(string selection);	
		void resetHand();
		Hand();	
	
	private:
		vector<Dice*> curr_Hand;
		bool* reroll = new bool[5];
};

Hand ::Hand(){
	for (int i = 0; i < HAND_SIZE; i++){
		Dice* dice = new Dice();
		this->curr_Hand.push_back(dice);
	}
}

void Hand::resetHand(){
	for (int i = 0; i < HAND_SIZE; i++){
		this->curr_Hand.at(i)->roll();
	}
}

void Hand::show(){
	cout << "Hand: "; 
	for (int i = 0; i < HAND_SIZE; i++){
		cout << curr_Hand.at(i)->reveal() << " ";
	}	
	cout << endl;
}


Dice* Hand::getDice(int diceNum){
	return this->curr_Hand.at(diceNum);
}
void Hand::play(){
	for(int i = 0; i < HAND_SIZE; i++){
		if(reroll[i]){
			this->curr_Hand.at(i)->roll();
		}
	}
}
void Hand::setSelection(string selection){
	for(int i = 0; i < HAND_SIZE; i++){
        this->reroll[i] = true;
    }
    for(int i = 0; i < selection.size(); i++){
        int hand_ind = selection.at(i) - 49;
        this->reroll[hand_ind] = false;
    }
}

//######################################################################


//List of rows in the board
const int ONES = 0;
const int TWOS = 1;
const int THREES = 2;
const int FOURS = 3;
const int FIVES = 4;
const int SIXES = 5;
const int THREE_OF_KIND = 6;
const int FOUR_OF_KIND = 7;
const int FULL_HOUSE = 8;
const int SMALL_STRAIGHT = 9;
const int LARGE_STRAIGHT = 10;
const int CHANCE = 11;
const int YAHTZEE = 12;

const int BOARD_SIZE=13; 

class Game {

	public:
		//calcScore returns a score of a hand (5 dice) for given row in the board. 
		//Note the rows are indexed from zero. 
		//For example if the hand is 2 1 1 5 1 then calcScore(hand, ONES) returns 3 and  calcScore(hand, TWOS) returns 2.
		int calcScore(Hand* hand, int row); 
		
		
		//Display the board
		void show();
		

		//Returns the score of the upper part of the board
		int getUpperScore();
		
		//Returns the score of the upper part of the board
		int getLowerScore();
	

		//Returns the bonus points
		int getBonusScore();

		//Returns the total score
		int getTotalScore();

		//Play a hand based on the selected row
		void play(Hand* hand, int row);

		
		//Check to see if a row has been played or not (returns true if a row has been played)
		bool isPlayed(int row);
		
		//Check to see if all rows haven been played or not (returns true if all rows have been played)
		bool isFinished();

		Game();
    
    void reset_rolls();

    void count_rolls(Hand* hand);

		int sum_hand(Hand* hand);

		void print(int row);

    int* rolls = new int[6];

	private:
	int* board = new int[BOARD_SIZE];
	bool* played = new bool[BOARD_SIZE];
};

Game::Game(){
	for(int i = 0; i < BOARD_SIZE; i++){
		board[i] = -1;
		played[i] = false;
	}
}

void Game::count_rolls(Hand* hand){
  int dice_roll;
  for(int i = 0; i < HAND_SIZE; i++){
    dice_roll = hand->getDice(i)->reveal();
    rolls[dice_roll-1]++;
  }
}

void Game::reset_rolls(){
  for(int i = 0; i < 6; i++){
    rolls[i] = 0;
  }
}
int Game::calcScore(Hand* hand,int row){
  reset_rolls();
  count_rolls(hand);
	if(row < 6){
		return rolls[row] * (row+1);
	}
	else if(row == THREE_OF_KIND){
		for(int i = 0; i < HAND_SIZE; i++){
      if (rolls[i] >= 3){
        return sum_hand(hand);
      }
    }
    return 0;
	}
	else if(row == FOUR_OF_KIND){
		for(int i = 0; i < HAND_SIZE; i++){
      if (rolls[i] >= 4){
        return sum_hand(hand);
      }
    }
    return 0;
	}
	else if(row == FULL_HOUSE){
		bool row3 = false;
    bool row2 = false;
    for (int i = 0; i < 6; i++){
      if (rolls[i] == 3){
        row3 = true;
      }
      if (rolls[i] == 2){
        row2 = true;
      }
    }
    if (row3 && row2){
      return 25;
    }
    else{
      return 0;
    }
  }
	else if(row == SMALL_STRAIGHT){
		if(rolls[0] >= 1 && rolls[1] >= 1 && rolls[2] >= 1 && rolls[3] >= 1){
      return 30;
    }
    else if(rolls[1] >= 1 && rolls[2] >= 1 && rolls[3] >= 1 && rolls[4] >= 1){
      return 30;
    }
    else if(rolls[2] >= 1 && rolls[3] >= 1 && rolls[4] >= 1 && rolls[5] >= 1){
      return 30;
    }
    else{
      return 0;
    }
	}
	else if(row == LARGE_STRAIGHT){
    if ((rolls[0] == 1 && rolls[1] == 1 && rolls[2] == 1 && rolls[3] == 1 && rolls[4] == 1) || (rolls[1] == 1 && rolls[2] == 1 && rolls[3] == 1 && rolls[4] == 1 && rolls[5] == 1)){
      return 40;
    }
    else{
      return 0;
    }
  }
	else if(row == CHANCE){
		return sum_hand(hand);
	}
	else if (row == YAHTZEE){
    for(int i = 0; i < 6; i++){
      if (rolls[i] == 5){
        return 50;      
      }
    }
	return 0;
}
  return 0;
}

int Game::sum_hand(Hand* hand){
	int sum = 0;
	for(int i = 0; i < HAND_SIZE; i++){
		sum += hand->getDice(i)->reveal();
	}
	return sum;
}
bool Game::isPlayed(int row){
	return played[row];
}
bool Game::isFinished(){
	for(int i = 0; i < BOARD_SIZE; i++){
		if(!isPlayed(i)){
			return false;
		}
	}
	return true;
}

int Game::getUpperScore(){
	int upper = 0;
	for(int i = 0; i < 6; i++){
		if(played[i]){
			upper += board[i];
		}
	}
	return upper;
}
int Game::getLowerScore(){
	int lower = 0;
	for(int i = 6; i < BOARD_SIZE; i++){
		if(played[i]){
			lower += board[i];
		}
	}
	return lower;
}
int Game::getBonusScore(){
	int bonus = getUpperScore();
	if(bonus >= 63){
		return 35;
	}
	return 0;
}
int Game::getTotalScore(){
	int total = getUpperScore() + getLowerScore() + getBonusScore();
	return total;
}
void Game::play(Hand* hand, int row){
	int score = calcScore(hand,row);
	board[row] = score;
	played[row] = true;
}
void Game::show(){
	cout << "1.  Ones:";
	print(ONES);
	cout << "2.  Twos:";
	print(TWOS);
	cout << "3.  Threes:";
	print(THREES);
	cout << "4.  Fours:";
	print(FOURS);
	cout << "5.  Fives:"; 
	print(FIVES);
	cout << "6.  Sixes:"; 
	print(SIXES);
	cout << "Sum:" << getUpperScore() << endl;
	cout << "Bonus:" << getBonusScore() << endl;
	cout << "7.  Three of a kind:";
	print(THREE_OF_KIND);
	cout << "8.  Four of a kind:";
	print(FOUR_OF_KIND);
	cout << "9.  Full House:"; 
	print(FULL_HOUSE);
	cout << "10.  Small straight:";
	print(SMALL_STRAIGHT);
	cout << "11.  Large straight:";
	print(LARGE_STRAIGHT);
	cout << "12.  Chance:"; 
	print(CHANCE);
	cout << "13.  Yahtzee:";
	print(YAHTZEE);
	cout << "Total:" << getTotalScore() << endl;
}

void Game::print(int row){
	if(played[row]){
		cout << board[row] << endl;
	}
	else{
		cout << '-' << endl;
	}
}
//#######################################################################################

//The run function is the main loop of your program
void run() {
	Game board;
	Hand hand;
	Hand* phand = &hand;
	string select;
	int comb;
	int i;
	bool notplay;
	while (!board.isFinished()){
		i = 0;
		notplay = true;
		while(notplay){
			board.show();
			if (i < 2 ){
				phand->show();
				cout << "Keep Dice (s to stop rolling): ";
				cin >> select;
				cout << endl;
				if(select != "s"){
					phand->setSelection(select);
					phand->play();
					i++;
				}
				else{
					board.show();
					phand->show();
					cout << "Select a combination to play: ";
					cin >> comb;
					cout << endl;
					while(board.isPlayed(comb-1)){
						cout << "This combination has been played. Select another one: ";
						cin >> comb;
					}
					board.play(phand,comb-1);
          			phand->resetHand();
					notplay = false;
				}
			}
			else{
				board.show();
				phand->show();
				cout << "Select a combination to play: ";
				cin >> comb;
				cout << endl;
				while(board.isPlayed(comb-1)){
					cout << "This combination has been played. Select another one: ";
					cin >> comb;
				}
				board.play(phand,comb-1);
				phand->resetHand();
				notplay = false;
			}
		}
	}
  board.show();
}


//For these types of programs there exists many combinations that you might have missed. 
//The best way to check them all is to write test cases for your program. 
//Here you are given with an example of checking large straight. You can design your own test cases for different parts.

void test_case() {
	Game game;
	Hand* hand = new Hand();
	hand->getDice(0)->setValue(1);
	hand->getDice(1)->setValue(2);
	hand->getDice(2)->setValue(3);
	hand->getDice(3)->setValue(4);
	hand->getDice(4)->setValue(5);

  game.count_rolls(hand);
  for(int i = 0; i < 6; i++){
    cout << game.rolls[i];
  }
  cout << endl;
  
	bool check1 = true;
	if (game.calcScore(hand, LARGE_STRAIGHT) != 40) {
		check1 = false;
	}
	if (check1) {
		cout << "check 1 passed\n"; 
	}

	hand->getDice(0)->setValue(1);
	hand->getDice(1)->setValue(1);
	hand->getDice(2)->setValue(1);
	hand->getDice(3)->setValue(1);
	hand->getDice(4)->setValue(1);

  game.reset_rolls();
  game.count_rolls(hand);
  for(int i = 0; i < 6; i++){
    cout << game.rolls[i];
  }
  cout << endl;
  
	bool check2 = true;
	if (game.calcScore(hand, YAHTZEE) != 50) {
     cout << game.calcScore(hand, YAHTZEE) << endl;
		check2 = false;
	}

	if (check2) {
		cout << "check 2 passed\n"; 
	}

	hand->getDice(0)->setValue(2);
	hand->getDice(1)->setValue(2);
	hand->getDice(2)->setValue(2);
	hand->getDice(3)->setValue(2);
	hand->getDice(4)->setValue(2);

  game.reset_rolls();
  game.count_rolls(hand);
  for(int i = 0; i < 6; i++){
    cout << game.rolls[i];
  }
  cout << endl;
	bool check3 = true;
	if (game.calcScore(hand, YAHTZEE) != 50) {
		check3 = false;
	}

	if (check3) {
		cout << "check 3 passed\n"; 
	}

	hand->getDice(0)->setValue(1);
	hand->getDice(1)->setValue(2);
	hand->getDice(2)->setValue(3);
	hand->getDice(3)->setValue(4);
	hand->getDice(4)->setValue(6);

  game.reset_rolls();
  game.count_rolls(hand);
  for(int i = 0; i < 6; i++){
    cout << game.rolls[i];
  }
  cout << endl;
	bool check4 = true;
	if (game.calcScore(hand, YAHTZEE) != 50) {
		check4 = false;
	}

	if (check4) {
		cout << "check 4 passed\n"; 
	}

  hand->getDice(0)->setValue(3);
  hand->getDice(1)->setValue(3);
  hand->getDice(2)->setValue(3);
  hand->getDice(3)->setValue(3);
  hand->getDice(4)->setValue(3);

  game.reset_rolls();
  game.count_rolls(hand);
  for(int i = 0; i < 6; i++){
    cout << game.rolls[i];
  }
  cout << endl;
  bool check5 = true;
if (game.calcScore(hand,YAHTZEE) != 50){
  check5 = false;
}
if(check5){
  cout << "check 5 passed\n";
}
  hand->getDice(0)->setValue(4);
  hand->getDice(1)->setValue(4);
  hand->getDice(2)->setValue(4);
  hand->getDice(3)->setValue(4);
  hand->getDice(4)->setValue(4);

  game.reset_rolls();
  game.count_rolls(hand);
  for(int i = 0; i < 6; i++){
    cout << game.rolls[i];
  }
  cout << endl;
  bool check6 = true;
  if (game.calcScore(hand, YAHTZEE) != 50) {
    check6 = false;
  }

  if (check6) {
    cout << "check 6 passed\n"; 
    cout << game.calcScore(hand, YAHTZEE) << endl;
  }
  hand->getDice(0)->setValue(5);
  hand->getDice(1)->setValue(5);
  hand->getDice(2)->setValue(5);
  hand->getDice(3)->setValue(5);
  hand->getDice(4)->setValue(5);

  game.reset_rolls();
  game.count_rolls(hand);
  for(int i = 0; i < 6; i++){
    cout << game.rolls[i];
  }
  cout << endl;
  bool check7 = true;
  if (game.calcScore(hand, YAHTZEE) != 50) {
    check7 = false;
  }

  if (check7) {
    cout << "check 7 passed\n"; 
    cout << game.calcScore(hand, YAHTZEE) << endl;
  }
  hand->getDice(0)->setValue(6);
  hand->getDice(1)->setValue(6);
  hand->getDice(2)->setValue(6);
  hand->getDice(3)->setValue(6);
  hand->getDice(4)->setValue(6);

  game.reset_rolls();
  game.count_rolls(hand);
  for(int i = 0; i < 6; i++){
    cout << game.rolls[i];
  }
  cout << endl;
  bool check8 = true;
  if (game.calcScore(hand, YAHTZEE) != 50) {
    check8 = false;
  }

  if (check8) {
    cout << "check 8 passed\n"; 
    cout << game.calcScore(hand, YAHTZEE) << endl;
  }
}

// THE AUTOGRADER WILL REPLACE THE MAIN FUNCTION. 
// DO NOT ADD ANY LOGIC OF YOUR PROGRAM HERE.
int main() {
	
	run();
	
	//test_case();
	return 0;
}
