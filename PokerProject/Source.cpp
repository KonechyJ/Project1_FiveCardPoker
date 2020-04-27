#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include <list>
#include <map>
#include <string>
#include <unordered_map>
#include <stack>
#include <time.h>
#include <algorithm>
using namespace std;

//Object Declaration 
struct card
{
	string cardType;
	int cardValue;

	card() {}
	card(string name, int value)
	{
		cardType = name;
		cardValue = value;
	}
};

//Prottypes
int findWinner(list<card> &playerHand);
float blinds(float originalBettingPool, int &rounds);
float addAiBlinds(float newBettingPool, int &rounds);
float aiBetting(float newBettingPool, float aiChips);
bool isPair(list<card> playerHand);
bool isTwoPair(list<card> playerHand);
bool isThreeOfAKind(list<card> playerHand);
bool isIsFourOfaKind(list<card> playerHand);
bool isFullHouse(list<card> playerHand);
bool isFlush(list<card> playerHand);
bool isStraight(list<card> playerHand);
card drawCard(stack<card>& deck);
stack<card> shuffleDeck(stack<card> &deck);
void displayWinnerPhrase(int result, int aiRoundResult, char aiAnswer);
void dealHand(list<card> &playerHand, stack<card> &deck);
void dealAIHand(list<card> &ai_1_Hand, stack<card> &deck);
void swapCards(list<card> &playerHand, stack<card> &deck);
void displayHand(list<card> playerHand);

int main()
{
	// Generate new random numbers each time the program is run
	srand(time(NULL));

	int option = 0;
	int roundResult = 0;
	int aiRoundResult = 0;
	int rounds = 0;
	int numPlayers = 0;
	float originalBettingPool = 0;
	float newBettingPool = 0;
	float yourBet = 0;
	float yourChips = 0;
	float aiChips = 0;
	float chipsToAdd = 0;
	float yourChipLimit = 1000;
	char aiAnswer;
	bool stop = false;

	// Creates the Players hands and deck
	list<card> playerHand;
	list<card> ai_1_Hand;
	stack<card> deck;

	// Loop to keep menu running
	while (stop != true)
	{
		// sends an empty deck to be filled and shuffled
		deck = shuffleDeck(deck);

		// Creates an option menu
		cout << " /==============\\" << endl;
		cout << "/                \\" << endl;
		cout << "  Five Card Poker" << endl;
		cout << "\\                /" << endl;
		cout << " \\==============/" << endl << endl;
		cout << "1. Play" << endl;
		cout << "2. Exit" << endl << endl;
		cout << "Choose your option: ";
		cin >> option;

		// Used to to set up a single Ai Player
		if (option != 2)
		{
			cout << "Would you like to add another Player? ( AI )" << endl;
			cin >> aiAnswer;
			if (toupper(aiAnswer) == 'Y')
			{
				cout << "So, another player has entered the game." << endl;
				cout << endl;
				
				dealAIHand(ai_1_Hand, deck);
			}

			//Asks the player for chips and assigns the amount to their amount; while checking to make sure the amount is valid
			cout << "Your Total Chips are: " << yourChips << endl;
			cout << "You need Chips to Play, How many would you like? (Your Chip Limit is " << yourChipLimit << ") " << endl;
			cin >> chipsToAdd;
			while (chipsToAdd + yourChips > yourChipLimit) 
			{
				cout << "Sorry invalid amount, Try again." << endl;
				cin >> chipsToAdd;
			}

			// Ups chip limit for the following rounds.
			yourChips = chipsToAdd + yourChips;
			yourChipLimit = yourChipLimit * 2.25;
		
			cout << "Ok, your buy in is " << yourChips << endl;

			// Sets ai chips = to the player chip amount
			aiChips = yourChips;

			//Blinds and minuses blinds from chip amount
			newBettingPool = blinds(originalBettingPool, rounds);
			yourChips = yourChips - newBettingPool;

			cout << "Your total after blinds is " << yourChips << endl;
			
			// checks is ai is valid and applies ai blinds
			if (toupper(aiAnswer) == 'Y')
			{
				newBettingPool = addAiBlinds(newBettingPool, rounds);
				cout << "Okay, the Pot is " << newBettingPool << " after the Blinds" << endl;
			}
			else 
			{
				cout << "Okay, the Pot is " << newBettingPool << " after the Blinds" << endl;
			}

			// Deal 5 cards to the player
			dealHand(playerHand, deck);
		
		   //displays the player hand
			displayHand(playerHand);
		
			// Give the player the option to swap their cards
			char yesOrNo;
			cout << endl << endl << "Do you want to change cards? (y/n): ";
			cin >> yesOrNo;
			if (toupper(yesOrNo) == 'Y') 
			{
				// Function call to swap the players cards
				swapCards(playerHand, deck);
				cout << endl << "Your new cards are:" << endl;

				//displays the player hand
				displayHand(playerHand);
				cout << endl;
			}
			//sequence to ask the player if and how much they want to bet
			cout << "Would you like to bet? (Y/N)" << endl;
			char altYesOrNo;
			cin >> altYesOrNo;
			if (toupper(altYesOrNo) == 'Y')
			{
				cout << "How much would you like to bet?" << endl;
				cin >> yourBet;
				while ((yourBet <= 0) || (yourBet > yourChips))
				{
					cout << "Sorry you don't have that many chips, Try again." << endl;
					cin >> yourBet;
				}
				yourChips = yourChips - yourBet;
			}
			(newBettingPool = newBettingPool + yourBet);
		
			//function call to add a random amount to the pot if the ai is active
			if (toupper(aiAnswer) == 'Y')
			{
				newBettingPool = aiBetting(newBettingPool, aiChips);
				aiChips = aiChips - (newBettingPool - yourBet);
			}
			yourBet = 0;

			//sequence to ask the player if and how much they want to bet
			cout << "Okay, Pot is Right. The Pot is " << newBettingPool << endl;
			cout << "Round 2: would you like to change your hand again?" << endl;
			cin >> yesOrNo;
			if (yesOrNo == 'y')
			{
				swapCards(playerHand, deck);
				cout << endl << "Your new cards are:" << endl;

				//displays the player hand
				displayHand(playerHand);
				cout << endl;
			}

			cout << "Would you like to bet?" << endl;
			cin >> altYesOrNo;
			if (altYesOrNo == 'y')
			{
				cout << "How much would you like to bet?" << endl;
				cin >> yourBet;
				while ((yourBet <= 0) || (yourBet > yourChips))
				{
					cout << "Sorry you don't have that many chips, Try again." << endl;
					cin >> yourBet;
				}
				yourChips = yourChips - yourBet;
				
			}
			(newBettingPool = newBettingPool + yourBet);

			//function call to add a random amount to the pot if the ai is active
			if (toupper(aiAnswer) == 'Y')
			{
				newBettingPool = aiBetting(newBettingPool, aiChips);
				aiChips = aiChips - (newBettingPool - yourBet);
			}
			yourBet = 0;

			//sequence to ask the player if and how much they want to bet
			cout << "Okay, Pot is Right. The New Pot is " << newBettingPool << endl;
			cout << "Round 3: Final round, would you like to change any cards?" << endl;
			cin >> yesOrNo;
			if (yesOrNo == 'y')
			{
				swapCards(playerHand, deck);
				cout << endl << "Your new cards are:" << endl;

				//displays the player hand
				displayHand(playerHand);
				cout << endl;
			}

			cout << "Final Bet?" << endl;
			cin >> altYesOrNo;
			if (altYesOrNo == 'y')
			{
				cout << "How much would you like to bet?" << endl;
				cin >> yourBet;
				while ((yourBet <= 0) || (yourBet > yourChips))
				{
					cout << "Sorry you don't have that many chips, Try again.." << endl;
					cin >> yourBet;
				}
				yourChips = yourChips - yourBet;
			
			}
			(newBettingPool = newBettingPool + yourBet);

			//function call to add a random amount to the pot if the ai is active
			if (toupper(aiAnswer) == 'Y')
			{
				newBettingPool = aiBetting(newBettingPool, aiChips);
				aiChips = aiChips - (newBettingPool - yourBet);
			}
			yourBet = 0;
			cout << "Okay, Pot is Right. The Pot is " << newBettingPool << endl;
			
			//-----------------------------------------------------------------------------------------------------------------------------
			// Find a winner in the hand
			// Function call to score the players hand
			// sends the playerHand list
			roundResult = findWinner(playerHand);

			// Function call to score the ai hand
			// sends the playerHand list if ai is active
			if (toupper(aiAnswer) == 'Y')
			{
				aiRoundResult = findWinner(ai_1_Hand);
			}

			// Function t display a winning phrase, 
			//is sent players score and AI score(which is 0 if ai is not active)
			displayWinnerPhrase(roundResult, aiRoundResult, aiAnswer);

			//Calculates the amount of chips that won or lost based on the players scores
			if (aiRoundResult == roundResult && aiAnswer == 'Y')
			{
				yourChips = yourChips + (newBettingPool / 2);
				aiChips = aiChips + (newBettingPool / 2);
				cout << "Your New chip Amount is " << yourChips << endl;
			}
			else if (roundResult > aiRoundResult) 
			{
				yourChips = yourChips + newBettingPool;
				cout << "Your New chip Amount is " << yourChips << endl;
			}
			
			else if (roundResult < aiRoundResult)
			{
				cout << "Your New chip Amount is " << yourChips << endl;
			}
			else 
			{
				cout << "Your New chip Amount is " << yourChips << endl;
			}
			rounds++;

			// Pause program and return to menu
			cin.ignore(256, '\n');

			// clear hands
			playerHand.clear();
			ai_1_Hand.clear();
			cout << endl << "Hit return to return to menu...";
			cin.get();
			system("CLS");
		}
		else 
		{
			stop = true;
		}
	}
	return 0;
}

// Deals five random cards to player
// is sent the empty hand list and the shuffled deck stack
void dealHand(list<card> &playerHand, stack<card> &deck)
{
	for (int i = 0; i < 5; i++)
	{
		card myCard = drawCard(deck);
		playerHand.push_back(myCard);
	}
}
// Deals five random cards to ai
// is sent the empty hand list and the shuffled deck stack
void dealAIHand(list<card> &ai_1_Hand, stack<card> &deck)
{

	for (int i = 0; i < 5; i++)
	{
		card myCard = drawCard(deck);
		ai_1_Hand.push_back(myCard);
	}
}

// Allows player to swap cards within their hand
// is sent the full player hand and the shuffled deck minus the players cards
void swapCards(list<card>& playerHand, stack<card> &deck)
{
	char cardYesOrNo;

	//Remove card from player hand and replace with new card
	for (int i = 0; i < 5; i++)
	{
		cout << "Would you like to swap card number " << (i + 1) << "?" << endl;
		cin >> cardYesOrNo;

		if (toupper(cardYesOrNo == 'y'))
		{
			list<card>::iterator it2 = playerHand.begin();
			advance(it2, ((i+1) - 1));
			playerHand.erase(it2);
			card myCard = drawCard(deck);
			playerHand.push_back(myCard);
		}
		else
		{
			continue;
		}
	}
}

// Evaluate whether there is a winner in the hand
// Is set the parameter Playerhand by reference to alter the empty hand, and pass back a score
// This function is used for both the player and AI hand
int findWinner(list<card> &playerHand)
{
	if (playerHand.size() == 0)
	{
		return 0;
	}
		
	int result = 0;//Sorry, better luck next time!
	bool straightFlush = false;
	bool flush = false;
	bool straight = false;
	bool fourOfAKind = false;
	bool fullHouse = false;
	bool threeOfAKind = false;
	bool twoPair = false;
	bool onePair = false;

	//Functions calls to check all the possbile winning card combinations
	//Playerhand is this case can be the Players or the Ai depending on when the function was called in main
	//returns to a bool to be used to compare which combination is better

	straight = isStraight(playerHand);
	flush = isFlush(playerHand);
	fourOfAKind = isIsFourOfaKind(playerHand);
	fullHouse = isFullHouse(playerHand);
	threeOfAKind = isThreeOfAKind(playerHand);
	twoPair = isTwoPair(playerHand);
	onePair = isPair(playerHand);

	// Looks for a straight flush
	if ((straight == true) && (flush == true))
	{
		straightFlush = true;
	}
	
	//checks to see which bools are true and then goes on to assign a score based on the highest ranking card combinations
	if (onePair == true && twoPair == true && fullHouse == true && threeOfAKind == true)
	{
		result = 5;
	}
	else if (onePair == true && twoPair == true && fourOfAKind == true)
	{
		result = 4;
	}
	else if (onePair == true && twoPair == true && fullHouse == true)
	{
		result = 5;
	}
	else if (twoPair == true && fullHouse == true)
	{
		result = 5;
	}
	else if (fourOfAKind == true && fullHouse == true)
	{
		result = 4;
	}
	else if (onePair == true && fullHouse == true)
	{
		result = 5;
	}
	else if (onePair == true && twoPair == true)
	{
		result = 3;
	}
	else if (twoPair == true && fourOfAKind == true)
	{
		result = 4;
	}
	else if (flush == true && fourOfAKind == true)
	{
		result = 4;
	}
	else if (flush == true && threeOfAKind == true)
	{
		result = 6;
	}
	else if (flush == true && twoPair == true)
	{
		result = 6;
	}
	else if (flush == true && onePair == true)
	{
		result = 6;
	}
	else if (straightFlush == true)
	{
		result = 1;//You have a Straight Flush
	}
	else if (fourOfAKind == true)
	{
		result = 4; //You have a Four Of A Kind!
	}
	else if (fullHouse == true)
	{
		result = 5; //"You have a Full House!
	}
	else if (flush == true)
	{
		result = 6;// You have a Flush!
	}
	else if (straight == true)
	{
		result = 7;//You have a Straight!
	}
	else if (threeOfAKind == true)
	{
		result = 8;//You have a Three Of A Kind!
	}
	else if (twoPair == true)
	{
		result = 3;//You have a Two Pair!
	}
	else if (onePair == true)
	{
		result = 2;//You have a Pair!
	}
	return result;
}

// Adds the blinds to the pot and keeps track of the rounds
// orginalBettingPool is the pot before the player or AI bets
//Rounds is used to switch the blinds between players
float blinds(float originalBettingPool, int &rounds)
{
	
	int numOfRounds = rounds;
	
	if (numOfRounds % 2 == 0) 
	{
		(originalBettingPool += 25);
		cout << "You're in the Big Blind." << endl;
	}
	else 
	{
		(originalBettingPool += 10);
		cout << "You're in the Small Blind." << endl;
	}
	return originalBettingPool;
}

// Adds the blinds to the pot and keeps track of the rounds for the ai
// newBettingPool is the pot after the player blinds and only if AI is active
//Rounds is used to switch the blinds between players
float addAiBlinds(float newBettingPool, int &rounds)
{
	int numOfRounds = rounds;

	if (numOfRounds % 2 == 0)
	{
		(newBettingPool += 10);
	}
	else
	{
		(newBettingPool += 25);
	}
	return newBettingPool;
}

//A fucntion that is used to randomly generate an amount to add to the pot for the AI's "Bets" 
//NewBettingPool is the pot that is being sent each round, if the AI is active. The AiChips is the amount of chips the ai has to make sure he doesn't bet more than he has.
float aiBetting(float newBettingPool, float aiChips)
{
	int thisNum = rand() % 100;
	int thatNum = 0;
	thatNum = (aiChips / 2);
	if(thisNum > 50)
	{
		(newBettingPool += rand() % thatNum);
	}
	return newBettingPool;
}

//displays the players hand
// is sent the list that is players hand
void displayHand(list<card> playerHand)
{
	int i = 1;
	for (auto& x : playerHand)
	{
		cout << endl << i << " - " << x.cardValue << " of " << x.cardType;
		i++;
	}
}

//This function just spits out a phrase to congratulate the player if they win, lose, or tie
//This function is sent the players result, the airesults, and if the Ai is active
void displayWinnerPhrase(int result, int aiRoundResult, char aiAnswer)
{
	if (aiRoundResult > result)
	{
		cout << "Sorry, you loose to the Computer." << endl;
	}
	else if (aiRoundResult < result) 
	{
		cout << "You are the Winner!" << endl;
		
		if (result == 0)
		{
			cout << "Sorry, better luck next time!" << endl;
		}
		else if (result == 1)
		{
			cout << "You have a Straight Flush!" << endl;
		}

		else if (result == 2)
		{
			cout << "You have a Pair!" << endl;
		}

		else if (result == 3)
		{
			cout << "You have a Two Pair!" << endl;
		}

		else if (result == 4)
		{
			cout << "You have a Four Of A Kind!" << endl;
		}

		else if (result == 5)
		{
			cout << "You have a Full House!" << endl;
		}

		else if (result == 6)
		{
			cout << "You have a Flush!" << endl;
		}

		else if (result == 7)
		{
			cout << "You have a Straight!" << endl;
		}

		else if (result == 8)
		{
			cout << "You have a Three Of A Kind!" << endl;
		}
	}

	if (toupper(aiAnswer) == 'Y')
	{
		if (aiRoundResult == result)
		{
			cout << "It's a tie, congratulations." << endl;
		}
	}
	else
	{
		cout << "Sorry, you have nothing!" << endl;
	}
}

//This fucntion creates an unordered map  called new deck and fills it with all 52 cards in order. Then using an iterator, it puts the elements of the map into the stack that was sent. 
// it does this in a random order to "Shuffle" the deck
// The function is sent the empty stack of type card, by reference
stack<card> shuffleDeck(stack<card> &deck)
{
	if (deck.size() > 0)
	{
		while (!deck.empty())
		{
			deck.pop();
		}
	}
	 
	unordered_map<int, card> newDeck;
	stack<card> doneDeck;
	srand(time(NULL));
	
	int counter = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 1; j < 14; j++)
		{
			card newCard;

			switch (i)
			{
				case 0:
					newCard.cardType = "Spades";
					newCard.cardValue = j;
					break;
				case 1:
					newCard.cardType = "Clubs";
					newCard.cardValue = j;
					break;
				case 2:
					newCard.cardType = "Diamonds";
					newCard.cardValue = j;
					break;
				case 3:
					newCard.cardType = "Hearts";
					newCard.cardValue = j;
					break;
			}
				newDeck.emplace(counter, newCard);
				counter++;
		}
	}

	
	while (newDeck.size() > 0)
	{
		unordered_map<int, card>::iterator random_it = newDeck.begin();
		advance(random_it, (rand() % newDeck.size()));
		doneDeck.push(random_it->second);
		newDeck.erase(random_it);
	}
	return doneDeck;
}
//Function used to pop a card off the top oif the stack in the swap card function
//The fucntion is sent the stack deck by reference
card drawCard(stack<card>& deck)
{
	card newCard = deck.top();
	deck.pop();
	return newCard;
}

// This Bool function is checking the hand that is sent to it to see if the cards make a pair. A bool is returned to the findWinner fucntion
//playerhand is NOT sent by reference because we are not altering it
bool isPair(list<card> playerHand) 
{
	bool isPair = false;

	// nested for loops to compare the cards to each other
	for (auto& x : playerHand)
	{
		for (auto& y : playerHand)
		{
			if ((x.cardValue == y.cardValue) && (&x !=  &y))
			{
				isPair =  true;
			}
		}		
	}

	return isPair;
}


// This Bool function is checking the hand that is sent to it to see if the cards make a two pair. A bool is returned to the findWinner fucntion
//playerhand is NOT sent by reference because we are not altering it
// Alist is used to keep track of the amount of times that cards are matching (but no the same card)
bool isTwoPair(list<card> playerHand)
{
	bool twoPair = false;
	list<string> temp;
	for (auto& x : playerHand)
	{
		for (auto& y : playerHand)
		{
			// Checks if the value of the card are the same but makes sure they are not the same card
			if ((x.cardValue == y.cardValue) && (&x != &y))
			{
				temp.push_back(x.cardType);
			}
		}
	}

	if (temp.size() >= 4)
	{
		twoPair = true;
	}
	else
	{
		twoPair = false;
	}
	return twoPair;
}

//This Bool function is checking the hand that is sent to it to see if the cards make a three of a kind. A bool is returned to the findWinner fucntion
//playerhand is NOT sent by reference because we are not altering it
//A simple conuter is used this time to count the amount of times cards match but are not the same
bool isThreeOfAKind(list<card> playerHand)
{
	bool threeOfAkind = false;
	
	int counter = 0;
	// nested for loops to compare the cards to each other
	for (auto& x : playerHand)
	{
		for(auto& y : playerHand)
		{
			// Checks if the value of the card are the same but makes sure they are not the same card
			if ((x.cardValue == y.cardValue) && (&x != &y))
			{
				counter++;
			}
		}
	}

	if (counter >= 6)
	{
		threeOfAkind = true;
	}

	return threeOfAkind;
}

//This Bool function is checking the hand that is sent to it to see if the cards make a four of a kind. A bool is returned to the findWinner fucntion
//playerhand is NOT sent by reference because we are not altering it
//A simple conuter is used this time to count the amount of times cards match but are not the same
bool isIsFourOfaKind(list<card> playerHand)
{
	bool fourOfAkind = false;
	int counter = 0;
	// nested for loops to compare the cards to each other
	for (auto& x : playerHand)
	{
		for (auto& y : playerHand)
		{
			// Checks if the value of the card are the same but makes sure they are not the same card
			if ((x.cardValue == y.cardValue) && (&x != &y))
			{
				counter++;
			}
		}

		if (counter >= 9)
		{
			fourOfAkind = true;
		}
	}
	return fourOfAkind;
}

//This Bool function is checking the hand that is sent to it to see if the cards make a three of a kind. A bool is returned to the findWinner fucntion
//playerhand is NOT sent by reference because we are not altering it
//A simple conuter is used with two bools to check if this time to count the amount of times cards match but are not the same
bool isFullHouse(list<card> playerHand)
{
	bool fullHouse = false;
	bool three = false;
	bool two = false;
	int size = 0;

	// nested for loops to compare the cards to each other
	for (auto& x : playerHand)
	{
		for (auto& y : playerHand)
		{
			// Checks if the value of the card are the same but makes sure they are not the same card
			if ((x.cardValue == y.cardValue) && (&x != &y))
			{
				size++;
			}
		}
	}
	if (size >= 8)
	{
		three = true;
		two = true;
	}
	else 
	{
		three = false;
		two = false;
	}

	if (three == true && two == true)
	{
		fullHouse = true;
	}

	return fullHouse;
}

//Function checks the hand sent to it and check to see if a straight is true
//Para: passes copy of the playerHand to function to see if they have a straight
bool isStraight(list<card> playerHand)
{
	bool str = true;
	// list containing 1-13 to check for a straight
	list<int> temp1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };

	playerHand.sort([](const card& a, const card& b) { return a.cardValue < b.cardValue; }); //sort by the given inline function

	list<int>::iterator it = temp1.begin();
	int num = 0;

	//  for loop to compare the cards to the list above containing 1-13
	for (auto& x : playerHand)
	{
		//first if set num equal to the lowest card in the sorted hand
		if (num == 0)
		{
			// Advacne the iterator up to match the card value
			advance(it, (x.cardValue - 1));
			num = *it;
		}
		else if ((num + 1) != x.cardValue)
		{
			str = false;
			break;
		}
		else
		{
			num++;
		}
	}
	return str;
}

// Bool functions to check the card to see if a flush is present in the hand
// is pasted player hand to compare it to temp
bool isFlush(list<card> playerHand)
{
	bool flush = false;
	// List created to copmpare to playerhand and an iterator to go through the list
	list<string> temp1 = { "Hearts", "Diamonds", "Spades", "Clubs" };
	list<string>::iterator it = temp1.begin();
	string suit = "";
	//  for loop to compare the cards to the list above containing the card types
	for (auto& x : playerHand)
	{
		if (suit == "")
		{
			suit = x.cardType;
		
		}
		else if ((suit) == x.cardType)
		{
			flush = true;
		}
		else
		{
			flush = false;
			break;
		}
	}
	return flush;
}